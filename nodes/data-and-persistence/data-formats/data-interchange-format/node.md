---
tag: 7GXT8V
kind: concept
slug: data-interchange-format
name: Data interchange format
aliases: [interchange format, serialization format, data serialization format, wire format, data format]
area: data-and-persistence/data-formats
level: intro
summary: >-
  A data interchange format is an agreed layout of bytes for structured data, so that one program can write a value and another program, in any language, can read it back.
applies_to: core-languages
history:
  year: 1987
  people: [Sun Microsystems]
  note: RFC 1014 of June 1987 published XDR, the External Data Representation of Sun RPC and NFS. XML 1.0 followed in 1998 and JSON in 2001, and Google released Protocol Buffers as open source in 2008.
see_also: [json, csv, object-serialization, deserialization-of-untrusted-data, read-and-write-json]
same_as:
  wikidata: Q1127410
  wikipedia: Comparison_of_data-serialization_formats
sources:
- url: https://cheatsheetseries.owasp.org/cheatsheets/Deserialization_Cheat_Sheet.html
  title: OWASP Cheat Sheet Series, Deserialization Cheat Sheet
  locator: What is Deserialization
- url: https://www.rfc-editor.org/rfc/rfc8259
  title: "T. Bray (Ed.), RFC 8259: The JavaScript Object Notation (JSON) Data Interchange Format, December 2017"
  locator: the abstract; 6 Numbers
- url: https://www.rfc-editor.org/rfc/rfc8949
  title: "C. Bormann, P. Hoffman, RFC 8949: Concise Binary Object Representation (CBOR), December 2020"
  locator: the abstract
- url: https://github.com/msgpack/msgpack/blob/master/spec.md
  title: MessagePack specification
  locator: the introduction
- url: https://www.rfc-editor.org/rfc/rfc1014
  title: "Sun Microsystems, RFC 1014: XDR, External Data Representation Standard, June 1987"
  locator: 1 Introduction; 2 Basic Block Size
- url: https://protobuf.dev/overview/
  title: Protocol Buffers Documentation, Overview
  locator: the introduction; When are Protocol Buffers not a Good Fit; Protobuf Open Source Philosophy
- url: https://avro.apache.org/docs/1.12.0/specification/
  title: Apache Avro 1.12.0 Specification
  locator: Data Serialization and Deserialization
- url: https://docs.python.org/3/library/pickle.html
  title: The Python Standard Library, pickle, Python object serialization
  locator: the warning at the top; Comparison with json
- url: https://www.rfc-editor.org/rfc/rfc4180
  title: "Y. Shafranovich, RFC 4180: Common Format and MIME Type for Comma-Separated Values (CSV) Files, October 2005"
  locator: 2 Definition of the CSV Format
- url: https://jsonlines.org/
  title: JSON Lines
  locator: the introduction
- url: https://yaml.org/spec/1.2.2/
  title: YAML Ain't Markup Language (YAML) version 1.2.2, October 2021
  locator: Chapter 1 Introduction to YAML
- url: https://toml.io/en/v1.0.0
  title: "TOML: Tom's Obvious Minimal Language, v1.0.0"
  locator: Objectives
- url: https://pkg.go.dev/encoding
  title: Go standard library, package encoding
  locator: the overview
- url: https://nodejs.org/api/v8.html
  title: Node.js documentation, V8, Serialization API
  locator: Serialization API; v8.serialize
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/module-summary.html
  title: Java SE 25 API, module java.xml
  locator: the module description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/DataOutputStream.html
  title: Java SE 25 API, java.io.DataOutputStream
  locator: the class description
- url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/overview
  title: .NET documentation, JSON serialization and deserialization in .NET
  locator: the overview
- url: https://serde.rs/
  title: Serde, the overview
  locator: the first paragraph
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/convertto-csv
  title: PowerShell documentation, ConvertTo-Csv
  locator: Description
- url: https://en.wikipedia.org/wiki/XML
  title: XML, Wikipedia
  locator: the introduction
- url: https://mamund.site44.com/rwcbook/the-json-saga/
  title: Douglas Crockford, The JSON Saga (transcript of the talk)
  locator: the first message
---

## Idea

Two programs that share data must agree on how the data looks as bytes. A program holds a record as objects in its own memory, with pointers, padding and a byte order that belong to one machine and one language. Those bytes have no meaning to another process. A data interchange format is the agreement: it says how to write a value as a sequence of bytes (serialization) and how to read the bytes back into a value (deserialization). The OWASP cheat sheet defines serialization as "the process of turning some object into a data format that can be restored later"[^1].

JSON calls itself "a lightweight, text-based, language-independent data interchange format"[^2], and that phrase names the main questions to ask of any format. Is it text or binary? Is it tied to one language? Must the reader know a schema? Can a program read it one record at a time? Can a person edit it?

The example writes one record in five formats with only the Python standard library. Each format keeps a different part of the record.

```python run id=one-record-five-formats
import csv, io, json, pickle, struct
import xml.etree.ElementTree as ET

record = {"id": 42, "name": "Ada", "tags": ["admin", "ops"], "active": True}

# Text, self-describing: the field names travel with the data.
as_json = json.dumps(record, separators=(",", ":")).encode()

# Text, a table: CSV has no lists and no types, so the tags become one string.
buf = io.StringIO()
w = csv.DictWriter(buf, fieldnames=list(record), lineterminator="\n")
w.writeheader()
w.writerow({**record, "tags": ";".join(record["tags"])})
as_csv = buf.getvalue().encode()

# Text, a tree of elements.
root = ET.Element("user", id=str(record["id"]), active="true")
ET.SubElement(root, "name").text = record["name"]
for t in record["tags"]:
    ET.SubElement(root, "tag").text = t
as_xml = ET.tostring(root)

# Binary, Python only.
as_pickle = pickle.dumps(record, protocol=5)

# Binary, the schema lives in the code: id (u32), active (bool), name (u8 length + bytes).
name = record["name"].encode()
as_struct = struct.pack("<I?B", record["id"], record["active"], len(name)) + name

for label, data in [("json", as_json), ("csv", as_csv), ("xml", as_xml),
                    ("pickle", as_pickle), ("struct", as_struct)]:
    print(f"{label:7}{len(data):4} bytes  {data[:40]!r}")

# A round trip shows what each format keeps.
print(json.loads(as_json) == record)
print(next(csv.DictReader(io.StringIO(as_csv.decode()))))
print(ET.fromstring(as_xml).get("id"), type(ET.fromstring(as_xml).get("id")).__name__)
print(pickle.loads(as_pickle) == record)
uid, active, n = struct.unpack_from("<I?B", as_struct)
print(uid, active, as_struct[6:6 + n].decode())
```

```output for=one-record-five-formats
json     59 bytes  b'{"id":42,"name":"Ada","tags":["admin","o'
csv      42 bytes  b'id,name,tags,active\n42,Ada,admin;ops,Tru'
xml      81 bytes  b'<user id="42" active="true"><name>Ada</n'
pickle   71 bytes  b'\x80\x05\x95<\x00\x00\x00\x00\x00\x00\x00}\x94(\x8c\x02id\x94K*\x8c\x04name\x94\x8c\x03Ada\x94\x8c\x04tags'
struct    9 bytes  b'*\x00\x00\x00\x01\x03Ada'
True
{'id': '42', 'name': 'Ada', 'tags': 'admin;ops', 'active': 'True'}
42 str
True
42 True Ada
```

JSON and pickle give back the same record. CSV and XML give back strings, so the reader must know that `id` is a number. The `struct` bytes are the smallest, because they hold no names and no types, but only a reader that knows the exact layout can decode them.

## How it works

A format has two parts: a data model and an encoding. The data model is the set of values that the format can hold, for example objects, arrays, strings, numbers, true, false and null for JSON. The encoding is the rule that maps each value to bytes. A program maps its own types to the data model when it writes, and maps them back when it reads. Each mapping can lose information. JSON has no date type, CSV has no nested values, and a JSON number has no fixed size.

**Text against binary.** A text format writes values as characters, usually in UTF-8. A person can read it, a diff tool can compare two versions, and a text editor can change it. The cost is size and speed: the number 42 takes two characters, and a parser must scan every character for quotes and escapes. A binary format writes numbers in a fixed or variable-length integer encoding and puts the length before each string, so a reader can skip a field without scanning it. CBOR is a binary format "whose design goals include the possibility of extremely small code size, fairly small message size, and extensibility without the need for version negotiation"[^3]. MessagePack says of itself that it "is an object serialization specification like JSON"[^4].

**Self-describing against schema-based.** In a self-describing format such as JSON, XML, CBOR or MessagePack, each record carries its field names and the type of each value, so a reader with no prior knowledge can build a tree. In a schema-based format, the writer and the reader share a schema, and the bytes hold only the values. XDR was an early example: "XDR uses implicit typing"[^5], and "The representation of all items requires a multiple of four bytes (or 32 bits) of data"[^5]. Protocol Buffers and Avro use the same idea. The Protocol Buffers documentation says that a message cannot be fully interpreted "without access to its corresponding .proto file"[^6]. The Avro specification says that "Binary encoded Avro data does not include type information or field names"[^7], and therefore "files or systems that store Avro data should always include the writer’s schema for that data"[^7].

A schema gives small messages, a check of the types, and generated code. It also needs a rule for change: a new reader must still read old data, and an old reader must still read new data. Avro lets a reader map the writer's schema to its own schema, and its specification says that this "facilitates both schema evolution as well as processing disparate datasets"[^7].

**Language-neutral against language-specific.** Some formats belong to one language. Python's pickle, Java object serialization, Go's gob and the V8 serializer of Node.js write the objects of their own runtime, with types that other languages do not have. The pickle documentation compares the two kinds: "JSON is interoperable and widely used outside of the Python ecosystem, while pickle is Python-specific"[^8]. A language-specific format is convenient for a cache or for messages between two processes of the same program. It is the wrong choice for a public interface or for data that must outlive the program.

**Streams of records.** A program reads a single JSON document or XML document as one value, so the parser must see the end before the value is complete. A format for large data lets a program read one record at a time. CSV puts one record on each line[^9]. JSON Lines puts one JSON value on each line, and its site calls it "a convenient format for storing structured data that may be processed one record at a time"[^10]. Avro and other binary formats write a sequence of length-prefixed records in a container file.

**Human editing.** Configuration files are read by programs but written by people, so formats for them add comments and a lighter syntax. YAML is "a data serialization language designed to be human-friendly"[^11], and "TOML aims to be a minimal configuration file format that's easy to read due to obvious semantics"[^12].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `json`, `csv`, `xml.etree`, `tomllib` (read only), `plistlib` and the Python-only `pickle` and `marshal` |
| JavaScript | native | the `JSON` object in the language; Node.js adds the binary `v8.serialize` |
| TypeScript | native | the `JSON` object of the JavaScript runtime; types do not survive `JSON.parse` |
| Java | stdlib | XML in the module `java.xml`; `DataOutputStream` and object serialization; no JSON in Java SE |
| Kotlin | library | the JVM classes of Java, or the official `kotlinx.serialization` library for JSON and other formats |
| C# | stdlib | `System.Text.Json`, `System.Xml` and `XmlSerializer`; `BinaryWriter` for a fixed layout |
| Go | stdlib | `encoding/json`, `encoding/xml`, `encoding/csv` and the Go-only `encoding/gob` |
| Rust | library | no format in `std`; the `serde` framework with crates such as `serde_json` |
| C | library | no format in the standard library; libraries for JSON, XML, CBOR and Protocol Buffers |
| C++ | library | no format in the standard library; libraries such as nlohmann/json and Protocol Buffers |
| Bash | library | text tools for CSV-like lines; `jq` for JSON |
| PowerShell | stdlib | `ConvertTo-Json`, `ConvertTo-Csv`, `ConvertTo-Xml` and the matching `ConvertFrom` cmdlets |
| SQL | native | JSON functions and types in the database; bulk load and export of CSV |
| Regex | n/a | a pattern can check a token but cannot parse nested data |
| HTML | n/a | a document format; a `script` element of type `application/json` can carry JSON |
| CSS | n/a | style sheets hold no data records |

Most languages read and write JSON with the standard library, and the languages that do not, such as C, C++ and Rust, have common libraries for it. Binary formats with a schema, such as Protocol Buffers and Avro, always need a library and a code generator or a schema parser. Python, Java, Go and JavaScript also have a binary format of their own runtime.

The standard libraries describe these tools in their own words. In Go, packages that implement the shared interfaces of `encoding` "include encoding/gob, encoding/json, and encoding/xml"[^13]. The V8 serializer of Node.js writes values "in a way that is compatible with the HTML structured clone algorithm"[^14]. The Java module `java.xml` "Defines the Java APIs for XML Processing (JAXP)"[^15], and `DataOutputStream` "lets an application write primitive Java data types to an output stream in a portable way"[^16]. In .NET, `System.Text.Json` "provides functionality for serializing to and deserializing from (or marshalling and unmarshalling) JavaScript Object Notation (JSON)"[^17]. Rust leaves formats to libraries, and "Serde is a framework for serializing and deserializing Rust data structures efficiently and generically"[^18]. PowerShell converts objects to text with cmdlets, for example "The ConvertTo-Csv cmdlet returns a series of character-separated value (CSV) strings that represent the objects that you submit"[^19].

The example files show the same contrasts in other languages. JavaScript shows that `JSON.stringify` rejects a `BigInt` and turns a `Date` into a string and a `Map` into `{}`, while `v8.serialize` keeps all three. Go writes one struct as JSON, XML, CSV and gob, and shows that the first gob value carries its type. Java writes a record with `DataOutputStream`, reads the same bytes with the wrong layout, and writes XML with the DOM of the JDK. C# writes JSON with `System.Text.Json`, XML with `XmlWriter` and a fixed layout with `BinaryWriter`.

## Variations and related ideas

- **Interchange against storage.** An interchange format moves data between programs. A storage format, such as the file format of a database, is read by one program and can change between versions of that program.
- **Canonical forms.** Many formats allow more than one encoding of the same value, for example with keys in a different order or with extra spaces. A canonical form fixes one encoding, so that two programs can compare or sign the bytes.
- **Schemas for self-describing formats.** JSON Schema and the XML schema languages add a schema to a format that does not need one, for validation and documentation.
- **Object serialization.** A language runtime can write an object graph with its types and references, see [[object-serialization]]. This is the language-specific end of the range.

## Pitfalls

- **Untrusted input to a language-specific format.** "The pickle module is not secure. Only unpickle data you trust."[^8] The same risk applies to other formats that rebuild objects of any class, such as Java object serialization, see [[deserialization-of-untrusted-data]]. Use a data-only format such as JSON for input from outside.
- **Large integers in JSON.** JSON does not limit the size of a number, but many parsers read numbers as IEEE 754 doubles. RFC 8259 notes that "good interoperability can be achieved by implementations that expect no more precision or range than these provide"[^2]. An identifier above 2^53 can change in JavaScript. Send large identifiers as strings.
- **Types that do not exist in the format.** Dates, decimal numbers, binary data, sets and maps with non-string keys have no JSON type. Agree on a string form, for example an ISO 8601 date, and document it.
- **Split CSV on commas.** A field can hold a comma, a quote or a line break inside quotes[^9]. Use a CSV parser, not a split on `,`.
- **Character encoding.** A text format is only text after the reader knows the encoding. Write UTF-8, and say so in the format or the protocol.
- **Schema changes.** A schema-based reader with an old schema can read new data wrongly with no error, as the Java example shows. Change a schema only in ways that old readers can handle, and keep the writer's schema with the data, as Avro does[^7].
- **Whole-message limits.** The Protocol Buffers documentation warns: "For data that exceeds a few megabytes, consider a different solution"[^6]. A format that the reader must load as one value is a bad fit for large data sets. Use a stream of records.

## Cost

The size and the speed of a format depend on the data and on the library more than on the format alone, so measure with your own data. Some general rules hold. A self-describing format repeats each field name in each record, and a schema-based format does not, so the schema-based form of many small records is smaller. A text format must convert each number from digits and scan each string for escapes. A binary format with a length before each string lets a reader skip fields. Compression such as gzip removes much of the repeated field names in a text format, at the cost of CPU time. Protocol Buffers "Messages are not compressed"[^6], so a large message is often compressed before it is sent.

The cost that matters most is often not speed but change. A format with a public specification and many parsers is cheap to keep for years. A format that only one language or one version of a library can read becomes a cost when the program changes.

## History

Programs exchanged binary records long before standard formats, with each pair of programs agreeing on its own layout. In June 1987, Sun Microsystems published XDR as RFC 1014, "a standard for the description and encoding of data"[^5] that Sun RPC and NFS used. The RFC compares XDR with "X.409, ISO Abstract Syntax Notation"[^5]. CBOR still names "ASN.1 and MessagePack"[^3] as earlier binary serializations.

According to Wikipedia, the W3C published XML 1.0 as a Recommendation in 1998[^20]. Douglas Crockford and Chip Morningstar sent the first JSON message in 2001[^21]. RFC 8259 is the current IETF specification of JSON[^2]. The OWASP cheat sheet sums up the change: "Today, the most popular data format for serializing data is JSON. Before that, it was XML."[^1]

Binary formats with schemas came back at a large scale. Google used Protocol Buffers inside the company, and "Protobuf was open sourced in 2008"[^6]. Avro[^7] and the schemaless MessagePack[^4] and CBOR[^3] are other binary formats, and CBOR is an Internet Standard of the IETF (STD 94)[^3]. For configuration, YAML[^11] and TOML[^12] are text formats that people can write by hand.

## References

- RFC 8259, the JSON standard, with its notes on the interoperability of numbers[^2].
- RFC 8949, CBOR, a binary format with the data model of JSON and more[^3].
- RFC 1014, XDR, an early schema-based binary format[^5].
- The Protocol Buffers overview and the Avro specification, two schema-based binary formats[^6][^7].
- The pickle documentation, for a language-specific format and its security warning[^8].
- The OWASP Deserialization Cheat Sheet[^1].

---
tag: TNYDZ9
kind: concept
slug: object-serialization
name: Serialization
aliases: [serialization, serialisation, deserialization, marshalling, unmarshalling, pickling, encoding and decoding, data serialization, object persistence]
area: data-and-values/serialization
level: core
summary: >-
  Serialization turns values in memory into bytes or text that a file or a network can carry, and deserialization rebuilds the values. The format decides what survives the trip.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1987
  people: [Douglas Crockford, Jeff Dean, Sanjay Ghemawat, Kenton Varda, Erick Tryzelaar, David Tolnay]
  note: Sun published XDR in 1987, a format that fixes the byte order and the alignment of values sent between machines. Java 1.1 added object serialization in 1997, and JSON followed in 2001.
see_also: [read-and-write-json, json, csv, deserialization-of-untrusted-data, pointers-and-references, hash-codes]
same_as:
  wikidata: Q1127410
  wikipedia: Serialization
sources:
- url: https://protobuf.dev/overview/
  title: Overview, Protocol Buffers documentation
  locator: Overview and Open Source Philosophy
- url: https://pkg.go.dev/encoding/json
  title: Package json, Go standard library documentation
  locator: func Marshal
- url: https://serde.rs/
  title: Overview, Serde
  locator: Overview
- url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/overview
  title: JSON serialization and deserialization in .NET, overview
  locator: Overview
- url: https://docs.python.org/3/library/pickle.html
  title: pickle, Python object serialization, Python documentation
  locator: Introduction and Pickler
- url: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Structured_clone_algorithm
  title: The structured clone algorithm, MDN Web Docs
  locator: Description and things that do not work
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
  title: JSON.stringify(), MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/Serializable.html
  title: Interface Serializable, Java SE 25 API documentation
  locator: Interface description
- url: https://pkg.go.dev/encoding/gob
  title: Package gob, Go standard library documentation
  locator: Overview
- url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/binaryformatter-security-guide
  title: Deserialization risks in use of BinaryFormatter and related types, .NET documentation
  locator: Caution and note
---

## Idea

Values in memory are objects, pointers and structures that only one running program understands. Serialization turns them into a sequence of bytes or characters that a file, a database or a network can carry, and deserialization builds equal values from that sequence, in the same program later or in another program on another machine. The format is the contract between the two sides. It decides which types can travel, how numbers and text are written, and what is lost on the way: shared references, cycles, exact types, functions and private fields. Most bugs and attacks in this area come from a difference between what one side writes and what the other side expects.

```python run id=idea
import pickle
import struct


def dump(v, out):  # one tag byte, then the value; numbers are big-endian
    if isinstance(v, int):
        out += b"i" + struct.pack(">q", v)
    elif isinstance(v, str):
        data = v.encode()
        out += b"s" + struct.pack(">I", len(data)) + data
    elif isinstance(v, list):
        out += b"l" + struct.pack(">I", len(v))
        for x in v:
            dump(x, out)
    elif isinstance(v, dict):
        out += b"d" + struct.pack(">I", len(v))
        for k, x in v.items():
            dump(k, out)
            dump(x, out)
    else:
        raise TypeError(f"cannot serialize {type(v).__name__}")
    return out


def load(buf, i=0):  # returns the value and the index after it
    tag, i = buf[i:i + 1], i + 1
    if tag == b"i":
        return struct.unpack_from(">q", buf, i)[0], i + 8
    n, i = struct.unpack_from(">I", buf, i)[0], i + 4
    if tag == b"s":
        return buf[i:i + n].decode(), i + n
    items = []
    for _ in range(n * (2 if tag == b"d" else 1)):
        x, i = load(buf, i)
        items.append(x)
    return (dict(zip(items[::2], items[1::2])) if tag == b"d" else items), i


ada = {"name": "Ada", "born": 1815, "tags": ["math", "poet"]}
data = bytes(dump(ada, bytearray()))
print(len(data), "bytes:", data[:14])
print("round trip equal:", load(data)[0] == ada)

shared = ["x"]
back = load(bytes(dump([shared, shared], bytearray())))[0]
print("shared list after load: same object?", back[0] is back[1])
back = pickle.loads(pickle.dumps([shared, shared]))
print("with pickle: same object?", back[0] is back[1])

old = load(bytes(dump({"name": "Alan", "born": 1912}, bytearray())))[0]
print("old record, new reader:", old["name"], old.get("tags", []))
try:
    dump(lambda: 0, bytearray())
except TypeError as err:
    print("TypeError:", err)
```

```output for=idea
72 bytes: b'd\x00\x00\x00\x03s\x00\x00\x00\x04name'
round trip equal: True
shared list after load: same object? False
with pickle: same object? True
old record, new reader: Alan []
TypeError: cannot serialize function
```

The program defines a small binary format: a tag byte gives the type, a 4-byte length comes before each string, list and dictionary, and numbers take 8 bytes in big-endian order. A dictionary with three fields takes 72 bytes, and reading it gives an equal dictionary. A list that appears twice comes back as two separate lists, because the format writes values, not references; `pickle` writes a reference the second time and keeps one list. A record written before the `tags` field existed still loads, and the reader gives the missing field a default. A function cannot be written at all, because the format has no tag for code.

## How it works

**Formats.** A text format, such as JSON, XML, CSV or YAML, writes values as characters that a person can read and that every language can parse. A binary format, such as Protocol Buffers, CBOR, MessagePack or a language's own format, writes numbers in a fixed number of bytes and is smaller and faster to read. A format is self-describing when the data carries the names of its fields, as JSON does, or schema-based when both sides share a definition of the fields, as Protocol Buffers does: "You define how you want your data to be structured once, then you can use special generated source code to easily write and read your structured data".[^1]

**Mapping types to the format.** Some code must walk each value and write its parts. There are three ways. Reflection reads the fields of a type when the program runs, as Go's `encoding/json` and Java's object streams do; Go uses the "json" key in "the struct field's tag" to give names and options.[^2] Code generation writes the serializer when the program compiles, as Rust's serde derive macros, the .NET source generator and the code from a `.proto` file do. Serde describes the design: "data structures that know how to serialize and deserialize themselves along with data formats that know how to serialize and deserialize other things".[^3] The third way, writing the code by hand, as the C, C++ and Rust example files do, gives full control of the bytes at the cost of more code to keep in step with the type.

**What survives the trip.** A serialized form holds data, not behavior: .NET says that "The serialized form doesn't include any information about an object's associated methods."[^4] Tree formats such as JSON write each value where it appears, so shared objects become copies, and a cycle is an error or an endless loop. Graph formats keep identity: Python's pickler remembers the objects it has seen, "so that shared or recursive objects are pickled by reference and not by value".[^5] JavaScript's structured clone keeps "a map of previously visited references, to avoid infinitely traversing cycles".[^6] Types can also change on the way. JSON has no dates, sets or tuples, and `JSON.stringify` drops values it cannot write: "undefined, Function, and Symbol values are not valid JSON values. If any such values are encountered during conversion, they are either omitted (when found in an object) or changed to null (when found in an array)."[^7] Large integers lose precision in JavaScript, whose numbers are doubles.

**Language formats and neutral formats.** Several languages have a format of their own that can write almost any object: `pickle` in Python, object streams in Java, `gob` in Go and, formerly, `BinaryFormatter` in .NET. In Python, "“Pickling” is the process whereby a Python object hierarchy is converted into a byte stream, and “unpickling” is the inverse operation".[^5] In Java, "Serializability of a class is enabled by the class implementing the java.io.Serializable interface."[^8] Go's gob carries "binary values exchanged between an Encoder (transmitter) and a Decoder (receiver)".[^9] These formats are easy to use but tie the data to one language, and often to one version of the classes. Neutral formats such as JSON and Protocol Buffers work across languages; Protobuf calls itself "a language-neutral, platform-neutral extensible mechanism for serializing structured data".[^1]

**Versions.** Data often outlives the program that wrote it, so a format must survive changes to the types. The usual rules are: a reader ignores fields it does not know, a missing field gets a default, a field is never reused with a new meaning, and a version number marks changes that break these rules. Protobuf follows them with numbered fields and supports "the addition of new fields and the deletion of existing fields, to any Protobuf message without breaking existing services".[^1] Java object streams compare a version number, the serialVersionUID, and if the reader's class "has a different serialVersionUID than that of the corresponding sender's class, then deserialization will result in an InvalidClassException".[^8] The Go, C# and Rust example files show both choices: ignore unknown fields, or reject them.

**Security.** A format that names the classes to create lets the bytes choose what code runs while they are read. The Python documentation warns: "The pickle module is not secure. Only unpickle data you trust. It is possible to construct malicious pickle data which will execute arbitrary code during unpickling."[^5] Java's warning is "Deserialization of untrusted data is inherently dangerous and should be avoided."[^8] Microsoft ended the .NET format: "BinaryFormatter is insecure and can't be made secure."[^10] Read data from outside the program only with a data format such as JSON or Protobuf into types the program chooses, or with a filter that accepts only expected classes. See [deserialization of untrusted data](/deserialization-of-untrusted-data/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `pickle`, `json`, `marshal`; dataclasses through `asdict` |
| JavaScript | native | the `JSON` object; `structuredClone` on the web and in Node |
| TypeScript | native | as JavaScript; parsed values need a check |
| Java | stdlib | `Serializable` and object streams; Jackson for JSON |
| Kotlin | library | kotlinx.serialization with a compiler plugin |
| C# | stdlib | `System.Text.Json` with source generation |
| Go | stdlib | `encoding/json` with struct tags, `encoding/gob` |
| Rust | library | serde with derive macros, serde_json |
| C | idiom | fields written byte by byte; libraries |
| C++ | idiom | stream operators with `std::quoted`; libraries |
| Bash | idiom | `declare -p`; jq for JSON |
| PowerShell | stdlib | `ConvertTo-Json`, `Export-Clixml` |
| SQL | native | JSON functions; dumps of rows |
| Regex | n/a | no values to serialize |
| HTML | n/a | a serialization of a document tree |
| CSS | n/a | no values to serialize |

JavaScript is native because the `JSON` object is part of the language, and SQL is native because the standard has JSON functions. Python, Java, C#, Go and PowerShell ship serialization in the standard library. Rust and Kotlin leave it to libraries, serde and kotlinx.serialization, which nearly every program uses and which generate code when the program compiles. C and C++ have no serialization, and programs write the bytes themselves or use a library.

The example files show the main tradeoffs. Python, Java and Go compare a language format, `pickle`, object streams or `gob`, with JSON, and show what each keeps. JavaScript shows what JSON loses and how structured clone keeps it. C# and Go show the choice between ignoring and rejecting unknown fields, and Java shows a filter that limits the classes that deserialization can create. Rust and C write binary formats by hand with a fixed byte order, and C++ writes a text format where quoting keeps strings with spaces intact.

## Variations and related ideas

- **Schema languages.** Protocol Buffers, Thrift, Avro, Cap'n Proto and FlatBuffers define messages in a schema and generate code for each language. Cap'n Proto and FlatBuffers use a layout that a program can read without a separate decoding step.
- **Binary JSON.** CBOR, MessagePack and BSON keep the data model of JSON and write it in binary.
- **Text formats.** [JSON](/json/), XML, YAML, TOML and [CSV](/csv/) are text formats; see [read and write JSON](/read-and-write-json/) for the common task.
- **Deep copy.** Serializing and reading back is a slow way to make a deep copy, and structured clone does the same in one step.
- **Remote calls.** RPC systems such as gRPC serialize the arguments and results of calls across a network.
- **Persistence.** Object databases and ORMs map objects to stored data, a form of serialization with queries.
- **Canonical forms.** Signatures and hashes need one exact byte sequence for each value, so formats such as canonical CBOR and the JSON Canonicalization Scheme fix the order of keys and the form of numbers. The built-in [hash codes](/hash-codes/) are not stable enough for this.

## Pitfalls

- **Deserializing untrusted data with a language format.** Pickle, Java object streams and `BinaryFormatter` can run code chosen by the sender. Use a data format, or a filter of allowed classes.
- **Writing memory as it is.** Writing a C struct or a Rust value as raw bytes copies padding, pointers and the byte order of the machine. Write each field in a defined order and size.
- **Lost types.** A date becomes a string, a set an empty object, a tuple a list, and a 64-bit integer a double in JavaScript. Convert these types on both sides, or choose a format that has them.
- **Shared references and cycles.** Tree formats copy shared objects and fail on cycles. Use IDs in the data, or a graph format.
- **Silent changes to the schema.** Renaming a field, changing its type or reusing a field number breaks old data with no error. Keep old names, add fields with defaults, and test reading data from older versions.
- **Trusting lengths and counts.** A length prefix of four billion should give an error, not an attempt to allocate four billion bytes; the Rust file checks lengths against the remaining input. Limit the size of the input and the depth of nesting.
- **Floating-point text.** Writing a float with too few digits changes its value on the way back. Most JSON libraries write the shortest form that reads back as the same value; others need care.

## Cost

Serialization reads every part of a value, so its time grows with the size of the data. Reflection costs a lookup of the type's fields at run time, usually cached after the first call; generated code avoids it, which is why .NET's source generator and serde are fast. Text formats cost more than binary ones, both to write and to parse, and they are often two to ten times larger before compression. Formats with names in each record, such as JSON, repeat the names; schema formats such as Protobuf write small field numbers. Formats that need no decoding step, such as FlatBuffers, read a field in constant time, at the cost of a larger and more fixed layout.

## History

Network protocols needed formats that every machine could read. Xerox's Courier defined one in the early 1980s, ASN.1 became a standard in 1984, and Sun published XDR, with a fixed byte order and alignment, for its remote procedure calls in 1987. Distributed objects in CORBA and Java RMI needed to send objects, and Java 1.1 added object serialization in 1997.[^8] XML, from 1998, became the text format of web services, and Douglas Crockford described JSON in 2001 as a simpler subset of JavaScript.

Google built Protocol Buffers for its internal services and released it as open source in 2008.[^1] Facebook released Thrift in 2007, and Avro, MessagePack, Cap'n Proto and FlatBuffers followed. Attacks with Java gadget chains in 2015 showed the danger of formats that create any class, and Java added deserialization filters in 2017. Erick Tryzelaar started serde for Rust in 2014, and David Tolnay later became its main author. .NET Core 3.0 added `System.Text.Json` in 2019,[^4] and .NET 9 disabled `BinaryFormatter` in 2024.[^10]

## References

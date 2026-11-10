---
tag: NEW-json
kind: format
slug: json
name: JSON
aliases: [JavaScript Object Notation]
area: data-and-persistence/data-formats
level: core
summary: >-
  JSON is a text format for structured data, based on the object and array literals of JavaScript. A value is an object, an array, a string, a number, true, false or null.
history:
  year: 2001
  people: [Douglas Crockford, Chip Morningstar]
  note: Crockford and Morningstar sent the first JSON message in 2001 at State Software. Crockford put the grammar on the site json.org late in 2002.
same_as:
  wikidata: Q2063
  wikipedia: JSON
sources:
  - url: https://www.rfc-editor.org/rfc/rfc8259
    title: "T. Bray (Ed.), RFC 8259: The JavaScript Object Notation (JSON) Data Interchange Format, December 2017"
    locator: 2 JSON Grammar; 4 Objects; 6 Numbers; 7 Strings; 8.1 Character Encoding; 8.2 Unicode Characters; 9 Parsers
  - url: https://ecma-international.org/publications-and-standards/standards/ecma-404/
    title: ECMA-404, The JSON data interchange syntax, 2nd edition, December 2017
    locator: the whole standard
  - url: https://www.rfc-editor.org/rfc/rfc7493
    title: "T. Bray (Ed.), RFC 7493: The I-JSON Message Format, March 2015"
    locator: 2.1 Encoding and Characters; 2.2 Numbers; 2.3 Object Constraints; 4 Recommendations for Protocol Design
  - url: https://www.rfc-editor.org/rfc/rfc8785
    title: "A. Rundgren, B. Jordan, S. Erdtman, RFC 8785: JSON Canonicalization Scheme (JCS), June 2020"
    locator: 3.1 Creation of Input Data; 3.2 Generation of Canonical JSON Data
  - url: https://www.json.org/json-en.html
    title: Introducing JSON, json.org
    locator: the first paragraph and the grammar diagrams
  - url: https://mamund.site44.com/rwcbook/the-json-saga/
    title: Douglas Crockford, The JSON Saga (transcript of the talk)
    locator: the first message, the quoted keys, the site json.org and the removal of comments
  - url: https://www.rfc-editor.org/rfc/rfc4627
    title: "D. Crockford, RFC 4627: The application/json Media Type for JavaScript Object Notation (JSON), July 2006"
    locator: the header and the abstract
  - url: https://www.rfc-editor.org/rfc/rfc7159
    title: "T. Bray (Ed.), RFC 7159: The JavaScript Object Notation (JSON) Data Interchange Format, March 2014"
    locator: the header (obsoletes RFC 4627)
  - url: https://tc39.es/ecma262/2026/#sec-json-object
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 25.5 The JSON Object; 6.1.6.1.20 Number::toString
  - url: https://github.com/tc39/proposal-json-superset
    title: TC39 proposal, Subsume JSON (JSON superset)
    locator: the README
  - url: https://spec.json5.org/
    title: The JSON5 Data Interchange Format, version 1.0.0, March 2018
    locator: 1 Introduction; 1.2 Summary of Features
  - url: https://jsonlines.org/
    title: JSON Lines
    locator: the three requirements and the file extension
  - url: https://www.rfc-editor.org/rfc/rfc7464
    title: "N. Williams, RFC 7464: JavaScript Object Notation (JSON) Text Sequences, February 2015"
    locator: the abstract; 2.2 JSON Text Sequence Encoding
  - url: https://json-schema.org/draft/2020-12/json-schema-core
    title: JSON Schema, A Media Type for Describing JSON Documents, draft 2020-12
    locator: the whole document
  - url: https://json-schema.org/draft/2020-12/json-schema-validation
    title: JSON Schema Validation, A Vocabulary for Structural Validation of JSON, draft 2020-12
    locator: 6 A Vocabulary for Structural Validation
  - url: https://docs.python.org/3.14/library/json.html
    title: The Python Standard Library, json, Python 3.14
    locator: Basic Usage; Standard Compliance and Interoperability
  - url: https://pkg.go.dev/encoding/json
    title: Go standard library, package encoding/json
    locator: func Unmarshal; func Marshal; Decoder.UseNumber
  - url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/overview
    title: .NET documentation, JSON serialization and deserialization in .NET
    locator: the overview
  - url: https://docs.rs/serde_json/1.0.154/serde_json/
    title: serde_json 1.0.154 documentation
    locator: the crate documentation, Value and from_str
  - url: https://github.com/google/gson
    title: Gson
    locator: the README
  - url: https://json.nlohmann.me/
    title: JSON for Modern C++ (nlohmann/json) documentation
    locator: the home page and the features
  - url: https://www.sqlite.org/json1.html
    title: SQLite, JSON Functions And Operators
    locator: the overview and the list of functions
  - url: https://jqlang.org/manual/
    title: jq manual
    locator: the invocation and the basic filters
  - url: https://kotlinlang.org/docs/serialization.html
    title: Kotlin documentation, Serialization
    locator: the JSON section
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/convertfrom-json
    title: PowerShell documentation, ConvertFrom-Json
    locator: the description and the parameters
---

## What it is

JSON (JavaScript Object Notation) is a text format for structured data. A JSON text is one value. A value is an object, an array, a number, a string, or one of the three literal names `true`, `false` and `null`.[^1] An object is a set of name and value pairs, and an array is an ordered list of values. These two structures map to the record, the dictionary and the list of most programming languages. For this reason, JSON is the usual format of web APIs, configuration files and log records.

Two documents define the format. RFC 8259 of the IETF defines the grammar and adds rules for interoperability, such as the character encoding.[^1] ECMA-404 defines the same grammar and nothing more.[^2] The grammar is very small. It has no comments, no dates, no binary data, no references and no schema. Each of these gaps causes a family of pitfalls, and other documents fill some of them (see Versions and Tools and libraries).

JSON describes syntax, not a data model. The text `1.0` and the text `1` are two different texts, and the grammar does not say if they are the same number. The text `{"a": 1, "a": 2}` matches the grammar, and the grammar does not say which value wins. Each parser makes its own choice, and the choices differ between languages. The examples of this page show these differences.

### History

Douglas Crockford and Chip Morningstar sent the first JSON message in 2001, at their company State Software. They used it for the communication between the browser and the server, and between the servers of their platform.[^6] The format was a subset of the JavaScript object literal, so a browser could read it with the JavaScript compiler. The first message failed because it had the name `do`, a reserved word. In the third edition of ECMAScript, a reserved word must have quotes when it is a name in an object literal. Crockford did not want to put the list of reserved words in the grammar, so he made quotes necessary for all names.[^6] The first grammar had comments. Crockford removed them because people used them to give instructions to the parser, and this broke interoperability.[^6]

Crockford bought the domain json.org late in 2002 and put the grammar on one page.[^6] The page says that JSON is based on a subset of the third edition of ECMA-262.[^5] Crockford says that he discovered JSON and did not invent it, because the notation was already in JavaScript.[^6] RFC 4627 registered the media type `application/json` in July 2006.[^7] RFC 7159 replaced it in March 2014.[^8] RFC 8259 and the second edition of ECMA-404 came out together in December 2017, with the same grammar.[^1][^2]

## Syntax or structure

RFC 8259 gives the grammar in ABNF. The rules below are copied from Sections 2 to 7.[^1]

```abnf
JSON-text = ws value ws

value = false / null / true / object / array / number / string

object = begin-object [ member *( value-separator member ) ] end-object
member = string name-separator value
array = begin-array [ value *( value-separator value ) ] end-array

number = [ minus ] int [ frac ] [ exp ]
int = zero / ( digit1-9 *DIGIT )
frac = decimal-point 1*DIGIT
exp = e [ minus / plus ] 1*DIGIT

string = quotation-mark *char quotation-mark
char = unescaped /
    escape (
        %x22 /          ; "    quotation mark  U+0022
        %x5C /          ; \    reverse solidus U+005C
        %x2F /          ; /    solidus         U+002F
        %x62 /          ; b    backspace       U+0008
        %x66 /          ; f    form feed       U+000C
        %x6E /          ; n    line feed       U+000A
        %x72 /          ; r    carriage return U+000D
        %x74 /          ; t    tab             U+0009
        %x75 4HEXDIG )  ; uXXXX                U+XXXX
unescaped = %x20-21 / %x23-5B / %x5D-10FFFF

ws = *( %x20 / %x09 / %x0A / %x0D )
```

The structural characters are `[`, `]`, `{`, `}`, `:` and `,`. Whitespace is allowed around each of them. Only four characters are whitespace: space, tab, line feed and carriage return.[^1]

The top level can be any value, not only an object or an array. RFC 4627 allowed only an object or an array at the top level, and RFC 7159 removed this limit.[^8]

Names are strings. RFC 8259 says that the names in an object SHOULD be unique. When they are not unique, "the behavior of software that receives such an object is unpredictable".[^1] The order of the members is not part of the meaning of an object, but the order of the values in an array is.

A number is a decimal with an optional sign, fraction and exponent. The grammar has no leading zeros, no leading plus sign, no hexadecimal, no `NaN` and no `Infinity`. The grammar puts no limit on the size or the precision of a number. RFC 8259 lets an implementation set limits. It says that IEEE 754 binary64 (the double type) is widely used. So only integers in the range from -(2^53)+1 to (2^53)-1 are certain to keep their exact value between implementations.[^1]

A string is a sequence of Unicode characters in double quotes. The quotation mark, the reverse solidus and the control characters U+0000 to U+001F must be escaped. Any other character can be in the string as it is, or as a `\uXXXX` escape. A character outside the Basic Multilingual Plane is written as two escapes, a UTF-16 surrogate pair. The grammar also accepts a single escape of a surrogate, such as `"\uDEAD"`. This is not a valid Unicode string, and RFC 8259 says that the behaviour of software on it is unpredictable.[^1]

JSON text that goes between systems that are not part of a closed ecosystem MUST be encoded in UTF-8. A generator MUST NOT add a byte order mark, and a parser MAY ignore one.[^1] A parser MUST accept every text that matches the grammar. It MAY accept extensions, and it MAY set limits on the size of a text, the depth of nesting, the range of numbers and the length of strings.[^1]

## Examples

The text below is one JSON value. It shows each type of the grammar.

```text
{
  "id": 9007199254740993,
  "total": 0.10,
  "tags": ["new", "paid"],
  "customer": {"name": "Hà Nội Café", "vip": true},
  "note": null
}
```

The integer `9007199254740993` is 2^53 + 1. It is a valid JSON number, but a double cannot hold it. A parser that reads every number as a double gets 2^53. Python reads an integer without a fraction as an `int`, so it keeps the value. The option `parse_int=float` shows what a double-based parser does.[^16]

```python run id=big-integer
import json

text = '{"id": 9007199254740993}'
print(json.loads(text)["id"])
print(json.loads(text, parse_int=float)["id"])
print(float(2**53) == float(2**53 + 1))
```

```output for=big-integer
9007199254740993
9007199254740992.0
True
```

A JSON string is a sequence of characters, but escapes use UTF-16 code units. The musical symbol G clef, U+1D11E, is one character and takes two escapes. A lone surrogate escape parses, but the result cannot be encoded as UTF-8.

```python run id=surrogates
import json

clef = json.loads('"\\ud834\\udd1e"')
print(clef, len(clef), len(clef.encode("utf-8")))

lone = json.loads('"\\udead"')
print(len(lone))
try:
    lone.encode("utf-8")
except UnicodeEncodeError as error:
    print(error.reason)
```

```output for=surrogates
𝄞 1 4
1
surrogates not allowed
```

Two texts can hold the same data and differ as bytes. To compare or sign JSON data, a program must write it in one fixed form. The function `canonical` below sorts the names and removes the whitespace. This is close to the scheme of RFC 8785, but it is not the same (see Versions).

```python run id=canonical
import json

first = {"b": [1, 2], "a": "x"}
second = json.loads('{ "a" : "x", "b" : [ 1, 2 ] }')
print(json.dumps(first) == json.dumps(second))


def canonical(value):
    return json.dumps(value, sort_keys=True, separators=(",", ":"), ensure_ascii=False)


print(canonical(first))
print(canonical(first) == canonical(second))
```

```output for=canonical
False
{"a":"x","b":[1,2]}
True
```

The example files show the parse and serialise functions of each language, and the differences on numbers, repeated names, invalid strings and `NaN`.

## Versions

The grammar has not changed in its substance since RFC 4627. The changes between the documents are about the top level, the encoding and the wording.

| Document | Date | Status | Change |
|---|---|---|---|
| json.org | 2002 | web page | the first published grammar[^6] |
| RFC 4627 | July 2006 | Informational | registers `application/json`; the top level is an object or an array[^7] |
| ECMA-404, 1st edition | October 2013 | Ecma standard | the grammar only |
| RFC 7159 | March 2014 | Standards Track | any value at the top level; obsoletes RFC 4627[^8] |
| RFC 8259 | December 2017 | Internet Standard (STD 90) | UTF-8 is required for open exchange; obsoletes RFC 7159[^1] |
| ECMA-404, 2nd edition | December 2017 | Ecma standard | the same grammar as RFC 8259; also ISO/IEC 21778:2017[^2] |

JSON was a subset of JavaScript in its intent, but not in fact. A JSON string can hold the characters U+2028 (line separator) and U+2029 (paragraph separator) as they are, and a JavaScript string literal could not. The TC39 proposal "Subsume JSON" changed ECMAScript in 2019 to allow these two characters in string literals.[^10] The fifth edition of ECMA-262 added the built-in object `JSON` with `parse` and `stringify`, and the language specification still defines them.[^9]

### I-JSON

RFC 7493 defines I-JSON, a profile of JSON for messages on the Internet. An I-JSON text is also a JSON text, with more rules.[^3] It MUST be UTF-8, and its strings MUST NOT hold surrogates or noncharacters. Its numbers SHOULD stay within the range and the precision of binary64. A value that needs more range or more precision, such as a 64-bit identifier, is RECOMMENDED to be a string. The names in an object MUST be unique. The document also recommends an object or an array at the top level, the time format of RFC 3339 for dates, and base64url for binary data.[^3]

### Canonical JSON (RFC 8785)

The JSON Canonicalization Scheme (JCS) of RFC 8785 makes one fixed byte form for each JSON value, so a program can hash or sign it. The input must be I-JSON. The output has no whitespace. The names of each object are sorted, at each level, by their UTF-16 code units. Strings use the shortest escapes, and numbers use the algorithm `Number::toString` of ECMAScript. `NaN`, the infinities and lone surrogates are errors. The output is UTF-8.[^4]

The function `canonical` in Examples differs from JCS on two points. Python sorts names by code point, not by UTF-16 code unit, so the order can differ for names with characters outside the Basic Multilingual Plane. Python also writes numbers in its own form: it writes `1e20` as `1e+20` and `-0.0` as `-0.0`, and ECMAScript writes `100000000000000000000` and `0`.

### JSON5

JSON5 is a superset of JSON, based on the syntax of ECMAScript 5.1. Its version 1.0.0 is from March 2018.[^11] It adds comments, trailing commas, names without quotes, strings in single quotes, strings over more than one line, hexadecimal numbers, `Infinity`, `NaN`, a decimal point at the start or the end of a number, and a plus sign. JSON5 is for files that people write, such as configuration. A JSON parser does not accept JSON5.

### JSON Lines and JSON text sequences

JSON Lines puts one JSON value on each line of a UTF-8 file, with no byte order mark. The line separator is `\n`, and a blank line is not valid. The extension is `.jsonl`.[^12] A program can append a record to the file and read the file one line at a time, without the parse of the whole file. A value in JSON Lines must not hold a raw line feed. A compact JSON writer never puts one there, because a line feed inside a string must be escaped.

RFC 7464 defines JSON text sequences, with the media type `application/json-seq`. Each JSON text starts with the ASCII record separator (0x1E) and ends with a line feed.[^13] The record separator lets a parser find the start of the next text after a broken one.

## Pitfalls

- **Large integers.** A parser that maps every number to a double changes integers above 2^53 with no error. JavaScript, the `any` path of Go, and the default `Map` path of Gson all do this. Send identifiers and money amounts as strings, or use a parser option that keeps the exact text.
- **Decimal fractions.** `0.10` becomes the nearest double. Python with `parse_float=decimal.Decimal`, C# with `decimal` and Go with `json.Number` keep the decimal value.
- **Repeated names.** Most parsers keep the last value with no warning. Some reject the text, and the .NET tree keeps both members. When two systems read the same text with different rules, they can see different data. Reject repeated names at a trust boundary, as I-JSON requires.[^3]
- **`NaN` and the infinities.** They are not JSON. Python writes `NaN` and `Infinity` by default, and its parser accepts them. JavaScript, serde_json and nlohmann/json write `null`. Go, Gson and System.Text.Json raise an error.
- **Dates, bytes and other types.** JSON has no date and no binary type. Each library writes them in its own way, or refuses. Agree on a form, such as RFC 3339 strings for times and base64url for bytes.[^3]
- **Types that do not round-trip.** A JSON object name is always a string, so an integer key comes back as a string. A tuple or a set comes back as an array. A field that is missing in the text gets its default value in Go, Java and Rust structs, with no error.
- **Invalid Unicode.** A lone surrogate escape passes the grammar. Go replaces invalid UTF-8 with U+FFFD, and nlohmann/json throws when it writes it. Validate strings at the boundary.
- **Deep nesting and large texts.** A recursive parser can run out of stack on a text with very deep nesting. Set limits on depth and size for input from outside, as RFC 8259 allows.[^1]
- **JSON inside HTML.** The text `</script>` inside a JSON string ends a script element. Go, Gson and .NET escape `<`, `>` and `&` by default for this reason. Python and JavaScript do not.
- **`eval`.** JSON was once read with the JavaScript `eval` function. This runs any code in the text. Use `JSON.parse`.
- **Comments in configuration files.** JSON has no comments. A file that has them is not JSON, and a strict parser rejects it. JSON5 allows them.

## Tools and libraries

| Language | Verdict | Parse and serialise |
|---|---|---|
| Python | stdlib | `json.loads`, `json.dumps`[^16] |
| JavaScript, TypeScript | native | `JSON.parse`, `JSON.stringify`[^9] |
| Go | stdlib | `json.Unmarshal`, `json.Marshal`[^17] |
| C# | stdlib | `JsonSerializer.Deserialize`, `JsonSerializer.Serialize`[^18] |
| Rust | library | `serde_json::from_str`, `serde_json::to_string`[^19] |
| Java | library | Gson `fromJson`, `toJson`; Jackson `ObjectMapper`[^20] |
| C++ | library | nlohmann/json `json::parse`, `dump`[^21] |
| Kotlin | library | kotlinx.serialization `Json.decodeFromString`, `Json.encodeToString`[^24] |
| PowerShell | stdlib | `ConvertFrom-Json`, `ConvertTo-Json`[^25] |
| SQL | native | JSON functions; in SQLite, `json`, `json_extract` and `->>`[^22] |
| Bash | library | the command-line tool `jq`[^23] |
| C | library | cJSON, Jansson, json-c |

The tool `jq` reads JSON from standard input, applies a filter and writes JSON. For example, `jq .id` prints the member `id` of each input value.[^23] Most languages also have a streaming API that reads tokens one at a time, for texts that are too large for memory.

### JSON Schema

JSON Schema is a vocabulary to describe the shape of JSON data, written in JSON. A schema can say that a value is an object, that it has the member `id`, that `id` is an integer and that `tags` is an array of unique strings. A validator checks a value against a schema and reports each violation. The draft 2020-12 has two documents: Core, which defines the schema structure and references, and Validation, which defines keywords such as `type`, `required`, `minimum`, `pattern` and `enum`.[^14][^15] Code generators, API description formats and editors use the schemas.

## References

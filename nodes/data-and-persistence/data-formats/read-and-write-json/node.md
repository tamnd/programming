---
tag: NEW-read-and-write-json
kind: task
slug: read-and-write-json
name: Read and write JSON
aliases: [parse JSON, serialize JSON, encode and decode JSON, get a value from JSON, write canonical JSON]
area: data-and-persistence/data-formats
level: core
summary: Parse a JSON text with the standard library, follow a path of names and indexes to one value, and write that value back as canonical JSON with no loss of integer digits.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, csharp, go]
sources:
  - url: https://www.rfc-editor.org/rfc/rfc8259
    title: "T. Bray (Ed.), RFC 8259: The JavaScript Object Notation (JSON) Data Interchange Format, December 2017"
    locator: 2 JSON Grammar; 4 Objects; 6 Numbers; 7 Strings; 8.1 Character Encoding; 8.2 Unicode Characters; 9 Parsers
  - url: https://www.rfc-editor.org/rfc/rfc6901
    title: "P. Bryan (Ed.), K. Zyp, M. Nottingham (Ed.), RFC 6901: JavaScript Object Notation (JSON) Pointer, April 2013"
    locator: 4 Evaluation, array-index
  - url: https://www.rfc-editor.org/rfc/rfc8785
    title: "A. Rundgren, B. Jordan, S. Erdtman, RFC 8785: JSON Canonicalization Scheme (JCS), June 2020"
    locator: 3.2.2.2 Serialization of Strings; 3.2.3 Sorting of Object Properties
  - url: https://docs.python.org/3.14/library/json.html
    title: The Python Standard Library, json, Python 3.14
    locator: json.dumps (ensure_ascii, allow_nan, separators, sort_keys); json.loads (parse_constant, object_pairs_hook); JSONDecodeError; Standard Compliance and Interoperability
  - url: https://docs.python.org/3.14/library/stdtypes.html#int-max-str-digits
    title: The Python Standard Library, Built-in Types, Python 3.14
    locator: Integer string conversion length limitation
  - url: https://tc39.es/ecma262/2026/#sec-json-object
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 25.5 The JSON Object (JSON.parse, JSON.stringify); OrdinaryOwnPropertyKeys
  - url: https://github.com/tc39/proposal-json-parse-with-source
    title: TC39 proposal, JSON.parse source text access
    locator: the README, the context argument of the reviver
  - url: https://nodejs.org/api/buffer.html#static-method-buffercomparebuf1-buf2
    title: Node.js documentation, Buffer
    locator: Buffer.compare(buf1, buf2)
  - url: https://pkg.go.dev/encoding/json
    title: Go standard library, package encoding/json
    locator: func Marshal; func Unmarshal; Decoder.Decode; Decoder.UseNumber; Encoder.SetEscapeHTML; SyntaxError
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.json.jsondocument
    title: .NET API reference, JsonDocument
    locator: Parse(String, JsonDocumentOptions), Exceptions
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.json.jsondocumentoptions
    title: .NET API reference, JsonDocumentOptions
    locator: AllowDuplicateProperties; AllowTrailingCommas; CommentHandling; MaxDepth
  - url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/character-encoding
    title: .NET documentation, How to customize character encoding with System.Text.Json
    locator: the default encoder, UnsafeRelaxedJsonEscaping
---

## Task

Parse a JSON text, get one value from it, and write that value back as JSON text. Two texts with the same data give the same output.

Each solution is a function `json_get(text, path)`. The path is a list of steps from the root. In an object, a step is the name of a member. In an array, a step is a decimal index with no sign and no leading zero, as in a JSON Pointer (RFC 6901, section 4). An empty path selects the root.

The canonical output follows four rules:

- There is no white space outside strings.
- The members of each object are sorted by name in code point order.
- Each integer has all its digits, and `-0` becomes `0`.
- A string escapes `"` and `\`. The control characters below U+0020 become `\b`, `\t`, `\n`, `\f`, `\r` or `\u00` and two lowercase hexadecimal digits. Every other character is written as itself in UTF-8.

They are close to the JSON Canonicalization Scheme of RFC 8785, with two differences. RFC 8785 sorts names by UTF-16 code units, and it writes each number as an IEEE 754 double. This task keeps integers exact.

The solution reports two errors. `invalid-json` means the text is not a JSON text as RFC 8259 defines it. `not-found` means the path names no value.

The contract limits the input in three ways. Each number is an integer, because the libraries write fractions and exponents in different forms. No string holds an unpaired surrogate, because RFC 8259 (section 8.2) says that the behaviour of a parser is then unpredictable. No string holds U+2028 or U+2029, because the Go encoder always escapes them (see the pitfalls).

## Solutions

The standard libraries of Java, Rust, C, C++ and Bash have no JSON parser, so this page has no solution in those languages. The usual choices are Jackson or Gson for Java, serde_json for Rust, cJSON or Jansson for C, nlohmann/json for C++, and jq for Bash. The file `matrix.yaml` records these verdicts.

- **Python.** `json.loads` parses the text, and each JSON integer becomes an `int` with all its digits. `json.loads` accepts `NaN`, `Infinity` and `-Infinity` by default. The `parse_constant` hook gets these names, so the solution raises a `ValueError` there. `json.dumps` with `ensure_ascii=False`, `separators=(",", ":")` and `sort_keys=True` writes the canonical form. Python sorts strings by code point. An invalid text raises `json.JSONDecodeError`, a subclass of `ValueError`. A missing value raises `KeyError`.
- **JavaScript and TypeScript (Node.js).** `JSON.parse` makes each number a double. The reviver gets a third argument, `context`, with the source text of each number in `context.source`. The solution makes a `BigInt` from that text. `JSON.stringify` throws a `TypeError` on a `BigInt`, so a short function writes the arrays and objects. It still calls `JSON.stringify` for each string. It sorts the names with `Buffer.compare` on their UTF-8 bytes, because UTF-8 byte order is code point order. An invalid text throws a `SyntaxError`, and a missing value throws a `RangeError`. The TypeScript file adds types only.
- **C#.** `JsonDocument.Parse` keeps each number as its source text, so `GetRawText` gives all the digits. With the default options, it rejects comments and trailing commas, and it throws a `JsonException`. The encoders of `System.Text.Json` escape non-ASCII characters, so the solution writes the output with a `StringBuilder` and escapes each string itself. It sorts the names by their UTF-8 bytes. A missing value throws `KeyNotFoundException`.
- **Go.** `json.NewDecoder` with `UseNumber` keeps each number as a `json.Number`, which is its source text. `Decode` reads one value and stops, so the solution then calls `Token` and requires `io.EOF`. An `Encoder` with `SetEscapeHTML(false)` writes the value. The encoder sorts the keys of a map by their bytes. A small function changes `-0` to `0`. An invalid text gives a `*json.SyntaxError` or `io.ErrUnexpectedEOF`, and a missing value gives `ErrNotFound`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the text, the path, and the canonical output.

- `big-int`, `beyond-int64` and `int64-bounds`: 9007199254740993 is 2^53 + 1, the first integer that a double cannot hold. 18446744073709551617 is above the range of a 64-bit integer. All the digits stay.
- `negative-zero`: `[-0, 0]` gives `[0,0]`.
- `duplicate-name` and `duplicate-name-whole`: `{"a": 1, "a": 2}`. RFC 8259 says that names SHOULD be unique, and that many parsers report only the last pair. Here the last member wins.
- `key-order-ascii`: the names `"10"` and `"9"` sort as strings, so `"10"` comes first. A JavaScript object puts integer-like names first in numeric order, so `Object.keys` gives `"9"` first.
- `key-order-astral`: `"ﬁ"` (U+FB01) sorts before `"😀"` (U+1F600) in code point order. In UTF-16 order the emoji comes first, because its first code unit is 0xD83D.
- `non-ascii` and `escaped-non-ascii`: `"é😀"` in the input gives `"é😀"` in the output.
- `escapes` and `control-chars`: `\/` becomes `/`, U+001F becomes `\u001f`, and U+007F stays as itself, because RFC 8259 requires escapes only below U+0020.
- `html-chars`: `<`, `>`, `&` and `'` stay as they are.
- `empty-containers`, `scalar-root` and `string-root`: `{}`, `[]`, `""` and a root that is not an array or object. RFC 8259 allows any value at the root.
- `whitespace` and `whitespace-inside`: the four white space characters around the tokens.
- `null-member` and `false-member`: a member with the value `null` or `false` is found. A test for truth would miss it.
- `proto-name` and `proto-path`: the names `__proto__` and `constructor` are normal names. `JSON.parse` makes them own properties of the object.
- `array-index-10`, `numeric-name` and `deep-path`: the index 10, an object member named `"0"`, and a path of six steps.

The cases in `cases/errors.jsonl` are failures. The toolchain does not load this file yet, so the test files hold these cases by name:

- `invalid-json`: a trailing comma, single quotes, a name with no quotes, `NaN`, `Infinity`, `-Infinity`, `01`, `+1`, `0x10`, an empty text, data after the value, a comment, a raw tab in a string, `\x41`, and a text with no end. RFC 8259 allows none of these.
- `byte-order-mark`: RFC 8259 (section 8.1) says that a parser MAY ignore a byte order mark. All four libraries reject it in a string, so the contract makes it an error.
- `not-found`: a missing name, the inherited name `toString`, the name `length` on an array, the indexes `2`, `01`, `-1`, `+1` and `١` (ARABIC-INDIC DIGIT ONE) on an array of two, and a step into a number or into `null`.

The contract also limits the size of the input. The Python `int` conversion refuses more than 4300 digits by default, and `json.loads` uses it. `JsonDocument` refuses more than 64 levels of nesting by default.

## Pitfalls

- **A JavaScript number loses digits.** `JSON.parse` makes each number a double, and the last digit changes with no error. The reviver can read the source text:

```javascript wrong id=js-big-int
const text = '{"id": 9007199254740993}';
console.log(JSON.parse(text).id);
const exact = JSON.parse(text, (key, value, context) =>
  typeof value === "number" ? BigInt(context.source) : value,
);
console.log(exact.id);
try {
  JSON.stringify(exact);
} catch (e) {
  console.log(e.name);
}
```

```output
9007199254740992
9007199254740993n
TypeError
```

- **An inherited property looks like a member.** In JavaScript, `value[name]` also finds the properties of `Object.prototype`. Use `Object.hasOwn`:

```javascript run id=js-inherited
const value = JSON.parse('{"a": 1}');
console.log(typeof value["toString"], Object.hasOwn(value, "toString"));
```

```output
function false
```

- **Python reads and writes NaN.** `json.loads` accepts `NaN` and `Infinity`, and `json.dumps` writes them. Both are invalid JSON:

```python wrong id=python-nan
import json

print(json.loads("[NaN, Infinity]"))
print(json.dumps(float("nan")))
try:
    json.dumps(float("nan"), allow_nan=False)
except ValueError as e:
    print("ValueError:", e)
```

```output
[nan, inf]
NaN
ValueError: Out of range float values are not JSON compliant: nan
```

Give `allow_nan=False` to `json.dumps`, and a `parse_constant` that raises an error to `json.loads`.

- **A repeated name is silently dropped.** Each solution keeps the last member. If two parts of a system read the same object, and one keeps the first member, they see different data. Reject repeated names when the text comes from a source that you do not trust. In Python, `object_pairs_hook` gets all the pairs:

```python run id=python-duplicates
import json


def unique(pairs):
    names = [name for name, _ in pairs]
    if len(names) != len(set(names)):
        raise ValueError(f"repeated name in {names}")
    return dict(pairs)


print(json.loads('{"role": "user", "role": "admin"}'))
try:
    json.loads('{"role": "user", "role": "admin"}', object_pairs_hook=unique)
except ValueError as e:
    print("ValueError:", e)
```

```output
{'role': 'admin'}
ValueError: repeated name in ['role', 'role']
```

In C#, `JsonDocument` keeps both members by default. The option `AllowDuplicateProperties = false` makes `JsonDocument.Parse` throw a `JsonException`:

```csharp run id=csharp-duplicates
using System.Text.Json;

string text = """{"role": "user", "role": "admin"}""";
using (JsonDocument document = JsonDocument.Parse(text))
{
    Console.WriteLine(document.RootElement.EnumerateObject().Count());
}
try
{
    JsonDocument.Parse(text, new JsonDocumentOptions { AllowDuplicateProperties = false });
}
catch (JsonException e)
{
    Console.WriteLine(e.Message);
}
```

```output
2
Duplicate property 'role' encountered during deserialization.
```

- **The Go decoder ignores data after the value.** `Decoder.Decode` reads one value and stops. It also makes a number a `float64` unless `UseNumber` is called:

```go wrong id=go-decoder
package main

import (
	"encoding/json"
	"fmt"
	"strings"
)

func main() {
	dec := json.NewDecoder(strings.NewReader("[9007199254740993] x"))
	var value any
	err := dec.Decode(&value)
	fmt.Println(value, err)
	_, err = dec.Token()
	fmt.Println(err)
}
```

```output
[9.007199254740992e+15] <nil>
invalid character 'x' looking for beginning of value
```

`json.Unmarshal` rejects data after the value, but it has no `UseNumber`.

- **Go escapes HTML characters.** `json.Marshal` writes `<`, `>` and `&` as escapes. An `Encoder` with `SetEscapeHTML(false)` writes them as they are, but it still escapes U+2028 and U+2029:

```go run id=go-html
package main

import (
	"encoding/json"
	"fmt"
	"os"
)

func main() {
	s := "<b>&\u2028"
	out, _ := json.Marshal(s)
	fmt.Println(string(out))
	enc := json.NewEncoder(os.Stdout)
	enc.SetEscapeHTML(false)
	enc.Encode(s)
}
```

```output
"\u003cb\u003e\u0026\u2028"
"<b>&\u2028"
```

- **C# escapes all non-ASCII characters by default.** The default encoder of `System.Text.Json` escapes non-ASCII characters and HTML characters. `JavaScriptEncoder.UnsafeRelaxedJsonEscaping` keeps `<`, `&` and `é`, but it still escapes the emoji:

```csharp wrong id=csharp-escaping
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Json;

string s = "é<&>😀";
foreach (JavaScriptEncoder? encoder in new[] { null, JavaScriptEncoder.UnsafeRelaxedJsonEscaping })
{
    using var stream = new MemoryStream();
    using (var writer = new Utf8JsonWriter(stream, new JsonWriterOptions { Encoder = encoder }))
    {
        writer.WriteStringValue(s);
    }
    Console.WriteLine(Encoding.UTF8.GetString(stream.ToArray()));
}
```

```output
"\u00E9\u003C\u0026\u003E\uD83D\uDE00"
"é<&>\uD83D\uDE00"
```

The output is valid JSON in both cases. It is not the canonical form, so the C# solution escapes the strings itself.

## Related tasks

- [[json]]: the format and the libraries of each language.
- [[parse-an-integer]]: convert the digits to an integer type.
- [[integer-overflow]]: a large JSON integer in a fixed-size type.
- [[deserialization-of-untrusted-data]]: parse untrusted JSON into data, not into objects.
- [[utf-8]]: the encoding of a JSON text that goes between systems.
- [[compute-a-sha-256-hash]]: hash the canonical form, which is the same for the same data.
- [[base64-encode-and-decode]]: binary data in a JSON string.
- [[sort-a-list-of-records]]: sort records by a key.
- [[read-a-file-line-by-line]]: read JSON Lines, one JSON text on each line.
- [[read-from-standard-input]]: read the JSON text from a pipe.
- [[write-a-file]]: save the JSON text to a file.

## References

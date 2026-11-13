---
tag: 1TN7KC
kind: task
slug: pretty-print-json
name: Pretty-print JSON
aliases: [format JSON, indent JSON, beautify JSON, JSON pretty printer]
area: data-and-persistence/data-formats
level: intro
summary: >-
  Parse a JSON text and write it back with one member or element on each line, an indent of two spaces for each level, and the names of each object sorted, so the output is the same for the same data.
see_also: [json, read-and-write-json, data-interchange-format]
sources:
- url: https://www.rfc-editor.org/rfc/rfc8259
  title: "T. Bray (Ed.), RFC 8259: The JavaScript Object Notation (JSON) Data Interchange Format, December 2017"
  locator: 2 JSON Grammar; 4 Objects
- url: https://jqlang.org/manual/
  title: jq Manual
  locator: Invoking jq (--indent, --sort-keys); Basic filters, Identity
- url: https://docs.python.org/3/library/json.html
  title: The Python Standard Library, json, JSON encoder and decoder
  locator: json.dump (indent, sort_keys, ensure_ascii); Command-line interface
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
  title: MDN Web Docs, JSON.stringify()
  locator: Parameters (space)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/parse
  title: MDN Web Docs, JSON.parse()
  locator: Parameters (reviver, context.source)
- url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/how-to
  title: .NET documentation, How to write .NET objects as JSON (serialize)
  locator: Serialize to formatted JSON
- url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/character-encoding
  title: .NET documentation, How to customize character encoding with System.Text.Json
  locator: the introduction
- url: https://pkg.go.dev/encoding/json
  title: Go standard library, package encoding/json
  locator: func Marshal; func MarshalIndent; Encoder.SetIndent; Encoder.SetEscapeHTML
---

## Task

Parse a JSON text and write the same value back as a text that a person can read. Each solution is a function `pretty_json(text)` that returns the new text.

JSON allows white space between all tokens: "Insignificant whitespace is allowed before or after any of the six structural characters"[^1]. A pretty printer uses this freedom in one fixed way:

- Each element of an array and each member of an object is on its own line, with two more spaces of indent than its container. The closing `]` or `}` is on its own line.
- An empty array is `[]` and an empty object is `{}`.
- A name is followed by `: `, and a line ends with `,` when another item follows.
- The members of each object are sorted by name in code point order, and the last member of a repeated name wins.
- Integers keep all their digits, `-0` becomes `0`, and strings use the escapes of the page [[read-and-write-json]]. Non-ASCII characters are written as themselves.
- The output has no line break at the end.

With sorted names, two texts with the same data give the same output, so a diff of two files shows only the changes in the data. The output is also a valid input: a second run gives the same text.

The solution reports `invalid-json` when the text is not a JSON text. The contract has the same limits as [[read-and-write-json]]: the numbers are integers, and no string holds an unpaired surrogate, U+2028 or U+2029.

## Solutions

The standard libraries of Java, Rust, C, C++ and Bash have no JSON parser, so this page has no solution in those languages. `matrix.yaml` names the usual libraries. On the command line, `jq .` pretty-prints its input: the jq manual says that the main use of the program `.` "is to validate and pretty-print the inputs"[^2]. Add `--sort-keys` to sort the names, and `--indent n` for another indent[^2].

- **Python.** `json.dumps` does all the work. With `indent=2`, "JSON array elements and object members will be pretty-printed with that indent level"[^3], and with `sort_keys=True` "the output of dictionaries will be sorted by key"[^3]. `ensure_ascii=False` keeps non-ASCII characters, and a `parse_constant` hook rejects `NaN` and `Infinity`. From a shell, `python -m json` does the same job[^3].
- **JavaScript and TypeScript (Node.js).** `JSON.stringify(value, null, 2)` indents, because a number for `space` "indicates the number of space characters to be used as indentation"[^4]. But it keeps the key order of the object and it cannot write a `BigInt`, so the solution has a short `write` function that indents arrays and objects and calls `JSON.stringify` for each string. The reviver reads `context.source`, "The original JSON string representing this value"[^5], to keep the digits of large integers.
- **C#.** `JsonSerializerOptions.WriteIndented` pretty-prints[^6], but "By default, the serializer escapes all non-ASCII characters"[^7]. The solution reads the text with `JsonDocument`, which keeps each number as its source text, and writes the indented text with a `StringBuilder`.
- **Go.** An `Encoder` with `SetEscapeHTML(false)` and `SetIndent` with an indent of two spaces writes the value. A decoder with `UseNumber` keeps each number as its text. The encoder sorts maps: "The map keys are sorted and used as JSON object keys"[^8].

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the text and the pretty output, and each test also checks that a second run gives the same text.

- `key-order-ascii`, `key-order-prefix`, `key-order-astral` and `key-order-nested`: `"10"` sorts before `"9"`, `""` comes first, `"ﬁ"` (U+FB01) sorts before `"😀"` (U+1F600), and the names of nested objects are sorted too.
- `duplicate-name`: `{"a": 1, "b": 0, "a": {"c": 3}}` has one member `"a"` with the value `{"c": 3}`.
- `big-int`, `beyond-int64` and `negative-zero`: 9007199254740993 and ±18446744073709551617 keep all their digits, and `-0` becomes `0`.
- `non-ascii`, `escaped-non-ascii`, `escapes`, `control-chars` and `html-chars`: the escape `\u00e9` becomes `é`, `\/` becomes `/`, U+001F stays escaped, and `<`, `>` and `&` stay as they are.
- `empty-containers`, `empty-object-root` and `empty-array-root`: `{}` and `[]` stay on one line, also when the input is `[ ]`.
- `scalar-root`, `string-root` and `literals`: a number, a string, `true`, `false` and `null` have no indent.
- `compact-input`, `whitespace-inside` and `already-pretty`: the white space of the input has no effect on the output.
- `deep`: `[[[[1]]]]` has five lines of indent.

`cases/errors.jsonl` lists the texts that are not JSON, as in [[read-and-write-json]]. The toolchain does not load this file yet, so each test file holds these cases by name.

## Pitfalls

- **Python escapes non-ASCII and keeps the input order by default.** `ensure_ascii` is true by default, and `sort_keys` is false:

```python run id=python-defaults
import json

value = json.loads('{"name": "Zürich", "id": 2, "area": 88}')
print(json.dumps(value, indent=2))
print(json.dumps(value, indent=2, sort_keys=True, ensure_ascii=False))
```

```output for=python-defaults
{
  "name": "Z\u00fcrich",
  "id": 2,
  "area": 88
}
{
  "area": 88,
  "id": 2,
  "name": "Zürich"
}
```

- **`JSON.stringify` does not keep the order or the digits.** A JavaScript object lists integer-like names first in numeric order, then the other names in insertion order, and a large integer becomes a double:

```javascript wrong id=js-stringify-order
const value = JSON.parse('{"b": 1, "a": 2, "10": 3, "9": 4, "id": 9007199254740993}');
console.log(JSON.stringify(value, null, 2));
```

```output for=js-stringify-order
{
  "9": 4,
  "10": 3,
  "b": 1,
  "a": 2,
  "id": 9007199254740992
}
```

- **Go escapes HTML characters.** `json.MarshalIndent` writes `<`, `>` and `&` as `\u003c`, `\u003e` and `\u0026`. The output is valid JSON but harder to read. Use an `Encoder` with `SetEscapeHTML(false)`:

```go run id=go-escape-html
package main

import (
	"encoding/json"
	"fmt"
	"os"
)

func main() {
	value := map[string]string{"html": "<b>Tom & Jerry</b>"}
	out, _ := json.MarshalIndent(value, "", "  ")
	fmt.Println(string(out))

	enc := json.NewEncoder(os.Stdout)
	enc.SetEscapeHTML(false)
	enc.SetIndent("", "  ")
	enc.Encode(value)
}
```

```output for=go-escape-html
{
  "html": "\u003cb\u003eTom \u0026 Jerry\u003c/b\u003e"
}
{
  "html": "<b>Tom & Jerry</b>"
}
```

- **Pretty-printing changes the bytes.** A hash or a signature of the original text does not match the pretty text. Compute it on the original bytes, or on a canonical form, see [[read-and-write-json]].
- **A tool adds a line break.** The command-line tools jq and `python -m json` end their output with a line break. The solutions on this page do not, because a function returns a value, not a file.

## Related tasks

- [[read-and-write-json]] parses JSON, gets one value and writes it back in a compact canonical form.
- [[stream-parse-a-large-json-file]] reads a JSON text that does not fit in memory.

## References

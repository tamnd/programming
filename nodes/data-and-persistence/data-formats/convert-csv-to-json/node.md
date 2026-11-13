---
tag: 6XH89E
kind: task
slug: convert-csv-to-json
name: Convert CSV to JSON
aliases: [CSV to JSON, turn CSV rows into JSON objects, export a CSV file as JSON]
area: data-and-persistence/data-formats
level: intro
summary: Convert CSV text with a header line to a JSON array with one object for each record, with the members in the order of the header and each value kept as a string.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, csharp, go]
sources:
- url: https://www.rfc-editor.org/rfc/rfc4180
  title: "Y. Shafranovich, RFC 4180: Common Format and MIME Type for Comma-Separated Values (CSV) Files, October 2005"
  locator: 2 Definition of the CSV Format
- url: https://docs.python.org/3/library/csv.html
  title: The Python Standard Library, csv
  locator: csv.reader; Dialect.strict
- url: https://docs.python.org/3/library/json.html
  title: The Python Standard Library, json
  locator: json.dump (ensure_ascii, separators)
- url: https://pkg.go.dev/encoding/csv
  title: Go standard library, package encoding/csv
  locator: Overview; type Reader
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html#sec-ordinaryownpropertykeys
  title: ECMA-262, ECMAScript 2026 Language Specification
  locator: 10.1.11.1 OrdinaryOwnPropertyKeys
- url: https://pkg.go.dev/encoding/json
  title: Go standard library, package encoding/json
  locator: Marshal; HTMLEscape; Encoder.SetEscapeHTML
- url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/character-encoding
  title: .NET documentation, How to customize character encoding with System.Text.Json
  locator: the introduction; Serialize all characters
- url: https://www.rfc-editor.org/rfc/rfc8259
  title: "T. Bray (Ed.), RFC 8259: The JavaScript Object Notation (JSON) Data Interchange Format, December 2017"
  locator: 4 Objects
---

## Task

Convert CSV text to JSON text. The first record of the CSV is the header. The result is a JSON array with one object for each record after the header. Each object has one member for each name of the header, in the order of the header, and the value of the member is the field in the same column.

The input follows RFC 4180: commas separate the fields, and "Fields containing line breaks (CRLF), double quotes, and commas should be enclosed in double-quotes."[^1] A double quote inside a quoted field is written twice.[^1] The solution accepts LF as well as CRLF at the end of a record. A CRLF inside a quoted field becomes LF. An empty line gives no record.

For example, this CSV text:

```text
name,age
Ada,36
"Lovelace, Ada",36
```

gives this JSON text:

```json
[{"name":"Ada","age":"36"},{"name":"Lovelace, Ada","age":"36"}]
```

Each value is a JSON string, also `36`. CSV has no types, so the solution does not guess them. A program that needs a number converts the field when it reads the JSON.

The JSON text has no white space outside the strings. Each string writes `"` and `\` with a backslash before them, a tab as `\t` and an LF as `\n`. Every other character is written as itself, also `<`, `&` and characters outside ASCII. These rules make one exact text for each input, so the cases can compare the text.

The solution reports an error and returns no text in two cases: a record has more or fewer fields than the header (`field-count`), and a quoted field has no closing quote or has text after its closing quote (`bad-quote`).

## Solutions

Each solution reads the CSV into a list of records, checks the number of fields, and writes the JSON array. Python's `json.dumps` keeps the order of a `dict`, so it writes the whole list. The JavaScript, C# and Go solutions write the brackets, the braces, the colons and the commas themselves, because their JSON writers change the order of the members or escape more characters (see the pitfalls).

- **Python.** `csv.reader` reads the text from an `io.StringIO` with `newline=''`. With `strict=True`, it raises `csv.Error` on a bad quote, and "The default is False."[^2] The code checks the number of fields and skips the empty list that the reader gives for an empty line. Each record is a `dict`, which keeps the order of insertion, so `json.dumps` can write the whole list. `ensure_ascii=False` keeps the characters outside ASCII, and `separators=(',', ':')` removes the spaces.[^3]
- **JavaScript and TypeScript (Node.js).** Node.js has no CSV parser, so `parseCsv` is a small state machine for RFC 4180. It is the same as the one in [[read-a-csv-file]]. `JSON.stringify` writes each name and each value as a string, and the code joins them. The error is a `CsvError` with a `kind` field. The TypeScript file adds types only.
- **C#.** `Parse` is the same state machine. A small `WriteString` writes each JSON string, because `Utf8JsonWriter` escapes characters that the contract writes as themselves (see the pitfalls). `CsvException` extends `FormatException` and has a `Kind`.
- **Go.** `csv.NewReader` reads the records. "Blank lines are ignored."[^4] "The Reader converts all \r\n sequences in its input to plain \n, including in multiline field values".[^4] With `FieldsPerRecord` at 0, a record with a different number of fields gives an error that wraps `csv.ErrFieldCount`. A `json.Encoder` with `SetEscapeHTML(false)` writes each string, and the code removes the newline that `Encode` adds. The error is a `*CSVError` with a `Kind`.

Java, Rust, C, C++ and Bash have no JSON writer in their standard library, so this page has no solution in those languages. The file `matrix.yaml` names the libraries to use.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the CSV text and the JSON text that the solution must return.

- `simple`, `quoted-comma`, `header-only` and `no-final-newline`: the common shapes. A header with no records gives `[]`.
- `escaped-quote`: the CSV field `"She said ""hi""."` becomes the JSON string `"She said \"hi\"."`.
- `newline-in-field` and `crlf`: a line break inside a quoted field becomes `\n` in the JSON, also when the CSV has CRLF.
- `backslash-and-tab`: `C:\temp` becomes `"C:\\temp"`, and a tab becomes `\t`.
- `numeric-text`: `007`, `02134`, `true` and `1e3` stay strings. A number would lose the leading zeros, and `true` would become a Boolean.
- `numeric-names`: the names `2024`, `2023` and `name` keep the order of the header.
- `html-chars`: `a<b && c>d` is written as it is, with no `\u003c`.
- `empty-fields`, `spaces` and `blank-lines`: an empty field becomes `""`, spaces stay, and empty lines give no record.
- `empty-text`: no text gives `[]`.
- `non-ascii`: `Zürich`, `東京` and `🙂` are written as themselves.

The cases in `cases/errors.jsonl` must give an error: a record with too few fields, a record with too many, a quote with no end and text after a closing quote. The toolchain does not load this file yet, so each test file reads it itself.

## Pitfalls

- **`JSON.stringify` puts names that look like numbers first.** `JSON.stringify` writes the members in the order of the own keys of the object. For an ordinary object, `OrdinaryOwnPropertyKeys` takes each key that "is an array index, in ascending numeric index order", and only then the other strings "in ascending chronological order of property creation".[^5] So a header with years changes order:

```javascript wrong id=js-key-order
const header = ["2024", "2023", "name"];
const row = ["10", "9", "x"];
console.log(JSON.stringify(Object.fromEntries(header.map((name, k) => [name, row[k]]))));
```

```output for=js-key-order
{"2023":"9","2024":"10","name":"x"}
```

The JavaScript solution writes each member itself, in the order of the header.

- **Go sorts the names of a map and escapes HTML characters.** For a map, "The map keys are sorted and used as JSON object keys".[^6] `json.Marshal` also writes `<`, `>` and `&` as `\u003c`, `\u003e` and `\u0026`, "so that the JSON will be safe to embed inside HTML <script> tags."[^6]

```go wrong id=go-marshal-map
package main

import (
	"encoding/json"
	"fmt"
)

func main() {
	record := map[string]string{"name": "Ada", "age": "36", "expr": "a<b && c>d"}
	b, _ := json.Marshal(record)
	fmt.Println(string(b))
}
```

```output for=go-marshal-map
{"age":"36","expr":"a\u003cb \u0026\u0026 c\u003ed","name":"Ada"}
```

The text is valid JSON with the same values, but the order of the header is lost. "This replacement can be disabled when using an Encoder, by calling Encoder.SetEscapeHTML (false)."[^6] The Go solution does that, and it writes the members in order.

- **Python escapes the characters outside ASCII by default.** With `ensure_ascii=True`, "the output is guaranteed to have all incoming non-ASCII and non-printable characters escaped."[^3] The default separators also add a space after each comma and colon:[^3]

```python wrong id=python-ensure-ascii
import json

records = [{"city": "Zürich", "word": "東京"}]
print(json.dumps(records))
print(json.dumps(records, ensure_ascii=False, separators=(",", ":")))
```

```output for=python-ensure-ascii
[{"city": "Z\u00fcrich", "word": "\u6771\u4eac"}]
[{"city":"Zürich","word":"東京"}]
```

Both lines are valid JSON. The first one is longer, and a person cannot read the escaped text.

- **`System.Text.Json` escapes many characters.** "By default, the serializer escapes all non-ASCII characters."[^7] `JavaScriptEncoder.UnsafeRelaxedJsonEscaping` "doesn't escape HTML-sensitive characters such as <, >, &, and '."[^7] But it still escapes a character outside the BMP, such as an emoji, as two `\u` escapes:

```csharp wrong id=csharp-encoder
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Json;

foreach (JavaScriptEncoder? encoder in new[] { null, JavaScriptEncoder.UnsafeRelaxedJsonEscaping })
{
    using var buffer = new MemoryStream();
    using (var writer = new Utf8JsonWriter(buffer, new JsonWriterOptions { Encoder = encoder }))
    {
        writer.WriteStartObject();
        writer.WriteString("word", "a<b Zürich 🙂");
        writer.WriteEndObject();
    }
    Console.WriteLine(Encoding.UTF8.GetString(buffer.ToArray()));
}
```

```output for=csharp-encoder
{"word":"a\u003Cb Z\u00FCrich \uD83D\uDE42"}
{"word":"a<b Zürich \uD83D\uDE42"}
```

A JSON reader gets the same string back from each line. If your program only needs valid JSON, the relaxed encoder is enough. The contract asks for one exact text, so the C# solution writes the strings itself.

- **Guessing types changes the data.** A converter that turns each field that looks like a number into a JSON number breaks codes such as postal codes and IDs:

```python wrong id=python-guess-types
import json

record = {"id": "007", "zip": "02134", "price": "1e3"}
guessed = {k: float(v) if k == "price" else int(v) for k, v in record.items()}
print(json.dumps(guessed))
```

```output for=python-guess-types
{"id": 7, "zip": 2134, "price": 1000.0}
```

Keep each field as a string, or convert only the columns that you know, with [[parse-an-integer]] and a check of the result.

- **Two columns with the same name give a bad object.** RFC 8259 says: "The names within an object SHOULD be unique."[^8] For an object with a name twice, "Many implementations report the last name/value pair only."[^8] A Python `dict` or a JavaScript object keeps only the last value too, so a column is lost with no error. The contract says that the names of the header are distinct. If your input can break this rule, check the header before the conversion.

- **A CSV reader that is not strict accepts bad quotes.** Python's `csv.reader` reads `"x"y` as the field `xy` unless `strict=True`, as [[read-a-csv-file]] shows.[^2] The converter then writes JSON from a broken file and reports nothing.

## Related tasks

- [[read-a-csv-file]]: read a CSV file into records.
- [[write-a-csv-file]]: write records to a CSV file.
- [[read-and-write-json]]: read a JSON text and write a value back.
- [[csv]] and [[json]]: the two formats.
- [[parse-an-integer]]: convert a field to a number.

## References

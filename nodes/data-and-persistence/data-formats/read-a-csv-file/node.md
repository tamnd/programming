---
tag: PY75PY
kind: task
slug: read-a-csv-file
name: Read a CSV file
aliases: [parse a CSV file, read comma-separated values, load a CSV file into records, read a CSV file with a header]
area: data-and-persistence/data-formats
level: core
summary: Read a CSV file with a header line into one record for each line, with quoted fields that hold commas, double quotes and line breaks, as RFC 4180 defines them.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
sources:
  - url: https://www.rfc-editor.org/rfc/rfc4180
    title: "Y. Shafranovich, RFC 4180: Common Format and MIME Type for Comma-Separated Values (CSV) Files, October 2005"
    locator: 2 Definition of the CSV Format, rules 1 to 7 and the ABNF grammar
  - url: https://docs.python.org/3/library/csv.html
    title: The Python Standard Library, csv
    locator: csv.reader; csv.field_size_limit; csv.DictReader (restkey, restval); Dialect.strict; csv.Error; footnote 1
  - url: https://docs.python.org/3/library/codecs.html#encodings-and-unicode
    title: The Python Standard Library, codecs
    locator: Encodings and Unicode, utf-8-sig
  - url: https://pkg.go.dev/encoding/csv
    title: Go standard library, package encoding/csv
    locator: Overview; Variables (ErrQuote, ErrFieldCount); type Reader (FieldsPerRecord, LazyQuotes)
  - url: https://tc39.es/ecma262/2026/#sec-object.fromentries
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 20.1.2.7 Object.fromEntries
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html#readString(java.nio.file.Path)
    title: Java SE 25 API, java.nio.file.Files
    locator: readString(Path)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.readalltext?view=net-10.0
    title: .NET API reference, File.ReadAllText
    locator: ReadAllText(String, Encoding), Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualbasic.fileio.textfieldparser.readfields?view=net-10.0
    title: .NET API reference, TextFieldParser.ReadFields
    locator: Remarks
  - url: https://doc.rust-lang.org/std/fs/fn.read_to_string.html
    title: The Rust Standard Library, std::fs::read_to_string
    locator: Errors
  - url: https://en.cppreference.com/w/c/io/fopen.html
    title: cppreference.com, fopen
    locator: the file access mode flag "b"
---

## Task

Read a CSV file with a header line. Return one record for each line after the header. A record maps each name of the header to the field in the same column.

The format is the one that RFC 4180 (section 2) defines:

- Commas separate the fields. A line break ends a record. The last record can have a line break or not.
- A field that holds a comma, a double quote or a line break is enclosed in double quotes. A double quote inside such a field is written twice, so `"She said ""hi""."` is the field `She said "hi".`.
- Spaces are part of a field. The solution does not trim them.
- Each record has the same number of fields as the header.

RFC 4180 uses CRLF as the line break. Many files use LF, so the solution accepts both. A line break inside a quoted field is returned as LF, so the result is the same for both kinds of file. The solution reads the file as UTF-8 and removes a byte order mark at the start, because some programs write one.

The solution skips an empty line. So an empty file, a file with only a header, and a file with an empty last line give no extra records. A file with one column must write an empty field as `""`, because an empty line gives no record.

The solution reports an error and returns no records in three cases: a record has more or fewer fields than the header (`field-count`), a quoted field has no closing quote or has text after its closing quote (`bad-quote`), and the file does not exist (`not-found`).

## Solutions

The standard libraries of Python and Go have a CSV reader. JavaScript, TypeScript, Java, C#, Rust, C and C++ have none, so their solutions hold a small parser of RFC 4180, with a test for each case. The parser is the same state machine in each language:

1. At the start of a record, an empty line is skipped.
2. A field that starts with `"` is quoted. A pair `""` gives one `"`, a CRLF gives LF, and any other character is part of the field. A single `"` ends the field. After it, the next character must be a comma, a line break or the end of the text, or the parser reports `bad-quote`. The end of the text inside the quotes is also `bad-quote`.
3. Any other field runs up to the next comma, line break or the end of the text.
4. A comma starts the next field of the same record. Anything else ends the record.

The parser works on whole text in memory. The contract limits each field to 100000 characters.

- **Python.** `csv.reader` reads a file that is opened with `encoding="utf-8-sig"` and `newline=""`. The `utf-8-sig` codec skips a byte order mark. Without `newline=""`, the csv module cannot read a line break inside a quoted field correctly. `strict=True` makes the reader raise `csv.Error` on a bad quote. The default is `False`. The reader gives an empty list for an empty line, and the code skips it. The code checks the number of fields itself and raises `csv.Error`. It changes a CRLF in a field to LF.
- **JavaScript and TypeScript (Node.js).** `readFile` with `"utf8"` reads the text, and the code removes a leading U+FEFF. `parseCsv` is the state machine. `Object.fromEntries` makes each record, and it creates each name as an own property of the object. So a header name such as `__proto__` is a normal key. The error is a `CsvError` with a `kind` field. The TypeScript file adds types only.
- **Java.** `Files.readString` decodes the file as UTF-8. The code removes a leading U+FEFF and runs the state machine. Each record is a `LinkedHashMap`, so the names keep the order of the header. `CsvException` extends `IOException` and has a `kind()`.
- **C#.** `File.ReadAllText` detects a byte order mark and does not return it. The code runs the state machine and makes a `Dictionary<string, string>` for each record. `CsvException` extends `FormatException`. The class `TextFieldParser` in `Microsoft.VisualBasic.FileIO` reads CSV, but it changes a field that holds an empty line (see the pitfalls).
- **Go.** `csv.NewReader` from `encoding/csv` reads the records. It removes a carriage return before a newline, also inside a quoted field, and it ignores blank lines. With `FieldsPerRecord` at 0, the reader takes the number of fields from the first record, and a record with another number gives an error that wraps `csv.ErrFieldCount`. With `LazyQuotes` false, a bad quote gives an error that wraps `csv.ErrQuote`. The code removes a byte order mark with `bufio.Reader.Peek` before the CSV reader sees it.
- **Rust.** `fs::read_to_string` returns an error if the file is not UTF-8. The code strips a leading U+FEFF and runs the state machine on the bytes. Each delimiter is an ASCII byte, so each field is still valid UTF-8. `CsvError` is an enum with `Io`, `FieldCount` and `BadQuote`.
- **C.** `fopen` with mode `"rb"` and `fread` read the whole file. The flag `b` has no effect on POSIX, and on Windows it stops the change of line breaks. The parser makes a `struct csv_table` with the header and an array of fields for each record, in the order of the header. `csv_field` finds a field by name. `read_csv` returns an `enum csv_status`, and `csv_table_free` frees the table.
- **C++.** An `std::ifstream` in binary mode and `std::istreambuf_iterator` read the file into a `std::string`. The parser works on a `std::string_view`. Each record is a `std::map<std::string, std::string>`. A bad file throws `csv_error`, and a file that cannot be opened throws `std::ios_base::failure`.

Bash has no CSV parser, and the word splitting of the shell cannot read a quoted field. So this page has no Bash solution. The file `matrix.yaml` records this.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case reads one file from `fixtures/`. The file `.gitattributes` marks the fixtures as `-text`, so Git does not change their line breaks.

- `crlf` and `no-final-newline`: the same records as `simple`, with CRLF line breaks, and with no line break at the end.
- `quoted-comma`, `escaped-quote` and `quoted-header`: `"Lovelace, Ada"` is one field. `""` inside quotes is one `"`. A header name can be quoted too.
- `newline-in-field`, `crlf-in-field` and `blank-line-in-field`: a quoted field holds a line break, a CRLF or an empty line. A reader that reads one line at a time splits the record. The result has LF in each case.
- `empty-last-line` and `blank-lines`: empty lines between and after the records give no record.
- `empty-fields` and `quoted-empty`: `,,` gives three empty fields. `""` gives one empty field, and that is the only way to write an empty field in a file with one column.
- `spaces`: ` x , y ` keeps its spaces.
- `header-only` and `empty-file`: no records.
- `bom`: the first name is `name`, with no U+FEFF before it.
- `non-ascii`: `Zürich` and `東京` in UTF-8.

The cases in `cases/errors.jsonl` must give an error. The toolchain does not load this file yet, so the test files hold these cases by name: a record with too few fields, a record with too many fields, a quote with no end, text after a closing quote, and a missing file.

## Pitfalls

- **Splitting on commas breaks a quoted field.** A split on `,` or on `\n` does not know about quotes. It is correct only for files that have no quotes. Use a CSV reader or a parser of the format:

```javascript wrong id=js-split
const line = '"Lovelace, Ada",1815';
console.log(line.split(","));
```

```output for=js-split
[ '"Lovelace', ' Ada"', '1815' ]
```

- **Python accepts bad quotes by default.** With the default `strict=False`, `csv.reader` reads text after a closing quote into the field, and a quote with no end takes the rest of the file into one field. Give `strict=True`:

```python wrong id=python-strict
import csv
import io

for text in ['a,"b"c\n', 'a,"b\nc,d\n']:
    print(list(csv.reader(io.StringIO(text, newline=""))))
    try:
        list(csv.reader(io.StringIO(text, newline=""), strict=True))
    except csv.Error as e:
        print("csv.Error:", e)
```

```output for=python-strict
[['a', 'bc']]
csv.Error: ',' expected after '"'
[['a', 'b\nc,d\n']]
csv.Error: unexpected end of data
```

- **`csv.DictReader` does not check the number of fields.** A missing field becomes `restval`, which is `None` by default. Extra fields go into a list under the key `restkey`, which is also `None` by default. Check the length of each row, as the Python solution does:

```python wrong id=python-dictreader
import csv
import io

text = "name,age\nAda\nAlan,41,London\n"
for record in csv.DictReader(io.StringIO(text, newline="")):
    print(record)
```

```output for=python-dictreader
{'name': 'Ada', 'age': None}
{'name': 'Alan', 'age': '41', None: ['London']}
```

- **Python limits the size of a field.** `csv.field_size_limit()` returns the limit, and a longer field raises `csv.Error`. A file with a large text field fails with the default:

```python wrong id=python-field-size
import csv
import io

print(csv.field_size_limit())
text = "note\n" + "x" * 200_000 + "\n"
try:
    list(csv.reader(io.StringIO(text, newline="")))
except csv.Error as e:
    print("csv.Error:", e)
```

```output for=python-field-size
131072
csv.Error: field larger than field limit (131072)
```

- **The byte order mark stays in the first name.** Node.js `readFile` with `"utf8"` returns U+FEFF as the first character. The first name of the header is then U+FEFF and `name`, so a lookup of `name` finds no field:

```javascript wrong id=js-bom
import { readFile, writeFile } from "node:fs/promises";

await writeFile("people.csv", "\uFEFFname,age\nAda,36\n");
const text = await readFile("people.csv", "utf8");
console.log(text.codePointAt(0).toString(16), text.startsWith("name"));
```

```output for=js-bom
feff false
```

- **`TextFieldParser` drops an empty line inside a quoted field.** In .NET, `TextFieldParser.ReadFields` skips blank lines. It also removes an empty line that is inside quotes, so the field changes:

```csharp wrong id=csharp-textfieldparser
using Microsoft.VisualBasic.FileIO;

File.WriteAllText("notes.csv", "id,note\n1,\"one\n\ntwo\"\n");
using var parser = new TextFieldParser("notes.csv");
parser.SetDelimiters(",");
while (!parser.EndOfData)
{
    string[] fields = parser.ReadFields()!;
    Console.WriteLine(string.Join(" | ", fields.Select(f => f.Replace("\n", "\\n"))));
}
```

```output for=csharp-textfieldparser
id | note
1 | one\ntwo
```

## Related tasks

- [[csv]]: the format, its history, its dialects and how to write it.
- [[split-a-string]]: split a line that has no quotes.
- [[read-a-file-line-by-line]]: read a text file one line at a time, which is not correct for CSV with quoted line breaks.
- [[read-and-write-json]]: read a data format that has types and nesting.
- [[parse-an-integer]]: change a field such as `age` from text to a number.
- [[sort-a-list-of-records]]: sort the records after you read them.

## References

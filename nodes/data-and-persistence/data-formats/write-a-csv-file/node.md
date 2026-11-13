---
tag: QY188M
kind: task
slug: write-a-csv-file
name: Write a CSV file
aliases: [write comma-separated values, export records to CSV, save records as a CSV file, write a CSV file with a header]
area: data-and-persistence/data-formats
level: core
summary: Write a header and records to a CSV file as RFC 4180 defines it, with quotes around each field that holds a comma, a double quote or a line break, and CRLF after each record.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
sources:
- url: https://www.rfc-editor.org/rfc/rfc4180
  title: "Y. Shafranovich, RFC 4180: Common Format and MIME Type for Comma-Separated Values (CSV) Files, October 2005"
  locator: 2 Definition of the CSV Format, rules 1 to 7
- url: https://docs.python.org/3/library/csv.html
  title: The Python Standard Library, csv
  locator: csv.writer; Dialect.lineterminator; csv.QUOTE_MINIMAL; footnote 1
- url: https://nodejs.org/api/fs.html#fswritefilesyncfile-data-options
  title: Node.js documentation, File system
  locator: fs.writeFileSync(file, data[, options])
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/Files.html#writeString(java.nio.file.Path,java.lang.CharSequence,java.nio.file.OpenOption...)
  title: Java SE 25 API, java.nio.file.Files
  locator: writeString(Path, CharSequence, OpenOption...)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.file.writealltext?view=net-10.0
  title: .NET API reference, File.WriteAllText
  locator: WriteAllText(String, String), Remarks
- url: https://doc.rust-lang.org/std/fs/fn.write.html
  title: The Rust Standard Library, std::fs::write
  locator: description
- url: https://en.cppreference.com/w/c/io/fopen.html
  title: cppreference.com, fopen
  locator: the file access mode flag "b"
- url: https://pkg.go.dev/encoding/csv
  title: Go standard library, package encoding/csv
  locator: type Writer (UseCRLF, Flush, Error)
- url: https://docs.python.org/3/library/codecs.html#encodings-and-unicode
  title: The Python Standard Library, codecs
  locator: Encodings and Unicode, utf-8-sig
- url: https://owasp.org/www-community/attacks/CSV_Injection
  title: OWASP Foundation, CSV Injection
  locator: Description; Mitigations
---

## Task

Write a list of records to a CSV file. The first record is the header. Each record is a list of fields, and each field is a string.

The format is the one that RFC 4180 (section 2) defines:

- "Each record is located on a separate line, delimited by a line break (CRLF)."[^1] Commas separate the fields.
- "Fields containing line breaks (CRLF), double quotes, and commas should be enclosed in double-quotes."[^1] "If double-quotes are used to enclose fields, then a double-quote appearing inside a field must be escaped by preceding it with another double quote."[^1]
For example, the field `She said "hi".` is written as `"She said ""hi""."`.

- "Spaces are considered part of a field and should not be ignored."[^1] The solution writes them as they are.
- Each record "should contain the same number of fields throughout the file."[^1]

RFC 4180 says that "The last record in the file may or may not have an ending line break."[^1] The solution writes CRLF after each record, also after the last one. Then a program can add more records to the end of the file.

The solution quotes a field that holds a comma, a double quote, a CR or an LF. It does not quote other fields. A record with one empty field is a special case: without quotes, its line is empty, and a reader such as the one of [[read-a-csv-file]] skips an empty line. So the solution writes that field as `""`.

The solution writes the file as UTF-8 with no byte order mark. No records give an empty file. A record with more or fewer fields than the header is an error (`field-count`), and the solution writes no file.

The entry point `format_csv` returns the text of the file, and the cases check that text. A second function writes the text to a path.

## Solutions

The standard libraries of Python and Go have a CSV writer. The Python solution uses it. The Go solution does not, because the Go writer changes some fields (see the pitfalls). JavaScript, TypeScript, Java, C#, Rust, C and C++ have no CSV writer, so their solutions hold a small writer. The writer is the same in each language:

1. Check that each record has the same number of fields as the header. If one does not, report `field-count` before you write a byte.
2. For each field, if it holds `,`, `"`, CR or LF, or if it is the only field of its record and it is empty, write `"`, then the field with each `"` written twice, then `"`. Otherwise write the field as it is.
3. Write `,` between the fields and CRLF after each record.

- **Python.** `csv.writer` with the default dialect `excel` writes the records. The `lineterminator` of the writer "defaults to '\r\n'".[^2] The default quoting is `QUOTE_MINIMAL`, which "Instructs writer objects to only quote those fields which contain special characters such as delimiter, quotechar, '\r', '\n' or any of the characters in lineterminator."[^2] The file is opened with `encoding='utf-8'` and `newline=''`. The code checks the number of fields itself, because the writer does not, and raises `CsvError`, a subclass of `ValueError`. `format_csv` writes to an `io.StringIO` with `newline=''`.
- **JavaScript and TypeScript (Node.js).** `formatCsv` is the small writer. `writeFileSync` writes the text, and the default of its `encoding` option is `'utf8'`.[^3] The error is a `CsvError` with a `kind` field. The TypeScript file adds types only.
- **Java.** `formatCsv` is the small writer, and `Files.writeString` writes the text. "Characters are encoded into bytes using the UTF-8 charset."[^4] `CsvException` extends `IllegalArgumentException` and has a `kind()`.
- **C#.** `WriteCsv.Format` is the small writer, and `File.WriteAllText` writes the text. "This method uses UTF-8 encoding without a Byte-Order Mark (BOM)".[^5] `CsvException` extends `FormatException`.
- **Go.** `FormatCSV` is the small writer, with a `strings.Builder`. `os.WriteFile` writes the bytes. A wrong number of fields gives a `*FieldCountError` with the record number and the two counts.
- **Rust.** `format_csv` is the small writer. It takes `&[Vec<S>]` with `S: AsRef<str>`, so it accepts both `String` and `&str` fields. `fs::write` writes the text. "This function will create a file if it does not exist, and will entirely replace its contents if it does."[^6] `CsvError` is an enum with `Io` and `FieldCount`.
- **C.** `format_csv` measures the text in a first pass, allocates it once, and fills it in a second pass. `write_csv` opens the file in the mode `wb`. The flag `b` "has no effect on POSIX systems, but on Windows it disables special handling of '\n' and '\x1A'."[^7] Without it, a Windows program writes each LF as CRLF, so CRLF becomes CR CR LF. Both functions return an `enum csv_status`.
- **C++.** `format_csv` returns a `std::string`. `write_csv` writes it with an `std::ofstream` in binary mode, for the same reason as in C. A wrong number of fields throws `csv_error`, and a file that cannot be written throws `std::ios_base::failure`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the records and the text that `format_csv` must return.

- `simple` and `header-only`: plain fields, and a file with only a header.
- `comma` and `double-quote`: `Lovelace, Ada` is written as `"Lovelace, Ada"`. A double quote in a field is written twice.
- `only-a-quote`: the field `"` is written as `""""`.
- `newline-in-field`, `crlf-in-field` and `cr-in-field`: a field with an LF, a CRLF or a lone CR is quoted, and its line breaks are written as they are.
- `empty-fields` and `one-empty-field`: in a record with more than one field, an empty field is written as nothing, so `a,,b` has an empty field in the middle. A record with one empty field is written as `""`.
- `spaces`: ` x ` keeps its spaces and has no quotes.
- `no-records`: the text is empty.
- `non-ascii`: `Zürich` and `東京` are written as they are, and the file holds them as UTF-8.

The cases in `cases/errors.jsonl` must give the error `field-count`: a record with too many fields and a record with too few. The toolchain does not load this file yet, so the test files hold these cases by name. Each test file also writes a file to a temporary directory and checks its bytes.

## Pitfalls

- **Joining fields with commas breaks a field that holds a comma.** The join does not add quotes, so a reader finds one field more than you wrote:

```python wrong id=python-join
import csv
import io

records = [["name", "born"], ["Lovelace, Ada", "1815"]]
text = "".join(",".join(record) + "\r\n" for record in records)
print(list(csv.reader(io.StringIO(text, newline=""))))
```

```output for=python-join
[['name', 'born'], ['Lovelace', ' Ada', '1815']]
```

A field with a double quote or a line break breaks the same way. Quote the fields as the solutions do, or use a CSV writer.

- **Go's `csv.Writer` changes some fields.** "As returned by NewWriter, a Writer writes records terminated by a newline".[^8] "If Writer.UseCRLF is true, the Writer ends each output line with \r\n instead of \n."[^8] With `UseCRLF`, the writer also changes an LF inside a quoted field to CRLF, and it drops a lone CR. It writes a record with one empty field as an empty line, and it quotes a field that starts with a space:

```go wrong id=go-csv-writer
package main

import (
	"encoding/csv"
	"fmt"
	"strings"
)

func main() {
	var b strings.Builder
	w := csv.NewWriter(&b)
	w.UseCRLF = true
	w.WriteAll([][]string{{"note"}, {""}, {"a\rb"}, {"c\nd"}, {" x"}})
	fmt.Printf("%q\n", b.String())
}
```

```output for=go-csv-writer
"note\r\n\r\n\"ab\"\r\n\"c\r\nd\"\r\n\" x\"\r\n"
```

A reader then reads `ab` and `c\r\nd`, and "Blank lines are ignored."[^8] So one record is lost. If you use `csv.Writer`, remember that "The writes of individual records are buffered."[^8] Call `Flush` and then check `Error`.

- **Python needs `newline=''` on the file.** The csv module writes its own line breaks. "If newline='' is not specified, newlines embedded inside quoted fields will not be interpreted correctly, and on platforms that use \r\n line endings on write an extra \r will be added."[^2] On Windows, a file opened with the default `newline` changes each `\n` to `\r\n`, so each CRLF of the writer becomes CR CR LF.

- **`utf-8-sig` writes a byte order mark.** "On encoding the utf-8-sig codec will write 0xef, 0xbb, 0xbf as the first three bytes to the file."[^9] A reader that does not remove them finds U+FEFF in the first name of the header, as [[read-a-csv-file]] shows. The solutions write no byte order mark. Add one only when the program that reads the file needs it.

- **A field can be a formula in a spreadsheet.** "When a spreadsheet program such as Microsoft Excel or LibreOffice Calc is used to open a CSV, any cells starting with = will be interpreted by the software as a formula."[^10] OWASP lists `=`, `+`, `-`, `@`, tab, CR and LF as dangerous first characters.[^10] Quotes do not stop this. If the fields come from users and the file goes to a spreadsheet, escape these fields as [[csv]] describes. The solutions do not, because the escape changes the data for a program that reads the file.

- **A record with a different number of fields gives a bad file.** A reader that checks the count rejects it. For Go's `csv.Reader`, "If FieldsPerRecord is 0, Read sets it to the number of fields in the first record, so that future records must have the same field count."[^8] Check the count before you write, as the solutions do. Then an error leaves no half-written file.

## Related tasks

- [[read-a-csv-file]]: read the file back into records.
- [[csv]]: the format, its history, its dialects and its security risks.
- [[write-a-file]]: write text to a file.
- [[read-and-write-json]]: write a data format that has types and nesting.
- [[sort-a-list-of-records]]: sort the records before you write them.

## References

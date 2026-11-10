---
tag: NEW-csv
kind: format
slug: csv
name: CSV
aliases: [Comma-Separated Values]
area: data-and-persistence/data-formats
level: core
summary: >-
  CSV is a text format for a table. Each line is a record, commas separate the fields, and double quotes enclose a field that holds a comma, a quote or a line break.
history:
  year: 2005
  people: [Yakov Shafranovich]
  note: Spreadsheets and databases used CSV for many years with no formal description. RFC 4180 of October 2005 documented the common format and registered the media type text/csv.
same_as:
  wikidata: Q935809
  wikipedia: Comma-separated values
sources:
  - url: https://www.rfc-editor.org/rfc/rfc4180
    title: "Y. Shafranovich, RFC 4180: Common Format and MIME Type for Comma-Separated Values (CSV) Files, October 2005"
    locator: the header; 1 Introduction; 2 Definition of the CSV Format; 3 MIME Type Registration of text/csv; 4 IANA Considerations
  - url: https://www.iana.org/assignments/media-types/text/csv
    title: IANA, Media type registration of text/csv
    locator: the whole template (registered by RFC 4180, last updated 2014-01-17)
  - url: https://www.rfc-editor.org/rfc/rfc7111
    title: "M. Hausenblas, E. Wilde, J. Tennison, RFC 7111: URI Fragment Identifiers for the text/csv Media Type, January 2014"
    locator: the header; 2 Fragment Identification Methods; 3 Fragment Identification Syntax; 5.1 The text/csv media type
  - url: https://www.w3.org/TR/2015/REC-tabular-data-model-20151217/
    title: "J. Tennison, G. Kellogg (Eds.), Model for Tabular Data and Metadata on the Web, W3C Recommendation, 17 December 2015"
    locator: the sections Best Practice CSV and Parsing Tabular Data; the appendix Existing Standards (RFC 4180, Excel, Google Spreadsheets)
  - url: https://peps.python.org/pep-0305/
    title: "K. Altis, D. Cole, A. McNamara, S. Montanaro, C. Wells, PEP 305: CSV File API, created 26 January 2003"
    locator: Abstract; Rationale; Managing Different Dialects
  - url: https://docs.python.org/3.14/library/csv.html
    title: The Python Standard Library, csv, Python 3.14
    locator: the introduction; Module Contents; Dialects and Formatting Parameters; Examples; footnote 1
  - url: https://pkg.go.dev/encoding/csv
    title: Go standard library, package encoding/csv
    locator: Overview; Variables (ErrBareQuote, ErrQuote, ErrFieldCount); type Reader; type Writer
  - url: https://docs.rs/csv/1.4.0/csv/
    title: csv 1.4.0 documentation (Rust crate)
    locator: ReaderBuilder (has_headers, flexible, terminator); WriterBuilder (terminator, quote_style)
  - url: https://commons.apache.org/proper/commons-csv/apidocs/org/apache/commons/csv/CSVFormat.html
    title: Apache Commons CSV, class CSVFormat
    locator: the predefined formats DEFAULT, EXCEL and RFC4180
  - url: https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualbasic.fileio.textfieldparser
    title: .NET API documentation, TextFieldParser class
    locator: Remarks; the properties TrimWhiteSpace, ErrorLine and ErrorLineNumber
  - url: https://owasp.org/www-community/attacks/CSV_Injection
    title: OWASP, CSV Injection
    locator: the description and the advice on how to escape cells
  - url: https://support.microsoft.com/en-us/office/import-or-export-text-txt-or-csv-files-5250ac4c-663c-47ce-937b-339e391393ba
    title: Microsoft Support, Import or export text (.txt or .csv) files
    locator: Import a text file by opening it in Excel; Change the default list separator for saving files as text (.csv) in Excel
  - url: https://doi.org/10.1186/s13059-016-1044-7
    title: "M. Ziemann, Y. Eren, A. El-Osta, Gene name errors are widespread in the scientific literature, Genome Biology 17:177, 2016"
    locator: the abstract
  - url: https://sqlite.org/cli.html
    title: SQLite, Command Line Shell For SQLite
    locator: 7.5 Importing files as CSV or other formats; 7.6 Export to CSV
  - url: https://www.postgresql.org/docs/18/sql-copy.html
    title: PostgreSQL 18 documentation, COPY
    locator: the section CSV Format
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/import-csv
    title: PowerShell documentation, Import-Csv
    locator: the description and the parameters Delimiter, Encoding and UseCulture
---

## What it is

CSV (comma-separated values) is a text format for a table. Each line of the text is a record. Commas separate the fields of a record. The first line can be a header that gives a name to each column.[^1] The Python documentation calls CSV the most common import and export format for spreadsheets and databases.[^6]

A CSV file has no types. Each field is a string of characters. The text `007` can be a number, a code with leading zeros or a name. The format does not say which. A CSV file also has no schema. It does not say which columns must be there, which fields can be empty, or what the encoding of the text is. The reader must know these facts from somewhere else, or guess them.

CSV is also not one format. There are many dialects. They differ in the delimiter, the quote character, the way to escape a quote and the line end.[^5] RFC 4180 describes the dialect that most programs share, and this page uses it as the reference.[^1] Each pitfall on this page comes from a difference between a dialect and RFC 4180, or from a fact that the format does not record.

### History

CSV was in use for many years before anyone described it in a standard way.[^6] RFC 4180 says that the format "has never been formally documented", and that no MIME type for it was registered with IANA. At that time, different programs and operating systems used different MIME types for CSV.[^1]

PEP 305 added the module `csv` to Python 2.3. The PEP says that the format is "not formally defined by a stable specification". It also says that it is "subtle enough" that a split on commas is "eventually bound to fail".[^5] The module defined the dialect `excel`, for the CSV that Excel 97 and Excel 2000 export.[^5]

Yakov Shafranovich wrote RFC 4180 in October 2005. It is an Informational RFC. It "does not specify an Internet standard of any kind". It documents the format "that seems to be followed by most implementations", and it registers the media type `text/csv`.[^1] RFC 7111 updated the registration in January 2014.[^3] The W3C CSV on the Web Working Group published a model for tabular data in December 2015. The model says that "there is no standard for CSV".[^4]

## Syntax or structure

RFC 4180 gives seven rules and a grammar in ABNF. The grammar below is a copy of the one in Section 2.[^1]

```abnf
file = [header CRLF] record *(CRLF record) [CRLF]
header = name *(COMMA name)
record = field *(COMMA field)
name = field
field = (escaped / non-escaped)
escaped = DQUOTE *(TEXTDATA / COMMA / CR / LF / 2DQUOTE) DQUOTE
non-escaped = *TEXTDATA
COMMA = %x2C
CR = %x0D
DQUOTE = %x22
LF = %x0A
CRLF = CR LF
TEXTDATA = %x20-21 / %x23-2B / %x2D-7E
```

These are the seven rules of Section 2, in short form:[^1]

1. Each record is on its own line, and a line break (CRLF) ends it.
2. The last record can end with a line break or without one.
3. The first line can be a header with the names of the fields. It has the same form as a record and should have the same number of fields.
4. Commas separate the fields. Each line should have the same number of fields. Spaces are part of a field and should not be ignored. The last field must not have a comma after it.
5. A field can be in double quotes. A field that is not in double quotes cannot hold a double quote.
6. A field that holds a line break, a double quote or a comma should be in double quotes.
7. A double quote inside a quoted field is written as two double quotes.

The rows below show the rules. The field `note` of the second record holds a comma, two quotes and a line break.

```text
id,name,note
7,"Smith, Ann","said ""hi""
and left"
8,Bo,
```

The record of `id` 7 takes two lines of text. A program that reads the file line by line sees four lines, but the file has three records. The record of `id` 8 has an empty last field. Its line ends with a comma, but this comma separates the second field from the third. It is not a comma after the last field.

The rule `TEXTDATA` allows only the printable ASCII characters. The CSVW model says that this limit "is routinely ignored in practice".[^4] Most CSV files hold other characters in UTF-8 or in a legacy encoding.

Section 3 registers the media type `text/csv`. It has two optional parameters. The parameter `charset` names the encoding of the text. The parameter `header` has the value `present` or `absent`, and it says if the first line is a header.[^1] The registration says that `text/csv` uses CRLF for line breaks, as Section 4.1.1 of RFC 2046 says. It also warns that "some implementations may use other values".[^2] The registration repeats the robustness principle of RFC 793: "be conservative in what you do, be liberal in what you accept from others".[^2] The file extension is `.csv`.[^1]

The grammar has a consequence for blank lines. A `record` can be one empty field, because `non-escaped` can be empty. So the grammar reads a blank line as a record with one empty field. A file that ends with two line breaks has an extra record of this kind. The Go reader skips blank lines instead.[^7]

## Examples

Python has the module `csv` in its standard library. The reader below gets three records from the four lines of the text. The line break inside the quoted field stays in the field.[^6]

```python run id=read
import csv
import io

text = 'id,name,note\r\n7,"Smith, Ann","said ""hi""\r\nand left"\r\n8,Bo,\r\n'

rows = list(csv.reader(io.StringIO(text, newline="")))
print(len(rows))
for row in rows:
    print(row)

print(len(text.splitlines()))
```

```output for=read
3
['id', 'name', 'note']
['7', 'Smith, Ann', 'said "hi"\r\nand left']
['8', 'Bo', '']
4
```

The writer quotes a field only when it holds the delimiter, the quote character or a line break. This is the mode `QUOTE_MINIMAL`. The writer ends each record with `\r\n` by default, as RFC 4180 does.[^6] The integer `7` becomes the text `7`, because CSV has no types.

```python run id=write
import csv
import io

out = io.StringIO(newline="")
writer = csv.writer(out)
writer.writerow(["id", "name", "note"])
writer.writerow([7, "Smith, Ann", 'said "hi"\nand left'])
writer.writerow([8, "Bo", ""])
print(repr(out.getvalue()))
```

```output for=write
'id,name,note\r\n7,"Smith, Ann","said ""hi""\nand left"\r\n8,Bo,\r\n'
```

A split on commas is the usual first attempt at a CSV reader. It breaks the quoted field `"Smith, Ann"` into two fields and keeps the quotes. PEP 305 gives this case as the reason for the module.[^5]

```python wrong id=split
line = '7,"Smith, Ann",paid'
print(line.split(","))
```

```output for=split
['7', '"Smith', ' Ann"', 'paid']
```

A file must be opened with `newline=""`. The Python documentation says that line breaks inside quoted fields are not read correctly without it.[^6] In the run below, the `\r\n` inside the field becomes `\n`.

```python run id=newline
import csv
from pathlib import Path

path = Path("notes.csv")
path.write_bytes(b'id,note\r\n7,"line one\r\nline two"\r\n')

with open(path, encoding="utf-8", newline="") as file:
    print(list(csv.reader(file)))

with open(path, encoding="utf-8") as file:
    print(list(csv.reader(file)))
```

```output for=newline
[['id', 'note'], ['7', 'line one\r\nline two']]
[['id', 'note'], ['7', 'line one\nline two']]
```

The example files show the CSV reader and writer of each language. They also show the behaviour on a byte order mark, on a bad quote and on a row with the wrong number of fields.

## Versions

CSV has no version number. The documents below describe it or the media type.

| Document | Date | Status | Content |
|---|---|---|---|
| PEP 305 | January 2003 | Python Enhancement Proposal, Final | the `csv` module and its dialects[^5] |
| RFC 4180 | October 2005 | Informational | the common format; registers `text/csv` with the parameters `charset` and `header`[^1] |
| RFC 7111 | January 2014 | Informational; updates RFC 4180 | fragment identifiers for `text/csv`; UTF-8 is the default charset[^3] |
| CSVW tabular data model | December 2015 | W3C Recommendation | a model for tables, a CSV syntax that allows LF, and a parse algorithm with dialect flags[^4] |

### RFC 7111

RFC 7111 defines fragment identifiers for `text/csv`. A URI can point to one row, one column, one cell or a range. For example, `#row=5-7` selects rows 5 to 7, `#col=2` selects the second column, and `#cell=4,1` selects the cell in row 4 and column 1. Rows and columns start at 1.[^3] The scheme `row` selects records, not lines of text.[^3]

RFC 7111 also changes the registration of `text/csv`. It follows RFC 6657: the parameter `charset` SHOULD be used, and if it is absent, UTF-8 SHOULD be assumed.[^3][^2] RFC 4180 had said that the common usage was US-ASCII.[^1]

### CSV on the Web

The W3C model for tabular data describes a CSV syntax for publishers.[^4] It differs from RFC 4180 in one point: rows can end with LF as well as CRLF. Each file that follows RFC 4180 also follows this syntax. The model gives these rules:

- The file should be in UTF-8, in Unicode Normalization Form C. Another encoding should be named in the parameter `charset`.
- Each line should have the same number of values.
- There should be no whitespace before or after the double quotes of a field.
- The first line should be a header. A file with no header should say `header=absent` in the media type.

The model also defines an algorithm to parse CSV files that do not follow these rules. Flags control it, such as `delimiter`, `encoding`, `lineTerminators`, `skipRows` and `trim`. The default delimiter is `,`, the default encoding is UTF-8, and both CRLF and LF end a row by default.[^4] Metadata can give each column a name and a datatype, and a dialect description can set the flags. Other specifications, csv2json and csv2rdf, define the conversion of a table to JSON and RDF.[^4]

## Pitfalls

- **The delimiter depends on the locale.** Excel uses the list separator of the system. Where the decimal separator is a comma, Excel uses a semicolon as the list separator.[^12] A file from such a system has the text `Tea;3,50`. Commons CSV says that "the actual value delimiter used by Excel is locale-dependent".[^9] Ask for the delimiter, or detect it. Python has `csv.Sniffer` to guess the dialect from a sample.[^6]
- **The byte order mark.** A UTF-8 file can start with a byte order mark (BOM), the bytes EF BB BF. A reader that decodes the file as plain UTF-8 makes the BOM part of the first header name. Then the name is `\ufeffzip` and not `zip`, and a lookup of `zip` fails. Python reads it correctly with the encoding `utf-8-sig`. In the example runs, Go keeps the BOM and the Rust `csv` crate removes it.
- **LF line ends.** RFC 4180 uses CRLF. The CSVW model also allows LF, because LF is important on platforms other than Windows.[^4] The readers of Python, Go and Rust accept both.[^6][^7][^8] Writers differ. The Python writer uses CRLF, but the Go writer and the Rust writer use LF by default.[^6][^7][^8]
- **A line break inside a field.** A quoted field can hold a line break, so a record can take more than one line. A tool that works line by line counts lines and not records. It can cut a record in two.
- **Leading zeros, dates and gene names.** Excel uses its default data formats when it opens a `.csv` file. Microsoft gives a column of numbers with leading zeros as a case for the Import Text Wizard, which can import the column as text and keep the zeros.[^12] The CSVW model reports that Excel for Mac 2011 detects dates in the form YYYY-MM-DD and formats them again.[^4] A study of the scientific literature found that Excel changes some gene names to dates and to floating-point numbers. About one fifth of the papers with supplementary gene lists in Excel files had such errors.[^13]
- **CSV injection.** A spreadsheet reads a cell that starts with `=` as a formula. OWASP lists `=`, `+`, `-`, `@`, tab, carriage return and line feed as the dangerous first characters. The risk comes when a website puts untrusted input into a CSV file. A crafted formula can attack the computer of the user through a flaw in the spreadsheet program, or copy data out of the spreadsheet.[^11] OWASP advises to put a single quote before such a cell, to put each cell in double quotes, and to double each double quote in a cell. It warns that Excel can lose this escape when a user saves the file and opens it again.[^11]
- **Ragged rows.** A row can have more or fewer fields than the header. Go and the Rust `csv` crate return an error by default. Python `DictReader` fills missing fields with `None`.[^7][^8][^6] In the example run, Commons CSV returns the row as it is. Check the field count when the data comes from outside.
- **Bad quotes.** A quote inside an unquoted field, or text after the closing quote, breaks rule 5 or rule 7. Go returns `ErrBareQuote` or `ErrQuote`.[^7] The Python reader accepts both by default. With `strict=True`, Python rejects text after a closing quote.[^6] A lenient mode, such as `LazyQuotes` in Go, can merge the rest of the line into one field.
- **No declared encoding.** A CSV file does not say what its encoding is. Python decodes a file with the system default encoding unless the program gives `encoding=`.[^6] The CSVW model reports that Excel for Mac 2011 with an English locale writes Windows-1252 and changes other characters to underscores.[^4] Google Sheets writes UTF-8 with no BOM.[^4]
- **A trailing empty line.** A blank line at the end, or between records, is an empty record under the grammar. Go skips blank lines.[^7] The Commons CSV format `DEFAULT` allows empty lines, but the format `RFC4180` does not ignore them.[^9]
- **Whitespace around fields.** RFC 4180 says that spaces are part of a field.[^1] `TextFieldParser` in .NET removes them by default, and the CSVW parser trims cells by default.[^10][^4] So the field ` Bo` can come back as `Bo` or as ` Bo`, and the result depends on the reader.
- **NULL and the empty string.** CSV has no null value. The PostgreSQL documentation says that "the CSV format has no standard way to distinguish a NULL value from an empty string". PostgreSQL `COPY` writes NULL as an empty field with no quotes, and an empty string as `""`.[^15] The SQLite example does the same.

## Tools and libraries

| Language | Verdict | Read and write |
|---|---|---|
| Python | stdlib | `csv.reader`, `csv.writer`, `csv.DictReader`, `csv.DictWriter`[^6] |
| Go | stdlib | `encoding/csv`, `csv.NewReader`, `csv.NewWriter`[^7] |
| C# | stdlib | `TextFieldParser` reads; no writer in the base library[^10] |
| Rust | library | the `csv` crate, `Reader` and `Writer`, with `serde`[^8] |
| Java | library | Apache Commons CSV, `CSVFormat`, `CSVParser`, `CSVPrinter`[^9] |
| JavaScript, TypeScript | idiom | a short parser; no CSV API in the language or in Node.js |
| SQL | idiom | SQLite shell `.mode csv` and `.import --csv`; PostgreSQL `COPY ... (FORMAT csv)`[^14][^15] |
| PowerShell | stdlib | `Import-Csv`, `Export-Csv`, `ConvertFrom-Csv`, `ConvertTo-Csv`[^16] |

The SQLite shell reads a CSV file into a table with `.import --csv`. If the table does not exist, the first row gives the names of the columns. The option `--skip 1` skips a header row when the table exists.[^14] The mode `.mode csv` writes the result of a query as CSV "per RFC 4180".[^14]

PostgreSQL reads and writes CSV with the command `COPY` and the option `FORMAT csv`. Other options set the header, the quote character, the escape character and the columns that are always quoted. In CSV format, "all characters are significant", so the spaces around a value are part of it.[^15]

PowerShell `Import-Csv` makes one object for each row, with one property for each column. The parameter `Delimiter` sets the delimiter, and the parameter `UseCulture` uses the list separator of the current culture. `ConvertFrom-Csv` does the same with strings from the pipeline.[^16]

## References

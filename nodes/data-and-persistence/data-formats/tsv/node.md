---
tag: HXEWJ0
kind: format
slug: tsv
name: TSV
aliases: [tab-separated values, tab-delimited text, tab-separated-values]
area: data-and-persistence/data-formats
level: core
summary: >-
  TSV is a text format for a table. Each line is a record and a tab separates the fields. A field cannot hold a tab or a line break, so a program can read the format with a split and no parser.
see_also: [csv, read-a-csv-file, split-a-string, read-a-file-line-by-line]
sources:
- url: https://www.iana.org/assignments/media-types/text/tab-separated-values
  title: IANA, Media type registration of text/tab-separated-values
  locator: the whole template
- url: https://github.com/eBay/tsv-utils/blob/master/docs/comparing-tsv-and-csv.md
  title: eBay tsv-utils, Comparing TSV and CSV formats
  locator: the whole page
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/cut.html
  title: "The Open Group Base Specifications Issue 8 (POSIX.1-2024), cut"
  locator: OPTIONS (-d, -f)
- url: https://www.postgresql.org/docs/current/sql-copy.html
  title: PostgreSQL documentation, COPY
  locator: Parameters (DELIMITER, NULL); File Formats, Text Format
- url: https://docs.python.org/3/library/csv.html
  title: The Python Standard Library, csv
  locator: csv.excel_tab; csv.QUOTE_NONE; Dialect.escapechar
- url: https://pkg.go.dev/encoding/csv
  title: Go standard library, package encoding/csv
  locator: type Reader (Comma, LazyQuotes); Variables (ErrBareQuote)
---

## What it is

TSV (tab-separated values) is a text format for a table. The registration of the media type `text/tab-separated-values` defines it in a few sentences: "Each record is represented as a single line. Each field value is represented as text. Fields in a record are separated from each other by a tab character."[^1] It also says: "Note that fields that contain tabs are not allowable in this encoding."[^1]

The registration calls TSV "a popular method of data interchange among databases and spreadsheets and word processors with Mail-merge functions."[^1] Its contact is the "U of MN Internet Gopher Team", the team of the Gopher protocol at the University of Minnesota.[^1]

TSV and [[csv]] look alike, but they handle a delimiter in the data in different ways. "CSV uses an escape syntax to represent commas and newlines in the data. TSV takes a different approach, disallowing TABs and newlines in the data."[^2] So a program reads TSV with a line reader and a split on the tab. "No special parser is needed."[^2] The cost is that a field cannot hold a tab or a line break. Text that people write, such as notes and addresses, can hold them. The tsv-utils documentation says that TSV suits "the large tabular data sets common in data mining and machine learning environments", and adds: "These data sets rarely need TAB and newline characters in the fields."[^2]

Like CSV, TSV has no types. Each field is text, and the reader must know what each column holds.

## Syntax or structure

The registration gives this grammar:[^1]

```text
field ::= [character]+ # multiple characters
name ::= [character]+
nameline ::= name [TAB name]+ EOL
record ::= field [TAB field]+ EOL # at least one field, or more
tsv ::= nameline record+
```

It adds three rules:

- "The first line of this encoding is special, it contains the name of each field, separated by tabs."[^1]
- "Each record must have the same number of fields."[^1]
- A field cannot hold a tab. A field also cannot hold a line break, because a line break ends the record.

The example of the registration is a table of three records with the columns `Name`, `Age` and `Address`.[^1] In the block below, each gap is one tab character:

```text
Name	Age	Address
Paul	23	1115 W Franklin
Bessy the Cow	5	Big Farm Way
Zeke	45	W Main St
```

The field `Bessy the Cow` holds spaces. A space is a normal character in TSV, so the field needs no quotes. Quotes are normal characters too, so `5" disk` is a field of seven characters.

The grammar is short, and it leaves some points open:

- `[character]+` asks for one character or more in each field, so the grammar has no empty field. The code on this page accepts an empty field.
- `[TAB name]+` asks for two columns or more, but the comment of `record` says "at least one field".[^1]
- The registration does not say which characters end a line. A file can use LF or CRLF.
- The registration lists "Character Set" as a required parameter but names no default.[^1]

## Examples

A split on the tab reads TSV. The code below reads a header and two records into dictionaries:

```python run id=split-lines
text = "name\tage\tcity\nAda\t36\tLondon\nAlan\t41\tWilmslow\n"
lines = text.removesuffix("\n").split("\n")
header = lines[0].split("\t")
for line in lines[1:]:
    print(dict(zip(header, line.split("\t"))))
```

```output for=split-lines
{'name': 'Ada', 'age': '36', 'city': 'London'}
{'name': 'Alan', 'age': '41', 'city': 'Wilmslow'}
```

The tools of the Unix shell read TSV in the same way. The utility `cut` selects fields. Its option `-d` sets the delimiter, and "The default is the <tab>."[^3] So `cut -f 3 people.tsv` prints the third column. The tsv-utils documentation names more tools: "Unix utilities can be called by providing the correct field delimiter".[^2] Examples are `awk -F '\t'` and `sort -t $'\t'`.[^2]

Some programs need a tab or a line break in a field. They use an extended form of TSV with backslash escapes. The text format of the PostgreSQL command `COPY` is an example. Its delimiter "is a tab character in text format".[^4] "In particular, the following characters must be preceded by a backslash if they appear as part of a column value: backslash itself, newline, carriage return, and the current delimiter character."[^4] `COPY FROM` reads `\t` as a tab, `\n` as a newline and `\r` as a carriage return.[^4] The code below writes and reads fields in this form:

```python run id=escape
ESCAPES = {"\\": "\\\\", "\t": "\\t", "\n": "\\n", "\r": "\\r"}
UNESCAPES = {"\\": "\\", "t": "\t", "n": "\n", "r": "\r"}


def escape(field):
    return "".join(ESCAPES.get(c, c) for c in field)


def unescape(field):
    out, i = [], 0
    while i < len(field):
        if field[i] == "\\" and i + 1 < len(field):
            out.append(UNESCAPES.get(field[i + 1], field[i + 1]))
            i += 2
        else:
            out.append(field[i])
            i += 1
    return "".join(out)


record = ["7", "line one\nline two", "a\tb", "C:\\temp"]
line = "\t".join(escape(f) for f in record)
print(line)
print([unescape(f) for f in line.split("\t")] == record)
```

```output for=escape
7	line one\nline two	a\tb	C:\\temp
True
```

After the escape, each record is one line again and each tab is a delimiter. So the split still works. The split must come first and the unescape second.

## Versions

TSV has one definition, the media type registration of `text/tab-separated-values`. It has no version number and no RFC.[^1] The registration does not give the file extension.

Three forms of the format are in use:

| Form | Tab or line break in a field | Quotes | Example |
|---|---|---|---|
| Plain TSV | not allowed | normal characters | the IANA registration[^1] |
| TSV with backslash escapes | written as `\t`, `\n` and `\r` | normal characters | the text format of PostgreSQL `COPY`[^4] |
| CSV with a tab delimiter | allowed in a quoted field | as in CSV | the `excel-tab` dialect of Python[^5] |

The third form is CSV, not TSV, although its files often have the name TSV. The Python documentation says that `excel_tab` "defines the usual properties of an Excel-generated TAB-delimited file."[^5] A reader for one form reads some files of another form incorrectly, as the pitfalls show. The tsv-utils documentation warns: "Note that many CSV-to-TSV conversion tools don't actually remove the CSV escapes."[^2]

## Pitfalls

- **A tab or a line break in a field breaks the record.** The format has no escape, so a writer that joins the fields with tabs makes extra fields and extra records:

```python wrong id=tab-in-field
record = ["7", "a\tb", "line one\nline two"]
text = "\t".join(record) + "\n"
print([line.split("\t") for line in text.removesuffix("\n").split("\n")])
```

```output for=tab-in-field
[['7', 'a', 'b', 'line one'], ['line two']]
```

Check each field before you write it. Reject a field with a tab, CR or LF, replace them with a space, or use the backslash escapes of the examples. If the data often holds them, use [[csv]].

- **A CSV reader with a tab delimiter changes fields that hold quotes.** In plain TSV, the double quote is a normal character. The `excel-tab` dialect of Python reads a field that starts with a double quote as a quoted field. Give `quoting=csv.QUOTE_NONE`, which "Instructs reader objects to perform no special processing of quote characters."[^5]

```python wrong id=excel-tab
import csv
import io

text = 'title\tsize\n"Hi," she said\t5" disk\n'
print(list(csv.reader(io.StringIO(text, newline=""), dialect="excel-tab")))
print(list(csv.reader(io.StringIO(text, newline=""), delimiter="\t", quoting=csv.QUOTE_NONE)))
```

```output for=excel-tab
[['title', 'size'], ['Hi, she said', '5" disk']]
[['title', 'size'], ['"Hi," she said', '5" disk']]
```

- **Go's CSV reader rejects a quote in a field.** With `Comma` set to `'\t'`, `csv.Reader` still reads CSV quotes, and a `"` inside a field that does not start with one gives `ErrBareQuote`:[^6]

```go wrong id=go-bare-quote
package main

import (
	"encoding/csv"
	"fmt"
	"strings"
)

func main() {
	r := csv.NewReader(strings.NewReader("title\tsize\nfloppy\t5\" disk\n"))
	r.Comma = '\t'
	records, err := r.ReadAll()
	fmt.Println(records)
	fmt.Println(err)
}
```

```output for=go-bare-quote
[]
parse error on line 2, column 9: bare " in non-quoted-field
```

For plain TSV, read the lines with `bufio.Scanner` and split each line with `strings.Split(line, "\t")`.

- **A CRLF leaves a CR in the last field.** A split on LF keeps the CR of a file from Windows, so the last column has an extra character, and a comparison such as `city == "London"` fails:

```python wrong id=crlf
text = "name\tcity\r\nAda\tLondon\r\n"
for line in text.removesuffix("\n").split("\n"):
    print(line.split("\t"))
```

```output for=crlf
['name', 'city\r']
['Ada', 'London\r']
```

Remove a CR at the end of each line before the split.

- **The escape of Python is not the escape of PostgreSQL.** With `quoting=csv.QUOTE_NONE` and `escapechar="\\"`, the Python writer puts the escape character before a delimiter, a quote character, CR or LF.[^5] It writes a backslash and then the tab itself, not `\t`:

```python wrong id=python-escapechar
import csv
import io

out = io.StringIO(newline="")
csv.writer(out, delimiter="\t", quoting=csv.QUOTE_NONE, escapechar="\\").writerow(["a\tb", "c"])
print(repr(out.getvalue()))
```

```output for=python-escapechar
'a\\\tb\tc\r\n'
```

A split on the tab cuts this field in two. Use code like the `escape` function of the examples for files that PostgreSQL or a split reads.

- **An empty field and a null are the same text.** Plain TSV has no null. PostgreSQL writes a null as `\N` in its text format: the default null string "is \N (backslash-N) in text format".[^4] A program that reads such a file must turn `\N` into a null, and a program that writes one must not write a real `\N` field as it is.

- **The number of fields must be the same in each record.** "Each record must have the same number of fields."[^1] A split does not check this, so a short line gives a short record and the columns move. Check the length of each record against the header. PostgreSQL does: "COPY FROM will raise an error if any line of the input file contains more or fewer columns than are expected."[^4]

## Tools and libraries

- Python: `str.split('\t')` on each line, or the module `csv` with `delimiter='\t'` and `quoting=csv.QUOTE_NONE`.[^5]
- Go: `bufio.Scanner` and `strings.Split`. The package `encoding/csv` reads TSV only when no field holds a quote.[^6]
- The Unix shell: `cut`, `awk -F '\t'` and `sort -t $'\t'`.[^3][^2]
- eBay tsv-utils: command line tools for TSV, such as `tsv-uniq`, `tsv-sample` and `csv2tsv`.[^2]
- PostgreSQL: `COPY ... TO` and `COPY ... FROM` in the text format.[^4]

The task [[split-a-string]] shows the split in each language, and [[read-a-file-line-by-line]] shows the line loop.

## References

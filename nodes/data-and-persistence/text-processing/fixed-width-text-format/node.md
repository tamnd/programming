---
tag: QZ75Q7
kind: format
slug: fixed-width-text-format
name: Fixed-width text format
aliases: [fixed-width file, fixed-length records, fixed-length format, column-based format, flat file, logical record length, LRECL, fwf]
area: data-and-persistence/text-processing
level: core
summary: >-
  A fixed-width text file puts each field of a record at fixed columns and pads it with spaces. A layout outside the file gives the columns, so the reader cuts the line at those positions.
see_also: [delimited-text, csv, tsv, line-ending, utf-8, read-a-file-line-by-line]
sources:
- url: https://en.wikipedia.org/wiki/Flat-file_database
  title: Flat-file database, Wikipedia
  locator: Format
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Fixed-length-records/00-TASK.txt
  title: Rosetta Code, Fixed length records (task text, from acmeism/RosettaCodeData)
  locator: the whole task
- url: https://docs.python.org/3/library/string.html
  title: The Python Standard Library, string
  locator: Format Specification Mini-Language
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/dd.html
  title: The Open Group, POSIX.1-2024, dd
  locator: Operands, conv=block and conv=unblock
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/cut.html
  title: The Open Group, POSIX.1-2024, cut
  locator: Description, Options (-b, -c)
- url: https://www.unicode.org/reports/tr11/
  title: 'Unicode Standard Annex #11: East Asian Width'
  locator: 4 Definitions
- url: https://docs.python.org/3/library/unicodedata.html
  title: The Python Standard Library, unicodedata
  locator: east_asian_width
- url: https://www.postgresql.org/docs/current/datatype-character.html
  title: PostgreSQL Documentation, Character Types
  locator: Table 8.4, character(n)
- url: https://pandas.pydata.org/docs/reference/api/pandas.read_fwf.html
  title: pandas documentation, pandas.read_fwf
  locator: Parameters (colspecs, widths)
---

## What it is

A fixed-width text file stores a table as lines of text, and each field of a record starts at the same column in every line. "With fixed-width formats, each field has a fixed length with extra spaces added as needed"[^1]. The file has no delimiters and no quotes. A layout gives the name, the first column and the width of each field, and "The fixed lengths can be predefined and known ahead of time (i.e. stated in the format's specification), or parsed from a header"[^1].

The format is old. The Rosetta Code task links it to punched cards, which that "handled 80 columns per card", and to devices that "assumed/demanded a fixed line width of 80 characters, newlines were not required (and could not even be encoded in some systems)"[^2]. "These fixed length formats are still in wide use on mainframes, with JCL and with COBOL"[^2], and "Most of the large players in day to day financial transactions know all about fixed length records"[^2].

A field cannot be longer than its width: "With predefined lengths, fields are limited to a maximum length"[^1]. The reader needs no search for delimiters: "With delimiter-separated formats, determining the field boundaries requires finding the delimiters, which incurs some computational overhead . This is not needed for fixed-width formats"[^1]. The cost is space: "fixed-width formats can lead to unnecessarily large file sizes if fields tend to be shorter than the lengths reserved for them"[^1].

## Syntax or structure

A fixed-width file has three parts, and only one of them is in the file:

- **The layout.** A table, outside the file, gives each field a name, a first column and a width. It also says how each field is aligned and padded, and how to read numbers, for example with implied decimals.
- **The records.** Each record has the same length, the sum of the widths. In a text file a newline ends each record. In a file of fixed-length records there is no newline, and the reader counts bytes. A flat file in plain text "is typically formatted as one record per line"[^1].
- **The padding.** A short value is filled to its width. A layout often aligns text to the left and numbers to the right, the same defaults as the format strings of Python[^3]. Text is padded with spaces, and numbers with zeros or spaces.

A layout for the examples below:

| Field | First column | Width | Alignment | Padding |
|---|---|---|---|---|
| id | 0 | 5 | right | zeros |
| name | 5 | 12 | left | spaces |
| city | 17 | 10 | left | spaces |
| amount | 27 | 9 | right | zeros, two implied decimals |

## Examples

**Read with a layout.** Cut each line at the columns of the layout, and remove the padding:

```python run id=read
# A layout gives the name, the first column (from 0) and the width of each field.
LAYOUT = [("id", 0, 5), ("name", 5, 12), ("city", 17, 10), ("amount", 27, 9)]
RECORD_LENGTH = 36

lines = [
    "00042Ada LovelaceLondon    000012550",
    "00043Alan Turing Wilmslow  -00000075",
    "00044Grace",
]

def parse(line):
    record = {}
    for name, start, width in LAYOUT:
        record[name] = line[start:start + width].rstrip(" ")
    return record

for line in lines:
    print(len(line) == RECORD_LENGTH, parse(line))

# The layout says that "amount" has two implied decimals: 000012550 is 125.50.
r = parse(lines[0])
print(int(r["id"]), f'{int(r["amount"]) / 100:.2f}')
```

```output for=read
True {'id': '00042', 'name': 'Ada Lovelace', 'city': 'London', 'amount': '000012550'}
True {'id': '00043', 'name': 'Alan Turing', 'city': 'Wilmslow', 'amount': '-00000075'}
False {'id': '00044', 'name': 'Grace', 'city': '', 'amount': ''}
42 125.50
```

The amount `000012550` has two implied decimals, so it is 125.50. The third line is shorter than a record. A slice past the end of a string gives an empty string, so the reader returns empty fields with no error, and only the check of the length finds the short line.

**Write with a layout.** Pad each value to its width, and cut a value that is too long:

```python run id=write
LAYOUT = [("id", 5, ">", "0"), ("name", 12, "<", " "), ("city", 10, "<", " ")]

def field(value, width, align, fill):
    text = f"{value:{fill}{align}{width}}"
    # The width in a format is a minimum: a longer value makes a longer field.
    return text[:width] if align == "<" else text[-width:]

def record(values):
    return "".join(field(v, w, a, f) for v, (_, w, a, f) in zip(values, LAYOUT))

print(repr(f"{'Wilhelmina Murray':<12}"))
for values in [(42, "Ada Lovelace", "London"), (7, "Wilhelmina Murray", "Whitby")]:
    line = record(values)
    print(repr(line), len(line))
```

```output for=write
'Wilhelmina Murray'
'00042Ada LovelaceLondon    ' 27
'00007Wilhelmina MWhitby    ' 27
```

In a format string, "The width is a decimal integer defining the minimum total field width"[^3], and "'<' Forces the field to be left-aligned within the available space"[^3]. The width is a minimum, so a format alone does not cut a long name: the first line of the output is 17 characters. The `field` function cuts it, so each record has 27 characters, and the long name loses its end.

**Records with no newline.** The `dd` utility converts between lines and fixed-length records. With `conv=block`, "Each record shall be converted to a record with a fixed length specified by the conversion block size"[^4], "<space> characters shall be appended to lines that are shorter than their conversion block size to fill the block"[^4], and "Lines that are longer than the conversion block size shall be truncated"[^4]. With `conv=unblock`, `dd` will "delete all trailing <space> characters, and append a <newline>"[^4]. The same two steps in Python:

```python run id=block
# Records with no newline: each record is exactly 20 bytes, padded with spaces.
WIDTH = 20
lines = ["Line 1", "  Indented line 2", "", "Line 4 is longer than twenty"]

# block: pad or cut each line to WIDTH, and drop the newlines.
data = b"".join(line.encode("ascii")[:WIDTH].ljust(WIDTH) for line in lines)
print(len(data), data[:40])

# unblock: cut the data into WIDTH bytes, remove trailing spaces, add newlines.
records = [data[i:i + WIDTH] for i in range(0, len(data), WIDTH)]
print([r.rstrip(b" ").decode("ascii") for r in records])
```

```output for=block
80 b'Line 1                Indented line 2   '
['Line 1', '  Indented line 2', '', 'Line 4 is longer tha']
```

The four lines become 80 bytes with no newline. The way back gives the lines again, but the long line has lost its end, and a line that ended with spaces would lose them too.

**Width in characters, bytes and screen cells.** A layout must say what it counts. `cut` has one option for each: "-b list Cut based on a list of bytes"[^5] and "-c list Cut based on a list of characters"[^5]. A screen shows some characters in two cells. Unicode gives each character an East Asian Width, and "In a broad sense, wide characters include W, F, and A (when in East Asian context)"[^6]. Python returns the property with `unicodedata.east_asian_width`[^7].

```python run id=wide
import unicodedata

# Three ways to count the width of a field: code points, bytes and screen cells.
def cells(s):
    return sum(2 if unicodedata.east_asian_width(c) in "WF" else 1 for c in s)

for name in ["Lovelace", "Zoë", "東京"]:
    print(f"{name!r:12} chars={len(name)} utf8={len(name.encode('utf-8'))} cells={cells(name)}")

# A pad to 6 characters gives the same number of characters, not the same columns.
for name in ["Lovelace"[:6], "東京"]:
    print(f"|{name:<6}|")
```

```output for=wide
'Lovelace'   chars=8 utf8=8 cells=8
'Zoë'        chars=3 utf8=4 cells=3
'東京'         chars=2 utf8=6 cells=4
|Lovela|
|東京    |
```

The name `東京` has 2 characters, 6 bytes in UTF-8 and 4 cells on a screen. A pad to 6 characters gives 6 characters, but 8 cells, so the column after it moves on the screen. A layout in bytes cuts UTF-8 text in the middle of a character when the field is full.

## Versions

The format has no standard and no version. Each layout is its own format, and the producer of the file publishes it. The forms differ in a few ways:

| Form | End of record | Character set | Example |
|---|---|---|---|
| Text with lines | a newline after each record | ASCII or UTF-8 | one record per line[^1] |
| Fixed-length records | none; the reader counts bytes | often EBCDIC with COBOL on mainframes[^2] | the 80-column records of the Rosetta task[^2] |
| Text with a header | a newline | ASCII or UTF-8 | a header line whose column names show the widths[^1] |

SQL has a fixed-width text type. In PostgreSQL, `character(n)` is "fixed-length, blank-padded"[^8], and "If the string to be stored is shorter than the declared length, values of type character will be space-padded"[^8].

## Pitfalls

- **A format width does not cut.** The width in a format string is a minimum[^3], so a long value moves each field after it, as the write example shows. Cut each value to its width, and decide whether a long value is an error.
- **A short line gives empty fields.** A slice past the end of a line gives an empty string, so a short record reads as a record with empty fields. Check the length of each record before you cut it. Editors and tools that remove trailing spaces make short lines, and `dd conv=unblock` removes them on purpose[^4].
- **Trailing spaces in the data.** The reader removes the padding, so it cannot tell a value with trailing spaces from a shorter value. Leading zeros in a text field, such as an account number, are data, not padding.
- **Bytes, characters and cells.** A layout in bytes cuts a UTF-8 character in two. A layout in characters does not line up on a screen when a field holds wide characters[^6], as the last example shows. Decode the bytes first, and say in the layout what a column counts.
- **Implied decimals and signs.** A number field often has no decimal point, and the layout says where it is. A field such as `-00000075` puts the sign in the first column. Read numbers by the rules of the layout, not with a general number parser.
- **No newline in the record.** A file of fixed-length records can hold a newline byte in its data. The Rosetta task says: "newline characters allowed in the data"[^2]. Read such a file in blocks of the record length, not with a line reader.

## Tools and libraries

- Python: slices of `str` and `bytes`, `str.ljust` and `str.rjust`, and format strings with a width[^3]. pandas has `read_fwf` to "Read a table of fixed-width formatted lines into DataFrame"[^9]. Its `colspecs` parameter is "A list of tuples giving the extents of the fixed-width fields of each line as half-open intervals"[^9].
- The Unix shell: `cut -c` and `cut -b` for columns[^5], `dd conv=block` and `conv=unblock` for records with no newline[^4], and `printf` with a width for output.
- Other languages: a substring call cuts the fields, and the formatter of the language pads them, such as `String.format("%-12s", s)` in Java or `fmt.Sprintf("%-12s", s)` in Go.
- Databases: `character(n)` keeps a value padded to `n` characters[^8].

The page [[delimited-text]] shows the other common way to store a table as text.

## References

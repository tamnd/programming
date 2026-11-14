---
tag: MTQA77
kind: concept
slug: delimited-text
name: Delimited text
aliases: [delimiter-separated values, DSV, delimited file, field separator, field delimiter, record separator, delimiter collision, flat file]
area: data-and-persistence/text-processing
level: intro
summary: >-
  Delimited text stores a table as lines of text: each line is a record and a delimiter character separates its fields. A format must say what happens when a field holds the delimiter.
applies_to: core-languages
viz: optional
formal: none
see_also: [csv, tsv, line-ending, split-a-string, read-a-csv-file, read-a-file-line-by-line]
same_as:
  wikidata: Q3021669
  wikipedia: Delimiter-separated_values
sources:
- url: https://en.wikipedia.org/wiki/Delimiter-separated_values
  title: Delimiter-separated values, Wikipedia
  locator: Introduction, Format, Delimiter collision
- url: https://man7.org/linux/man-pages/man5/passwd.5.html
  title: passwd(5), Linux manual page
  locator: Description
- url: https://docs.python.org/3/library/csv.html
  title: The Python Standard Library, csv
  locator: csv.writer, csv.reader
- url: https://www.iana.org/assignments/media-types/text/tab-separated-values
  title: IANA, Media type registration of text/tab-separated-values
  locator: Definition of tab-separated-values
- url: https://www.rfc-editor.org/rfc/rfc4180.txt
  title: 'RFC 4180: Common Format and MIME Type for Comma-Separated Values (CSV) Files'
  locator: 2. Definition of the CSV Format
- url: http://www.catb.org/esr/writings/taoup/html/ch05s02.html
  title: Eric S. Raymond, The Art of Unix Programming, Chapter 5, Data File Metaformats
  locator: DSV Style, Cookie-Jar Format
- url: https://docs.python.org/3/library/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: str.split
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
  title: The Open Group, POSIX.1-2024, awk
  locator: Variables and Special Variables (FS, OFS, RS)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html
  title: The Open Group, POSIX.1-2024, Shell Command Language
  locator: 2.5.3 Shell Variables, IFS
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/cut.html
  title: The Open Group, POSIX.1-2024, cut
  locator: Description, Options (-d, -f)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: String, Java SE 25 API
  locator: split(String, int)
- url: https://pkg.go.dev/strings
  title: Go standard library, package strings
  locator: Fields, Split, SplitN
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: Rust standard library, primitive type str
  locator: split, split_whitespace
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
  title: String.prototype.split(), MDN Web Docs
  locator: Parameters, Description
- url: https://en.cppreference.com/w/c/string/byte/strtok
  title: strtok, strtok_s, cppreference.com
  locator: Description, Notes
- url: https://man7.org/linux/man-pages/man3/strsep.3.html
  title: strsep(3), Linux manual page
  locator: Description, History
- url: https://en.cppreference.com/w/cpp/string/basic_string/getline
  title: std::getline, cppreference.com
  locator: Description
---

## Idea

Delimited text stores a table as plain text. Delimiter-separated values (DSV) are "a way of storing tabular data by separating the fields (values) of each row with a specific character as a delimiter"[^1]. Each line is a record, and one character separates its fields. "Any character may be used to separate field values, and the more commonly used include comma , tab , colon , vertical bar (a.k.a. pipe) and space"[^1]. A split at the delimiter reads such a line, until a field holds the delimiter.

```python run id=idea
import csv
import io

# Each line is a record, and a delimiter separates its fields.
line = "alice:x:1000:1000:Alice:/home/alice:/bin/sh"
fields = line.split(":")
print(len(fields), fields[0], fields[-1])

# Delimiter collision: a field holds the delimiter.
row = ["25 May", "Bloggs, Fred", "C"]
naive = ",".join(row)
print(naive, "->", len(naive.split(",")), "fields")

# CSV puts quotes around such a field, and a CSV reader finds three fields.
buf = io.StringIO()
csv.writer(buf, lineterminator="\n").writerow(row)
text = buf.getvalue()
print(text, end="")
print(next(csv.reader(io.StringIO(text))))
```

```output for=idea
7 alice /bin/sh
25 May,Bloggs, Fred,C -> 4 fields
25 May,"Bloggs, Fred",C
['25 May', 'Bloggs, Fred', 'C']
```

The first line has the shape of a line of `/etc/passwd`, where "Each line of the file describes a single user, and contains seven colon-separated fields"[^2]. A split at the colon gives the seven fields. The second record has a comma in a field, so a split at the comma gives four fields, not three. This is delimiter collision: "what happens when the delimiter character is embedded in a field value without accommodation for doing so"[^1]. The `csv` module[^3] puts quotes around the field, and its reader finds the three fields again.

## How it works

**Records and fields.** "In general a format is categorized as DSV if it is lines of delimiter-separated values (where lines are newline -separated)"[^1]. "The first row is sometimes a special record containing the column names"[^1]. DSV is a family of formats: "DSV is a categorization of data format , not a particular format"[^1]. So each file needs a convention: the delimiter, the line ending, the header row, and what to do when a field holds the delimiter or a newline.

**Three ways to handle collision.** "Some DSV conventions simply disallow a delimiter in a value while others provide a mechanism that allows for embedding delimiters"[^1]. The next block shows the three common ways on the same record.

```python run id=three-ways
import csv
import io

row = ["C:\\temp", 'say "hi"', "a:b", ""]

# 1. Forbid: TSV has no way to write a tab in a field, so check and refuse.
def tsv_line(fields):
    for f in fields:
        if "\t" in f or "\n" in f:
            raise ValueError(f"field {f!r} holds a tab or a newline")
    return "\t".join(fields)

try:
    tsv_line(["ok", "a\tb"])
except ValueError as e:
    print("forbid:", e)

# 2. Quote: CSV puts quotes around a field and doubles each quote in it.
buf = io.StringIO()
csv.writer(buf, delimiter=":", lineterminator="\n").writerow(row)
print("quote: ", buf.getvalue(), end="")
print("        back:", next(csv.reader(io.StringIO(buf.getvalue()), delimiter=":")) == row)

# 3. Escape: a backslash makes the next character ordinary, as in Unix files.
def escape(f, sep=":"):
    return f.replace("\\", "\\\\").replace(sep, "\\" + sep).replace("\n", "\\n")

def split_escaped(line, sep=":"):
    fields, cur, i = [], [], 0
    while i < len(line):
        c = line[i]
        if c == "\\" and i + 1 < len(line):
            nxt = line[i + 1]
            cur.append("\n" if nxt == "n" else nxt)
            i += 2
            continue
        if c == sep:
            fields.append("".join(cur))
            cur = []
        else:
            cur.append(c)
        i += 1
    fields.append("".join(cur))
    return fields

line = ":".join(escape(f) for f in row)
print("escape:", line)
print("        back:", split_escaped(line) == row)
```

```output for=three-ways
forbid: field 'a\tb' holds a tab or a newline
quote:  "C:\temp":"say ""hi""":"a:b":
        back: True
escape: C\:\\temp:say "hi":a\:b:
        back: True
```

- **Forbid.** TSV does not let a field hold the delimiter: "Note that fields that contain tabs are not allowable in this encoding"[^4]. The reader is a plain split, but the writer must check each field and refuse a field with a tab or a newline.
- **Quote.** CSV puts quotes around a field. "Fields containing line breaks (CRLF), double quotes, and commas should be enclosed in double-quotes"[^5], and "If double-quotes are used to enclose fields, then a double-quote appearing inside a field must be escaped by preceding it with another double quote"[^5]. The reader must track whether it is inside quotes, so a plain split does not work.
- **Escape.** Unix files put a backslash before the delimiter. "Data files in this style are expected to support inclusion of colons in the data fields by backslash escaping"[^6]. "The Unix way would be to simply escape the separator with a backslash, and have a double escape represent a literal backslash"[^6]. This gives "a single special case (the escape character) to check for when parsing the file"[^6].

The quote form and the escape form both give the record back, as the two `back: True` lines show. The escape form also needs a rule for the newline, so the code writes it as `\n`.

**Split rules.** A split at one character and a split at white space give different fields. In Python, "If sep is given, consecutive delimiters are not grouped together and are deemed to delimit empty strings"[^7]. With no separator, "runs of consecutive whitespace are regarded as a single separator, and the result will contain no empty strings at the start or end if the string has leading or trailing whitespace"[^7].

```python run id=empty-fields
# A split at one character keeps empty fields.
print("a::b:".split(":"))
print("".split(":"))

# A split at white space merges runs and drops the ends.
print("  a  b ".split())
print("  a  b ".split(" "))

# The ASCII unit separator (0x1F) and record separator (0x1E) rarely occur in text.
US, RS = "\x1f", "\x1e"
data = RS.join(US.join(r) for r in [["a, b", "c\td"], ["e", ""]])
print(repr(data))
print([r.split(US) for r in data.split(RS)])
```

```output for=empty-fields
['a', '', 'b', '']
['']
['a', 'b']
['', '', 'a', '', 'b', '']
'a, b\x1fc\td\x1ee\x1f'
[['a, b', 'c\td'], ['e', '']]
```

The record `a::b:` has four fields, and two of them are empty. A split at white space suits text that people align with spaces, but it cannot show an empty field. `awk` has the same two rules. "If FS is <space>, skip leading and trailing <blank> and <newline> characters; fields shall be delimited by sets of one or more <blank> or <newline> characters"[^8]. "Otherwise, if FS is any other character c , fields shall be delimited by each single occurrence of c"[^8]. The shell splits words at the characters of `IFS`, and "The shell shall set IFS to <space><tab><newline> when it is invoked"[^9].

The last lines of the block use two control characters as delimiters. "ASCII and Unicode include control characters that are intended to be used as delimiters: File Separator , Group Separator , Record Separator , and Unit Separator"[^1]. A comma, a tab or a newline in a field then needs no quotes. But "Use of these in DSV data is relatively uncommon"[^1], and people cannot see or type them easily.

**The Unix convention.** Many Unix files and tools use delimited text. "/etc/passwd format (one record per line, colon-separated fields) is very traditional under Unix and frequently used for tabular data"[^6]. "This format is most appropriate when the data is tabular, keyed by a name (in the first field), and records are typically short (less than 80 characters long). It works well with traditional Unix tools"[^6]. The `cut` utility takes fields: "-d delim Set the field delimiter to the character delim . The default is the <tab>"[^10]. "Old-school Unix practice used to favor tabs, a preference reflected in the defaults for cut(1) and paste(1)"[^6]. `awk` writes its output fields with `OFS`, "The print statement output field separator; <space> by default"[^8]. These tools have no quotes, so they work when no field holds the delimiter.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `str.split(sep)` keeps empty fields; `str.split()` splits at white space; the `csv` module handles quotes |
| JavaScript | stdlib | `String.prototype.split` keeps empty fields; its `limit` drops the rest of the line |
| TypeScript | stdlib | the `split` of JavaScript |
| Java | stdlib | `String.split` takes a regular expression and drops trailing empty strings unless the limit is negative |
| Kotlin | stdlib | its own `split` for strings, and the `String.split` of the JVM |
| C# | stdlib | `String.Split` with `StringSplitOptions` |
| Go | stdlib | `strings.Split`, `strings.SplitN`, `strings.Fields`; `encoding/csv` for quotes |
| Rust | stdlib | `str::split`, `splitn`, `split_whitespace` |
| C | stdlib | `strtok` merges delimiters; `strsep` (BSD) keeps empty fields |
| C++ | stdlib | `std::getline` with a delimiter character |
| Bash | native | `IFS` and `read`; the tools `cut -d` and `awk -F` |
| PowerShell | native | the `-split` operator; `Import-Csv -Delimiter` |
| SQL | stdlib | bulk loaders such as `COPY` in PostgreSQL; `split_part` in PostgreSQL |
| Regex | native | a split at a pattern; a pattern such as `[^:]*` for one field |
| HTML | n/a | markup has no fields to split |
| CSS | n/a | style rules have no fields to split |

Each language has a split, but the splits differ at the edges. Java's `split` drops empty fields at the end: "Trailing empty strings are therefore not included in the resulting array"[^11]. With a negative limit, "the pattern will be applied as many times as possible and the array can have any length"[^11]. Go's `Fields` splits at white space, and "Every element of the returned slice is non-empty"[^12]. In Rust, "Contiguous separators are separated by the empty string"[^13]. The `limit` of JavaScript "splits the string at each occurrence of the specified separator , but stops when limit entries have been placed in the array"[^14], so the rest of the line is lost.

C has two calls. `strtok` "searches the search target for the first character that is not contained in the separator string"[^15], so it skips a run of delimiters, and "This function is destructive: it writes the '\0' characters in the elements of the string str"[^15]. "The strsep () function was introduced as a replacement for strtok(3) , since the latter cannot handle empty fields"[^16]. In C++, `std::getline` with a delimiter reads one field at a time, but after the last delimiter, "If no characters were extracted for whatever reason (not even the discarded delimiter), getline sets failbit and returns"[^17], so the loop loses an empty last field.

The example files show the same record `a::b:` in each language.

## Variations and related ideas

- **CSV and TSV.** The two common members of the family have their own pages, [[csv]] with quotes and [[tsv]] with no collision at all.
- **Fixed width.** The other common way to store a table as text puts each column at fixed character positions, which limits the length of each value. Wikipedia names it as the alternative, and says that DSV "supports field values of any length"[^1].
- **Other record separators.** A format can use something other than a newline to end a record. The cookie-jar format of `fortune` "simply uses newline followed by %% (or sometimes newline followed by % ) as a record separator"[^6], and `awk` reads paragraphs as records when `RS` is empty[^8].
- **Split a string.** The split call itself is in [[split-a-string]], and the line endings that end each record are in [[line-ending]].

## Pitfalls

- **A field holds the delimiter.** A split then gives too many fields, as the first example shows. A rare delimiter does not fix it: "even though a character may seem unusual, in practice, it might be used and then result in a processing error"[^1]. Check the number of fields of each record, and use quotes or escapes for data that you do not control.
- **Empty fields vanish.** `strtok`, Java's `split` with no limit, `std::getline` and `read` in Bash lose some empty fields, as the example files show[^15][^11][^17]. A record with an empty last field then has one field fewer than the header.
- **Spaces and tabs look the same.** The Art of Unix Programming describes "the many small irritations that ensue from the fact that tabs and spaces are not visually distinguishable"[^6]. An editor that turns tabs into spaces breaks a file that uses tabs.
- **The separator is a pattern.** Java's `split` works "around matches of the given regular expression"[^11], so a separator such as `|` or `.` does not split at that character. Quote the separator.
- **A CR at the end of the last field.** A program that splits a file with CR LF endings at LF leaves a CR in the last field of each record. See [[line-ending]].
- **Quotes differ between programs.** For CSV, "because the format rules are complex and underspecified, different implementations diverge in their handling of edge cases"[^6]. Use a CSV library for CSV, and test a file from another program before you trust it.

## Cost

A split looks at each character once, so it is linear in the length of the line. It makes one new string for each field, and a program that needs only one field can stop at that field. A reader that handles quotes keeps one state, inside or outside quotes, and is still one pass. A program can read a file one record at a time, so the memory depends on the longest record, not on the size of the file. In CSV a quoted field can hold newlines, so one record can span many lines.

## References

---
tag: TCXQ0P
kind: format
slug: json-lines
name: JSON Lines
aliases: [JSONL, NDJSON, newline-delimited JSON, line-delimited JSON]
area: data-and-persistence/data-formats
level: core
summary: >-
  JSON Lines is a text format with one JSON value on each line. A program appends a record with one write and reads the file one record at a time. Logs and data dumps often use it.
see_also: [json, stream-parse-a-large-json-file, read-a-file-line-by-line, csv]
sources:
- url: https://jsonlines.org/
  title: JSON Lines, jsonlines.org
  locator: the three requirements; Conventions
- url: https://docs.python.org/3.14/library/json.html
  title: The Python Standard Library, json, Python 3.14
  locator: json.dump, the note on framing; json.dumps, indent; JSONDecodeError
- url: https://www.rfc-editor.org/rfc/rfc8259
  title: "T. Bray (Ed.), RFC 8259: The JavaScript Object Notation (JSON) Data Interchange Format, December 2017"
  locator: 2 JSON Grammar; 7 Strings
- url: https://github.com/ndjson/ndjson-spec
  title: "T. Hoeger, C. Dew, F. Pauls, J. Wilson, NDJSON: Newline delimited JSON, version 1.0.0, 2014"
  locator: 3.1 Serialization; 3.2 Parsing; 3.3 MediaType and File Extensions
- url: https://cloud.google.com/bigquery/docs/loading-data-cloud-storage-json
  title: Google Cloud documentation, Loading JSON data from Cloud Storage
  locator: the introduction
- url: https://nodejs.org/api/readline.html
  title: Node.js documentation, Readline
  locator: readline.createInterface, crlfDelay; rl[Symbol.asyncIterator]()
- url: https://pkg.go.dev/encoding/json
  title: Go standard library, package encoding/json
  locator: Encoder.Encode
- url: https://pkg.go.dev/bufio
  title: Go standard library, package bufio
  locator: MaxScanTokenSize; ErrTooLong; Scanner.Buffer; ScanLines
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.json.jsonserializer.deserializeasyncenumerable
  title: .NET API reference, JsonSerializer.DeserializeAsyncEnumerable
  locator: Overloads; the parameter topLevelValues
- url: https://jqlang.org/manual/
  title: jq manual
  locator: Invoking jq, --compact-output, --slurp, --seq
- url: https://www.rfc-editor.org/rfc/rfc7464
  title: "N. Williams, RFC 7464: JavaScript Object Notation (JSON) Text Sequences, February 2015"
  locator: 1 Introduction; 2.1 JSON Text Sequence Parsing; 2.2 JSON Text Sequence Encoding
- url: https://docs.python.org/3.14/library/stdtypes.html#str.splitlines
  title: The Python Standard Library, Built-in Types, Python 3.14
  locator: str.splitlines, the table of line boundaries
---

## What it is

JSON Lines is a text format for a sequence of JSON values. Each line of the file holds one JSON value, and a line feed ends it. The site jsonlines.org describes the format with three requirements: the text is UTF-8, "Each Line is a Valid JSON Value", and the line terminator is `\n`[^1]. The site calls it "a convenient format for storing structured data that may be processed one record at a time"[^1].

A JSON text is one value, so a list of records in plain JSON is one large array. A program must read to the closing `]` before the array is complete, and it cannot add a record without a change to the end of the file. The Python documentation says the same thing in another way: JSON "is not a framed protocol", so repeated calls to `dump()` on one file give "an invalid JSON file"[^2]. JSON Lines adds the frame. A writer appends a record with one write of one line, and a reader parses each line alone. The memory that a reader needs depends on the longest line, not on the size of the file.

This works because a JSON value never needs a raw line break. RFC 8259 allows white space, which includes the line feed, only between tokens[^3], and a string must escape the control characters below U+0020[^3]. A compact writer therefore puts each value on one line.

The same format has a second name, NDJSON (newline delimited JSON). The NDJSON specification gives the same rules for one JSON text on each line[^4]. Google BigQuery, which loads these files, says that "The ndJSON format is the same as the JSON Lines format"[^5]. Common uses are log files, exports of database tables, data sets for machine learning, and messages between processes through a pipe.

## Syntax or structure

A file is a sequence of lines. The rules of the two descriptions are close:

| Rule | JSON Lines[^1] | NDJSON[^4] |
|---|---|---|
| Encoding | UTF-8, with no byte order mark | UTF-8 |
| A line | any JSON value, "but a blank line is not" a value | a JSON text that conforms to RFC 8259 |
| Line end | `\n`; `\r\n` works because the JSON parser ignores the `\r` | `\n`, which "MAY be preceded by a carriage return" |
| Last line | a line terminator is "strongly recommended but not required" | each text is "followed by the newline character" |
| Empty lines | not valid | the parser "MAY silently ignore empty lines" |
| File name | `.jsonl` | `.ndjson` |
| Media type | `application/jsonl`, "not yet standardized" | `application/x-ndjson` |

The text below is a JSON Lines file with three records. Each value is an object, but the format allows any JSON value on a line.

```text
{"id": 1, "msg": "start"}
{"id": 2, "msg": "two\nlines"}
{"id": 3, "city": "Zürich"}
```

The second record holds a line break in its string, written as the escape `\n`. The file still has three lines.

## Examples

Python writes one line with `json.dumps` and a `"\n"`, and reads the file with a loop over its lines. `json.dumps` with no indent "selects the most compact representation"[^2], which has no line break.

```python run id=write-and-read
import json

records = [{"id": 1, "msg": "start"}, {"id": 2, "msg": "two\nlines"}, {"id": 3, "city": "Zürich"}]
text = "".join(json.dumps(r, ensure_ascii=False) + "\n" for r in records)
print(text.count("\n"), "lines")

for line in text.split("\n")[:-1]:
    print(json.loads(line))
```

```output for=write-and-read
3 lines
{'id': 1, 'msg': 'start'}
{'id': 2, 'msg': 'two\nlines'}
{'id': 3, 'city': 'Zürich'}
```

The example files of this page show the same work in other languages:

- **JavaScript.** `JSON.stringify` writes one line, and `readline` with `for await` reads the file line by line. The option `crlfDelay: Infinity` makes `\r` followed by `\n` "always be considered a single newline"[^6].
- **Go.** `Encoder.Encode` "writes the JSON encoding of v to the stream, followed by a newline character"[^7], so a sequence of calls writes JSON Lines. A `bufio.Scanner` reads the lines. Its default limit for one line is `MaxScanTokenSize = 64 * 1024` bytes[^8], and `Scanner.Buffer` sets a larger limit.
- **C#.** `JsonSerializer.Serialize` writes one line. `DeserializeAsyncEnumerable` with the parameter `topLevelValues`, "Whether to deserialize from a sequence of top-level JSON values"[^9], reads the file as a sequence of values.
- **Python.** A reader that reports the line number of a bad line, and a file that a second writer appends to.

On the command line, jq reads a sequence of JSON values with no special option. The option `--compact-output` writes "each JSON object on a single line"[^10], and `--slurp` reads "the entire input stream into a large array"[^10]. So `jq -c .` converts a sequence of pretty JSON values to JSON Lines, and `jq -s .` converts JSON Lines to one JSON array.

## Versions

There is no RFC for JSON Lines or NDJSON. Each is a short specification on a web site, and the site jsonlines.org says that the media type `application/jsonl` "is not yet standardized"[^1]. The NDJSON specification has the version 1.0.0[^4].

RFC 7464 defines a related IETF standard, JSON text sequences, with the media type `application/json-seq`[^11]. Each JSON text starts with the ASCII record separator RS (0x1E) and ends with a line feed[^11]. With RS, a parser can find the start of the next value after a value that is cut short, and a value can contain line breaks. The RFC says that a large sequence of values in one JSON array is "difficult to work with", and that a sequence can be parsed "incrementally without having to have a streaming parser"[^11]. jq writes and reads this format with the option `--seq`[^10].

## Pitfalls

- **A pretty-printed value breaks the format.** An indent puts line breaks inside the value, so each line is only part of a value:

```python wrong id=indent
import json

line = json.dumps({"id": 1, "tags": ["a", "b"]}, indent=2) + "\n"
print(line.count("\n"), "lines for one record")
try:
    json.loads(line.split("\n")[0])
except json.JSONDecodeError as error:
    print("JSONDecodeError:", error.msg)
```

```output for=indent
7 lines for one record
JSONDecodeError: Expecting property name enclosed in double quotes
```

Write each value in its most compact form. Do not pass an indent.

- **`str.splitlines` splits on more than the line feed.** The method splits on `\n`, `\r` and also on `\v`, `\f`, `\x1c`, `\x1d`, `\x1e`, `\x85`, `\u2028` and `\u2029`[^12]. JSON allows U+2028 and U+2029 in a string with no escape[^3], and `json.dumps` with `ensure_ascii=False` writes them as themselves:

```python wrong id=splitlines
import json

text = json.dumps({"note": "a\u2028b"}, ensure_ascii=False) + "\n"
print(len(text.splitlines()), "parts from splitlines")
print(len(text.split("\n")[:-1]), "line from split on \\n")
try:
    json.loads(text.splitlines()[0])
except json.JSONDecodeError as error:
    print("JSONDecodeError:", error.msg)
```

```output for=splitlines
2 parts from splitlines
1 line from split on \n
JSONDecodeError: Unterminated string starting at
```

Split on `\n` only, or iterate over the lines of a file opened in text mode, as in the examples.

- **A long line stops the Go scanner.** A `bufio.Scanner` returns the error `bufio.Scanner: token too long` for a line longer than its limit[^8]. The Go example shows this error at the default limit of 64 KiB, and the fix with `Scanner.Buffer`.
- **The last line is incomplete.** If a writer stops in the middle of a record, the file ends with part of a line. With a line terminator after each value, a reader can tell a complete last record from a cut one: a complete file ends with `\n`, and the text after the last `\n` is incomplete.

```python run id=cut-last-line
import json

text = '{"id": 1}\n{"id": 2}\n{"id": 3, "msg": "sto'
*complete, rest = text.split("\n")
for line in complete:
    print(json.loads(line))
print("incomplete:", repr(rest))
```

```output for=cut-last-line
{'id': 1}
{'id': 2}
incomplete: '{"id": 3, "msg": "sto'
```

- **Two writers share one file.** Each writer must write a whole line in one write, or lock the file. Two writers that write parts of lines can mix the bytes of two records on one line.
- **Empty lines.** A blank line is not valid JSON Lines[^1], but NDJSON lets a parser skip it[^4]. A strict reader reports an error, as in the Python example. Choose one rule and document it.
- **A byte order mark.** JSON Lines does not allow one[^1]. A strict parser rejects a first line that starts with one.

## Tools and libraries

- Python: the `json` module and a loop over the lines of the file.
- JavaScript (Node.js): `JSON.parse`, `JSON.stringify` and the `readline` module[^6].
- Go: `json.Encoder` to write, and `bufio.Scanner` with `json.Unmarshal`, or one `json.Decoder` for the whole file, to read[^7][^8].
- C#: `JsonSerializer.Serialize`, and `JsonSerializer.DeserializeAsyncEnumerable` with `topLevelValues`[^9].
- jq for the command line[^10].
- Data systems such as Google BigQuery load newline-delimited JSON[^5].

The page [[stream-parse-a-large-json-file]] reads one large JSON array, the other way to store many records in JSON. [[read-a-file-line-by-line]] shows the line loop of each language.

## References

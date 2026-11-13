---
tag: H5Z0HS
kind: task
slug: stream-parse-a-large-json-file
name: Stream-parse a large JSON file
aliases: [streaming JSON parser, incremental JSON parsing, iterate a JSON array, parse JSON that does not fit in memory, pull-parse JSON]
area: data-and-persistence/data-formats
level: advanced
summary: >-
  Read a JSON file that holds one large array, one element at a time, so that the memory used stays the same for a file of any length, and sum one member of each element.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, csharp, go]
see_also: [json, read-and-write-json, pretty-print-json, read-and-write-with-buffers]
sources:
- url: https://docs.python.org/3.14/library/json.html
  title: The Python Standard Library, json, Python 3.14
  locator: json.load; JSONDecoder.raw_decode; parse_constant
- url: https://www.rfc-editor.org/rfc/rfc8259
  title: "T. Bray (Ed.), RFC 8259: The JavaScript Object Notation (JSON) Data Interchange Format, December 2017"
  locator: 2 JSON Grammar; 8.1 Character Encoding; 9 Parsers
- url: https://github.com/FasterXML/jackson-core
  title: jackson-core, the core of the Jackson JSON processor
  locator: README, Overview; Usage, general
- url: https://serde.rs/stream-array.html
  title: Serde documentation, Array of values without buffering
- url: https://jqlang.org/manual/
  title: jq manual
  locator: Invoking jq, --stream
- url: https://docs.python.org/3.14/library/codecs.html#encodings-and-unicode
  title: The Python Standard Library, codecs, Python 3.14
  locator: Encodings and Unicode, utf-8-sig
- url: https://github.com/ICRAR/ijson
  title: ijson, an iterative JSON parser for Python
  locator: README, Usage (items, kvitems, parse, basic_parse); Backends
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/parse
  title: MDN Web Docs, JSON.parse()
  locator: Parameters, text
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder/decode
  title: MDN Web Docs, TextDecoder.decode()
  locator: Parameters, options.stream; Exceptions
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.json.jsonserializer.deserializeasyncenumerable
  title: .NET API reference, JsonSerializer.DeserializeAsyncEnumerable
  locator: Overloads; the parameter topLevelValues
- url: https://pkg.go.dev/encoding/json
  title: Go standard library, package encoding/json
  locator: func NewDecoder; Decoder.Decode, Example (Stream); Decoder.More; Decoder.Token; Decoder.UseNumber
---

## Task

A JSON file can be larger than the memory of the program that reads it. A log export or a database dump is often one large array of objects. The usual call reads the whole file and builds all the values before it returns. For example, Python `json.load` takes "the JSON document to be deserialized"[^1] and returns one Python object, so it holds the whole array. A streaming parser reads a small part of the file at a time, and gives each element to the program as soon as the element is complete. The memory then depends on the size of the largest element, not on the size of the file.

Each solution is a function `stream_sum(path)`. The file holds one JSON array of objects. The function returns two numbers: the count of the elements, and the sum of the member `amount` of all of them.

```json
[{"id": 1, "amount": 10}, {"id": 2, "amount": 25}, {"id": 3, "amount": -5}]
```

For this file the result is `[3, 30]`. An empty array gives `[0, 0]`.

The solution holds at most one element and one buffer in memory at a time. It skips all other members, also when they hold nested arrays and objects. It ignores a byte order mark at the start of the file, because RFC 8259 says that parsers "MAY ignore the presence of a byte order mark rather than treating it as an error"[^2].

The solution reports three errors:

- `invalid-json`: the file is not one JSON array. This includes a syntax error in any element, an empty file, `NaN`, and data after the closing `]`.
- `bad-record`: an element is not an object with a number member `amount`.
- `not-found`: the file does not exist.

A streaming solution finds a syntax error only when it reads the part of the file with the error. It can read many elements before it reports `invalid-json`. The function returns a result only after the closing `]`, so a caller never gets the sum of part of the file.

The contract limits the input in two ways. Each amount is an integer, and each partial sum is in the range of a double that holds integers exactly, from -(2^53 - 1) to 2^53 - 1. No object has two members with the same name.

## Solutions

The standard libraries of Java, Rust, C, C++ and Bash have no JSON parser. The libraries for these languages also have streaming interfaces. In Java, jackson-core has the incremental (streaming) parser of Jackson, and "All reading is by using `JsonParser`"[^3]. In Rust, the Serde documentation shows how to process an array "without holding the whole array in memory all at once"[^4] with a custom visitor. On the command line, `jq --stream` outputs "arrays of path and leaf values", which "is useful for processing very large inputs"[^5]. The file `matrix.yaml` records these verdicts.

- **Python.** The `json` module has no streaming parser, but `JSONDecoder.raw_decode` parses one value from the start of a string and returns the index where the value ends. The documentation says that it "can be used to decode a JSON document from a string that may have extraneous data at the end"[^1]. The solution reads the file in chunks of 65536 characters. It skips the `[`, then calls `raw_decode` at the start of each element, and checks for `,` or `]` after it. The text before the current element is dropped when the next chunk is read. The file is opened with the encoding `utf-8-sig`, which "will skip those three bytes if they appear as the first three bytes in the file"[^6]. The library ijson, "an iterative JSON parser with standard Python iterator interfaces"[^7], does the same job for any nesting, and `ijson.items` with the prefix `item` yields the elements of a top-level array, because "A literal `item` part is appended when the parser is parsing elements of a JSON array"[^7].
- **JavaScript and TypeScript (Node.js).** `JSON.parse` takes one string, "The string to parse as JSON"[^8], so it needs the whole text in memory. The solution reads the file with `readSync` in chunks of 65536 bytes, and decodes each chunk with a `TextDecoder` in `stream` mode. The flag `stream` is "Set to true if processing the data in chunks"[^9], so a UTF-8 sequence that is split between two chunks is decoded correctly. A small scanner follows the strings, the escapes and the depth of the brackets, and finds the `,` or `]` at depth 0 that ends each element. `JSON.parse` then parses the text of that element only, and it throws a `SyntaxError` if the element is not valid.
- **C#.** `JsonSerializer.DeserializeAsyncEnumerable` can "deserialize root-level JSON arrays in a streaming manner"[^10]. The solution reads each element as a `JsonElement` from a `FileStream`, and `ToBlockingEnumerable` makes the asynchronous sequence a normal one. Another overload has a parameter `topLevelValues` for a "sequence of top-level JSON values"[^10], as in JSON Lines.
- **Go.** A `json.Decoder` reads from an `io.Reader`, so it reads the file in parts. `Token` returns the `[`, then `Decode` reads one element at a time while `More` "reports whether there is another element in the current array or object being parsed"[^11]. The Go documentation has the same loop in an example that "uses a Decoder to decode a streaming array of JSON objects"[^11]. A last `Token` must return `io.EOF`, because "At the end of the input stream, Token returns nil, io.EOF"[^11]. `UseNumber` keeps each number as its text, and `Int64` converts it. Before it makes the decoder, the solution reads the first three bytes with `bufio.Reader.Peek` and discards a byte order mark.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case names a file in `fixtures/` and gives the count and the sum. The test files build the same texts in temporary files.

- `three-records`, `one-record` and `empty-array`: the basic shapes. `one-record` has no line break at the end.
- `pretty`, `crlf` and `whitespace`: the four white space characters of JSON around the tokens, with LF and CRLF line ends.
- `extra-members`: an element with nested arrays, a nested object, `true` and `null`. A nested member named `amount` is not the member of the element.
- `strings-with-brackets`: strings that hold `]`, `}`, `,`, `[`, `{`, an escaped quote and an escaped backslash. A scanner that does not follow the strings ends the element too early.
- `non-ascii`: names with `ü`, `😀` and `日本語`, which take 2, 4 and 3 bytes in UTF-8.
- `bom`: a byte order mark before the `[`.
- `big-total` and `negative-total`: the sums 9007199254740991 and -9007199254740991, which are 2^53 - 1 and its negative.
- `long-string`: a string of 70000 characters, longer than one chunk.
- `many-records`: 2500 elements. The digits of one amount are across the offset 65536, so the first chunk ends in the middle of the number.

The cases in `cases/errors.jsonl` are failures. The toolchain does not load this file yet, so the test files hold these cases by name:

- `invalid-json`: a root that is an object, a trailing comma, a missing comma, a file that ends after an element or in an element, data after the array, two arrays, an empty file, an element with no value after a name, and `NaN`.
- `bad-record`: an element that is a number or an array, an object with no `amount`, and an `amount` that is a string, `null` or `true`.
- `not-found`: a path with no file.

## Pitfalls

- **A number at the end of a chunk looks complete.** When a chunk ends in the middle of a number, the parser sees a shorter number and no error:

```python wrong id=python-cut-number
import json

decoder = json.JSONDecoder()
print(decoder.raw_decode('[{"amount": 12', 12))
print(decoder.raw_decode('[{"amount": 123}]', 12))
```

```output
(12, 14)
(123, 15)
```

The first call reads `12`, but the file holds `123`. The Python solution accepts a value only when it ends before the end of the buffer, or when the file has no more data. Otherwise it reads the next chunk and parses the element again. The case `many-records` tests this.

- **A scanner that does not know strings.** A `]` in a string is text, not the end of the array:

```python wrong id=python-naive-split
text = '[{"note": "a ] b", "amount": 1}]'
end = text.index("]")
print(text[: end + 1])
```

```output
[{"note": "a ]
```

The JavaScript scanner keeps one flag that is true in a string and one flag that is true after a backslash, and it counts brackets only outside strings.

- **A UTF-8 character across two chunks.** A chunk of bytes can end in the middle of a character. Decode each chunk with a decoder that keeps the incomplete bytes for the next chunk, as `TextDecoder` does with `stream: true`[^9].
- **The sum of part of the file.** A streaming parser yields the first elements before it reads the last bytes. If the program acts on each element as it comes, for example it writes rows to a database, a syntax error at the end of the file leaves part of the work done. Collect the results first, or make the work for each element safe to repeat.
- **The memory of one element.** A streaming parser holds each element whole. If an element can be very large, the memory is still unbounded. An event parser such as `ijson.parse` or `ijson.basic_parse`[^7], or the tokens of a Jackson `JsonParser`[^3], goes down to single values. RFC 8259 also lets a parser "set limits on the size of texts that it accepts"[^2]. Set a limit on the size of an element when the file comes from a source that you do not trust.
- **Large integers in JavaScript.** `JSON.parse` makes each number a double, so an amount above 2^53 - 1 loses digits with no error. The page [[read-and-write-json]] shows how the reviver reads the source text of a number.

## Related tasks

- [[read-and-write-json]]: parse a whole JSON text and get one value from it.
- [[pretty-print-json]]: write a JSON text with one item on each line.
- [[read-and-write-with-buffers]]: read a file in blocks of a fixed size.
- [[read-a-file-line-by-line]]: read a file with one JSON text on each line, as in JSON Lines.
- [[utf-8]]: the encoding of the file, and the lengths of the byte sequences.

## References

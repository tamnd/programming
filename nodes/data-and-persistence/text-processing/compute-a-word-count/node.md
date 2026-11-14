---
tag: JRAFFZ
kind: task
slug: compute-a-word-count
name: Compute a word count
aliases: [word count, wc, count the words of a file, count lines words and bytes]
area: data-and-persistence/text-processing
level: intro
summary: Count the lines, words and bytes of a UTF-8 file as wc does, with a word as a run of code points that do not have the Unicode property White_Space.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/wc.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), wc"
  locator: DESCRIPTION; ENVIRONMENT VARIABLES (LC_CTYPE)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap07.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 7: Locale"
  locator: 7.3.1 LC_CTYPE, the keyword space
- url: https://www.unicode.org/Public/UCD/latest/ucd/PropList.txt
  title: "Unicode Character Database, PropList.txt"
  locator: White_Space
- url: https://docs.python.org/3/library/codecs.html#codecs.IncrementalDecoder
  title: The Python Standard Library, codecs
  locator: getincrementaldecoder; IncrementalDecoder.decode
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder/TextDecoder
  title: MDN Web Docs, TextDecoder() constructor
  locator: options fatal and ignoreBOM
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder/decode
  title: MDN Web Docs, TextDecoder.decode()
  locator: the option stream; Exceptions
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/InputStreamReader.html
  title: Java SE 25 API, java.io.InputStreamReader
  locator: InputStreamReader(InputStream, CharsetDecoder)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/charset/CharsetDecoder.html
  title: Java SE 25 API, java.nio.charset.CharsetDecoder
  locator: decode, CodingErrorAction.REPORT
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.utf8encoding.-ctor
  title: .NET API reference, UTF8Encoding constructors
  locator: UTF8Encoding(Boolean, Boolean)
- url: https://pkg.go.dev/bufio#Reader.ReadRune
  title: Go standard library, package bufio
  locator: Reader.ReadRune
- url: https://pkg.go.dev/unicode#IsSpace
  title: Go standard library, package unicode
  locator: IsSpace
- url: https://doc.rust-lang.org/std/io/trait.BufRead.html#method.read_until
  title: The Rust Standard Library, std::io::BufRead
  locator: read_until
- url: https://doc.rust-lang.org/std/str/fn.from_utf8.html
  title: The Rust Standard Library, std::str::from_utf8
  locator: Errors
- url: https://doc.rust-lang.org/std/primitive.char.html#method.is_whitespace
  title: The Rust Standard Library, char
  locator: is_whitespace
- url: https://www.rfc-editor.org/rfc/rfc3629.html
  title: F. Yergeau, RFC 3629, UTF-8, a transformation format of ISO 10646 (2003)
  locator: Section 3 and Section 10
- url: https://docs.python.org/3/library/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: str.isspace; str.split
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Regular_expressions/Character_class_escape
  title: "MDN Web Docs, Character class escape: \\d, \\D, \\w, \\W, \\s, \\S"
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
  title: MDN Web Docs, Lexical grammar
  locator: White space; Line terminators
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Character.html
  title: Java SE 25 API, java.lang.Character
  locator: isWhitespace(int)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.char.iswhitespace
  title: .NET API reference, Char.IsWhiteSpace
  locator: Remarks
- url: https://en.cppreference.com/w/c/string/wide/iswspace
  title: cppreference.com, iswspace
---

## Task

Count the lines, the words and the bytes of a UTF-8 file, as `wc` does. Each solution returns three numbers:

- **lines**: the number of LF bytes (`\n`, 0x0A) in the file, the number that `wc -l` prints. A last line with no LF is not counted.
- **words**: the number of words. POSIX says: "The wc utility shall consider a word to be a non-zero-length string of characters delimited by white space."[^1] Here a word is a run of one or more code points that do not have the Unicode property White_Space.
- **bytes**: the size of the file in bytes, the number that `wc -c` prints.

POSIX does not give one set of white space characters. For `wc`, the locale category `LC_CTYPE` decides "which characters are defined as white-space characters."[^1] For the POSIX locale, the set is "exactly <space>, <form-feed>, <newline>, <carriage-return>, <tab>, and <vertical-tab>".[^2] Other locales add other characters, so one file can give different word counts. This task uses a fixed set, the 25 code points of White_Space in the Unicode Character Database[^3]:

| Code points | Characters |
|---|---|
| U+0009 to U+000D | tab, LF, VT, FF, CR |
| U+0020, U+0085, U+00A0 | space, NEXT LINE (NEL), NO-BREAK SPACE |
| U+1680, U+2000 to U+200A | OGHAM SPACE MARK and 11 spaces of fixed widths |
| U+2028, U+2029 | LINE SEPARATOR, PARAGRAPH SEPARATOR |
| U+202F, U+205F, U+3000 | NARROW NO-BREAK SPACE, MEDIUM MATHEMATICAL SPACE, IDEOGRAPHIC SPACE |

ZERO WIDTH SPACE (U+200B), SOFT HYPHEN (U+00AD), U+FEFF and the control characters U+001C to U+001F are not White_Space, so they are part of a word.

The file is UTF-8 with no byte order mark. If its bytes are not valid UTF-8, or the file does not exist, the solution reports an error and returns no result. An empty file gives (0, 0, 0). The file can be larger than the memory.

## Solutions

Each solution decodes the bytes as strict UTF-8 and keeps one flag: is the last code point part of a word?

1. Set `lines`, `words` and `bytes` to 0, and `in_word` to false.
2. Read a chunk of bytes, add its length to `bytes`, and decode it.
3. For each code point: if it is White_Space, set `in_word` to false, and add 1 to `lines` if it is LF. Else, if `in_word` is false, add 1 to `words` and set `in_word` to true.
4. At the end of the file, check that no part of a UTF-8 sequence is left, and return the three numbers.

A chunk can end in the middle of a UTF-8 sequence. So a solution decodes with an incremental decoder that keeps the start of the sequence for the next chunk, or it cuts the file at LF bytes. An LF byte is never part of a longer UTF-8 sequence, so each line decodes on its own.

- **Python.** `codecs.getincrementaldecoder('utf-8')` with `errors='strict'`, on chunks of 64 KiB that `open(path, 'rb')` gives. The last call is `decode(b'', final=True)`. "If final is true the decoder must decode the input completely and must flush all buffers."[^4] A sequence that the end of the file cuts raises `UnicodeDecodeError`. A set of the 25 code points tests for White_Space.
- **JavaScript and TypeScript (Node.js).** A `for await` loop over `fs.createReadStream(path)`, and a `TextDecoder` with `fatal: true` and `ignoreBOM: true`. With `fatal`, the decoder throws "a TypeError when decoding invalid data", and else it "will substitute malformed data with a replacement character."[^5] Each chunk goes to `decode(chunk, { stream: true })`, because `stream` must be true "if processing the data in chunks".[^6] The last call, `decode()`, throws if a sequence is cut. `ignoreBOM: true` keeps a U+FEFF at the start as a character. A `Set` of the 25 code points tests for White_Space. The TypeScript file adds types only.
- **Java.** An `InputStreamReader` with a `CharsetDecoder` for UTF-8 whose malformed action is `CodingErrorAction.REPORT`. The constructor "Creates an InputStreamReader that uses the given charset decoder."[^7] A malformed result is reported only "if the malformed action of this decoder is CodingErrorAction.REPORT"; "otherwise the malformed input will be ignored or replaced".[^8] Bad bytes throw `MalformedInputException`. A `FilterInputStream` under the reader counts the bytes. The test for White_Space is a method of its own, not `Character.isWhitespace` (see Pitfalls).
- **C#.** A `StreamReader` with `new UTF8Encoding(false, true)` and `detectEncodingFromByteOrderMarks: false`. The second argument is `throwOnInvalidBytes`, and Microsoft recommends to "enable error detection by calling a constructor with a throwOnInvalidBytes parameter and setting its value to true".[^9] Bad bytes throw `DecoderFallbackException`. After the last read, `stream.Position` is the number of bytes. Each White_Space code point is one UTF-16 unit, so the test runs on each `char`.
- **Go.** `bufio.Reader.ReadRune` with a buffer of 64 KiB. "If the encoded rune is invalid, it consumes one byte and returns unicode.ReplacementChar (U+FFFD) with a size of 1."[^10] A real U+FFFD in the file has a size of 3, so the code reports `ErrInvalidUTF8` only for the size 1. `unicode.IsSpace` "reports whether the rune is a space character as defined by Unicode's White Space property".[^11]
- **Rust.** `BufRead::read_until(b'\n', &mut line)` gives one line with its LF: it reads "until the delimiter or EOF is found".[^12] Then `std::str::from_utf8` checks the line. It "Returns Err if the slice is not UTF-8".[^13] The code maps that error to `ErrorKind::InvalidData`. `char::is_whitespace` "Returns true if this char has the White_Space property."[^14] A line is in memory as a whole, so a file of one very long line needs that much memory.
- **C.** POSIX `getline` reads one line with its LF. A function of about 30 lines decodes the UTF-8 by RFC 3629 and refuses overlong forms, surrogates and code points above U+10FFFF.[^15] On bad bytes, `word_count` returns -1 and sets `errno` to `EILSEQ`. A function of its own tests for White_Space, because `iswspace` depends on the locale (see Pitfalls).
- **C++.** `std::getline` on a binary `std::ifstream`, the same decoder as in C, and `std::expected` (C++23) for the result. The errors are `cannot_open`, `invalid_utf8` and `read_failed`.
- **Bash.** `iconv -f UTF-8 -t UTF-8` checks the bytes and stops with a nonzero status on bad bytes. `wc -l` and `wc -c` give the lines and the bytes. For the words, `sed` in the C locale changes the UTF-8 bytes of each White_Space code point above U+007F to a space. Then `wc -w` counts in the C locale, where the white space is the six characters of the POSIX locale.[^2] After `iconv` has checked the file, a match of those bytes cannot start inside another character.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`. The result is (lines, words, bytes).

- `simple`, `multiple-spaces` and `tabs-and-lines`: a run of white space, at the start, in the middle or at the end, does not make an empty word.
- `empty`: (0, 0, 0). `only-white-space`: lines but no words, (2, 0, 4).
- `no-final-newline`: `one two` gives (0, 2, 7). The last line is not counted, as in `wc -l`.
- `crlf`: CR is White_Space, so CRLF does not change the words, and each CRLF holds one LF.
- `non-ascii`: `naïve café` has 10 characters and 12 bytes before the LF. The bytes count is 13, not 11.
- `unicode-spaces` and `nel`: NO-BREAK SPACE, IDEOGRAPHIC SPACE, LINE SEPARATOR and NEL separate words. LINE SEPARATOR and NEL do not add to `lines`.
- `not-white-space`: ZERO WIDTH SPACE, SOFT HYPHEN and U+FEFF do not separate words, so `a`, `b`, `c` and `d` with them between are one word.
- `control-separators`: U+001C and U+001F are not White_Space, so (1, 1, 6).

The tests also write a file of 100000 lines `a b`, which is larger than one chunk, and a file in which the first 64 KiB end inside the 3 bytes of U+3000. The cases in `cases/errors.jsonl` must give the error `invalid-utf8`: the bytes `FF FE`, a sequence that the end of the file cuts (`E2 82`) and an encoded surrogate (`ED A0 80`). RFC 3629 says that UTF-8 prohibits "encoding character numbers between U+D800 and U+DFFF".[^15] A missing file must give `not-found`. The toolchain does not load that file yet, so each test file holds these cases by name.

## Pitfalls

- **Each language has its own white space.** The function or pattern that a language calls white space is often not White_Space. Python `str.split()` with no argument splits at runs of whitespace.[^16] For Python, a character is whitespace if its general category is Zs, or if its bidirectional class is WS, B or S.[^16] The bidirectional class of U+001C to U+001E is B, and of U+001F it is S, so `split` cuts a word there:

```python run id=python-split
# Six code points between 'a' and 'b'. Only the first three have the
# Unicode property White_Space.
samples = ['\u00a0', '\u0085', '\u2028', '\u001c', '\u200b', '\ufeff']
print('code point: parts')
for sep in samples:
    text = 'a' + sep + 'b'
    print(f'U+{ord(sep):04X}: {len(text.split())}')
```

```output for=python-split
code point: parts
U+00A0: 2
U+0085: 2
U+2028: 2
U+001C: 2
U+200B: 1
U+FEFF: 1
```

In JavaScript, `\s` "Matches any whitespace or line terminator character."[^17] That set has U+FEFF, and for U+0085, MDN says: "U+0085 NEXT LINE has no special role".[^18] So `\s` does not cut at NEL, but it cuts at U+FEFF:

```javascript run id=javascript-split
// Six code points between 'a' and 'b'. Only the first three have the
// Unicode property White_Space.
const samples = ['\u00a0', '\u0085', '\u2028', '\u001c', '\u200b', '\ufeff'];
console.log('code point: parts');
for (const sep of samples) {
  const text = 'a' + sep + 'b';
  const hex = sep.codePointAt(0).toString(16).toUpperCase().padStart(4, '0');
  console.log(`U+${hex}: ${text.split(/\s+/).length}`);
}
```

```output for=javascript-split
code point: parts
U+00A0: 2
U+0085: 1
U+2028: 2
U+001C: 1
U+200B: 1
U+FEFF: 2
```

Java `Character.isWhitespace` is true if the character "is a Unicode space character ( SPACE_SEPARATOR , LINE_SEPARATOR , or PARAGRAPH_SEPARATOR ) but is not also a non-breaking space", and for U+0009 to U+000D and U+001C to U+001F.[^19] The non-breaking spaces are U+00A0, U+2007 and U+202F. So `isWhitespace` is false for NO-BREAK SPACE and NEL, and true for U+001C:

```java run id=java-iswhitespace
public class Main {
    public static void main(String[] args) {
        // Six code points. Only the first three have the Unicode property White_Space.
        int[] samples = {0x00a0, 0x0085, 0x2028, 0x001c, 0x200b, 0xfeff};
        for (int cp : samples) {
            System.out.printf("U+%04X: isWhitespace %b%n", cp, Character.isWhitespace(cp));
        }
    }
}
```

```output for=java-iswhitespace
U+00A0: isWhitespace false
U+0085: isWhitespace false
U+2028: isWhitespace true
U+001C: isWhitespace true
U+200B: isWhitespace false
U+FEFF: isWhitespace false
```

.NET `Char.IsWhiteSpace` lists the Space_Separator category, U+2028, U+2029, U+0009 to U+000D and "NEXT LINE (U+0085)".[^20] That is the White_Space set. Go `unicode.IsSpace`[^11] and Rust `char::is_whitespace`[^14] also use White_Space. Pick one definition, say which one in the code, and test it with the cases above.

- **`wc -w` depends on the locale.** The locale decides which characters are white space,[^1] and C `iswspace` adds "any whitespace character specific to the current locale".[^21] Two systems, or two locales on one system, can give two word counts for one file. Set the locale, or count with a fixed set, as the Bash solution does.
- **Replacement of bad bytes.** A decoder that is not strict puts U+FFFD in place of bad bytes, and U+FFFD is not White_Space. So a word count over a file in Latin-1 gives a result and no error. Use a strict decoder: `errors='strict'`, `fatal: true`, `CodingErrorAction.REPORT` or `throwOnInvalidBytes: true`.
- **A chunk that cuts a character.** A sequence of 2 to 4 bytes can start at the end of one chunk and end in the next. If each chunk is decoded on its own, a strict decoder reports an error for valid text, and a decoder that is not strict puts U+FFFD in place of the character. Keep the state of the decoder across chunks, or cut the file at LF bytes.
- **Counting `split` parts.** `text.split(' ')` gives an empty string for each extra space and for a space at the start or the end. Count runs of characters that are not white space, not the parts of a split at one character.
- **Characters are not bytes.** The `bytes` number is the size of the file. The length of the decoded string is a number of code points in Python and of UTF-16 units in JavaScript, Java and C#, and both are different from the number of bytes for text that is not ASCII.

## Related tasks

- [[count-the-lines-of-a-file]]: count the LF bytes without decoding the file.
- [[compute-word-frequencies]]: count how often each word occurs.
- [[read-a-file-line-by-line]]: read a UTF-8 file one line at a time.
- [[line-ending]]: LF, CRLF and CR.

## References

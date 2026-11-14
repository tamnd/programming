---
tag: XB02RR
kind: concept
slug: line-ending
name: Line ending
aliases: [newline, end of line, EOL, line break, line terminator, LF, CRLF, carriage return, line feed, universal newlines]
area: data-and-persistence/text-processing
level: intro
summary: >-
  A line ending marks the end of a line of text. Unix uses LF, Windows uses CR LF and classic Mac OS used CR, so a reader must know which endings to accept and whether to keep them.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1964
  people: []
  note: Teletype printers needed a carriage return and a line feed to start a new line, so many early systems used CR LF. Multics began in 1964 and used LF alone, and Unix followed it. MS-DOS took CR LF from CP/M, and Windows kept it.
see_also: [read-a-file-line-by-line, string-concept, utf-8, csv, git]
same_as:
  wikidata: Q184914
  wikipedia: Newline
sources:
- url: https://en.wikipedia.org/wiki/Newline
  title: Newline, Wikipedia
  locator: Introduction, Representation, Unicode, History, Issues with different newline formats
- url: https://docs.python.org/3/library/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: str.splitlines
- url: https://docs.python.org/3/library/functions.html
  title: The Python Standard Library, Built-in Functions
  locator: open, the newline parameter
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: The Open Group, POSIX.1-2024, Base Definitions, Definitions
  locator: 3.172 Incomplete Line, 3.185 Line, 3.387 Text File
- url: https://en.cppreference.com/w/c/io/FILE.html
  title: FILE, cppreference.com
  locator: Binary and text modes
- url: https://docs.python.org/3/glossary.html
  title: Python Glossary
  locator: universal newlines
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/BufferedReader.html
  title: BufferedReader, Java SE 25 API
  locator: readLine
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.streamreader.readline
  title: StreamReader.ReadLine Method, .NET API, Microsoft Learn
  locator: Remarks
- url: https://nodejs.org/api/readline.html
  title: Node.js, Readline
  locator: Event 'line', readline.createInterface, crlfDelay
- url: https://pkg.go.dev/bufio
  title: Go standard library, package bufio
  locator: ScanLines
- url: https://doc.rust-lang.org/std/io/trait.BufRead.html
  title: Rust standard library, trait std::io::BufRead
  locator: read_line, lines
- url: https://en.cppreference.com/w/cpp/string/basic_string/getline
  title: std::getline, cppreference.com
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html
  title: System, Java SE 25 API
  locator: lineSeparator
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Formatter.html
  title: Formatter, Java SE 25 API
  locator: Line Separator
- url: https://learn.microsoft.com/en-us/dotnet/api/system.environment.newline
  title: Environment.NewLine Property, .NET API, Microsoft Learn
  locator: Property Value
- url: https://nodejs.org/api/os.html
  title: Node.js, OS
  locator: os.EOL
- url: https://www.rfc-editor.org/rfc/rfc5322.txt
  title: 'RFC 5322: Internet Message Format'
  locator: 2.1 General Description, 2.1.1 Line Length Limits
- url: https://html.spec.whatwg.org/multipage/parsing.html
  title: HTML Standard, Parsing HTML documents
  locator: 13.2.3.5 Preprocessing the input stream
- url: https://drafts.csswg.org/css-syntax/
  title: CSS Syntax Module Level 3
  locator: 3.3 Preprocessing the input stream
- url: https://git-scm.com/docs/gitattributes
  title: Git, gitattributes Documentation
  locator: Effects, text, eol, End-of-line conversion
- url: https://git-scm.com/docs/git-config
  title: Git, git-config Documentation
  locator: core.autocrlf, core.eol, core.safecrlf
- url: https://docs.python.org/3/library/re.html
  title: The Python Standard Library, re
  locator: Regular Expression Syntax, $
---

## Idea

Text is a sequence of lines, and a line ending marks where one line stops. Three endings are common. Unix-like systems use LF, the byte 0x0A. Windows uses CR LF, the bytes 0x0D 0x0A, and classic Mac OS used CR alone[^1]. A file from one system can come to a program on another system, and a file can mix the endings. So a program that reads lines must choose which endings it accepts, and whether it keeps the ending or removes it.

```python run id=idea
import os
import tempfile

text = "one\ntwo\r\nthree\rfour"
print(text.encode("ascii"))

# split("\n") knows only LF. splitlines() knows LF, CRLF, CR and more.
print(text.split("\n"))
print(text.splitlines())

with tempfile.TemporaryDirectory() as tmp:
    path = os.path.join(tmp, "mixed.txt")
    with open(path, "wb") as f:
        f.write(text.encode("ascii"))

    # Text mode with the default newline=None translates each ending to "\n".
    with open(path) as f:
        print(repr(f.read()))

    # newline="" finds the same lines but keeps each ending as it is.
    with open(path, newline="") as f:
        print(list(f))
```

```output for=idea
b'one\ntwo\r\nthree\rfour'
['one', 'two\r', 'three\rfour']
['one', 'two', 'three', 'four']
'one\ntwo\nthree\nfour'
['one\n', 'two\r\n', 'three\r', 'four']
```

The text has one line of each kind. A split at `\n` knows only LF, so the CR of `two\r` stays, and the lone CR does not split `three` from `four`. `splitlines()` breaks "at line boundaries"[^2] and finds all four lines. A file opened in text mode gives the same four lines: with the default `newline=None`, "Lines in the input can end in '\n' , '\r' , or '\r\n' , and these are translated into '\n' before being returned to the caller"[^3]. With `newline=''`, "universal newlines mode is enabled, but line endings are returned to the caller untranslated"[^3].

## How it works

**A line in POSIX.** POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character"[^4]. So the newline is part of the line, and the last line of a file also ends with one. Text after the last newline has its own name, an incomplete line: "A sequence of one or more non-<newline> characters at the end of the file"[^4]. A text file is "A file that contains characters organized into zero or more lines"[^4].

```python run id=count
# POSIX counts a line only when it ends with a newline.
for data in [b"a\nb\n", b"a\nb", b"", b"\n"]:
    newlines = data.count(b"\n")
    print(f"{data!r:10} newlines={newlines} splitlines={len(data.splitlines())}")
```

```output for=count
b'a\nb\n'  newlines=2 splitlines=2
b'a\nb'    newlines=1 splitlines=2
b''        newlines=0 splitlines=0
b'\n'      newlines=1 splitlines=1
```

The two counts differ only when the last line has no newline. For `b'a\nb'`, a count of the newline characters gives 1, because `b` is an incomplete line, but `splitlines()` gives 2. A tool that follows the POSIX definition counts newlines, and a reader that returns the last piece of text counts one more.

**Text mode and binary mode.** C has two kinds of stream. "A text stream is an ordered sequence of characters that can be composed into lines; a line can be decomposed into zero or more characters plus a terminating '\n' (“newline”) character"[^5]. "Whether the last line requires a terminating '\n' is implementation-defined"[^5]. A text stream can change the bytes: "characters may have to be added, altered, or deleted on input and output"[^5]. C streams on Windows write each `'\n'` as CR LF, and read each CR LF as `'\n'`[^5]. A binary stream does not change them: "Data read in from a binary stream always equal the data that were earlier written out to that stream"[^5].

Python calls its way of reading universal newlines: "A manner of interpreting text streams in which all of the following are recognized as ending a line: the Unix end-of-line convention '\n' , the Windows convention '\r\n' , and the old Macintosh convention '\r'"[^6]. It also changes the text on output. "When writing output to the stream, if newline is None , any '\n' characters written are translated to the system default line separator, os.linesep"[^3], and "If newline is '' or '\n' , no translation takes place"[^3].

**Line readers.** Each language has a call that reads one line, and the calls do not agree on the endings:

- Java: "A line is considered to be terminated by any one of a line feed ('\n'), a carriage return ('\r'), a carriage return followed immediately by a line feed, or by reaching the end-of-file (EOF)"[^7].
- .NET: "A line is defined as a sequence of characters followed by a line feed ("\n"), a carriage return ("\r"), or a carriage return immediately followed by a line feed ("\r\n")"[^8].
- Node.js: "The 'line' event is emitted whenever the input stream receives an end-of-line input ( \n , \r , or \r\n )"[^9].
- Go: for `bufio.ScanLines`, "The end-of-line marker is one optional carriage return followed by one mandatory newline"[^10]. A CR alone does not end a line. "The last non-empty line of input will be returned even if it has no newline"[^10].
- Rust: `lines` removes the ending: "Each string returned will not have a newline byte (the 0xA byte) or CRLF ( 0xD , 0xA bytes) at the end"[^11]. `read_line` keeps it: "all bytes up to, and including, the delimiter (if found) will be appended to buf"[^11].
- C++: `std::getline` stops at `'\n'`, and "the delimiter character is extracted from input , but is not appended to str"[^12]. The CR of a CR LF stays in the string.

The example files show each reader on the same text.

**The ending of the platform.** A program that writes text can ask for the ending of its system. Java's `System.lineSeparator()` "Returns the system-dependent line separator string"[^13], which is `\n` on UNIX systems and `\r\n` on Microsoft Windows[^13]. In a Java format string, `%n` gives "the platform-specific line separator"[^14]. .NET has `Environment.NewLine`, which is "\r\n for non-Unix platforms, or \n for Unix platforms"[^15]. Node.js has `os.EOL`, "The operating system-specific end-of-line marker"[^16], and Python has `os.linesep`[^3].

**Protocols.** Many network formats fix the ending. "Most textual Internet protocols (including HTTP , SMTP , FTP , IRC , and many others) mandate the use of ASCII CR + LF ( \r\n , 0x0D 0x0A ) on the protocol level, but recommend that tolerant applications recognize lone LF ( \n , 0x0A ) as well"[^1]. For an email message, "A line is a series of characters that is delimited with the two characters carriage-return and line-feed"[^17], and "Each line of characters MUST be no more than 998 characters, and SHOULD be no more than 78 characters, excluding the CRLF"[^17].

**Unicode.** Unicode lists more line terminators than LF, CR and CR LF: vertical tab U+000B, form feed U+000C, next line U+0085, line separator U+2028 and paragraph separator U+2029[^1]. Python's `splitlines()` splits at these and also at the file, group and record separators `\x1c`, `\x1d` and `\x1e`[^2]. So `splitlines()` can split a line that a Unix tool sees as one line.

**Web formats.** HTML and CSS remove the CR before they parse. For HTML, "Before the tokenization stage, the input stream must be preprocessed by normalizing newlines"[^18], so "there are never any U+000D CR characters in the input to the tokenization stage"[^18]. CSS does the same: "Replace any U+000D CARRIAGE RETURN (CR) code points , U+000C FORM FEED (FF) code points , or pairs of U+000D CARRIAGE RETURN (CR) followed by U+000A LINE FEED (LF) in input by a single U+000A LINE FEED (LF) code point"[^19].

**Git.** Git can store each text file with LF and write the ending of the platform to the work tree. When a file has the `text` attribute, "When a matching file is added to the index, the file’s line endings are normalized to LF in the index"[^20]. On checkout, "the default is eol=crlf on Windows and eol=lf on all other platforms"[^20]. The older setting `core.autocrlf` does the same for all files: "Setting this variable to "true" is the same as setting the text attribute to "auto" on all files and core.eol to "crlf""[^21].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `open` with universal newlines and its `newline` argument; `str.splitlines`; `os.linesep` |
| JavaScript | stdlib | `split(/\r\n\|\r\|\n/)`; in Node.js, the `readline` module and `os.EOL` |
| TypeScript | stdlib | the JavaScript calls, with the types of `@types/node` |
| Java | stdlib | `BufferedReader.readLine` ends a line at LF, CR or CR LF; `System.lineSeparator()` and `%n` |
| Kotlin | stdlib | the `BufferedReader` and `System.lineSeparator()` of the JVM |
| C# | stdlib | `StreamReader.ReadLine` ends a line at LF, CR or CR LF; `Environment.NewLine` |
| Go | stdlib | `bufio.Scanner` with `ScanLines` removes LF or CR LF; a CR alone does not end a line |
| Rust | stdlib | `BufRead::lines` removes LF or CR LF; `read_line` keeps the ending |
| C | stdlib | `fgets` keeps the `'\n'`; a text stream translates the ending of the system |
| C++ | stdlib | `std::getline` removes the `'\n'` and keeps a CR |
| Bash | native | `read -r` removes the LF and keeps a CR |
| PowerShell | stdlib | the .NET `StreamReader.ReadLine` and `Environment.NewLine` |
| SQL | n/a | a string value can hold CR and LF, but SQL has no lines of a file |
| Regex | native | the dot does not match a newline; `$` matches before a final newline; `\r?\n` matches LF and CR LF |
| HTML | native | the parser turns CR LF and CR into LF before it makes tokens |
| CSS | native | the preprocessing turns CR LF, CR and form feed into LF |

The readers fall in two groups. Java, .NET, Node.js and Python in text mode accept LF, CR and CR LF, and remove the ending[^7][^8][^9][^3]. Go, Rust, C++ and Bash look for LF only, so a CR alone does not end a line, and C++ and Bash keep the CR of a CR LF[^10][^11][^12]. In a Python pattern, `$` "Matches the end of the string or just before the newline at the end of the string"[^22], so a CR before that newline is part of the text that the pattern sees.

## Variations and related ideas

- **Keep or remove.** A reader that keeps the ending, such as `read_line` in Rust or `fgets` in C, lets a program write each line back with the ending it had[^11]. A reader that removes it gives the text only.
- **Other separators.** IBM mainframes with EBCDIC use the character NL, 0x15, as their line ending[^1]. Unicode has its own line separator and paragraph separator, U+2028 and U+2029[^1].
- **Lines in a record format.** A line is the unit of many text formats, such as [[csv]] files and email messages. The format decides the ending, as RFC 5322 does for email[^17].
- **Read line by line.** See [[read-a-file-line-by-line]] for the read loop, and [[utf-8]] for the encoding of the bytes between the endings.

## Pitfalls

- **A split at LF keeps the CR.** A program that splits a CR LF file at each `\n` gets lines with a CR at the end, as the first example shows. A comparison with `two` then fails, and the CR can move the cursor when the line is printed. Use a reader that knows CR LF, or remove a final CR from each line.
- **A script with CR LF endings.** Wikipedia notes the problem of "a UNIX shell script written in a Windows text editor like Notepad"[^1]. Bash reads each command with a CR at its end, so a command or a file name has an extra character, as the Bash example shows.
- **The last line has no newline.** POSIX calls it an incomplete line[^4]. A count of newlines does not count it, and a loop such as `while read` in Bash can lose it, as the Bash example shows. A program that writes text should end the last line with a newline.
- **Text mode changes binary data.** A text stream can add, change or remove characters[^5], and Python in text mode changes each `'\n'` that it writes to `os.linesep`[^3]. Open images, archives and other binary files in binary mode.
- **A mixed file in Git.** "A file that contains a mixture of LF and CRLF before the commit cannot be recreated by Git"[^21]. Normalize the endings of a mixed file before it goes into a repository with `text` or `core.autocrlf`.
- **CR and LF in two reads.** A stream can give the CR in one chunk and the LF in the next. Node.js has an option for this: "If the delay between \r and \n exceeds crlfDelay milliseconds, both \r and \n will be treated as separate end-of-line input"[^9].
- **LF where the protocol needs CR LF.** "Another common problem is the use of \n when communicating using an Internet protocol that mandates the use of ASCII CR + LF for ending lines"[^1]. Write `\r\n` explicitly in protocol code, and do not use the ending of the platform.

## Cost

A line ending costs one byte per line with LF and two bytes per line with CR LF. A reader looks at each character once to find the endings, so a split or a translation is linear in the length of the text. A reader that accepts a CR alone must look at the next character after each CR, to see whether it is an LF. Translation in text mode and the normalization of Git are one more pass over the bytes.

## History

Teletype printers needed two actions to start a new line, a carriage return and a line feed, so many early systems with a Teletype as their console used the sequence CR LF[^1]. From 1963 to 1968, "the ISO draft standards supported the use of either carriage return plus line feed ( CR + LF ) or LF alone as a newline, while the ASA drafts supported only CR + LF"[^1]. "The Multics operating system began development in 1964 and used LF alone as its newline"[^1], and "Multics used a device driver to translate this character to whatever sequence a printer needed"[^1]. "Unix followed the Multics practice, and later Unix-like systems followed Unix"[^1]. "MS-DOS (1981) adopted CP/M 's CR + LF in order to be compatible, and this convention was inherited by Microsoft's later Windows operating system"[^1]. Classic Mac OS, the Apple II and the Commodore 64 used CR alone[^1].

## References

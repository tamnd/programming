---
tag: MNA48N
kind: task
slug: sort-the-lines-of-a-file
name: Sort the lines of a file
aliases: [sort a text file, sort lines in byte order, sort lines by code point, LC_ALL=C sort]
area: data-and-persistence/text-processing
level: intro
summary: Read a UTF-8 file, split it at each LF, and return its lines sorted by code point, which is the order of the UTF-8 bytes and the order of LC_ALL=C sort.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sort.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), sort"
  locator: DESCRIPTION; OPTIONS (-n); INPUT FILES
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 3, Definitions"
  locator: 3.172 Incomplete Line; 3.185 Line
- url: https://www.rfc-editor.org/rfc/rfc3629
  title: "RFC 3629, UTF-8, a transformation format of ISO 10646"
  locator: 1. Introduction
- url: https://docs.python.org/3/library/functions.html#open
  title: The Python Standard Library, Built-in Functions
  locator: open (newline)
- url: https://docs.python.org/3/reference/expressions.html#value-comparisons
  title: The Python Language Reference, Expressions
  locator: Value comparisons
- url: https://nodejs.org/api/buffer.html#static-method-buffercomparebuf1-buf2
  title: Node.js documentation, Buffer
  locator: Buffer.compare(buf1, buf2)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
  title: Java SE 25 API, java.util.Arrays
  locator: compareUnsigned(byte[], byte[])
- url: https://learn.microsoft.com/en-us/dotnet/api/system.memoryextensions.sequencecompareto
  title: .NET API reference, MemoryExtensions.SequenceCompareTo
- url: https://go.dev/ref/spec#Comparison_operators
  title: The Go Programming Language Specification
  locator: Comparison operators
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: The Rust Standard Library, str
  locator: lines; impl Ord for str
- url: https://en.cppreference.com/w/c/string/byte/memcmp
  title: cppreference.com, memcmp
- url: https://en.cppreference.com/w/cpp/string/char_traits/cmp
  title: cppreference.com, std::char_traits::eq, lt
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap07.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 7, Locale"
  locator: 7.3.2.6 LC_COLLATE Category in the POSIX Locale
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, Array.prototype.sort()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: compareTo
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.compareordinal
  title: .NET API reference, String.CompareOrdinal
- url: https://docs.python.org/3/builtins/stdtypes.html#str.splitlines
  title: The Python Standard Library, Built-in Types
  locator: str.splitlines
---

## Task

Read a text file and return its lines in sorted order. This is the job of the `sort` utility with no options. POSIX says that `sort` shall "Sort lines of all the named files together and write the result to the specified output."[^1]

The task needs three rules: what a line is, what the order is, and what to do with the lines that are equal. This page uses these rules:

- **A line ends at LF.** POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character."[^2] The LF is not part of the line in the result. A CR before the LF stays in the line, so a file with CRLF line ends gives lines that end in CR.
- **A last line with no LF is a line too.** POSIX calls it an incomplete line: "A sequence of one or more non-<newline> characters at the end of the file."[^2] So the file `b`, LF, `a` gives two lines, `a` and `b`.
- **An empty line is a line.** The file that holds only one LF gives one empty line. An empty file gives no lines.
- **The order is the order of the code points.** The file is UTF-8, and RFC 3629 says: "The byte-value lexicographic sorting order of UTF-8 strings is the same as if ordered by character numbers."[^3] So a solution can compare the bytes of the lines, and the result is the order of the code points. If one line is the start of a longer line, the shorter line comes first.
- **Equal lines stay.** Duplicate lines all stay in the result. Equal lines have the same bytes, so their order in the result is not important.

This order is the order of `LC_ALL=C sort`. It is not the order of a dictionary: all the uppercase ASCII letters come before all the lowercase ASCII letters, and `10` comes before `9`. If the file does not exist, the solution reports an error.

## Solutions

Each solution reads the whole file into memory, splits it at each LF, and sorts the lines. The sort is the sort of the language. The comparison is the part that needs care, because some languages compare strings in UTF-16 code units, and that order is not the order of the code points (see Pitfalls). The steps:

1. Read all the bytes of the file.
2. Split the bytes at each LF. If the last part is empty, remove it: it is the part after the LF at the end of the last line, or it is the only part of an empty file.
3. Sort the lines by their bytes, or by their code points.
4. Return the lines as strings.

- **Python.** `open(path, encoding='utf-8', newline='')` reads the file as text. With `newline=''`, "universal newlines mode is enabled, but line endings are returned to the caller untranslated",[^4] so `read()` keeps each CR. `split('\n')` splits at LF only. `list.sort()` sorts the lines, and "Strings (instances of `str`) compare lexicographically using the numerical Unicode code points (the result of the built-in function `ord()`) of their characters."[^5]
- **JavaScript and TypeScript (Node.js).** `readFileSync(path)` gives a `Buffer`. A loop with `indexOf(0x0a, start)` and `subarray` cuts it into lines. `lines.sort(Buffer.compare)` sorts them: Node.js says that `Buffer.compare` "Compares `buf1` to `buf2`, typically for the purpose of sorting arrays of `Buffer` instances."[^6] Then `toString('utf8')` changes each line to a string. The TypeScript file adds types only.
- **Java.** `Files.readAllBytes(path)` gives the bytes, and `Arrays.copyOfRange` copies each line to a `byte[]`. `lines.sort(Arrays::compareUnsigned)` sorts them. `compareUnsigned` "Compares two byte arrays lexicographically, numerically treating elements as unsigned."[^7] A Java `byte` is signed, and `Arrays.compare` compares two elements "as if by Byte.compare(byte, byte)", so it puts the bytes above 0x7F before the ASCII bytes.[^7] `new String(line, StandardCharsets.UTF_8)` changes each line to a string.
- **C#.** `File.ReadAllBytes(path)` gives the bytes. `Array.IndexOf` finds each LF, and `AsMemory` gives each line with no copy. The sort compares `a.Span.SequenceCompareTo(b.Span)`, which "Determines the relative order of two read-only sequences by comparing their elements" with `IComparable<T>.CompareTo`.[^8] The elements are `byte` values, which are unsigned. `Encoding.UTF8.GetString` changes each line to a string.
- **Go.** `os.ReadFile(path)` gives the bytes, and `strings.Split` at each LF gives the lines. `slices.Sort(lines)` sorts them. The Go specification says: "Two string values are compared lexically byte-wise."[^9]
- **Rust.** `fs::read_to_string(path)` reads the file and checks that it is UTF-8. `text.split('\n')` gives the lines, and `sort_unstable` sorts them. For `str`, "Strings are ordered lexicographically by their byte values."[^10] Equal lines have the same bytes, so an unstable sort gives the same result as a stable sort.
- **C.** A loop of `fread` reads the file into a buffer that grows. The code changes each LF to a NUL, so each line is a C string in the buffer, and keeps a pointer and a length for each line. `qsort` sorts them with `memcmp` on the shorter length, and then the shorter line first. `memcmp` compares bytes "both interpreted as unsigned char".[^11] `sort_lines` fills a `struct sorted_lines`, which the caller frees with `sort_lines_free`, or it returns -1 and sets `errno`.
- **C++.** A `std::istreambuf_iterator<char>` reads a binary `std::ifstream` into a `std::string`. `find('\n', start)` cuts it into a `std::vector<std::string>`, and `std::ranges::sort` sorts it. `std::string` compares its chars with `std::char_traits<char>`, and for `char`, `lt` "behaves identically to `static_cast<unsigned char>(a) < static_cast<unsigned char>(b)`".[^12] So the order is the order of the bytes, also on a machine where `char` is signed. The result is a `std::expected` (C++23), and the errors are `cannot_open` and `read_failed`.
- **Bash.** `LC_ALL=C sort` on the file does the work. POSIX says that the comparison "shall be performed using the collating sequence of the current locale",[^1] so `LC_ALL=C` selects the POSIX locale, where the order is the order of the bytes (see Pitfalls). The output is the lines, each with an LF at the end. `sort` adds the LF that a last line does not have: it adds one "to the end of a file ending with an incomplete last line".[^1]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`.

- `simple`: `pear`, `apple`, `fig` gives `apple`, `fig`, `pear`.
- `case`: `b`, `B`, `a`, `A` gives `A`, `B`, `a`, `b`. The uppercase letters are 0x41 to 0x5A, and the lowercase letters are 0x61 to 0x7A.
- `numbers`: `10`, `9`, `100` gives `10`, `100`, `9`. The lines are text, so `1` comes before `9`.
- `empty`: an empty file gives `[]`.
- `one-empty-line` and `empty-lines`: the file that holds only one LF gives one empty line, `""`. In `b`, an empty line, `a` and an empty line, the two empty lines come first.
- `duplicates`: `b`, `a`, `b` gives `a`, `b`, `b`.
- `no-final-newline`: `b`, LF, `a` with no LF at the end gives `a` and `b`.
- `crlf`: `b` CRLF `a` CRLF gives `a` CR and `b` CR. The CR stays in each line.
- `prefix`: `ab` and `a` gives `a`, `ab`. The shorter line comes first.
- `whitespace`: `b`, space `a`, tab `c` gives tab `c`, space `a`, `b`. Tab is 0x09 and space is 0x20, so both come before the letters.
- `non-ascii`: `é`, `z`, `e` gives `e`, `z`, `é`. U+00E9 is above U+007A, and its UTF-8 bytes C3 A9 are above the byte 7A.
- `supplementary`: U+1F600 and U+FF61 gives U+FF61 first. In UTF-16, U+1F600 is the two code units D83D DE00, and D83D is below FF61, so a sort of UTF-16 code units gives the other order.
- `other-line-breaks`: three lines `b` U+2028 `a`, `b` U+001C `a` and `b` U+000C `a` give U+000C first, then U+001C, then U+2028. These characters are not LF, so they do not split a line.

The tests also write a file of 50000 lines `b` and 50000 lines `a`, one after the other. The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **The locale changes the order of `sort`.** POSIX says that `sort` compares lines "using the collating sequence of the current locale".[^1] In a locale such as `en_US.UTF-8`, the order is close to the order of a dictionary, and it is different from one system to the next. In the POSIX locale, the order of the ASCII characters is fixed: "Characters in this list are in the same order" as in ASCII, and "If the listed characters have ASCII encoding, the other characters shall be in ascending order according to their coded character set values".[^13] So set `LC_ALL=C` when a program or a test needs the same order on each machine.
- **Numbers sort as text.** With no options, `sort` compares the characters, so `100` comes before `9`. The option `-n` is for numbers: "Restrict the sort key to an initial numeric string".[^1] The task of this page is the text order:

```bash run id=bash-sort-numbers
printf '10\n9\n100\n' | LC_ALL=C sort
echo ---
printf '10\n9\n100\n' | LC_ALL=C sort -n
```

```output for=bash-sort-numbers
10
100
9
---
9
10
100
```

- **UTF-16 order is not code point order.** JavaScript, Java and C# keep strings in UTF-16. The default sort of a JavaScript array compares "sequences of UTF-16 code unit values".[^14] Java `String.compareTo` "is based on the Unicode value of each character in the strings",[^15] and a Java `char` is one UTF-16 code unit. C# `string.CompareOrdinal` "Compares two String objects by evaluating the numeric values of the corresponding Char objects in each string."[^16] The two orders are the same for the code points up to U+FFFF. A character above U+FFFF is two code units, a surrogate pair. These code units are from D800 to DFFF, which RFC 3629 reserves "for use with the UTF-16 encoding form (as surrogate pairs)".[^3] So in UTF-16 order, a character above U+FFFF comes before the characters from U+E000 to U+FFFF. The solutions for these languages compare the UTF-8 bytes:

```javascript run id=javascript-utf16-order
const lines = ["\u{FF61}", "\u{1F600}"];
const hex = (s) => [...s].map((c) => c.codePointAt(0).toString(16)).join(" ");
console.log([...lines].sort().map(hex));
console.log([...lines].sort((a, b) => Buffer.compare(Buffer.from(a), Buffer.from(b))).map(hex));
const units = (s) => Array.from({ length: s.length }, (_, i) => s.charCodeAt(i).toString(16)).join(" ");
console.log(lines.map(units));
```

```output for=javascript-utf16-order
[ '1f600', 'ff61' ]
[ 'ff61', '1f600' ]
[ 'ff61', 'd83d de00' ]
```

- **Some functions split at more than LF.** Python `str.splitlines` splits at more characters: "the boundaries are a superset of universal newlines".[^17] The list includes U+001C, U+000C, U+0085 and U+2028. So a line that holds one of these characters becomes two lines:

```python run id=python-splitlines
text = "b\x1ca\nc\n"
print(text.splitlines())
print(text.split("\n"))
```

```output for=python-splitlines
['b', 'a', 'c']
['b\x1ca', 'c', '']
```

Rust `str::lines` also changes the lines: "Lines are split at line endings that are either newlines (`\n`) or sequences of a carriage return followed by a line feed (`\r\n`)."[^10] So it removes the CR of a CRLF line end. The Python and Rust solutions use `split` with LF.

- **The last line.** A split at each LF gives one empty string after the LF at the end of the file. If the code keeps it, the result has one empty line too many. If the code removes each empty string, it also removes the empty lines of the file. Remove only the last part, and only if it is empty.
- **Reading the whole file.** Each solution keeps the whole file in memory, and a sort needs all the lines before it can return the first one. A file that is larger than the memory needs an external sort, which this page does not cover.

## Related tasks

- [[count-the-lines-of-a-file]]: count the lines of a file, with the same rules for a line.
- [[remove-duplicate-lines]]: keep one copy of each line, as `sort -u` and `uniq` do.
- [[compute-word-frequencies]]: a pipeline that uses `LC_ALL=C sort` to count words.
- [[unix-text-processing-pipeline]]: join `sort` with other tools in a pipeline.

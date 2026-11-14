---
tag: 423MXR
kind: task
slug: align-columns-of-text
name: Align columns of text
aliases: [align columns, column alignment, pad fields to columns, format text as a table]
area: data-and-persistence/text-processing
level: core
summary: Align the fields of a text in columns, left, right or center, where a dollar sign separates two fields and the width of a column is the number of code points of its longest field.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Align-columns/00-TASK.txt
  title: Rosetta Code, Align columns (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: Text Sequence Type, str; str.center, str.ljust, str.rjust, str.split
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/length
  title: MDN Web Docs, String length
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/Symbol.iterator
  title: MDN Web Docs, String.prototype[Symbol.iterator]()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/String.html
  title: Java SE 21 API, java.lang.String
  locator: length; codePointCount
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.length
  title: .NET API reference, String.Length
  locator: Remarks
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.enumeraterunes
  title: .NET API reference, String.EnumerateRunes
  locator: Definition
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: String types
- url: https://pkg.go.dev/unicode/utf8
  title: Go standard library, package unicode/utf8
  locator: RuneCount; RuneCountInString
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: The Rust Standard Library, primitive type str
  locator: len; chars; split_terminator
- url: https://www.rfc-editor.org/rfc/rfc3629.txt
  title: "RFC 3629, UTF-8, a transformation format of ISO 10646"
  locator: 1 Introduction; 3 UTF-8 definition
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: GNU Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion; 5.2 Bash Variables (LC_ALL)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap07.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Locale"
  locator: 7.2 POSIX Locale
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap06.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Character Set"
  locator: 6.2 Character Encoding
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/padEnd
  title: MDN Web Docs, String.prototype.padEnd()
  locator: Parameters
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fprintf.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), fprintf"
  locator: DESCRIPTION (field width)
- url: https://www.unicode.org/reports/tr11/
  title: Unicode Standard Annex 11, East Asian Width
  locator: 4 Definitions (ED4)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/wcwidth.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), wcwidth"
  locator: DESCRIPTION
- url: https://www.unicode.org/reports/tr29/
  title: Unicode Standard Annex 29, Unicode Text Segmentation
  locator: Summary
---

## Task

Align the fields of a text in columns. Rosetta Code states the task like this: "Given a text file of many lines, where fields within a line are delineated by a single 'dollar' character, write a program that aligns each column of fields by ensuring that words in each column are separated by at least one space."[^1] The task also says: "Further, allow for each word in a column to be either left justified, right justified, or center justified within its column."[^1] The function takes the text and the alignment, `left`, `right` or `center`, and returns the aligned text.

The task leaves some rules open. This page uses these rules:

- **A line ends at LF.** A final LF ends the last line and does not start a new line. A last line with no LF is a line too.
- **A `$` separates two fields.** A `$` at the end of a line does not start a new field, because "The example input texts lines may, or may not, have trailing dollar characters."[^1] Two `$` with nothing between them make an empty field. An empty line has no fields, and stays empty in the result.
- **The width of a column is the number of code points of its longest field.** A line can have fewer fields than the other lines. Then it has no field in the last columns.
- **All the columns use the same alignment.** The task says: "All columns should share the same alignment."[^1]
- **One space separates two columns.** The task says: "The minimum space between columns should be computed from the text and not hard-coded."[^1] Here the space is the result of the widths that the code calculates, and one space more.
- **The padding goes where the alignment puts it.** For `left`, the spaces go after the field. For `right`, they go before the field. For `center`, the spaces go on the two sides, and if their number is odd, the odd space goes after the field.
- **The last field of a line has no padding after it.** The task says: "Consecutive space characters produced adjacent to the end of lines are insignificant for the purposes of the task."[^1] So no line of the result ends with a space that comes from padding.
- **Each line of the result ends with LF.** An empty text gives an empty result. An alignment that is not `left`, `right` or `center` is an error.

For example, the text `a$bb$c` LF `dd$e$fff` LF with `left` gives `a  bb c` LF `dd e  fff` LF.

## Solutions

The method is the same in each language:

1. Split the text into lines, and split each line into fields.
2. For each column, find the largest number of code points of a field in the column.
3. Write each line again. Before each field except the first, write one space. Then write the padding before the field, the field, and the padding after the field if it is not the last field of the line.

The number of code points is not the length of the string in all languages. Some languages count bytes, and some count UTF-16 code units. Each solution uses a function that counts code points, so the column of a field with the letter `é` or with a character outside the BMP has the correct width.

- **Python.** A `str` counts code points: "Strings are immutable sequences of Unicode code points."[^2] So `len(field)` is the width. `text.split('\n')` gives an empty last part after a final LF, and the code removes it. `line.split('$')` keeps empty fields, because "consecutive delimiters are not grouped together and are deemed to delimit empty strings".[^2] The code removes the last field if the line ends with `$`. The code builds the padding with `' ' * n`, and does not use `str.center` (see the pitfalls). A bad alignment raises `ValueError`.
- **JavaScript and TypeScript.** The string property `length` "contains the length of the string in UTF-16 code units."[^3] So the code counts code points with a `for...of` loop. The string iterator "yields the Unicode code points of the string value as individual strings".[^4] The code builds the padding with `' '.repeat(n)`, and does not use `padEnd` (see the pitfalls). A bad alignment throws a `RangeError`. The TypeScript file adds the type `Align` and the types of the cases.
- **Java.** `String.length()` counts UTF-16 units: "The length is equal to the number of Unicode code units in the string."[^5] The code uses `codePointCount(0, s.length())`, which "Returns the number of Unicode code points in the specified text range of this" string.[^5] A loop with `indexOf` splits the lines and the fields, and a `StringBuilder` builds the result. A bad alignment throws an `IllegalArgumentException`.
- **C#.** "The Length property returns the number of Char objects in this instance, not the number of Unicode characters."[^6] The code counts code points with `EnumerateRunes().Count()`. `EnumerateRunes` "Returns an enumeration of Rune from this string."[^7] `StringBuilder.Append(' ', n)` writes the padding. A bad alignment throws an `ArgumentException`.
- **Go.** "A string value is a (possibly empty) sequence of bytes", and `len` gives the number of bytes.[^8] The code uses `utf8.RuneCountInString`. "RuneCountInString is like RuneCount but its input is a string", and "RuneCount returns the number of runes in p."[^9] `strings.Split` splits the text and the lines, and a `strings.Builder` builds the result. A bad alignment gives the error `ErrBadAlign`.
- **Rust.** For `str::len`, "This length is in bytes".[^10] The code uses `chars().count()`. A Rust `char` "represents a Unicode Scalar Value".[^10] `split_terminator` splits the text into lines and each line into fields. It is like `split`, "except that the trailing substring is skipped if empty".[^10] So a final LF does not give an empty line, and a final `$` does not give an empty field. The alignment is the enum `Align`, and its `FromStr` gives the error `BadAlign` for a bad word.
- **C.** The code walks the bytes of the text. In UTF-8, each byte after the first byte of a code point has the form `10xxxxxx`: "The following octet(s) all have the higher-order bit set to 1 and the following bit set to 0".[^11] So the code counts the bytes that are not from 0x80 to 0xBF. The code can split the text at each `$` byte and LF byte, because "US-ASCII octet values do not appear otherwise in a UTF-8 encoded character stream."[^11] A first pass finds the size of the result, and a second pass writes it into one `malloc` block. `align_columns` returns the new string, or NULL with `errno` set to `EINVAL` for a bad alignment or `ENOMEM`.
- **C++.** The code is like the C code. It splits the text into `std::string_view` parts, counts code points with `std::ranges::count_if` on the bytes, and appends to a `std::string`. A bad alignment throws `std::invalid_argument`.
- **Bash.** The function reads the text from stdin, and prints the result. `read -r` reads each line, and the test `|| [[ -n $line ]]` keeps a last line with no LF. `${rest%%\$*}` and `${rest#*\$}` split each line at each `$`. In Bash, `${#parameter}` "Substitutes the length in characters of the value of" the parameter,[^12] and a character depends on the locale. So the function sets `local LC_ALL=C`. `LC_ALL` "overrides the value of LANG and any other LC_ variable specifying a locale category."[^12] The C locale is the POSIX locale,[^13] and "The POSIX locale shall contain 256 single-byte characters".[^14] Then `${#t}` counts bytes. The function removes the bytes from 0x80 to 0xBF and counts the bytes that stay, as the C code does. `printf -v out '%*s'` with an empty string makes the padding. A bad alignment prints a message and returns 2.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `rosetta-left`, `rosetta-right` and `rosetta-center`: the input text of the Rosetta Code task, with each of the three alignments.[^1]
- `two-lines`: `a$bb$c` and `dd$e$fff`, left. The result is `a  bb c` and `dd e  fff`.
- `empty`: an empty text gives an empty result.
- `empty-line`: `a$b`, an empty line, and `ccc$d`. The empty line stays empty, and the other lines are `a   b` and `ccc d`.
- `empty-field`: `a$$b` has three fields, and the second is empty. With `c$dd$e`, the result is `a    b` and `c dd e`.
- `trailing-dollar`: `a$b$` has two fields, not three.
- `ragged`: `a$b$c` and `ddd`, right. The second line has one field, so it is `ddd` with no space after it.
- `one-column`: `a`, `bbb`, `cc`, right. The result is `  a`, `bbb`, ` cc`.
- `no-final-newline`: the text ends with no LF. The result still ends with LF.
- `center-odd`: `a` in a column of width 4 is one space, `a`, two spaces. The odd space goes after the field.
- `center-last`: the last field of a line gets the spaces before it, and no spaces after it.
- `non-ascii`: `café` has 4 code points and 5 bytes in UTF-8, so its column has width 4.
- `outside-the-bmp`: U+1F600 is one code point and two UTF-16 code units, so it counts as 1.
- `spaces-in-field`: `a b` is one field, because only `$` separates fields.

The tests also align 100000 lines, `0$x` to `99999$x`, to the right. The first line of the result is `    0 x` and the last line is `99999 x`. The case in `cases/errors.jsonl` uses the alignment `justify` and must give the error `bad-align`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **The length of a string is often not the number of code points.** Go and Rust strings count bytes,[^8][^10] and Java, C# and JavaScript strings count UTF-16 code units.[^5][^6][^3] With such a length, `café` has width 5 in Go and Rust, and a character outside the BMP has width 2 in Java, C# and JavaScript. Then the column of that field is too wide by one.
- **`padEnd` and `padStart` pad to a number of UTF-16 code units.** The parameter `targetLength` is "The length of the resulting string once the current str has been padded."[^15] So a string with a character outside the BMP gets one space too few. Here the letter U+1D465 gets one space and `ab` also gets one space:

```javascript run id=javascript-padend
// U+1D465 is a letter outside the BMP, so it is two UTF-16 code units.
const rows = [["\u{1D465}", "1"], ["ab", "2"]];
for (const [a, b] of rows) {
  console.log(`[${a.padEnd(3)}] ${b}`);
}
console.log("\u{1D465}".length, [..."\u{1D465}"].length);
```

```output for=javascript-padend
[𝑥 ] 1
[ab ] 2
2 1
```

- **`printf` pads to a number of bytes.** POSIX says for the field width: "If the converted value has fewer bytes than the field width, it shall be padded with" spaces.[^16] The Bash `printf` does the same in a UTF-8 locale, but `${#word}` counts characters. So `café`, 5 bytes, gets no space, and `abc` gets two:

```bash run id=bash-printf
export LC_ALL=C.UTF-8
word='café'
printf '[%-5s]\n' "$word" abc
echo "${#word}"
```

```output for=bash-printf
[café]
[abc  ]
4
```

- **Python `str.center` does not put the odd space on the same side each time.** The documentation says only "Return centered in a string of length" width.[^2] It does not say where the odd space goes, and the side changes with the length of the string:

```python run id=python-center
for word, width in [("a", 4), ("ab", 5), ("abc", 6)]:
    print(f"[{word.center(width)}]")
```

```output for=python-center
[ a  ]
[  ab ]
[ abc  ]
```

So the solutions calculate the left padding as half of the padding, rounded down, and put the rest after the field.
- **A code point is not one column on the screen.** Some characters "occur only in the context of East Asian typography where they are wide characters",[^17] and a terminal shows them in two columns. POSIX `wcwidth` can "determine the number of column positions required for the wide character".[^18] Also, "nonspacing marks do not possess actual advance width."[^17] So a letter with a combining mark is two code points in one column. Unicode text segmentation finds the boundaries of "user-perceived characters, words, and sentences."[^19] This page counts code points. For a terminal, count the columns of each grapheme cluster.
- **A `$` at the end of a line can add an empty column.** If the code splits `a$b$` at each `$`, it gets the fields `a`, `b` and an empty field. Then the field `b` is not the last field, and the line gets padding after `b`. Remove an empty last field, or use a split that does not give it, such as Rust `split_terminator`.[^10]

## Related tasks

- [[delimited-text]]: text with a separator between fields, such as CSV and TSV.
- [[fixed-width-text-format]]: records in which each field has a fixed number of columns.
- [[find-the-length-of-a-string]]: the length of a string in bytes, code units, code points and graphemes.
- [[count-the-graphemes-of-a-string]]: count the user-perceived characters of a string.
- [[characters-and-code-points]]: what a code point is, and how it differs from a byte and a character.
- [[utf-8]]: the encoding that the C and Bash solutions read one byte at a time.

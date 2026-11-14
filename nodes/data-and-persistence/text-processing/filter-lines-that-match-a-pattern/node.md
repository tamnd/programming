---
tag: BM6FP4
kind: task
slug: filter-lines-that-match-a-pattern
name: Filter lines that match a pattern
aliases: [grep lines, select lines that match a regular expression, search a file for a pattern, grep -E]
area: data-and-persistence/text-processing
level: intro
summary: Read a UTF-8 file and return each line in which a regular expression matches some part, in the order of the file, as grep -E does.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/grep.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), grep"
  locator: DESCRIPTION; OPTIONS (-E, -F); EXIT STATUS
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 3, Definitions"
  locator: 3.172 Incomplete Line; 3.185 Line
- url: https://docs.python.org/3/library/functions.html#open
  title: The Python Standard Library, Built-in Functions
  locator: open (newline)
- url: https://docs.python.org/3/library/re.html
  title: The Python Standard Library, re
  locator: Regular expression syntax; re.search; re.escape
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Regular_expressions/Character_classes
  title: MDN Web Docs, Character classes
  locator: Wildcard
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/unicode
  title: MDN Web Docs, RegExp.prototype.unicode
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Pattern.html
  title: Java SE 25 API, java.util.regex.Pattern
  locator: Line terminators; UNIX_LINES
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.regularexpressions.regex.ismatch
  title: .NET API reference, Regex.IsMatch
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/anchors-in-regular-expressions
  title: .NET documentation, Anchors in regular expressions
- url: https://pkg.go.dev/regexp
  title: Go standard library, package regexp
  locator: Overview; Regexp.MatchString
- url: https://pkg.go.dev/regexp/syntax
  title: Go standard library, package regexp/syntax
  locator: Syntax
- url: https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
  title: "A Regular Expression Matcher, code by Rob Pike, exegesis by Brian Kernighan"
  locator: The Practice of Programming
- url: https://doc.rust-lang.org/std/io/trait.BufRead.html
  title: The Rust Standard Library, std::io::BufRead
  locator: read_line
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/regcomp.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), regcomp, regexec"
  locator: DESCRIPTION; RETURN VALUE
- url: https://en.cppreference.com/w/cpp/regex/syntax_option_type
  title: cppreference.com, std::regex_constants::syntax_option_type
- url: https://en.cppreference.com/w/cpp/regex/regex_search
  title: cppreference.com, std::regex_search
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap09.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 9, Regular Expressions"
  locator: 9.4.4 Periods in EREs; 9.4.6 EREs Matching Multiple Characters; 9.4.9 ERE Expression Anchoring
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/test
  title: MDN Web Docs, RegExp.prototype.test()
  locator: Description
---

## Task

Read a text file and a regular expression, and return the lines in which the regular expression matches. This is the job of `grep`. POSIX says that `grep` "shall search the input files, selecting lines matching one or more patterns".[^1] With `-E`, the pattern is an extended regular expression (ERE), and "If any entire ERE pattern matches some part of an input line excluding the terminating <newline>, the line shall be matched."[^1]

The task needs three rules: what a line is, what the pattern can hold, and what a match is. This page uses these rules:

- **A line ends at LF.** POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character."[^2] The LF is not part of the line in the result, and the pattern does not see it. A CR before the LF stays in the line.
- **A last line with no LF is a line too.** POSIX calls it an incomplete line: "A sequence of one or more non-<newline> characters at the end of the file."[^2]
- **The pattern uses a small set of the syntax.** A character that is not special matches itself. `.` matches any character. `*` after a character or `.` matches zero or more of it. `^` as the first character matches the start of the line, and `$` as the last character matches the end of the line. The regular expressions of all the languages on this page agree on this set, so each solution can use the library of its language.
- **A match in some part of the line is enough.** The pattern `an` matches `banana`. To match a whole line, put `^` at the start and `$` at the end.
- **The result keeps the order and the duplicates.** The lines that match come in the order of the file, and two equal lines that match are both in the result.

If the file does not exist, the solution reports an error.

## Solutions

Each solution compiles the pattern one time, reads the file one line at a time, and keeps each line in which the pattern matches. The steps:

1. Compile the pattern. If it is not valid, report an error.
2. Read the next line, and remove the LF at its end.
3. If the pattern matches some part of the line, add the line to the end of the result.
4. At the end of the file, return the result.

The Python, Go, Rust, C++ and Bash solutions read one line at a time. The JavaScript, Java, C# and C solutions read the whole file into memory, and then split it.

- **Python.** `re.compile(pattern)` compiles the pattern. `open(path, encoding='utf-8', newline='\n')` opens the file as text. With a value such as `'\n'`, "input lines are only terminated by the given string, and the line ending is returned to the caller untranslated",[^3] so a CR stays in the line. `line.removesuffix('\n')` removes the LF. `regex.search(line)` does the match: it does a "Scan through string looking for the first location where the regular expression pattern produces a match".[^4]
- **JavaScript and TypeScript (Node.js).** `readFileSync(path, 'utf8')` reads the file, and `split('\n')` gives the lines. If the last part is empty, the code removes it. `new RegExp(pattern, 'su')` compiles the pattern. The `s` flag is necessary because, with no flag, `.` "Matches any single character except line terminators", and the list includes `\r`.[^5] "If the dotAll (s) flag is enabled, also matches line terminators."[^5] With the `u` flag, "Surrogate pairs will be interpreted as whole characters instead of two separate characters."[^6] `regex.test(line)` does the match. The TypeScript file adds types only.
- **Java.** `Pattern.compile(pattern, Pattern.UNIX_LINES)` compiles the pattern. With no flag, Java treats a CR as a line terminator (see Pitfalls). With `UNIX_LINES`, "only the `'\n'` line terminator is recognized in the behavior of `.`, `^`, and `$`."[^7] `Files.readAllBytes(path)` gives the bytes, and a loop splits them at each LF. `regex.matcher(line).find()` looks for a match in some part of the line.
- **C#.** `new Regex(pattern)` compiles the pattern. `File.ReadAllBytes(path)` gives the bytes, `Array.IndexOf` finds each LF, and `Encoding.UTF8.GetString` decodes each line. `regex.IsMatch(line)` "Indicates whether the regular expression specified in the Regex constructor finds a match in a specified input string."[^8] By default, `$` matches "at the end of the string or before \n at the end of the string",[^9] and a line has no LF, so `$` matches only at the end of the line.
- **Go.** `regexp.Compile(pattern)` compiles the pattern, which uses "the same general syntax used by Perl, Python, and other languages."[^10] `bufio.Reader.ReadString('\n')` reads each line, and `strings.TrimSuffix` removes the LF. `re.MatchString(line)` "reports whether the string s contains any match of the regular expression re."[^10] In Go, `.` is "any character, possibly including newline (flag s=true)",[^11] so it matches a CR.
- **Rust.** The standard library has no regular expressions, so the solution has its own matcher for the set of this task. Brian Kernighan describes a matcher by Rob Pike for these constructs: "c matches any literal character c . matches any single character ^ matches the beginning of the input string $ matches the end of the input string * matches zero or more occurrences of the previous character".[^12] He says that this set "easily accounts for 95 percent of all instances" in his own use.[^12] The Rust code follows that matcher, on the `char` values of the line: `matches` tries each start in the line, `match_here` matches the pattern at one place, and `match_star` handles `*`. `BufReader::read_line` "Reads all bytes until a newline (the 0xA byte) is reached, and append them to the provided String buffer",[^13] so `pop` removes the LF at the end.
- **C.** A loop of `fread` reads the file into a buffer that grows. The code changes each LF to a NUL, so each line is a C string. `regcomp` with `REG_EXTENDED` compiles the pattern as an ERE, and `REG_NOSUB` tells it to "Report only success/fail in regexec ()."[^14] For each line, `regexec` returns 0 if the line matches: "Upon successful completion, the regexec () function shall return 0. Otherwise, it shall return REG_NOMATCH to indicate no match."[^14] `filter_lines` fills a `struct matched_lines`, which the caller frees with `matched_lines_free`, or it returns -1 and sets `errno`.
- **C++.** `std::regex(pattern, std::regex::extended | std::regex::nosubs)` compiles the pattern. The flag `extended` tells it to "Use the extended POSIX regular expression grammar".[^15] `std::getline` reads a binary `std::ifstream` one line at a time. `std::regex_search` "Determines if there is a match between the regular expression e and some subsequence in the target character sequence."[^16] The result is a `std::expected` (C++23), and the errors are `cannot_open` and `read_failed`.
- **Bash.** `LC_ALL=C grep -E -e PATTERN -- FILE` does the work. The `-e` before the pattern tells `grep` that the next argument is the pattern, also when it starts with `-`. `grep` returns the status 1 when "No lines were selected",[^1] but for this task, no lines is a result, not an error (see Pitfalls). So the function returns 0 when the status is 0 or 1.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`.

- `literal`: the pattern `an` in `apple`, `banana`, `cherry` gives `banana`.
- `several`: `cat` in `cat`, `dog`, `catalog` gives `cat` and `catalog`.
- `start` and `end`: `^a` in `ab`, `ba` gives `ab`, and `a$` gives `ba`.
- `dot`: `a.c` in `abc`, `ac`, `axc` gives `abc` and `axc`. The `.` must match one character, so `ac` does not match.
- `star`: `ab*c` in `ac`, `abc`, `abbc`, `adc` gives `ac`, `abc`, `abbc`. Zero `b` is a match too.
- `whole-line`: `^a$` in `a`, `ab`, `ba` gives `a`.
- `empty`: an empty file gives `[]`.
- `no-match`: `x` in `abc` gives `[]`.
- `empty-lines`: `^$` in `a`, an empty line, `b` and an empty line gives two empty lines.
- `match-all`: `.*` matches each line, also an empty line, because `*` can match zero characters.
- `duplicates`: `a` in `a`, `b`, `a` gives `a` two times.
- `no-final-newline`: `b` in `a`, LF, `ab` with no LF at the end gives `ab`.
- `crlf-end`: `b$` in `ab` CRLF and `ab` LF gives only the second `ab`. In the first line, the CR comes after the `b`.
- `crlf-dot`: `^a.$` in `a` CRLF gives `a` CR. The `.` matches the CR.
- `case`: `apple` in `Apple`, `apple` gives `apple`. The match is case sensitive.
- `non-ascii`: `é` in `café`, `cafe` gives `café`.

The tests also write a file of 100000 lines with the numbers 0 to 99999. The pattern `^9*$` must give `9`, `99`, `999`, `9999` and `99999`. The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **The syntax is not the same in each language.** C, C++ and `grep -E` use the POSIX ERE syntax. Python, JavaScript, Java, C# and Go use syntaxes that come from Perl. For the set of this task, they agree. In an ERE, `.` "shall match any character in the supported character set except NUL",[^17] and a character followed by `*` "shall match what zero or more consecutive occurrences of the ERE would match".[^17] Outside this set, the syntaxes are different. For example, `\d` is a digit in Python, JavaScript, Java, C# and Go, but it is not part of an ERE.
- **A CR can change `$` in Java.** With no flag, Java recognizes "A standalone carriage-return character" as a line terminator, and "The regular expression $ matches at the end of the entire input sequence, but also matches just before the last line terminator if this is not followed by any other input character."[^7] So `b$` matches `ab` CR, and `.` does not match the CR. The Java solution uses `UNIX_LINES`, so the results are the same as in the other languages. In JavaScript, `.` does not match a CR unless the `s` flag is on.[^5]
- **`grep` returns 1 when no line matches.** POSIX gives three exit values: "0 One or more lines were selected", "1 No lines were selected", and ">1 An error occurred."[^1] In a script with `set -e`, the status 1 stops the script, and an empty result is not an error:

```bash run id=bash-grep-status
printf 'apple\nbanana\n' | grep -E 'an'
echo "status $?"
printf 'apple\nbanana\n' | grep -E 'x'
echo "status $?"
```

```output for=bash-grep-status
banana
status 0
status 1
```

- **A `g` flag in JavaScript keeps state.** "JavaScript RegExp objects are stateful when they have the global or sticky flags set", and "They store a lastIndex from the previous match."[^18] If the code calls `test` with the same `/a/g` on each line, the second call starts at the index after the first match, and it does not find the `a`. So the code skips lines that match. The solution does not use the `g` flag:

```javascript run id=javascript-global-flag
const lines = ["a", "a", "a"];
const global = /a/g;
console.log(lines.filter((line) => global.test(line)));
const plain = /a/;
console.log(lines.filter((line) => plain.test(line)));
```

```output for=javascript-global-flag
[ 'a', 'a' ]
[ 'a', 'a', 'a' ]
```

- **What `.` matches.** A `.` matches one character, but code that works on bytes sees a character that is not ASCII as more than one byte. The letter `é` is two bytes in UTF-8, so `^caf.$` matches the text `café`, but not its UTF-8 bytes. The cases use `.` only with ASCII text:

```python run id=python-dot-bytes
import re

line = "caf\u00e9"
print(len(line), len(line.encode("utf-8")))
print(bool(re.search("^caf.$", line)))
print(bool(re.search(b"^caf.$", line.encode("utf-8"))))
```

```output for=python-dot-bytes
4 5
True
False
```

- **Text that holds special characters.** If the pattern is plain text such as `1.5`, the `.` matches any character, so `125` matches too. For plain text, use `grep -F`: "Treat each pattern specified as a string instead of a regular expression."[^1] In Python, `re.escape` does the same job: it does "Escape special characters in pattern".[^4]

## Related tasks

- [[count-the-lines-of-a-file]]: count the lines of a file, with the same rules for a line.
- [[line-ending]]: LF, CRLF and CR, and why a CR stays in the line.
- [[remove-duplicate-lines]]: keep one copy of each line.
- [[unix-text-processing-pipeline]]: join `grep` with other tools in a pipeline.

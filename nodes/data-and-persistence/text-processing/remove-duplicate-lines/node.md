---
tag: 33Z7MX
kind: task
slug: remove-duplicate-lines
name: Remove duplicate lines
aliases: [deduplicate lines, keep the first copy of each line, remove duplicate lines without sorting, "awk '!seen[$0]++'"]
area: data-and-persistence/text-processing
level: intro
summary: Read a UTF-8 file and return its lines with only the first copy of each line, in the order of the file, with a set of the lines that were seen, as awk '!seen[$0]++' does.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/uniq.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), uniq"
  locator: DESCRIPTION
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 3, Definitions"
  locator: 3.172 Incomplete Line; 3.185 Line
- url: https://docs.python.org/3/library/io.html
  title: The Python Standard Library, io
  locator: IOBase.readline; TextIOWrapper (newline)
- url: https://docs.python.org/3/builtins/stdtypes.html#dict
  title: The Python Standard Library, Built-in Types
  locator: Mapping types, dict (fromkeys)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set
  title: MDN Web Docs, Set
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/LinkedHashSet.html
  title: Java SE 25 API, java.util.LinkedHashSet
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.hashset-1.add
  title: .NET API reference, HashSet<T>.Add
  locator: Returns
- url: https://pkg.go.dev/bufio#Reader.ReadString
  title: Go standard library, package bufio
  locator: Reader.ReadString; Reader.ReadBytes
- url: https://doc.rust-lang.org/std/io/trait.BufRead.html
  title: The Rust Standard Library, std::io::BufRead
  locator: read_line; read_until
- url: https://doc.rust-lang.org/std/collections/struct.HashSet.html
  title: The Rust Standard Library, std::collections::HashSet
  locator: insert
- url: https://en.cppreference.com/w/cpp/string/basic_string/getline
  title: cppreference.com, std::getline
- url: https://en.cppreference.com/w/cpp/container/unordered_set/insert
  title: cppreference.com, std::unordered_set::insert
  locator: Return value
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), awk"
  locator: DESCRIPTION; Expressions in awk; Variables and Special Variables
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sort.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), sort"
  locator: OPTIONS (-u)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/normalize
  title: MDN Web Docs, String.prototype.normalize()
  locator: Description
---

## Task

Read a text file and return its lines, with only the first copy of each line. The lines stay in the order of the file. The `uniq` utility does not do this job alone. POSIX says that `uniq` shall "read an input file comparing adjacent lines, and write one copy of each input line on the output", but also that "Repeated lines in the input shall not be detected if they are not adjacent."[^1]

The task needs three rules: what a line is, when two lines are equal, and which copy stays. This page uses these rules:

- **A line ends at LF.** POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character."[^2] The LF is not part of the line in the result. A CR before the LF stays in the line, so `a` CRLF and `a` LF are two different lines.
- **A last line with no LF is a line too.** POSIX calls it an incomplete line: "A sequence of one or more non-<newline> characters at the end of the file."[^2] So the file `a`, LF, `b`, LF, `a` gives two lines, `a` and `b`.
- **Two lines are equal if their bytes are equal.** The solutions do not ignore case, do not remove spaces, and do not normalize Unicode text. So `A` and `a` are two lines, and `a` and `a` with a space at the end are two lines.
- **The first copy stays, at its place.** The result has the lines in the order of their first copies. So `b`, `a`, `b`, `c`, `a` gives `b`, `a`, `c`.
- **An empty line is a line.** Two empty lines are equal, so only the first one stays. An empty file gives no lines.

If the file does not exist, the solution reports an error.

## Solutions

Each solution keeps a set of the lines that it has seen. It reads the lines in order, and it keeps a line only if the set does not have it. The steps:

1. Read the next line, and remove the LF at its end.
2. If the set has the line, go to step 1.
3. Add the line to the set and to the end of the result.
4. At the end of the file, return the result.

The set is a hash set, so the solution does not compare each line with all the lines before it. The memory grows with the number of different lines, not with the number of lines. The Python, Go, Rust and C++ solutions read one line at a time. The JavaScript, Java, C# and C solutions read the whole file into memory, and then split it.

- **Python.** `open(path, 'rb')` opens the file in binary mode. For binary files, "The line terminator is always `b'\n'`",[^3] so a loop on the file gives each line with its LF, and a CR stays in the line. `line.removesuffix(b'\n')` removes the LF. `dict.fromkeys` makes a dictionary that has the lines as its keys. A line that comes again does not move its key: "Dictionaries preserve insertion order. Note that updating a key does not affect the order."[^4] The solution then decodes each key as UTF-8.
- **JavaScript and TypeScript (Node.js).** `readFileSync(path)` gives a `Buffer`. A loop with `indexOf(0x0a, start)` finds each LF, and `toString('utf8', start, end)` decodes each line. A `Set` keeps the lines. MDN says: "You can iterate through the elements of a set in insertion order. The insertion order corresponds to the order in which each element was inserted into the set by the add() method successfully (that is, there wasn't an identical element already in the set when add() was called)."[^5] So `[...unique]` gives the first copies in order. The TypeScript file adds types only.
- **Java.** `Files.readAllBytes(path)` gives the bytes. A loop finds each LF, and `new String(data, start, length, StandardCharsets.UTF_8)` decodes each line. A `LinkedHashSet<String>` keeps the lines in the order in which they were added: "Note that encounter order is not affected if an element is re-inserted into the set with the add method."[^6] `new ArrayList<>(lines)` copies them to a list.
- **C#.** `File.ReadAllBytes(path)` gives the bytes. `Array.IndexOf` finds each LF, and `Encoding.UTF8.GetString` decodes each line. A `HashSet<string>` with `StringComparer.Ordinal` keeps the lines that were seen, and `Add` returns "true if the element is added to the `HashSet<T>` object; false if the element is already present."[^7] A `HashSet<T>` has no order, so a `List<string>` keeps the result, and the solution adds a line to it only when `Add` returns true.
- **Go.** `bufio.Reader.ReadString('\n')` "reads until the first occurrence of delim in the input, returning a string containing the data up to and including the delimiter."[^8] At the end of the file, it "returns the data read before the error and the error itself (often io.EOF)",[^8] so a last line with no LF comes with `io.EOF`. `strings.TrimSuffix` removes the LF. A `map[string]bool` keeps the lines that were seen, and a slice keeps the result.
- **Rust.** `BufReader::read_line` "Reads all bytes until a newline (the 0xA byte) is reached, and append them to the provided String buffer."[^9] It returns `Ok(0)` at the end of the file, and it returns an error "if the read bytes are not valid UTF-8".[^9] The solution removes the LF with `pop`. A `HashSet<String>` keeps the lines that were seen. `contains` takes a `&str`, so a duplicate line is not copied. For a new line, `insert` puts a copy in the set, and `std::mem::take` moves the line into the result. `insert` "Returns whether the value was newly inserted."[^10]
- **C.** A loop of `fread` reads the file into a buffer that grows. The code changes each LF to a NUL, so each line is a C string in the buffer. The C standard library has no set, so the code has a small hash table: an array of slots, where each slot holds 0 or the index of a line plus 1. A collision goes to the next slot. When the table is half full, the code makes a table of double the size and puts the lines into it again. `remove_duplicate_lines` fills a `struct unique_lines`, which the caller frees with `unique_lines_free`, or it returns -1 and sets `errno`.
- **C++.** `std::getline` reads a binary `std::ifstream` one line at a time. The LF "is extracted from input, but is not appended to str".[^11] At the end of the file: "If no characters were extracted for whatever reason (not even the discarded delimiter), getline sets failbit and returns."[^11] So the loop also reads a last line with no LF, and then it stops. `std::unordered_set<std::string>::insert` returns a pair, and its second value is "a bool value set to true if and only if the insertion took place."[^12] The result is a `std::expected` (C++23), and the errors are `cannot_open` and `read_failed`.
- **Bash.** `LC_ALL=C awk '!seen[$0]++'` on the file does the work. POSIX says: "By default, a record is a line, less its terminating <newline>".[^13] `seen` is an array, and its "subscripts, or element identifiers, are strings, providing a type of associative array capability."[^13] The first time that awk reads a line, `seen[$0]` has no value, and "An uninitialized value shall have both a numeric value of zero and a string value of the empty string."[^13] The post-increment `++` gives the old value, 0, and then adds 1. So `!seen[$0]++` is true only for the first copy. The pattern has no action, and "a missing action shall be equivalent to: { print }".[^13] `print` writes ORS after the line, and ORS is "a <newline> by default",[^13] so each line of the output ends with an LF, also the last one.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`.

- `simple`: `b`, `a`, `b`, `c`, `a` gives `b`, `a`, `c`. Each line stays at the place of its first copy.
- `adjacent`: `a`, `a`, `b` gives `a`, `b`.
- `no-duplicates`: `c`, `b`, `a` gives `c`, `b`, `a`. The order does not change.
- `empty`: an empty file gives `[]`.
- `empty-lines`: an empty line, `a`, and two empty lines give `""` and `a`.
- `all-same`: `x`, `x`, `x` gives `x`.
- `no-final-newline`: `a`, LF, `b`, LF, `a` with no LF at the end gives `a` and `b`. The last `a` is a line, and it is a copy of the first line.
- `crlf`: `a` CRLF `a` LF gives `a` CR and `a`. The CR stays, so the two lines are not equal.
- `case`: `A`, `a`, `A` gives `A`, `a`.
- `whitespace`: `a` space, `a`, space `a`, `a` space gives `a` space, `a`, space `a`.
- `normalization`: U+00E9, then `e` and U+0301, then U+00E9 gives U+00E9 and `e` U+0301. The two lines look the same, but their bytes are not equal.

The tests also write a file of 100000 lines, with the numbers 0 to 999 one after the other, 100 times. The result must be the 1000 lines `0` to `999`. The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **`uniq` removes only adjacent duplicates.** "Repeated lines in the input shall not be detected if they are not adjacent."[^1] So `uniq` alone does not remove a duplicate line that comes later in the file. `sort -u` removes all the duplicates, "Unique: suppress all but one in each set of lines having equal keys",[^14] but the result is in sorted order, not in the order of the file. Only the awk program keeps the first copies in their order:

```bash run id=bash-dedup-tools
printf 'b\na\nb\n' | uniq | paste -s -d ' ' -
printf 'b\na\nb\n' | LC_ALL=C sort -u | paste -s -d ' ' -
printf 'b\na\nb\n' | awk '!seen[$0]++' | paste -s -d ' ' -
```

```output for=bash-dedup-tools
b a b
a b
b a
```

- **Text mode can change the lines.** In Python text mode with the default `newline`, "Lines in the input can end in `'\n'`, `'\r'`, or `'\r\n'`, and these are translated into `'\n'` before being returned to the caller."[^3] So `a` CRLF and `a` LF become the same line. With `newline=''`, the line ends stay, and the two lines are different. In both modes, a CR alone also ends a line. The solution opens the file in binary mode, where the LF is the only line end:

```python run id=python-newline
import io

data = b"a\r\na\n"
for newline in (None, ""):
    f = io.TextIOWrapper(io.BytesIO(data), encoding="utf-8", newline=newline)
    lines = [line.removesuffix("\n") for line in f]
    print(repr(newline), list(dict.fromkeys(lines)))
```

```output for=python-newline
None ['a']
'' ['a\r', 'a']
```

- **Lines that look the same can be different.** In Unicode, "two sequences of code points have canonical equivalence if they represent the same abstract characters, and should always have the same visual appearance and behavior".[^15] U+00E9 and `e` with U+0301 are canonically equivalent, but their code points are not equal, so the solutions keep both lines. If the task must treat them as equal, normalize each line before the set check. `normalize()` "returns the Unicode Normalization Form of this string":[^15]

```javascript run id=javascript-normalize
// U+00E9, and e with the combining acute accent U+0301.
const lines = ["\u00e9", "e\u0301"];
console.log(lines[0] === lines[1]);
console.log(new Set(lines).size);
console.log(new Set(lines.map((line) => line.normalize("NFC"))).size);
```

```output for=javascript-normalize
false
2
1
```

- **Case and spaces.** The solutions compare the bytes, so `A` and `a` are two lines, and so are `a` and `a` with a space at the end. To ignore case or spaces, put a key in the set, for example the line in lowercase with no spaces at the ends, but put the line itself in the result.
- **Memory.** The set holds a copy of each different line. If most of the lines are different, the set needs about as much memory as the file. If the order of the result is not important, `LC_ALL=C sort -u` gives the same lines in sorted order.

## Related tasks

- [[sort-the-lines-of-a-file]]: sort the lines of a file, with the same rules for a line.
- [[count-the-lines-of-a-file]]: count the lines of a file.
- [[compute-word-frequencies]]: count each word, which is a set with a count for each key.
- [[unix-text-processing-pipeline]]: join `sort`, `uniq` and `awk` in a pipeline.

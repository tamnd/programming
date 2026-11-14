---
tag: QY21BX
kind: task
slug: print-the-first-and-last-lines-of-a-file
name: Print the first and last lines of a file
aliases: [head and tail, first N lines of a file, last N lines of a file, head -n, tail -n]
area: data-and-persistence/text-processing
level: intro
summary: Read a UTF-8 file one time and return its first N lines and its last N lines, as head -n and tail -n do, with a buffer that holds only N lines for the end.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/head.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), head"
  locator: DESCRIPTION; OPTIONS
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tail.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), tail"
  locator: DESCRIPTION; OPTIONS (-n)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 3, Definitions"
  locator: 3.172 Incomplete Line; 3.185 Line
- url: https://docs.python.org/3/library/functions.html#open
  title: The Python Standard Library, Built-in Functions
  locator: open (newline)
- url: https://docs.python.org/3/library/collections.html
  title: The Python Standard Library, collections
  locator: deque
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/slice
  title: MDN Web Docs, Array.prototype.slice()
  locator: Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Java SE 25 API, java.util.List
  locator: subList
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.getrange
  title: .NET API reference, List<T>.GetRange
- url: https://pkg.go.dev/bufio
  title: Go standard library, package bufio
  locator: Reader.ReadString
- url: https://doc.rust-lang.org/std/collections/struct.VecDeque.html
  title: The Rust Standard Library, std::collections::VecDeque
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fread.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), fread"
  locator: DESCRIPTION
- url: https://en.cppreference.com/w/cpp/container/deque
  title: cppreference.com, std::deque
- url: https://docs.python.org/3/library/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 3
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
  title: MDN Web Docs, String.prototype.split()
  locator: Description
---

## Task

Read a text file and a count N, and return two lists: the first N lines of the file and the last N lines of the file. These are the jobs of `head` and `tail`. POSIX says that `head` "shall copy its input files to the standard output, ending the output for each file at a designated point",[^1] and that `tail` "shall copy its input file to the standard output beginning at a designated place".[^2] With `-n N`, `head` gives the first N lines and `tail` gives the last N lines.

The task needs rules for what a line is and for a file that is short. This page uses these rules:

- **A line ends at LF.** POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character."[^3] The LF is not part of the line in the result. A CR before the LF stays in the line.
- **A last line with no LF is a line too.** POSIX calls it an incomplete line: "A sequence of one or more non-<newline> characters at the end of the file."[^3]
- **A short file gives all its lines.** For `head`, POSIX says: "When a file contains less than number bytes or lines, it shall be copied to standard output in its entirety. This shall not be an error."[^1] Here, if the file has fewer than N lines, each list has all the lines. So a line can be in the two lists.
- **N can be 0.** Then the two lists are empty.
- **Each list keeps the order of the file.** The last list starts with the line that is N lines before the end.

If the file does not exist, the solution reports an error.

## Solutions

The first N lines are easy: read lines until there are N. The last N lines are more difficult, because the code does not know which lines are the last N until it gets to the end of the file. There are two methods:

- **Keep only N lines.** Read the file one line at a time, and keep the last N lines in a queue. When the queue is full, remove the oldest line before you add the new one. At the end of the file, the queue holds the last N lines. The memory is for N lines, also for a very large file.
- **Read all the lines.** Read the whole file, split it into lines, and take the lines from index `max(len - N, 0)` to the end. The code is short, but the memory is for the whole file.

The Python, Go, Rust and C++ solutions keep only N lines. The JavaScript, Java, C# and C solutions read all the lines. The Bash solution uses `head` and `tail`. POSIX lets `tail` keep the end of the file in memory too: "Tails relative to the end of the file may be saved in an internal buffer, and thus may be limited in length."[^2]

- **Python.** `open(path, encoding='utf-8', newline='\n')` opens the file as text. With a value such as `'\n'`, "input lines are only terminated by the given string, and the line ending is returned to the caller untranslated",[^4] so a CR stays in the line. `deque(maxlen=count)` is the queue: "Once a bounded length deque is full, when new items are added, a corresponding number of items are discarded from the opposite end."[^5] The documentation adds: "Bounded length deques provide functionality similar to the tail filter in Unix."[^5] A list holds the first lines while it has fewer than `count`.
- **JavaScript and TypeScript (Node.js).** `readFileSync(path, 'utf8')` reads the file, and `split('\n')` gives the lines. If the last part is empty, the code removes it (see Pitfalls). `slice(0, count)` gives the first lines. For the last lines, the code computes `start = Math.max(lines.length - count, 0)` and calls `slice(start)`. It does not use `slice(-count)`: for a negative start, "start + array.length is used",[^6] but `-0` is not negative, so `slice(-0)` is `slice(0)`, which is all the lines. The TypeScript file adds types only.
- **Java.** `Files.readAllBytes(path)` gives the bytes, and a loop splits them at each LF into an `ArrayList`. `subList(0, take)` and `subList(size - take, size)` select the two parts. `subList` "Returns a view of the portion of this list",[^7] not a copy, so `List.copyOf` copies each part. The result is a record `Ends(first, last)`.
- **C#.** `File.ReadAllBytes(path)` gives the bytes, `Array.IndexOf` finds each LF, and `Encoding.UTF8.GetString` decodes each line. `GetRange(0, take)` and `GetRange(Count - take, take)` select the two parts. The second argument of `GetRange` is "The number of elements in the range",[^8] not an end index. The result is a tuple `(First, Last)`.
- **Go.** `bufio.Reader.ReadString('\n')` reads each line: it "reads until the first occurrence of delim in the input, returning a string containing the data up to and including the delimiter."[^9] `strings.TrimSuffix` removes the LF. The queue is a ring: a slice of `count` strings, in which line number `n` goes to index `n % count`, over the line that came `count` lines before it. At the end, the code reads the ring from the oldest line to the newest.
- **Rust.** `BufReader::read_line` reads each line, and `pop` removes the LF. The queue is a `VecDeque`, "A double-ended queue implemented with a growable ring buffer."[^10] When it has `count` lines, `pop_front` removes the oldest line, and then `push_back` adds the new line.
- **C.** A loop of `fread` reads the file into a buffer that grows. `fread` "shall read into the array pointed to by ptr up to nitems elements",[^11] and the loop stops at the end of the file. The code changes each LF to a NUL, so each line is a C string, and it puts all the lines into one array. The two parts point into that array: the first part starts at index 0, and the last part starts at index `n - take`. `first_and_last_lines` fills a `struct first_and_last`, which the caller frees with `first_and_last_free`, or it returns -1 and sets `errno`.
- **C++.** `std::getline` reads a binary `std::ifstream` one line at a time. The queue is a `std::deque`, which "allows fast insertion and deletion at both its beginning and its end."[^12] The result is a `std::expected` (C++23) that holds a pair of vectors, and the errors are `cannot_open` and `read_failed`.
- **Bash.** `head -n N -- FILE` and `tail -n N -- FILE` do the work, and `mapfile -t` puts their output into two arrays. `mapfile -t` removes the LF at the end of each line, and keeps a CR. The function takes the names of the two arrays, and `local -n` refers to them, so it needs Bash 4.3 or later. If N is 0, the function does not run `head` (see Pitfalls).

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`.

- `two`: N = 2 on `a` to `e` gives `a`, `b` and `d`, `e`.
- `one`: N = 1 on `first`, `middle`, `last` gives `first` and `last`.
- `ten`: N = 10 on the numbers 1 to 12 gives 1 to 10 and 3 to 12. The default of `head` is 10 lines: "If no options are specified, head shall act as if -n 10 had been specified."[^1]
- `empty`: an empty file gives two empty lists.
- `zero`: N = 0 gives two empty lists.
- `short`: N = 5 on two lines gives the two lines in each list.
- `exact`: N = 2 on two lines gives the two lines in each list.
- `overlap`: N = 2 on `a`, `b`, `c` gives `a`, `b` and `b`, `c`. The line `b` is in the two lists.
- `one-line`: N = 1 on the one line `a` gives `a` in each list.
- `empty-lines` and `only-newline`: an empty line at the start or the end is a line, so N = 1 gives an empty line in each list.
- `no-final-newline`: N = 1 on `a`, `b`, `c` with no LF at the end gives `a` and `c`.
- `crlf`: N = 1 on `a` CRLF, `b` CRLF gives `a` CR and `b` CR.
- `non-ascii`: N = 1 on `café`, `naïve`, `zoë` gives `café` and `zoë`.

The tests also write a file of 100000 lines with the numbers 0 to 99999. N = 3 must give 0, 1, 2 and 99997, 99998, 99999. The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **`tail -n +N` counts from the start.** The sign of the number tells `tail` where to start: a `+` is "Relative to the beginning of the file", and a `-` or no sign is "Relative to the end of the file".[^2] "The origin for counting shall be 1; that is, -n +1 represents the first line of the file, -n -1 the last."[^2] So `tail -n +2` gives all the lines from line 2, not the last two lines:

```bash run id=bash-tail-plus
f=$(mktemp)
printf 'a\nb\nc\nd\ne\n' >"$f"
echo "tail -n 2:"
tail -n 2 "$f"
echo "tail -n +2:"
tail -n +2 "$f"
rm "$f"
```

```output for=bash-tail-plus
tail -n 2:
d
e
tail -n +2:
b
c
d
e
```

- **`head -n 0` can fail.** POSIX says for `head`: "The application shall ensure that the number option-argument is a positive decimal integer."[^1] 0 is not positive, so a version of `head` can refuse it. The Bash solution does not run `head` or `tail` when N is 0.
- **A negative index of 0 is not from the end.** In Python, for a negative index, "len(s) + i or len(s) + j is substituted. But note that -0 is still 0".[^13] So `lines[-count:]` is correct for a count of 1 or more, but for a count of 0, it gives all the lines. `lines[max(len(lines) - count, 0):]` is correct for each count:

```python run id=python-negative-zero
lines = ["a", "b", "c", "d"]
for count in [2, 1, 0]:
    print(count, lines[-count:], lines[max(len(lines) - count, 0):])
```

```output for=python-negative-zero
2 ['c', 'd'] ['c', 'd']
1 ['d'] ['d']
0 ['a', 'b', 'c', 'd'] []
```

- **The LF at the end of the file makes an empty part.** If the file ends with an LF, `split('\n')` gives an empty string after it. MDN says: "If separator appears at the beginning (or end) of the string, it still has the effect of splitting, resulting in an empty (i.e., zero length) string appearing at the first (or last) position of the returned array."[^14] So the last line seems to be empty. Remove the empty part before you take the last lines:

```javascript run id=javascript-split-end
const text = "a\nb\n";
const parts = text.split("\n");
console.log(parts);
console.log(parts.slice(-1));
if (parts.at(-1) === "") parts.pop();
console.log(parts.slice(-1));
```

```output for=javascript-split-end
[ 'a', 'b', '' ]
[ '' ]
[ 'b' ]
```

- **The memory for a large file.** A solution that reads all the lines needs memory for the whole file, also when N is small. For a large file, keep only N lines in a queue, as the Python, Go, Rust and C++ solutions do.

## Related tasks

- [[count-the-lines-of-a-file]]: count the lines of a file, with the same rules for a line.
- [[filter-lines-that-match-a-pattern]]: keep the lines in which a regular expression matches.
- [[line-ending]]: LF, CRLF and CR, and why a CR stays in the line.
- [[unix-text-processing-pipeline]]: join `head` and `tail` with other tools in a pipeline.

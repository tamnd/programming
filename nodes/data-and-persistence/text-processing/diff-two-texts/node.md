---
tag: XTRFFG
kind: task
slug: diff-two-texts
name: Diff two texts
aliases: [compare two files line by line, line diff, edit script of two files, shortest edit script, diff with a longest common subsequence]
area: data-and-persistence/text-processing
level: core
summary: Read two UTF-8 files, split each into lines, and return a shortest edit script of kept, deleted and added lines, made from a longest common subsequence of the lines.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/diff.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), diff"
  locator: DESCRIPTION; EXIT STATUS
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Base Definitions, Chapter 3, Definitions"
  locator: 3.172 Incomplete Line; 3.185 Line
- url: https://pkg.go.dev/strings
  title: Go standard library, package strings
  locator: Split
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: The Rust Standard Library, str
  locator: split_terminator
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Reference Manual, Bash Builtin Commands
  locator: mapfile
- url: https://www.gnu.org/software/diffutils/manual/html_node/Overview.html
  title: Comparing and Merging Files (GNU Diffutils manual), Overview
- url: https://docs.python.org/3/library/difflib.html
  title: The Python Standard Library, difflib
  locator: class Differ
- url: https://git-scm.com/docs/git-diff
  title: Git documentation, git-diff
  locator: OPTIONS (--minimal; --diff-algorithm)
- url: https://www.gnu.org/software/diffutils/manual/html_node/Incomplete-Lines.html
  title: Comparing and Merging Files (GNU Diffutils manual), Incomplete Lines
- url: https://docs.python.org/3/library/io.html
  title: The Python Standard Library, io
  locator: TextIOWrapper (newline)
---

## Task

Compare two text files line by line, and show how to change the lines of the first file into the lines of the second. The POSIX `diff` utility writes a list of such changes, and POSIX says: "This list should be minimal. No output shall be produced if the files are identical."[^1]

The list of changes is an edit script. In this task, the script has one line for each line of the two files that it uses. The task needs rules for what a line is, when two lines are equal, and which script to give. This page uses these rules:

- **A line ends at LF.** POSIX defines a line as "A sequence of zero or more non-<newline> characters plus a terminating <newline> character."[^2] The LF is not part of the line. A CR before the LF stays in the line, so `a` CRLF and `a` LF are two different lines.
- **A last line with no LF is a line too.** POSIX calls it an incomplete line: "A sequence of one or more non-<newline> characters at the end of the file."[^2] This page compares it as a full line, so the texts `a`, LF, `b` and `a`, LF, `b`, LF have the same lines.
- **Two lines are equal if their bytes are equal.** The solutions do not ignore case, do not remove spaces, and do not normalize Unicode text.
- **The script is a shortest script.** The lines that stay are a longest common subsequence (LCS) of the two lists of lines. An LCS is a longest list of lines that are in the two files in the same order, but not always next to each other. So the script has the smallest number of lines that go and lines that come.
- **Each line of the script is a mark and a line.** The mark is a space for a line that stays, `-` for a line of the old file that goes, and `+` for a line of the new file that comes. Each line of the script ends with an LF. Two empty files give an empty script.
- **Ties have a fixed order.** Many scripts can be equally short. The script starts at the first line of each file. If the next lines of the two files are equal, the line stays. If a `-` and a `+` give equally short scripts, the `-` comes first.

For example, the old file has the lines `a`, `b`, `c`, and the new file has the lines `a`, `c`, `d`. The LCS is `a`, `c`, and the script is:

```text
 a
-b
 c
+d
```

If a file does not exist, the solution reports an error.

## Solutions

All the solutions use the same method, which is dynamic programming on a table of LCS lengths. The steps:

1. Split each text into a list of lines: `a` has n lines and `b` has m lines. A final LF does not start a new line.
2. Make a table of (n+1) × (m+1) numbers. The cell (i, j) holds the length of an LCS of the lines from `a[i]` to the end of `a` and the lines from `b[j]` to the end of `b`. Row n and column m hold 0.
3. Fill the table from the last row and the last column back to the cell (0, 0). If `a[i]` and `b[j]` are equal, the cell is 1 more than the cell (i+1, j+1). If not, the cell is the larger of the cells (i+1, j) and (i, j+1).
4. Start at the cell (0, 0). If `a[i]` and `b[j]` are equal, write a space and the line, and go to the cell (i+1, j+1). If not, and the cell (i+1, j) is not smaller than the cell (i, j+1), write `-` and `a[i]`, and go to the cell (i+1, j). If not, write `+` and `b[j]`, and go to the cell (i, j+1).
5. When one list has no more lines, write `-` and each line that is left in `a`, and then `+` and each line that is left in `b`.

Each cell holds the length for the ends of the two lists, so step 4 can start at the first lines, and the script comes out in order. The table is one flat array, and the cell (i, j) is at the index i × (m+1) + j. The time and the memory grow with n × m.

- **Python.** `open(path, 'rb')` reads the bytes of each file, and `decode('utf-8')` makes a `str`. The solution does not use text mode, because text mode changes CRLF to LF (see the pitfalls). `text.split('\n')` gives an empty last part when the text ends with an LF, and `split_lines` removes it. The table is a list of `int`. A missing file gives `FileNotFoundError`.
- **JavaScript and TypeScript (Node.js).** `readFileSync(path, 'utf8')` reads each file as a string. `split('\n')` and the same removal of the empty last part give the lines. The table is an `Int32Array`. A missing file gives an error with the code `ENOENT`. The TypeScript file adds types only.
- **Java.** `Files.readString(path)` reads each file as UTF-8 text. A loop with `indexOf('\n', start)` splits the text, and it does not add an empty line after a final LF. `equals` compares two lines, and the table is an `int[]`. A missing file gives `NoSuchFileException`.
- **C#.** `File.ReadAllText(path, Encoding.UTF8)` reads each file. `Split('\n')` and the removal of the empty last part give the lines. `string.Equals` with `StringComparison.Ordinal` compares two lines, and the table is an `int[]`. A missing file gives `FileNotFoundException`.
- **Go.** `os.ReadFile` reads the bytes of each file. `strings.Split` "slices s into all substrings separated by sep and returns a slice of the substrings between those separators."[^3] So a text that ends with an LF gives an empty last part, and `SplitLines` removes it. The table is a `[]int32`, and a `strings.Builder` collects the script. A missing file gives an error that matches `fs.ErrNotExist`.
- **Rust.** `fs::read_to_string` reads each file. `split_terminator('\n')` gives the lines with no extra step. It works as `split` does, except that "the trailing substring is skipped if empty."[^4] The table is a `Vec<u32>`. A missing file gives an `io::Error` of the kind `NotFound`.
- **C.** `diff_files` reads each file in binary mode with `fread`. `split_lines` makes an array of `struct line`, which holds a pointer into the text and a length, so the lines are not copied. `memcmp` compares two lines of the same length. The table is an array of `size_t` from `calloc`, and the code makes sure that (n+1) × (m+1) cells fit in a `size_t` first. Each line of the two files is in the script once, so the result needs the bytes of the two texts, plus a mark and an LF for each line. `diff_lines` and `diff_files` return a new string that the caller frees, or `NULL` with `errno` set to the error of `fopen`, to `EIO`, or to `ENOMEM`.
- **C++.** `diff_files` reads each file with a binary `std::ifstream`. `split_lines` gives a `std::vector<std::string_view>` that points into the text, and the table is a `std::vector<std::size_t>`. The result is a `std::expected<std::string, DiffError>` (C++23), and the errors are `cannot_open` and `read_failed`.
- **Bash.** `mapfile -t a` reads the lines of its standard input, here the first file, into the array `a`. The `-t` option will "Remove a trailing delim (default newline) from each line read."[^5] A last line with no LF is also read, and a CR stays in its line. The table is an indexed array, and the code reads an element that is not set as 0. The function prints the script with one `printf` for all its lines. If a file cannot be read, the function prints an error message on the standard error and returns 1. The two loops run n × m steps in the shell, so this solution is slow for large files.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with the files in `fixtures/`.

- `simple`: `a`, `b`, `c` and `a`, `c`, `d` give `' a\n-b\n c\n+d\n'`.
- `change-a-line`: a line that changes is a `-` and a `+`: `' one\n-two\n+2\n three\n'`.
- `identical`: each line stays: `' a\n b\n'`. POSIX `diff` writes nothing for two equal files, but this script still has the lines.
- `lines-of-a-poem`: a line changes, and a line comes at the end.
- `both-empty`, `old-empty`, `new-empty`: two empty files give `''`. An empty old file gives a `+` for each line, and an empty new file gives a `-` for each line.
- `all-different`: `a`, `b` and `c`, `d` give `'-a\n-b\n+c\n+d\n'`. The `-` lines come first.
- `insert-at-start`, `delete-at-end`: `'+a\n b\n c\n'` and `' a\n b\n-c\n'`.
- `swap`: `a`, `b` and `b`, `a` give `'-a\n b\n+a\n'`. The script `'+b\n a\n-b\n'` is equally short, but the tie rule puts the `-` first.
- `repeated-lines`: `a`, `a`, `b` and `a`, `b`, `b` give `' a\n-a\n b\n+b\n'`.
- `empty-line`: `a`, an empty line, `b` and `a`, `b` give `' a\n-\n b\n'`. An empty line that goes is a `-` alone.
- `no-final-newline`: `'a\nb'` and `'a\nb\n'` give `' a\n b\n'`.
- `crlf`: `'a\r\nb\n'` and `'a\nb\n'` give `'-a\r\n+a\n b\n'`. The CR stays in the line.
- `trailing-space`: `'a \nb\n'` and `'a\nb\n'` give `'-a \n+a\n b\n'`.
- `normalization`: U+00E9, and `e` with U+0301, give a `-` and a `+`. The two lines look the same, but their bytes are not equal.

The tests also compare two texts of 1000 lines, the numbers 0 to 999. In the new text, the 10 lines 50, 150, and so on to 950 change from n to xn. The script must have 1010 lines, its lines 50 to 52 must be `' 49'`, `'-50'` and `'+x50'`, and its last line must be `' 999'`. The Bash test uses 100 lines and 10 changes, and the script must have 110 lines. The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **The table needs n × m memory.** For two files of 10000 lines each, the table has 10001 × 10001 = 100020001 cells. With 4 bytes for each cell, that is about 400 MB. GNU `diff` uses another method. Its manual says that the basic algorithm is the one that Eugene W. Myers describes in the 1986 paper An O(ND) Difference Algorithm and its Variations, and also: "Unless the --minimal option is used, diff uses a heuristic by Paul Eggert that limits the cost to O(N^1.5 log N) at the price of producing suboptimal output for large inputs with many differences."[^6] So the default script of GNU `diff` is not always a shortest script.
- **A shortest script is not always the clearest.** The Python `difflib` documentation says that "minimal diffs are often counter-intuitive for humans, because they synch up anywhere possible, sometimes at accidental matches 100 pages apart."[^7] Git has a choice of algorithms. Its default, myers, is "The basic greedy diff algorithm. Currently, this is the default."[^8] With `--minimal`, Git will "Spend extra time to make sure the smallest possible diff is produced."[^8] The patience and histogram algorithms are also available. Two tools can also give two different scripts of the same length. For the `swap` case, `difflib` keeps the line `a`, and this page keeps the line `b`:

```python run id=python-difflib-tie
import difflib

for line in difflib.ndiff(["a", "b"], ["b", "a"]):
    print(repr(line))
```

```output for=python-difflib-tie
'+ b'
'  a'
'- b'
```

- **An incomplete line is not a full line for `diff`.** This page compares a last line with no LF as a full line, so the script does not show that the LF is missing. GNU `diff` does not do this: "Incomplete lines do not match full lines unless differences in white space are ignored".[^9] It writes a line that starts with a backslash after the incomplete line. The exit status 1 means that "Differences were found and all differences were successfully output."[^1] A script that must rebuild the new file byte for byte needs such a mark:

```bash run id=bash-incomplete-line
dir=$(mktemp -d)
printf 'a\nb' >"$dir/old.txt"
printf 'a\nb\n' >"$dir/new.txt"
diff "$dir/old.txt" "$dir/new.txt"
echo "status $?"
rm -r "$dir"
```

```output for=bash-incomplete-line
2c2
< b
\ No newline at end of file
---
> b
status 1
```

- **Text mode can remove the CR.** In Python text mode with the default `newline`, a line in the input can end in `'\n'`, `'\r'` or `'\r\n'`, and the documentation says that "these are translated into" `'\n'` before the caller gets the line.[^10] So a CRLF file and an LF file seem to have the same lines, and the script does not show the change. With `newline=''`, the line ends stay. The solutions read the bytes, or a mode that does not change them:

```python run id=python-text-mode
import io


def lines(data, newline):
    f = io.TextIOWrapper(io.BytesIO(data), encoding="utf-8", newline=newline)
    return f.read().split("\n")


old, new = b"a\r\nb\n", b"a\nb\n"
print(lines(old, None) == lines(new, None))
print(lines(old, "") == lines(new, ""))
```

```output for=python-text-mode
True
False
```

- **Lines that look the same can be different.** A CR, a space at the end of a line, or another Unicode form of the same text makes two lines different, and the script shows a `-` and a `+` for them. To ignore such changes, compare a key for each line, for example the line with no spaces at the end, but write the line itself in the script.

## Related tasks

- [[remove-duplicate-lines]]: compare the lines of a file by their bytes, with the same rules for a line.
- [[sort-the-lines-of-a-file]]: sort the lines of a file, with the same rules for a line.
- [[count-the-lines-of-a-file]]: count the lines of a file, with the same rules for a line.
- [[line-ending]]: LF, CRLF, and a last line with no line ending.
- [[unix-text-processing-pipeline]]: join small text tools in a pipeline.

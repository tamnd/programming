---
tag: X4069W
kind: task
slug: apply-a-patch
name: Apply a patch
aliases: [apply a unified diff, patch a file, apply a diff to a text, apply hunks, strict patch]
area: data-and-persistence/text-processing
level: core
summary: Read a UTF-8 file and a unified diff, and return the new text. Each hunk must match the old lines at the place that its header gives, with no fuzz and no offset.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://www.gnu.org/software/diffutils/manual/html_node/Overview.html
  title: Comparing and Merging Files (GNU Diffutils manual), Overview
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/patch.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), patch"
  locator: DESCRIPTION; EXTENDED DESCRIPTION, Patch File Format, Patch Application
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/diff.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), diff"
  locator: STDOUT, Diff -u or -U Output Format
- url: https://www.gnu.org/software/diffutils/manual/html_node/Incomplete-Lines.html
  title: Comparing and Merging Files (GNU Diffutils manual), Incomplete Lines
- url: https://www.gnu.org/software/diffutils/manual/html_node/Inexact.html
  title: Comparing and Merging Files (GNU Diffutils manual), Helping patch Find Inexact Matches
- url: https://git-scm.com/docs/git-apply
  title: Git documentation, git-apply
  locator: OPTIONS (-C, --unidiff-zero, --reject)
- url: https://www.gnu.org/software/diffutils/manual/html_node/Unified-Format.html
  title: Comparing and Merging Files (GNU Diffutils manual), Unified Format
- url: https://www.gnu.org/software/diffutils/manual/html_node/Reversed-Patches.html
  title: Comparing and Merging Files (GNU Diffutils manual), Applying Reversed Patches
- url: https://www.gnu.org/software/diffutils/manual/html_node/Imperfect.html
  title: Comparing and Merging Files (GNU Diffutils manual), Applying Imperfect Patches
- url: https://docs.python.org/3/library/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: str.splitlines
---

## Task

Read a text file and a unified diff for it, and make the new text. The page [[unified-diff-format]] describes the format, and [[diff-two-texts]] finds the changes. The GNU manual says: "If you think of diff as subtracting one file from another to produce their difference, you can think of patch as adding the difference to one file to reproduce the other."[^1]

A patch tool must find the lines of each hunk in the old file, and it must decide what to do when they are not there. This page uses these rules:

- **A line ends after LF.** A CR before the LF stays in the line, so `a` CRLF and `a` LF are two different lines. A last line with no LF is a line too.
- **The lines outside the hunks are skipped.** These are the `---` and `+++` lines, and the `diff --git` and `index` lines of Git. POSIX says that "The patch file shall contain zero or more lines of header information followed by one or more patches."[^2] A patch with no hunk is an error.
- **A hunk starts with a header.** The header is `@@ -start,count +start,count @@`. A missing count is 1, and the text after the second `@@` is skipped. For a range with no lines, POSIX says: "If a range is empty, its beginning line number shall be the number of the line just before the range, or 0 if the empty range starts the file."[^3]
- **Each line of a hunk has a mark.** A space is a line that stays, `-` is a line that goes, and `+` is a line that comes. An empty line is an empty line that stays, because some tools remove the space at the end of a line. A line that starts with a backslash, such as `\ No newline at end of file`, removes the LF of the line before it[^4]. The two counts of the header give the end of the hunk.
- **No fuzz and no offset.** The space lines and the `-` lines of a hunk must be equal to the lines of the old file at the start of the old range. If the old range is empty, the hunk goes after the line `start`. The solution does not look for the lines at another place.
- **The hunks are in order.** POSIX says: "The line numbers for successive hunks within a patch shall occur in ascending order."[^2] The lines of the old file between two hunks stay.
- **Errors.** If a file does not exist, the solution reports `not-found`. If the patch is not a correct unified diff, it reports `malformed`. If the lines of a hunk are not in the old file at their place, it reports `mismatch`. The two patch errors give the number of the line of the patch where the error is.

For example, the old file has the lines `a`, `b`, `c`, and the patch is:

```text
--- old.txt
+++ new.txt
@@ -1,3 +1,3 @@
 a
-b
+B
 c
```

The old range starts at line 1, and the lines `a`, `b`, `c` are there. The new text is `a`, `B`, `c`.

## Solutions

All the solutions use the same method. They read the patch once, and they copy the old file once. The steps:

1. Split the old text after each LF, so that each line keeps its LF. Split the patch in the same way, and remove the LF of each line of the patch.
2. Skip the lines of the patch until a line starts with `@@`. Read the four numbers of the header. If the header is not correct, the patch is malformed.
3. Find the place of the hunk. If the old count is more than 0, the hunk starts at the line `start`, which has the index start − 1. If the old count is 0, the hunk starts after the line `start`, at the index `start`. If this index is before the next old line that is not used yet, or after the end of the old text, the hunk does not match. Copy the old lines up to the index.
4. Read the lines of the hunk until the two counts are used up. A space line counts in the two ranges, a `-` line in the old range, and a `+` line in the new range. A backslash line does not count, and it marks the line before it as a line with no LF. Another mark, a line that makes a count less than 0, or the end of the patch before the counts are used up, makes the patch malformed. After the counts, one more backslash line can come.
5. Apply the lines of the hunk. Each space line and each `-` line, with its LF or with no LF, must be equal to the next old line. If it is not, the hunk does not match. A space line and a `+` line go to the new text.
6. After the last hunk, copy the old lines that are left.

The time grows with the sizes of the old text and the patch. The new text is never longer than the two of them together, because each byte comes from an old line or from a `+` line of the patch.

- **Python.** `open(path, 'rb')` reads the bytes of each file, and `decode('utf-8')` makes a `str`. The bytes keep each CR. `split_lines` uses `str.find('\n')`, not `str.splitlines` (see the pitfalls). A regular expression from `re` reads the header. The errors are `PatchError`, a `ValueError` with the attributes `kind` and `line`, and `FileNotFoundError` for a missing file.
- **JavaScript and TypeScript (Node.js).** `readFileSync(path, 'utf8')` reads each file as a string. A `RegExp` reads the header. `PatchError` extends `Error` with the properties `kind` and `line`. A missing file gives an error with the code `ENOENT`. The TypeScript file adds types only, and the kind has the type `"malformed" | "mismatch"`.
- **Java.** `Files.readString(path)` reads each file as UTF-8 text. A `Pattern` and `Matcher.lookingAt` read the header. `ApplyPatch.PatchException` is a `RuntimeException` with a `Kind` of `MALFORMED` or `MISMATCH`, and a line number. A missing file gives `NoSuchFileException`.
- **C#.** `File.ReadAllText(path, Encoding.UTF8)` reads each file. A `[GeneratedRegex]` pattern reads the header. `string.Equals` with `StringComparison.Ordinal` compares two lines. `PatchException` has a `Kind` and a `Line`. A missing file gives `FileNotFoundException`.
- **Go.** `os.ReadFile` reads the bytes of each file. `regexp` reads the header. The error is a `*PatchError` with a `Kind` and a `Line`, and `errors.Is` matches it with `ErrMalformed` or `ErrMismatch`. A missing file gives an error that matches `fs.ErrNotExist`.
- **Rust.** `fs::read_to_string` reads each file, and `split_inclusive('\n')` gives the lines with their LF. A small parser with `strip_prefix` and `parse` reads the header, with no crate. `PatchError` is an enum with the variants `Io`, `Malformed` and `Mismatch`. A missing file gives `PatchError::Io` with an `io::Error` of the kind `NotFound`.
- **C.** `apply_patch_file` reads each file in binary mode with `fread`. The lines are arrays of `struct span`, a pointer into the text and a length, so the lines are not copied. Each hunk has two passes: the first pass checks the marks and the counts, and the second pass applies the lines. The result has space for the bytes of the two texts, from one `malloc`. The functions return an `enum patch_status`: `PATCH_OK`, `PATCH_IO` with `errno` set, `PATCH_NOMEM`, `PATCH_MALFORMED` or `PATCH_MISMATCH`. For the two patch errors, the line number goes to `*line`.
- **C++.** `apply_patch_file` reads each file with a binary `std::ifstream`. `split_lines` gives a `std::vector<std::string_view>` that points into the text, and `std::from_chars` reads the numbers of the header. The result is a `std::expected<std::string, PatchError>` (C++23). The kinds of the error are `cannot_open`, `read_failed`, `malformed` and `mismatch`.
- **Bash.** `mapfile lines` with no `-t` reads the old file, so each line keeps its LF and a last line with no LF has none. `mapfile -t p` reads the patch with no LF. The `=~` operator reads the header into `BASH_REMATCH`, with at most 18 digits for each number. The function prints the new text with `printf '%s'`. It returns 1 if a file cannot be read, 2 if a hunk does not match, and 3 if the patch is malformed, with a message on the standard error.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with the files in `fixtures/`.

- `change-a-line`: the example of the task.
- `two-hunks`: two hunks change `banana` and `grape`, and the lines between them stay.
- `git-header`: the `diff --git` and `index` lines are skipped, and the text `def main():` after the second `@@` is skipped.
- `add-at-end`: `@@ -1 +1,2 @@` adds a line after the last line.
- `old-empty`, `delete-all`: `@@ -0,0 +1,2 @@` makes two lines from an empty file, and `@@ -1,2 +0,0 @@` gives an empty text.
- `no-count`: `@@ -2 +2 @@` has no counts, and each range has one line.
- `zero-context`, `insert-at-start`: hunks with no context. In `@@ -6 +5,0 @@` the new range is empty, and in `@@ -7,0 +7 @@` the old range is empty, so the new line goes after line 7. `@@ -0,0 +1 @@` adds a line before the first line.
- `old-no-final-newline`, `new-no-final-newline`: a backslash line after the `-` line or after the `+` line. The first case adds the LF, and the second case removes it.
- `empty-context-line`, `space-context-line`: an empty line and a line with one space in a hunk are the same empty line that stays.
- `crlf`: the CR of each line stays, and the lines of the patch have the CR too.
- `header-like-line`: the old line `-- y` goes, so the patch has the line `--- y`. The counts say that this line is in the hunk, so it is not a header.
- `non-ascii`: `café` changes to `naïve`.

The case in `cases/errors.jsonl` with the name `not-found` must give the error `not-found`. The case `mismatch` has the `-` line `x` where the old file has `b`. The case `malformed` has a hunk that ends before its counts are used up. The tests also give `malformed` for a patch with no hunk and for a line with the mark `*`, and `mismatch` for a hunk after the end of the file. The tests also apply 10 hunks with no context to a text of 1000 lines, the numbers 0 to 999. The hunks change the lines 50, 150, and so on to 950 from n to xn. The Bash test uses 100 lines and 10 hunks. The toolchain does not load `cases/errors.jsonl` yet, so each test file holds these cases by name.

## Pitfalls

- **Other tools look for the hunk at another place.** POSIX says: "For each hunk, the patch utility shall begin to search for the place to apply the patch at the line number at the beginning of the hunk, plus or minus any offset used in applying the previous hunk."[^2] GNU `patch` also uses a fuzz factor: it can ignore the first and the last lines of context. The GNU manual says: "The default fuzz factor is 2"[^5], and "Note that a larger fuzz factor increases the odds of making a faulty patch."[^5] In this example, a line came at the top of the file, and then a line of context changed. GNU `patch` applies the hunk in the two files:

```bash run id=bash-patch-offset-and-fuzz
dir=$(mktemp -d) && cd "$dir" || exit 1
trap 'rm -rf "$dir"' EXIT
printf '%s\n' a b c d e f g >old.txt
sed 's/^d$/D/' old.txt >new.txt
diff -u old.txt new.txt | tail -n +3 >change.patch
head -n 1 change.patch
# A line came at the top of the file after the diff was made.
{ echo top; cat old.txt; } >moved.txt
patch moved.txt <change.patch
# A line of context changed after the diff was made.
sed 's/^a$/A/' old.txt >fuzzy.txt
patch fuzzy.txt <change.patch
paste -s -d ' ' moved.txt
paste -s -d ' ' fuzzy.txt
```

```output for=bash-patch-offset-and-fuzz
@@ -1,7 +1,7 @@
patching file moved.txt
Hunk #1 succeeded at 2 (offset 1 line).
patching file fuzzy.txt
Hunk #1 succeeded at 1 with fuzz 1.
top a b c D e f g
A b c D e f g
```

This page gives `mismatch` for the two files. That is safer when the file changed in a way that the patch does not know, but it fails for a correct patch of a file that changed in another place. Git does not use fuzz: for `git apply -C`, the documentation says "By default no context is ever ignored."[^6] Also, the GNU manual says that "the results are guaranteed only when the patch is applied to an exact copy of the file that the patch was generated from."[^5]

- **A hunk with no context fits at any place.** "For proper operation, patch typically needs at least three lines of context."[^7] Git says: "By default, git apply expects that the patch being applied is a unified diff with at least one line of context."[^6] A hunk that only adds lines, with an empty old range and no context, has no old lines to check. So this page applies such a hunk again if the patch is applied two times, and the lines come two times. A hunk with `-` lines or with context gives `mismatch` the second time. GNU `patch` takes a patch that was applied before as a reversed patch: "If you apply a patch that you have already applied, patch thinks it is a reversed patch and offers to un-apply the patch."[^8]
- **Line ends must be the same.** A patch with LF lines does not match a file with CRLF lines, because the CR is part of each line. GNU `patch` also stops, and it gives the reason:

```bash run id=bash-patch-crlf
dir=$(mktemp -d) && cd "$dir" || exit 1
trap 'rm -rf "$dir"' EXIT
printf 'a\r\nb\r\n' >file.txt
printf '%s\n' '--- file.txt' '+++ file.txt' '@@ -1,2 +1,2 @@' ' a' '-b' '+c' >lf.patch
patch --dry-run file.txt <lf.patch
echo "status $?"
```

```output for=bash-patch-crlf
checking file file.txt
Hunk #1 FAILED at 1 (different line endings).
1 out of 1 hunk FAILED
status 1
```

GNU `patch` can remove a CR in some cases: "If a context diff contains trailing carriage return on each line, patch automatically ignores the carriage return."[^9] This page does not change the CR. Make the patch from files with the same line ends as the file that gets the patch.

- **The backslash line changes the last line.** GNU `diff` writes `\ No newline at end of file` after a line with no LF, and "Incomplete lines do not match full lines unless differences in white space are ignored"[^4]. In this example, the old file and the new file have no LF at the end. `patch` keeps the new file with no LF, as `od -c` shows:

```bash run id=bash-patch-incomplete-line
dir=$(mktemp -d) && cd "$dir" || exit 1
trap 'rm -rf "$dir"' EXIT
printf 'a\nb' >old.txt
printf 'a\nc' >new.txt
diff -u old.txt new.txt | tail -n +3 >change.patch
cat change.patch
patch old.txt <change.patch
od -c old.txt
```

```output for=bash-patch-incomplete-line
@@ -1,2 +1,2 @@
 a
-b
\ No newline at end of file
+c
\ No newline at end of file
patching file old.txt
0000000   a  \n   c
0000003
```

A tool that skips the backslash line adds an LF to the last line. A tool that takes the backslash line as a line of the hunk counts one line too many.

- **Split the text only at LF.** In Python, `str.splitlines` splits at more characters than LF. The documentation says: "This method splits on the following line boundaries."[^10] Its list has LF, CR, CRLF, form feed, U+2028 and other characters. So a form feed, a lone CR or U+2028 in a line makes two lines, and the line numbers of the patch point at the wrong lines:

```python run id=python-splitlines
text = "a\x0cb\rc\u2028d\n"
print(text.splitlines(keepends=True))
print(text.split("\n"))
```

```output for=python-splitlines
['a\x0c', 'b\r', 'c\u2028', 'd\n']
['a\x0cb\rc\u2028d', '']
```

- **Read the hunk by its counts.** A deleted line with the text `-- y` is the line `--- y` in the patch. A reader that looks for a `---` header in each line takes it for the start of a new file. Look for a header only after the counts of the hunk are used up.
- **Do not change a part of the file.** If one hunk does not match, the solution returns an error and no text. Git does the same for the whole patch: "For atomicity, git apply by default fails the whole patch and does not touch the working tree when some of the hunks do not apply."[^6] GNU `patch` and `git apply --reject` apply the hunks that match, and write the other hunks to a `.rej` file[^2][^6].

## Related tasks

- [[diff-two-texts]]: find the lines that go and the lines that come.
- [[unified-diff-format]]: the format of the patch.
- [[line-ending]]: LF, CRLF, and a last line with no line ending.
- [[remove-duplicate-lines]]: compare the lines of a file by their bytes, with the same rules for a line.
- [[unix-text-processing-pipeline]]: join small text tools in a pipeline.

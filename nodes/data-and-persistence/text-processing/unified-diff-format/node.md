---
tag: 31R7DR
kind: format
slug: unified-diff-format
name: Unified diff format
aliases: [unified diff, unified format, diff -u, udiff, unidiff, patch file, hunk header]
area: data-and-persistence/text-processing
level: core
summary: >-
  A unified diff shows the changes between two text files as hunks. Each hunk has a header with the line ranges in the old and the new file, then the lines with a mark: a space, - or +.
see_also: [diff-two-texts, line-ending, git, json-patch]
sources:
- url: https://www.gnu.org/software/diffutils/manual/html_node/Unified-Format.html
  title: Comparing and Merging Files (GNU Diffutils manual), Unified Format
- url: https://www.gnu.org/software/diffutils/manual/html_node/Hunks.html
  title: Comparing and Merging Files (GNU Diffutils manual), Hunks
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/diff.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), diff"
  locator: OPTIONS (-u, -U); STDOUT, Diff -u or -U Output Format; CHANGE HISTORY
- url: https://www.gnu.org/software/diffutils/manual/html_node/Overview.html
  title: Comparing and Merging Files (GNU Diffutils manual), Overview
- url: https://www.gnu.org/software/diffutils/manual/html_node/Detailed-Unified.html
  title: Comparing and Merging Files (GNU Diffutils manual), Detailed Description of Unified Format
- url: https://docs.python.org/3/library/difflib.html
  title: The Python Standard Library, difflib
  locator: unified_diff
- url: https://www.gnu.org/software/diffutils/manual/html_node/Incomplete-Lines.html
  title: Comparing and Merging Files (GNU Diffutils manual), Incomplete Lines
- url: https://www.gnu.org/software/diffutils/manual/html_node/Example-Unified.html
  title: Comparing and Merging Files (GNU Diffutils manual), An Example of Unified Format
- url: https://git-scm.com/docs/git-diff
  title: Git documentation, git-diff
  locator: OPTIONS (-U, --unified); Generating patch text with -p
---

## What it is

A unified diff is a text format for the changes between two text files. `diff -u` writes it, and GNU `patch` reads it[^1]. The GNU manual says that `diff` finds "sequences of lines common to both files, interspersed with groups of differing lines called hunks"[^2]. A unified diff writes each hunk with a few unchanged lines around it, the context.

The format is "a variation on the context format that is more compact because it omits redundant context lines"[^1]. POSIX says that "the context lines are not repeated; instead, the context, deleted, and added lines are shown together, interleaved."[^3] So each line of a hunk is in the diff once, with a mark at its start.

"Wayne Davison designed and implemented the unified output format."[^4] "In the early 1990s, only GNU diff could produce this format and only GNU patch could automatically apply diffs in this format."[^1] POSIX added the `-u` option in Issue 7, POSIX.1-2008[^3].

## Syntax or structure

A unified diff of two files has a header of two lines, and then one or more hunks.

- **The header.** The first line is `---`, a space and the name of the old file. The second line is `+++`, a space and the name of the new file. POSIX puts a tab and the time of the last change of the file after each name[^3]. GNU `diff` writes a time such as `2002-02-21 23:30:39.942229878 -0800`[^5]. In Python `difflib`, the names and the times are strings that the caller gives, and the times are optional[^6].
- **The hunk header.** A hunk starts with a line such as `@@ -2,6 +2,6 @@`. The range after `-` is in the old file, and the range after `+` is in the new file. A range is a start line and a count of lines. POSIX allows the start alone, or the start and 1, "if the range contains exactly one line"[^3]. So `@@ -3 +3 @@` and `@@ -3,1 +3,1 @@` give the same ranges. "If a range is empty, its beginning line number shall be the number of the line just before the range, or 0 if the empty range starts the file."[^3] The lines of each file have the numbers 1, 2, 3 and so on.
- **The lines of a hunk.** Each line starts with a mark, and the rest of the line is a line of a file:

| Mark | Meaning | Counted in |
|---|---|---|
| a space | a line that is in the two files, a line of context | the old range and the new range |
| `-` | a line of the old file that goes | the old range |
| `+` | a line of the new file that comes | the new range |

- **The order of the lines.** The lines are in the order of the files. "A deleted line shall never be written immediately after an added line."[^3] So in each group of changes, the `-` lines come before the `+` lines.
- **The context.** With `-U n`, each change has up to n lines of context before it and after it, and "-u shall act like -U 3."[^3] If two changes have at most 2n unchanged lines between them, they are in one hunk[^3].
- **An incomplete line.** A last line with no LF is an incomplete line. "An incomplete line is normally distinguished on output from a full line by a following line that starts with"[^7] a backslash, such as `\ No newline at end of file`.

## Examples

**Write a unified diff with `diff`.** The old file has 7 lines. In the new file, `cherry` changes to `coconut`, `fig` goes, and `honeydew` comes at the end. The two header lines hold the times of the files, so the example removes them with `tail`:

```bash run id=diff-u
dir=$(mktemp -d)
printf '%s\n' apple banana cherry date elder fig grape >"$dir/old.txt"
printf '%s\n' apple banana coconut date elder grape honeydew >"$dir/new.txt"
cd "$dir" || exit 1
# Drop the two header lines, because they hold the times of the files.
diff -U 1 old.txt new.txt | tail -n +3
echo "--- with -U 0:"
diff -U 0 old.txt new.txt | tail -n +3
cd / && rm -r "$dir"
```

```output for=diff-u
@@ -2,6 +2,6 @@
 banana
-cherry
+coconut
 date
 elder
-fig
 grape
+honeydew
--- with -U 0:
@@ -3 +3 @@
-cherry
+coconut
@@ -6 +5,0 @@
-fig
@@ -7,0 +7 @@
+honeydew
```

With `-U 1`, the changes have 1 line of context. Between `coconut` and `fig` there are 2 unchanged lines, `date` and `elder`, so the three changes are in one hunk, `-2,6 +2,6`. With `-U 0`, each change is its own hunk. In `@@ -6 +5,0 @@`, `fig` is line 6 of the old file, and the new range is empty, so its start is the line before it, line 5. In `@@ -7,0 +7 @@`, `honeydew` comes after line 7 of the old file, and it is line 7 of the new file. The ranges of the new file include the changes of the hunks before: the GNU manual has an example hunk `@@ -9,3 +8,6 @@`, after a hunk that removed a line[^8].

**Write a unified diff in Python.** `difflib.unified_diff` gives the same hunk for the same lines. The documentation says that "Unified diffs are a compact way of showing just the lines that have changed plus a few lines of context."[^6] The header has the names `fromfile` and `tofile`, and no times:

```python run id=python-unified-diff
import difflib

old = ["apple\n", "banana\n", "cherry\n", "date\n", "elder\n", "fig\n", "grape\n"]
new = ["apple\n", "banana\n", "coconut\n", "date\n", "elder\n", "grape\n", "honeydew\n"]
diff = difflib.unified_diff(old, new, fromfile="old.txt", tofile="new.txt", n=1)
print("".join(diff), end="")
```

```output for=python-unified-diff
--- old.txt
+++ new.txt
@@ -2,6 +2,6 @@
 banana
-cherry
+coconut
 date
 elder
-fig
 grape
+honeydew
```

**Ranges with one line and with no lines.** For an empty old file, the old range is `0,0`. For a range of one line, `difflib` writes the start alone:

```python run id=python-ranges
import difflib

cases = [([], ["a\n", "b\n"]), (["a\n"], ["b\n"]), (["a\n", "b\n"], ["a\n"])]
for old, new in cases:
    hunk = [line for line in difflib.unified_diff(old, new) if line.startswith("@@")]
    print(repr(old), repr(new), hunk[0].strip())
```

```output for=python-ranges
[] ['a\n', 'b\n'] @@ -0,0 +1,2 @@
['a\n'] ['b\n'] @@ -1 +1 @@
['a\n', 'b\n'] ['a\n'] @@ -1,2 +1 @@
```

**Read the hunks.** A reader finds each hunk header with a pattern, and a missing count is 1. Then it reads lines until the counts of the two ranges are used up. A space line counts in the two ranges, a `-` line in the old range, and a `+` line in the new range. A line that starts with a backslash is a mark for the line before it, and it does not count. This reader takes the `-U 0` diff of the first example:

```python run id=python-read-hunks
import difflib
import re

HUNK = re.compile(r"@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@")


def hunks(lines):
    """Yields the old range, the new range and the lines of each hunk."""
    i = 0
    while i < len(lines):
        m = HUNK.match(lines[i])
        i += 1
        if not m:
            continue
        # A range with no count has one line.
        old = int(m[1]), int(m[2] or 1)
        new = int(m[3]), int(m[4] or 1)
        body, old_left, new_left = [], old[1], new[1]
        while old_left > 0 or new_left > 0:
            line = lines[i]
            i += 1
            if line.startswith("\\"):  # a mark for the line before it
                continue
            if line[0] in " -":
                old_left -= 1
            if line[0] in " +":
                new_left -= 1
            body.append(line)
        yield old, new, body


def span(start, count):
    return f"lines {start}-{start + count - 1}" if count else f"after line {start}"


old = ["apple", "banana", "cherry", "date", "elder", "fig", "grape"]
new = ["apple", "banana", "coconut", "date", "elder", "grape", "honeydew"]
patch = list(difflib.unified_diff(old, new, n=0, lineterm=""))
for (a, b), (c, d), body in hunks(patch):
    print(f"old {span(a, b)}, new {span(c, d)}: {body}")
```

```output for=python-read-hunks
old lines 3-3, new lines 3-3: ['-cherry', '+coconut']
old lines 6-6, new after line 5: ['-fig']
old after line 7, new lines 7-7: ['+honeydew']
```

## Versions

The format has no version number. POSIX defines the `-u` output of `diff`, and each tool adds its own parts:

| Form | Header | Additions |
|---|---|---|
| POSIX `diff -u` | the names, a tab and the times[^3] | none |
| GNU `diff -u` | the names, a tab and the times with fractional seconds and a time zone[^5] | the `\` line after an incomplete line[^7] |
| Python `difflib.unified_diff` | the names and optional dates[^6] | no `\` line, see the pitfalls |
| `git diff` | a `diff --git a/file1 b/file2` line, extended header lines, then `---` and `+++` with the prefixes `a/` and `b/`[^9] | the name of a function after the second `@@`[^9] |

The extended header lines of Git describe changes that are not in the lines of a file, such as `old mode`, `new mode`, `rename from`, `rename to` and `index`[^9]. "Hunk headers mention the name of the function to which the hunk applies."[^9] A reader that does not know this text must ignore everything after the second `@@`.

The context format of `diff -c` is the older form. It writes each hunk twice, once with the lines of the old file and once with the lines of the new file[^3].

## Pitfalls

- **A range with no count.** `@@ -3 +3 @@` has no counts, and each range has one line. A reader that expects `start,count` fails on it. Make the count optional, and use 1 when it is not there.
- **An empty range points before the change.** The start of an empty range is the line before it, so `-7,0` means after line 7, and `-0,0` means at the start of the file. A reader that takes the start as the first changed line puts the new lines one line too late.
- **An incomplete line.** Python `difflib` does not write the `\` line. If the last line of a list has no LF, the next line of the diff starts on the same line of text:

```python run id=python-incomplete-line
import difflib

old = ["a\n", "b"]  # the last line has no LF
new = ["a\n", "b\n"]
patch = "".join(difflib.unified_diff(old, new, "old.txt", "new.txt"))
print(patch.split("\n"))
```

```output for=python-incomplete-line
['--- old.txt', '+++ new.txt', '@@ -1,2 +1,2 @@', ' a', '-b+b', '']
```

GNU `diff` writes the `\` line after the line with no LF. "Incomplete lines do not match full lines unless differences in white space are ignored"[^7], so `b` with no LF and `b` with an LF are a `-` and a `+`:

```bash run id=diff-u-incomplete-line
dir=$(mktemp -d)
printf 'a\nb' >"$dir/old.txt"
printf 'a\nb\n' >"$dir/new.txt"
cd "$dir" || exit 1
diff -u old.txt new.txt | tail -n +3
cd / && rm -r "$dir"
```

```output for=diff-u-incomplete-line
@@ -1,2 +1,2 @@
 a
-b
\ No newline at end of file
+b
```

The `difflib` documentation says: "For inputs that do not have trailing newlines, set the lineterm argument to"[^6] an empty string, so that no line of the output has an LF. Then the caller adds the line ends.

- **A line of text that looks like a header.** A deleted line whose text starts with `-- ` is written as `--- ` and the rest of the text, the same start as a header line. A reader that finds the headers by their start can take such a line for a new file. Read each hunk by its counts, as the reader example does, and look for a header only after the counts are used up.
- **An empty line of context.** POSIX says: "It is implementation-defined whether an empty unaffected line is written as an empty line or a line containing a single <space> character."[^3] A reader must take an empty line in a hunk as a line of context. A tool that removes the spaces at the ends of lines changes the space line into an empty line.
- **Too little context.** "For proper operation, patch typically needs at least three lines of context."[^1] With less context, a patch tool has less text to find the place of a hunk when the file changed after the diff.
- **Names and times in the header.** POSIX says that if a name "contains a <tab> or a <newline>, or if it does not consist entirely of characters taken from the portable character set, the behavior is implementation-defined."[^3] A reader that splits the header line at the first tab can get a wrong name. Git quotes such names[^9].

## Tools and libraries

- `diff -u` and `diff -U n` write the format[^3]. GNU `diff` also has `--unified[=lines]`[^1].
- `git diff -p` writes patch text in this form, with the additions of Git[^9]. `-U <n>` or `--unified=<n>` sets the context, with the default "diff.context or 3 if the configuration variable is unset"[^9].
- Python: `difflib.unified_diff(a, b, fromfile, tofile, fromfiledate, tofiledate, n, lineterm)` writes it[^6].
- GNU `patch` reads the format and changes a copy of the old file into the new file[^1].

The page [[diff-two-texts]] finds the changes, and this format is one way to write them.

## References

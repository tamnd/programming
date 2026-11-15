---
tag: 1SGR7A
kind: technique
slug: unix-text-processing-pipeline
name: Unix text processing pipeline
aliases: [shell pipeline, pipes and filters in the shell, text filter pipeline, Unix pipeline, software tools approach]
area: data-and-persistence/text-processing
level: core
summary: >-
  Solve a text task with a chain of small filter programs that each read lines from standard input and write lines to standard output, connected with the shell operator |.
see_also: [process-a-large-text-file-as-a-stream, compute-word-frequencies, filter-lines-that-match-a-pattern, sort-the-lines-of-a-file, remove-duplicate-lines, print-the-first-and-last-lines-of-a-file, count-the-lines-of-a-file, run-a-subprocess]
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), Shell Command Language"
  locator: 2.9.2 Pipelines; set, -o pipefail
- url: https://www.gnu.org/software/coreutils/manual/html_node/Toolbox-introduction.html
  title: GNU Coreutils manual, Toolbox introduction
  locator: Opening the Software Toolbox
- url: https://www.gnu.org/software/bash/manual/html_node/Pipelines.html
  title: Bash Reference Manual, Pipelines
  locator: 3.2.3 Pipelines
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sort.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), sort"
  locator: DESCRIPTION
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tr.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), tr"
  locator: OPTIONS, -c and -s
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/uniq.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), uniq"
  locator: DESCRIPTION; OPTIONS, -c
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/write.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), write"
  locator: ERRORS, EPIPE
- url: https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html
  title: Bash Reference Manual, Exit Status
  locator: 3.7.5 Exit Status
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Variables.html
  title: Bash Reference Manual, Bash Variables
  locator: PIPESTATUS
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/stdin.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), stderr, stdin, stdout"
  locator: DESCRIPTION
- url: https://www.gnu.org/software/grep/manual/grep.html
  title: GNU Grep manual
  locator: 2.1.7 Other Options, --line-buffered
- url: https://docs.python.org/3/library/subprocess.html
  title: The Python Standard Library, subprocess, Subprocess management
  locator: Replacing shell pipeline
---

## Purpose

A Unix text processing pipeline does a text task with a chain of small programs. Each program is a filter: it reads lines from its standard input and writes lines to its standard output. The shell operator `|` connects two programs: "The standard output of command1 shall be connected to the standard input of command2".[^1] The GNU Coreutils manual says of programs that do one thing: "Such programs are simpler to design, write, and get right". It also says: "By combining several special purpose programs, you could accomplish a specific task that none of the programs was designed for".[^2]

The programs of a pipeline run at the same time. Bash says: "Each command in a multi-command pipeline, where pipes are created, is executed in its own subshell".[^3] A program reads the lines of the program before it while that program writes them, so most stages process a stream (see [[process-a-large-text-file-as-a-stream]]). A stage such as `sort` is different: it can write its first line only after it reads its last line, because the last input line can be the first output line.

## Steps

1. **Write the task as a series of steps from lines to lines.** For a list of the most frequent words: put each word on its own line, change the words to lower case, put equal words next to each other, count each group of equal words, and sort by the count.
2. **Find a filter for each step.** `grep` selects lines, `sed` and `tr` change text, `cut` and `awk` select fields, `sort` sorts, `uniq` removes or counts repeated lines, `head` and `tail` select the first or the last lines, and `wc` counts. If no filter does a step, write a small program that reads standard input and writes standard output.
3. **Connect the filters with `|`.** In a script, put each stage on its own line, with the `|` at the end of the line before it. Then each stage can have a comment.
4. **Set the locale.** `sort` compares lines with the rules of the locale: the comparison "shall be performed using the collating sequence of the current locale".[^4] A range such as `A-Z` in `tr` also depends on the locale. Set `LC_ALL=C` when the result must not depend on the locale of the user.
5. **Make the exit status show a failure of each stage.** Bash says: "The exit status of a pipeline is the exit status of the last command in the pipeline, unless the pipefail option is enabled".[^3] The POSIX option `set -o pipefail` tells the shell to "Derive the exit status of a pipeline from the exit statuses of all of the commands in the pipeline, not just the last (rightmost) command".[^1]
6. **Build the pipeline one stage at a time.** Run the first stage on a small input and look at the output. Then add the next stage, and look at the output again. When each stage is correct, run the pipeline on the full input.

## Example

In Bash. The pipeline prints the four most frequent words of a short text, with the count of each word. The first `tr` has the options `-c` and `-s`. With `-c`, the set of characters to change is all the characters that are not letters. With `-s`, "repeated sequences of the same character shall be replaced by one occurrence of the same character".[^5] So each run of characters that are not letters becomes one LF, and each word is on its own line. `uniq -c` puts a count before each group of equal lines, and `uniq` can do this only because `sort` put the equal lines next to each other. The second `sort` puts the highest count first, and sorts equal counts by the word:

```bash run id=bash-word-frequency
export LC_ALL=C # byte order for sort, and ASCII letters for tr

text='The cat sat on the mat. The dog sat on the log.
A cat and a dog: the cat ran.'

printf '%s\n' "$text" |
  tr -cs 'A-Za-z' '\n' | # each run of other characters becomes one LF
  tr 'A-Z' 'a-z' |       # lower case
  sort |                 # equal words become adjacent
  uniq -c |              # count each run of equal lines
  sort -k1,1nr -k2,2 |   # most frequent first, then by word
  head -n 4
```

```output for=bash-word-frequency
      5 the
      3 cat
      2 a
      2 dog
```

The example file does the same in Python. It starts each stage with `subprocess.Popen` and connects the stages with pipes, with no shell.

## When to use

Use a pipeline for a task on lines of text that is a series of simple steps: a report from a log file, a count of the values in a column, a list of the distinct values, or a quick check of a data file. Use it also to test an idea before you write a program. See [[compute-word-frequencies]] for the same task in many languages.

Do not use a pipeline when one record of the data can be on more than one line, as in JSON or in a CSV file with an LF in a quoted field (see [[delimited-text]]), because each filter reads one line as one record. Do not use it when the steps need a complex state, or when the program must handle each error in its own way. Then write the task as a program in one language.

## Pitfalls

- **`uniq` finds only adjacent repeated lines.** POSIX says: "Repeated lines in the input shall not be detected if they are not adjacent."[^6] Without `sort` before it, `uniq -c` counts each run of equal lines, not each distinct line. The option `-c` tells `uniq` to "Precede each output line with a count of the number of times the line occurred in the input."[^6] In the block, the line `b` comes two times, but not next to itself:

  ```bash run id=bash-uniq-adjacent
  export LC_ALL=C
  echo 'uniq -c only:'
  printf 'b\na\nb\n' | uniq -c
  echo 'sort | uniq -c:'
  printf 'b\na\nb\n' | sort | uniq -c
  ```

  ```output for=bash-uniq-adjacent
  uniq -c only:
        1 b
        1 a
        1 b
  sort | uniq -c:
        1 a
        2 b
  ```

- **The exit status hides a failure.** Without `pipefail`, the status of a pipeline is the status of its last command.[^3] In the block, `sort` fails because the file does not exist, but `wc` succeeds, so the status is 0. With `pipefail`, the status is 2, the status of `sort`. But `pipefail` can also report a failure when there is no error. `head` stops after two lines, and then `seq` writes to a pipe that has no reader. For `write`, POSIX gives the error `EPIPE`: "An attempt is made to write to a pipe or FIFO that is not open for reading by any process, or that only has one end open. A SIGPIPE signal shall also be sent to the thread."[^7] The signal stops `seq`. Bash gives the status 128 plus N to a command that a fatal signal with the number N stops.[^8] The array `PIPESTATUS` holds "a list of exit status values from the commands in the most-recently-executed foreground pipeline".[^9] Use it to see which stage failed:

  ```bash run id=bash-pipefail
  sort no-such-file.txt 2>/dev/null | wc -l
  echo "status: $?"

  set -o pipefail
  sort no-such-file.txt 2>/dev/null | wc -l
  echo "status with pipefail: $?"

  seq 1 1000000 | head -n 2
  echo "statuses: ${PIPESTATUS[*]}"
  ```

  ```output for=bash-pipefail
  0
  status: 0
  0
  status with pipefail: 2
  1
  2
  statuses: 141 0
  ```

- **A variable that a stage sets is lost.** Each stage runs in its own subshell, so a `while` loop at the end of a pipeline changes a copy of the variable.[^3] After the pipeline, the variable has its old value. Bash says that when the option `lastpipe` of `shopt` is on and job control is not active, "the last element of a pipeline may be run by the shell process".[^3] In the block, which runs as a script, the second loop runs in the shell process, so the count stays after the loop:

  ```bash run id=bash-subshell
  count=0
  printf 'a\nb\nc\n' | while IFS= read -r line; do
    count=$((count + 1))
  done
  echo "after the pipeline: $count"

  shopt -s lastpipe # the shell runs the last stage, when job control is off
  count=0
  printf 'a\nb\nc\n' | while IFS= read -r line; do
    count=$((count + 1))
  done
  echo "with lastpipe: $count"
  ```

  ```output for=bash-subshell
  after the pipeline: 0
  with lastpipe: 3
  ```

- **Output waits in a buffer.** A C program buffers its output in blocks when the output is a pipe: "stdin and stdout shall be fully buffered if and only if" "the file descriptor associated with the stream is determined not to be associated with an interactive device".[^10] With a file, this is not a problem. With an input that does not end, as in [[tail-a-growing-file]], the next stage can get no lines for a long time. GNU `grep` has the option `--line-buffered` to "Use line buffering for standard output, regardless of output device."[^11]
- **The locale changes the order.** The same pipeline can give a different order on a different computer, because `sort` uses "the collating sequence of the current locale".[^4] Set `LC_ALL=C` in the script, as in the example, when the output must be the same on each computer.

## Tools that do it

| Tool | Step | See |
|---|---|---|
| `grep` | Select the lines that match a pattern | [[filter-lines-that-match-a-pattern]] |
| `sed` | Change text with a regular expression, delete lines | [[remove-lines-from-a-file]] |
| `tr` | Change, delete or squeeze characters | [[compute-word-frequencies]] |
| `cut`, `awk` | Select fields | [[delimited-text]] |
| `sort` | Sort lines | [[sort-the-lines-of-a-file]] |
| `uniq` | Remove or count adjacent repeated lines | [[remove-duplicate-lines]] |
| `head`, `tail` | Select the first or the last lines | [[print-the-first-and-last-lines-of-a-file]] |
| `wc` | Count lines, words and bytes | [[count-the-lines-of-a-file]] |
| Python `subprocess` | Start a pipeline from a program, with no shell | [[run-a-subprocess]] |

The Python documentation shows a pipeline with `Popen`, and says: "The p1.stdout.close() call after starting the p2 is important in order for p1 to receive a SIGPIPE if p2 exits before p1."[^12] For a pipeline of filters inside one program, with no processes, see [[collection-pipeline]].

## References

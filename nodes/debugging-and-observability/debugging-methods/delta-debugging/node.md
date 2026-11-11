---
tag: 8J7Q8Z
kind: concept
slug: delta-debugging
name: Delta debugging
aliases: [ddmin, test case minimization, test case reduction, input minimization, scientific debugging]
area: debugging-and-observability/debugging-methods
level: core
summary: >-
  Delta debugging cuts a failing input into parts and tests smaller inputs until no single part can be removed and the failure stays. It turns the minimization of a test case into a search.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2002
  people: [Andreas Zeller, Ralf Hildebrandt]
  note: The IEEE TSE paper defines the ddmin algorithm and 1-minimality, and applies them to failing inputs of GCC, Mozilla and UNIX utilities. Zeller's ESEC/FSE 1999 paper applied the earlier dd algorithm to changes in program code.
see_also: [git, unit-testing, fuzz-testing, property-based-tests, exception-handling, closures, recursion]
same_as:
  wikidata: Q1184877
  wikipedia: Delta_debugging
sources:
  - paper: doi:10.1109/32.988498
    title: Andreas Zeller and Ralf Hildebrandt, Simplifying and Isolating Failure-Inducing Input, IEEE Transactions on Software Engineering 28(2), February 2002 (read from the copy at https://www.cs.purdue.edu/homes/xyzhang/fall07/Papers/delta-debugging.pdf)
    locator: Abstract; I Introduction; III-A Minimal Test Cases (Definition 10); III-B A Minimizing Algorithm (Figure 5, Propositions 11 to 13); IV-A GCC; IV-B Mozilla; IV-D The Precision Effect; V Isolating Failure-Inducing Differences; references [2] and [3]
  - url: https://www.debuggingbook.org/html/Intro_Debugging.html
    title: Andreas Zeller, Introduction to Debugging, in The Debugging Book
    locator: the sections From Defect to Failure, From Failure to Defect, The Scientific Method and Keep a Log
  - url: https://git-scm.com/docs/git-bisect
    title: Git documentation, git-bisect
    locator: Description; Bisect run; Examples
---

## Idea

A bug report often comes with a large input: a long web page, a source file of a thousand lines, or a long list of user actions. Most of it has nothing to do with the failure. A developer who reads the code cannot tell which part matters. A smaller input that still fails is easier to understand, easier to put in a [[unit-testing|unit test]] and points closer to the defect.

Delta debugging finds such an input without any knowledge of the program. It needs only a test function that runs the program on an input and says if the failure occurs. The algorithm `ddmin` cuts the input into parts and runs the test on smaller inputs. It keeps each smaller input that still fails, and it stops when the removal of any single part makes the failure go away.[^1]

In the case study of Zeller and Hildebrandt, Mozilla crashed when it printed a web page of 896 lines. Delta debugging reduced the page to the single line that caused the crash. Then it reduced that line, character by character, to the 8 characters `<SELECT>`.[^1]

The examples on this page run `ddmin` on one line of that page. The test function stands for the browser: it "crashes" when the text has `<SELECT` and a `>` after it. Each language prints the same trace, and the result is `<SELECT>` after 147 tests.

## How it works

### The scientific method of debugging

A failure is the end of a chain. A programmer makes a mistake, which puts a defect in the code. When the code runs, the defect causes a fault in the program state. The fault spreads until the program shows a failure that a user can see. Debugging goes back along this chain, from the failure to the defect.[^2]

Zeller describes this search as the scientific method:[^2]

1. Formulate a question about the failure.
2. Invent a hypothesis that may explain it.
3. From the hypothesis, derive a prediction.
4. Test the prediction in an experiment.
5. Repeat steps 2 to 4 until the hypothesis explains all the observations.

The method asks the developer to write down each hypothesis and each experiment in a log, so that the search does not go in circles.[^2] Delta debugging automates one kind of experiment. Each hypothesis is "this part of the input is not needed for the failure". Each test run confirms or refutes it.

### Changes, tests and outcomes

The paper models the input as a set of changes. The failing input `c` is the set of all its changes, and the empty set stands for a run that works. A change can be a line, a character, a token or a user action. The choice is the granularity of the search.[^1]

The function `test` runs the program on a subset of the changes and returns one of three outcomes:[^1]

- **✘ fail.** The failure of the bug report occurs.
- **✔ pass.** The program works.
- **? unresolved.** The test cannot decide. For example, the input is not valid, or the program fails in a different way.

`ddmin` treats an unresolved outcome as a pass. It keeps only the subsets that fail.

### 1-minimal, not minimal

The ideal result is the smallest failing subset of `c`, a global minimum. To find it with a test function alone, the algorithm must test all 2^|c| subsets. That cost is exponential.[^1]

`ddmin` finds a 1-minimal input instead. An input is 1-minimal if the removal of any single change makes the failure go away. Definition 10 of the paper generalizes this to n-minimal: the removal of any n or fewer changes makes the failure go away.[^1] A 1-minimal input is a local minimum. A different subset can be smaller.

### The ddmin algorithm

`ddmin(c)` calls `ddmin2(c, 2)`. `ddmin2` cuts the input into `n` parts of about equal size, `Δ1` to `Δn`. The complement `∇i` is the input with part `Δi` removed. Then it applies the first rule that fits:[^1]

```text
reduce to subset         if test(Δi) fails for some i:   ddmin2(Δi, 2)
reduce to complement     if test(∇i) fails for some i:   ddmin2(∇i, max(n − 1, 2))
increase granularity     else if n < |c|:                ddmin2(c, min(|c|, 2n))
done                     otherwise:                      return c
```

The first rule is a binary search. If the failure is in one half, the other half is discarded. The second rule removes one part at a time. It keeps the granularity, so the next round removes parts of the same size. The third rule makes the parts smaller when no part can go. When each part is a single change and no complement fails, every single change is needed, and the result is 1-minimal. Proposition 11 of the paper proves this.[^1]

The paper opens this section with the advice of Kernighan and Pike: "Proceed by binary search. Throw away half the input and see if the output is still wrong; if not, go back to the previous state and discard the other half of the input."[^1] `ddmin` is this advice made precise. It also handles the case where neither half fails alone.

### A trace

The input of the examples has 55 characters:

```text
<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>
```

Neither half fails alone. The first half has no `<SELECT`, and the second half starts in the middle of it. The complement of the first half is the second half, so it does not fail either. `ddmin` increases the granularity to 4 parts. The complement of the first quarter, `<td align=l`, fails, and the input shrinks to 42 characters. Each later step removes one more part. The trace shows the length after each step:

```text
complement 42
complement 28
...
complement 8
55 characters reduced to <SELECT> in 147 tests
```

No step reduces to a subset in this run, because each half breaks the tag. The last tests remove each of the 8 characters in turn and see the failure go away. These tests prove that the result is 1-minimal.

## In languages

No language has delta debugging in its standard library. The algorithm needs only a sequence that can be cut, a function value for the test and a loop, so every general language writes it in about 20 lines. The differences are in how a language cuts a string and stops at the first part that fails.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | slices for the parts, `next()` over a generator for the first failing part |
| JavaScript | idiom | `slice` and `Array.prototype.find` |
| TypeScript | idiom | as JavaScript, with the type `(text: string) => boolean` for the test |
| Java | idiom | `substring`, `Predicate<String>` and `stream().filter().findFirst()` |
| C# | idiom | ranges such as `chars[lo..hi]` and `FirstOrDefault` over a lazy `Select` |
| Go | idiom | byte slices of a string, a closure for the test |
| Rust | idiom | a `Vec<char>`, a lazy `map().find()` and an `FnMut` test |
| C++ | idiom | `substr`, `std::ranges::find_if` and `std::function` |
| C | idiom | `snprintf` with `%.*s` into a fixed buffer, a function pointer for the test |
| Kotlin | idiom | as Java, with `substring` and a lambda |
| Bash | idiom | `${text:offset:length}`; a test is a command and its exit status |
| PowerShell | idiom | `Substring` and a script block for the test |
| SQL, Regex, HTML, CSS | n/a | the language cannot run a test; HTML and CSS are common inputs to reduce |

The test function is the part that changes from one project to the next. In the examples it is a closure or a function with a counter, so the program can report the number of tests. See [[closures]]. In a real project the test starts the program as a separate process, feeds it the input and checks the exit status or the output.

Strings differ between languages. Go slices bytes and JavaScript slices UTF-16 code units, so a part can hold half of a character when the input is not ASCII. The Rust example cuts a `Vec<char>` to avoid this. A tool for real inputs often cuts at a coarser level, such as lines or tokens.

## Variations and related ideas

### Isolating a difference

When a passing input is also known, the general algorithm `dd` isolates the difference between the passing input and the failing one. It grows the passing input and shrinks the failing input until they differ by a 1-minimal set of changes. `dd` generalizes `ddmin`.[^1] Isolation needs fewer tests than minimization. In the examples of the paper, isolation needs 7 tests where minimization needs 26. The cost is that the developer must keep the common context of both inputs in mind.[^1]

### Hierarchical reduction

The Mozilla study ran `ddmin` twice. The first run used lines as changes and reduced 896 lines to one line in 57 tests. The second run used the characters of that line and reduced it to `<SELECT>`.[^1] A coarse first run removes large blocks with few tests. A fine second run removes the details.

### Fuzzing and property-based testing

A fuzzer or a property-based test finds a failing input at random, and that input is often long and full of noise. These tools shrink the input before they report it, with the same goal as `ddmin`. See [[fuzz-testing]] and [[property-based-tests]].

### git bisect

Delta debugging can also search the history of a program. The changes are then commits, not parts of an input. If an old version works and a new version fails, one of the commits between them introduced the failure. `git bisect` finds that commit with a binary search through the history.[^3] See [[git]] and [[commit]].

`git bisect start <bad> <good>` marks the two ends. `git bisect run <cmd>` then checks out a commit in the middle, runs the command and reads its exit status:[^3]

- exit 0 means the commit is good;
- exit 1 to 127, except 125, means the commit is bad;
- exit 125 means the commit cannot be tested, so Git skips it;
- any other exit status stops the search.

At the end, Git prints the first bad commit, and the ref `refs/bisect/bad` points at it.[^3] `git bisect reset` returns to the branch where the search started.

The script below makes a history of 8 commits. One commit changes `total.sh` so that it rounds the total to an even number, and `2 + 3` then gives 4. The test script lives outside the repository, so each checkout leaves it alone.

```bash run id=bisect
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
step() { printf '%s\n' "$2" > "$1"; git add "$1"; git commit -q -m "$3"; }
step total.sh 'echo $(( $1 + $2 ))' "Add the total script"
step README 'Adds two prices.' "Add a README"
step total.sh 'echo $(( ${1:-0} + ${2:-0} ))' "Treat a missing price as 0"
step README 'Adds two prices. A missing price is 0.' "Document missing prices"
step total.sh 'echo $(( (${1:-0} + ${2:-0}) / 2 * 2 ))' "Round the total to an even number"
step README 'Adds two prices and rounds the total.' "Document the rounding"
step NEWS 'Version 2' "Write the news file"
step NEWS 'Version 2, with rounding' "Update the news file"
cat > ../test.sh <<'EOF'
#!/bin/sh
# Exit 0 if the commit is good, 1 if it is bad.
test "$(sh total.sh 2 3)" = 5
EOF
chmod +x ../test.sh
git bisect start HEAD "$(git rev-list --max-parents=0 HEAD)"
git bisect run ../test.sh
git log -1 --format='first bad commit: %s' refs/bisect/bad
git bisect reset
```

```output for=bisect
Bisecting: 3 revisions left to test after this (roughly 2 steps)
[516ec22fbcffc6049c6904bf080876f24ab687a5] Document missing prices
running '../test.sh'
Bisecting: 1 revision left to test after this (roughly 1 step)
[e3344e5a8074285075230505583aa388fc81e23c] Document the rounding
running '../test.sh'
Bisecting: 0 revisions left to test after this (roughly 0 steps)
[456c71953887868b48a317631253c6e6a537eff0] Round the total to an even number
running '../test.sh'
456c71953887868b48a317631253c6e6a537eff0 is the first 'bad' commit
commit 456c71953887868b48a317631253c6e6a537eff0
Author: Ada <ada@example.com>
Date:   Mon Jan 5 10:00:00 2026 +0000

    Round the total to an even number

 total.sh | 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)
bisect found first 'bad' commit
first bad commit: Round the total to an even number
Previous HEAD position was 456c719 Round the total to an even number
Switched to branch 'main'
```

Git tested 3 of the 6 commits between the two ends. A linear search from the oldest commit would test up to 6. The gain grows with the length of the history, because each test halves the range.

`git bisect` assumes that the history has one point where the result changes from good to bad. `ddmin` makes no such assumption about an input, so it needs more tests. When the failure needs two commits together, a bisection finds the later of the two, not the pair.

## Pitfalls

### The search finds a different failure

The test function decides what counts as the failure. If it returns "fail" for any error, `ddmin` can move from the failure in the bug report to a different one. A smaller input often has new errors, such as an unclosed tag or a syntax error. The Python example shows this: a test that treats any exception as the failure reduces the page to `<`, which shows a different bug.

Zeller and Hildebrandt saw this effect. In their first runs, the test returned ✘ for any crash. For the NROFF input t1, the result had 2 characters. Then they made the test more precise: it returned ✘ only if the backtrace of the crash was the same as in the original failure, and unresolved for a crash with a different backtrace. The result then had 60 characters, and only those 60 caused the original failure.[^1] A test should compare the error type, the message or the location of the failure. Any other error is an unresolved outcome. See [[exception-handling]].

### A test that is not deterministic

`ddmin` assumes that the same input gives the same outcome each time. A failure that depends on timing, on random numbers or on the state of the machine breaks this assumption. The algorithm can then keep a part that does not matter or remove a part that does. Fix the seed, the clock and the environment before the search, or run each test several times.

### The loop does not stop

The third rule applies only while `n < |c|`. Without this check, `n` stays at `|c|` and the loop runs the same tests forever. The JavaScript example has a version with this bug.

### Many inputs are not valid

Most subsets of a structured input, such as a C program or a JSON file, are not valid. The test is then unresolved, and `ddmin` needs more tests. In the GCC case study, each attempt to remove one more character from the 77-character program gave a program that was not valid C.[^1] A reduction at the level of lines, tokens or syntax tree nodes keeps more inputs valid than a reduction at the level of characters.

### The cost of one test

Each test runs the program. If one run takes a minute, 1000 tests take most of a day. Start with a coarse granularity, set a time limit on each run, and make the test start the program as fast as possible.

## Cost

Proposition 12 of the paper gives the worst case: `ddmin` runs at most |c|² + 3|c| tests on an input of |c| changes. The worst case occurs when almost every test is unresolved. Proposition 13 gives the best case: if one change alone causes the failure, and every input with that change fails, `ddmin` runs at most 2 log₂ |c| tests.[^1]

The case studies of the paper fall between these bounds:[^1]

- **GCC.** A C program of 755 characters crashed GCC with optimization on. The first tests cut it to 377 and then 188 characters. 731 more tests reduced it to 77 characters, a 1-minimal program.
- **Mozilla.** 95 user actions were reduced to 3 in 82 test runs, which took 21 minutes. 896 lines of HTML were reduced to one line in 57 tests.
- **Total.** The abstract gives 139 test runs, or 35 minutes on a 500 MHz PC, for the Mozilla case study.

The example on this page needs 147 tests for 55 characters. The test function returns at once, so the run takes much less than a second. The cost of a real search is the number of tests times the cost of one run of the program.

## History

- **1999.** Mozilla's bug database held more than 370 open bug reports that nobody had simplified. Netscape asked volunteers to turn bug reports into minimal test cases in the Mozilla BugAThon. A volunteer who simplified 5 bug reports got an invitation to the launch party, and 20 bug reports earned a T-shirt.[^1]
- **1999.** Zeller presented the first delta debugging algorithm, `dd`, in "Yesterday, my program worked. Today, it does not. Why?" at ESEC/FSE 1999. It searched for the changes to program code that cause a failure.[^1]
- **2002.** Zeller and Hildebrandt published "Simplifying and Isolating Failure-Inducing Input" in IEEE Transactions on Software Engineering. The paper defines `ddmin`, 1-minimality and the general `dd` for inputs, and it applies them to GCC, Mozilla and UNIX utilities.[^1]

## References

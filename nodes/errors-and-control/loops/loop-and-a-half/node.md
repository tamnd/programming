---
tag: HVVV9Y
kind: concept
slug: loop-and-a-half
name: Loop and a half
aliases: [loop and a half, loop-and-a-half problem, n and a half times loop, middle-exit loop, test in the middle, mid-test loop, exit when, priming read, read loop, fencepost loop]
area: errors-and-control/loops
level: advanced
summary: >-
  A loop and a half runs the first part of its body n times and the second part n - 1 times, so its exit test belongs in the middle, written with break or with a read in the condition.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Donald Knuth, Edsger W. Dijkstra, Ole-Johan Dahl]
  note: Knuth knew the problem by 1967, and Dijkstra named the loop that runs "n and a half times". Dahl proposed a loop with the test in the middle in 1972.
see_also: [while-loop, do-while-loop, infinite-loops, loop, for-each-loop, control-flow]
sources:
- url: https://en.wikipedia.org/wiki/Control_flow
  title: Control flow, Wikipedia
  locator: Loop-and-a-half problem; Early exit; Middle test
- url: https://peps.python.org/pep-0572/
  title: "PEP 572, Assignment Expressions"
  locator: Capturing condition values; Examples from the Python standard library
- url: https://docs.python.org/3/library/functions.html
  title: Built-in Functions, The Python Standard Library
  locator: iter
---

## Idea

Many loops read something, test it, and then use it: read a line, stop at the end of the file, process the line. The read must happen once more than the processing, because the last read finds the end and nothing follows it. A loop like this does its first half n times and its second half n - 1 times. The natural place for the exit test is in the middle of the body, not at the top or the bottom.

```python run id=idea
# Read numbers until 0 and add them up. The test is in the middle.
data = iter([5, 3, 8, 0, 9])
total = 0
while True:
    x = next(data)  # first half: read
    if x == 0:  # the test
        break
    total += x  # second half: process
print("total:", total)

# The same loop without a break repeats the read before the loop.
data = iter([5, 3, 8, 0, 9])
total = 0
x = next(data)
while x != 0:
    total += x
    x = next(data)
print("total:", total)

# With := the read goes into the condition.
data = iter([5, 3, 8, 0, 9])
total = 0
while (x := next(data)) != 0:
    total += x
print("total:", total)

# A list with separators: the separator goes between items, n - 1 times.
out = ""
for i, item in enumerate(["a", "b", "c"]):
    if i > 0:
        out += ", "
    out += item
print(out)
```

```output for=idea
total: 16
total: 16
total: 16
a, b, c
```

The three loops compute the same sum and stop at the 0, so the 9 after it is never read. The first loop has its test in the middle and a `break`. The second has the test at the top, so it must read once before the loop and again at the end of the body: the read appears twice. The third puts the read into the condition with `:=`. The last loop shows the same shape in output: three items need two separators.

## How it works

**The problem.** "Common loop structures sometimes result in duplicated code, either repeated statements or repeated conditions"[^1]. "A common case is where the start of the loop is always executed, but the end may be skipped on the last iteration"[^1]. Dijkstra called it a loop that is performed "n and a half times"[^1]. Typical cases are "reading data in the first part, checking for end of data, and then processing the data in the second part; or processing, checking for end, and then preparing for the next iteration"[^1]. "In these cases, the first part of the loop is executed n times, but the second part is only executed n−1 times"[^1].

**Four solutions.**

1. **Repeat the first half.** Read once before a while loop and again at the end of the body. This is the priming read of the second loop in the example. It works with a plain while loop, but the read appears in two places, and a later change can update one copy and not the other.
2. **Test in the body with a flag or an `if`.** A conditional "inside the loop (possibly duplicating the condition but not the statements)"[^1] avoids a second copy of the code, but the loop tests the same thing twice.
3. **Exit in the middle with `break`.** "Since the 1990s this has been the most commonly taught solution, using a break statement"[^1]: `loop`, the first half, `if condition break`, the second half, `repeat`. Ada has a statement for it: `exit when X = 0;`[^1].
4. **Move the first half into the condition.** If the first half is a single read, an assignment in the loop condition does it: `while ((c = getchar()) != EOF)` in C, `while (line := f.readline())` in Python, `while (reader.ReadLine() is { } line)` in C# and `while let Some(x) = it.next()` in Rust.

**The reversed condition.** With `break`, the test is the condition to stop, not the condition to go on. "A subtlety of this solution is that the condition is the opposite of a usual while condition: rewriting while condition ... repeat with an exit in the middle requires reversing the condition: loop ... if not condition exit ... repeat"[^1].

**Assignment expressions in Python.** Python did not allow an assignment in a condition until PEP 572 added `:=`. The PEP labels the use `# Loop-and-a-half` and gives the example `while (command := input("> ")) != "quit":`[^2]. It also rewrites a loop from the standard library that started with `while True: line = fp.readline() if not line: break` as `while line := fp.readline():`[^2].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | `while True:` with `if ...: break`; `while (x := read()) != end:`; `iter(f, sentinel)` |
| JavaScript | idiom | `while (true)` or `for (;;)` with `break`; an assignment in the condition |
| TypeScript | idiom | the forms of JavaScript |
| Java | idiom | `while (true)` with `break`; `while ((line = r.readLine()) != null)` |
| Kotlin | idiom | `while (true)` with `break`; `generateSequence` with `takeWhile` |
| C# | idiom | `while (true)` with `break`; `while (r.ReadLine() is { } line)` |
| Go | idiom | `for { ...; if done { break }; ... }`; `for scanner.Scan() { }` |
| Rust | idiom | `loop` with `break`; `let ... else { break }`; `while let Some(x) = it.next()` |
| C | idiom | `for (;;)` with `break`; `while ((c = getchar()) != EOF)` |
| C++ | idiom | `while (std::getline(in, line))` and `while (in >> x)`; `for (;;)` with `break` |
| Bash | idiom | `while IFS= read -r line; do ...; done`; the read is the condition |
| PowerShell | idiom | `while ($true)` with `break` |
| SQL | idiom | no loops in queries; a cursor loop in PL/pgSQL uses `FETCH` and `EXIT WHEN NOT FOUND` |
| Regex | n/a | no loops |
| HTML | n/a | no loops |
| CSS | n/a | no loops |

Few languages have a loop statement with the test in the middle, so all the rows are idioms. Ada has `exit when`, and Dahl proposed a `loop ... while ... repeat` statement in 1972. The languages differ in how much of the first half can move into the condition: C, C++, Java, C#, Go, Rust and Python allow an assignment or a binding there, and the stream and scanner types of C++ and Go return a value that the condition tests.

The example files show these differences. Python loops with `iter(callable, sentinel)` and asks for input again until it is valid. JavaScript puts separators between items and reads commands up to a stop word. Java reads lines with a `break` and with an assignment in the condition. C# retries `int.TryParse` and reads lines with a pattern in the condition. Go reads with `ReadString` up to `io.EOF` and with a `Scanner`. Rust uses `loop` with `let ... else` and joins items with a first element taken out. C parses numbers with `strtol` and stops in the middle. C++ reads lines in the condition and joins them with a changing separator.

## Variations and related ideas

- **Middle-test statements.** Dahl's structure has the form `loop`, the first half, `while test;`, the second half, `repeat`. "If xxx1 is omitted, we get a loop with the test at the top (a traditional while loop). If xxx2 is omitted, we get a loop with the test at the bottom, equivalent to a do while loop in many languages"[^1].
- **Sentinel values.** Many read loops stop at a special value, such as 0, an empty line or the end of the file. Python's `iter(callable, sentinel)` makes an iterator from a read function: it calls the function each time, and "if the value returned is equal to sentinel, StopIteration will be raised, otherwise the value will be returned"[^3]. A for loop over this iterator is a loop and a half with no `break`.
- **Joining with separators.** The separator between items is the second half of a loop and a half. A join function, such as `", ".join(items)` in Python, hides the loop.
- **Input validation.** A loop that asks for input, tests it, and complains and asks again is a loop and a half: it asks n times and complains n - 1 times.

## Pitfalls

- **Two copies of the first half.** With a priming read, a change to one read and not to the other gives a loop that processes the wrong data. Use `break` or an assignment in the condition.
- **The negated test.** The `break` test stops the loop, so it is the negation of the while condition. Check it with the last item and with an empty input.
- **Several exits.** Each `break` is another way out of the loop, and a reader must check all of them. Keep one exit where possible.
- **The last partial item.** A read can return data and the end of the input at the same time, as `ReadString` does in Go when the last line has no newline. Process the data before the test, or test for both.
- **Assignment instead of comparison.** In C, `while (c = getchar() != EOF)` assigns the result of the comparison to `c`, because `!=` binds tighter than `=`. Write `while ((c = getchar()) != EOF)`.

## Cost

A loop and a half costs the same as the other loops: one test and one jump for each pass. The `break` form and the condition form compile to almost the same code. The choice is about reading and maintenance: the duplicated read is the only form that adds code, and it adds no speed.

## History

"This problem has been recognized at least since 1967 by Knuth, with Wirth suggesting solving it via early loop exit"[^1]. Dijkstra gave it its name. "The following structure was proposed by Dahl in 1972"[^1]: a loop with a `while` test in the middle, which "allows clear loop-and-a-half logic"[^1]. Most languages chose `break` instead of a new statement, and C, and later Python with PEP 572, allowed the read to move into the loop condition.

## References

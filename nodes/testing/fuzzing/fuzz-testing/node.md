---
tag: WENMVH
kind: concept
slug: fuzz-testing
name: Fuzz testing
aliases: [fuzzing, fuzz test, fuzzer, coverage-guided fuzzing]
area: testing/fuzzing
level: core
summary: >-
  Fuzz testing runs a program on a large number of generated or mutated inputs and watches for crashes, hangs and failed checks. Coverage-guided fuzzers keep the inputs that reach unseen code.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1990
  people: [Barton P. Miller, Lars Fredriksen, Bryan So]
  note: Miller, Fredriksen and So fed random characters to about 90 UNIX utilities on seven versions of UNIX and crashed or hung 24 to 33 percent of them on each system.
see_also: [property-based-tests, unit-testing, mutation-analysis, exception-handling, out-of-bounds-read, integer-overflow]
same_as:
  wikidata: Q1189053
  wikipedia: https://en.wikipedia.org/wiki/Fuzzing
sources:
  - url: http://www.cs.wisc.edu/paradyn/papers/fuzz.pdf
    title: Barton P. Miller, Lars Fredriksen and Bryan So, An Empirical Study of the Reliability of UNIX Utilities, Communications of the ACM 33(12), December 1990 (the report text)
    locator: Summary; 1 Introduction; 2.1 Fuzz, Generating Random Input Strings; 3 The Tests; 4 The Results and Analysis; 5.1 Comments on the Results
  - url: https://pages.cs.wisc.edu/~bart/fuzz/
    title: Barton P. Miller, Fuzz Testing of Application Reliability
    locator: the introduction (the three characteristics of classic fuzz testing); the list of papers (1988 project assignment, 1990 report, later studies)
  - url: https://lcamtuf.coredump.cx/afl/technical_details.txt
    title: Technical whitepaper for afl-fuzz
    locator: 1 Coverage measurements; 2 Detecting new behaviors; 3 Evolving the input queue; 5 Trimming input files; 6 Fuzzing strategies; 7 Dictionaries; 9 Investigating crashes; 10 The fork server
  - url: https://llvm.org/docs/LibFuzzer.html
    title: libFuzzer, a library for coverage-guided fuzz testing, LLVM documentation
    locator: Introduction; Status; Fuzz Target; Fuzzer Usage; Corpus; Options
  - url: https://go.dev/doc/security/fuzz/
    title: Go Fuzzing, the Go documentation
    locator: Requirements; Suggestions; Running fuzz tests; Failing input; Corpus file format; Glossary
  - url: https://rust-fuzz.github.io/book/cargo-fuzz/tutorial.html
    title: Rust Fuzz Book, cargo-fuzz, Tutorial
    locator: cargo fuzz init; fuzz_target!; cargo fuzz run
  - url: https://rust-fuzz.github.io/book/cargo-fuzz/setup.html
    title: Rust Fuzz Book, cargo-fuzz, Setup
    locator: Requirements (the nightly compiler)
  - url: https://github.com/google/atheris
    title: Atheris, a coverage-guided native Python fuzzer, README
    locator: the introduction
  - url: https://github.com/CodeIntelligenceTesting/jazzer
    title: Jazzer, fuzz testing for the JVM, README
    locator: the introduction; Running Jazzer, Regression mode and Fuzzing mode
  - url: https://github.com/CodeIntelligenceTesting/jazzer.js
    title: Jazzer.js, README
    locator: the introduction
  - url: https://github.com/Metalnem/sharpfuzz
    title: SharpFuzz, AFL-based fuzz testing for .NET, README
    locator: the introduction
  - url: https://lcamtuf.coredump.cx/afl/
    title: american fuzzy lop, home page
    locator: the note on AFL++
---

## Idea

A fuzz test runs a program on a large number of inputs that no person chose and checks one thing: that the program does not crash or hang. The study of Miller, Fredriksen and So began with line noise. One of them used a dial-up line in a storm, and the spurious characters on the line crashed common UNIX utilities.[^1] They then fed random characters to about 90 utilities on seven versions of UNIX and crashed more than 24 percent of them.[^1]

Miller lists three features of classic fuzz testing. The input is random. The test uses no model of the program, so it is a black box test. And the oracle is that the program must not crash or hang. It does not need to give a sensible answer, and it may exit quietly.[^2] The last feature is the key. A [[unit-testing|unit test]] needs an expected output for each input. A fuzz test needs none, so a machine can make the inputs.

The parser below reads records of the form `[tag][length][value]`. It does not check that the length byte exists. A blind fuzzer with a fixed seed finds the bug on its first input. A `ValueError` is the documented way to reject bad input, so the harness ignores it. Any other exception is a bug.

```python run id=idea
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def parse(data):
    """Reads records of the form [tag][length][value]."""
    records = []
    i = 0
    while i < len(data):
        tag = data[i]
        length = data[i + 1]  # bug: no check that the length byte exists
        records.append((tag, data[i + 2 : i + 2 + length]))
        i += 2 + length
    return records


state = 2463534242
crashes = {}
for run in range(1, 1001):
    state = xorshift32(state)
    data = bytearray()
    for _ in range(state % 9):  # lengths 0 to 8
        state = xorshift32(state)
        data.append(state & 0xFF)
    try:
        parse(bytes(data))
    except ValueError:
        pass  # a documented rejection is not a bug
    except Exception as e:
        name = type(e).__name__
        if name not in crashes:
            print(f"run {run}: {name}: {e} on {bytes(data)!r}")
        crashes[name] = crashes.get(name, 0) + 1
print("crashes:", crashes)
```

```output for=idea
run 1: IndexError: index out of range on b'z'
crashes: {'IndexError': 115}
```

The fuzzer knows nothing about the format. It found the bug because short inputs are common in its output, and every input of one byte crashes the parser.

## How it works

A fuzzer has five parts: a target, a source of inputs, an oracle, a corpus and a minimizer.

**The target.** Miller's tools fed a random stream to the standard input of each utility, and a second tool, `ptyjig`, drove the utilities that need a terminal.[^1] In-process fuzzers call a function instead. libFuzzer calls `LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)` many times in one process. The target must accept any input, must not call `exit`, and must be deterministic and fast.[^4] A Go fuzz test is a function `FuzzXxx(f *testing.F)` that calls `f.Fuzz` with a function of `*testing.T` and the fuzzing arguments.[^5] In Rust, `cargo fuzz` generates a target whose body is a `fuzz_target!` closure over a byte slice.[^6]

**The inputs.** Miller's `fuzz` program wrote random characters. Options chose printable characters, control characters or the NUL character, and a seed for the random generator made the tests repeatable.[^1] Blind random bytes rarely get past the first check of a structured format. A mutation fuzzer starts from a set of valid inputs, the seed corpus, and changes them. AFL first applies deterministic changes: bit flips, the addition and subtraction of small integers, and the insertion of interesting values such as 0, 1 and `INT_MAX`. Later it stacks random changes and splices test cases together.[^3]

**Coverage guidance.** The compiler adds code at each branch. AFL's instrumentation counts each pair of branch source and branch destination in a 64 kB map shared with the fuzzer.[^3] When a mutated input makes a pair that no earlier input made, the fuzzer adds the input to its queue as a starting point for more mutation. An input with no unseen pair is discarded.[^3] libFuzzer gets the same information from LLVM's SanitizerCoverage and mutates its corpus to maximize the code coverage.[^4] The Go fuzzer also keeps an input only when it expands the coverage.[^5]

The demonstration below shows why this matters. The bug needs the four bytes `FUZZ` in order. The target calls `hit` at each branch, as a compiler pass would. Both fuzzers change one to four bytes of an input from the corpus, with the same seed. The blind fuzzer always mutates the seed. The guided fuzzer also mutates each input that reached a branch not reached before.

```python run id=guided
class Rng:
    def __init__(self, seed):
        self.state = seed

    def next(self):
        y = self.state
        y ^= (y << 13) & 0xFFFFFFFF
        y ^= y >> 17
        y ^= (y << 5) & 0xFFFFFFFF
        self.state = y
        return y


def target(data, hit):
    """The bug needs the bytes F, U, Z, Z in order. hit(n) marks branch n as reached."""
    if len(data) > 0 and data[0] == ord("F"):
        hit(1)
        if len(data) > 1 and data[1] == ord("U"):
            hit(2)
            if len(data) > 2 and data[2] == ord("Z"):
                hit(3)
                if len(data) > 3 and data[3] == ord("Z"):
                    raise RuntimeError("reached the bug")


def mutate(rng, data):
    """Sets one to four random bytes of a 4-byte input to random values."""
    data = bytearray(data)
    for _ in range(1 + rng.next() % 4):
        data[rng.next() % 4] = rng.next() & 0xFF
    return bytes(data)


def fuzz(guided, runs=200_000):
    rng = Rng(2463534242)
    corpus = [b"AAAA"]  # the seed corpus
    seen = set()
    for run in range(1, runs + 1):
        data = mutate(rng, corpus[rng.next() % len(corpus)])
        reached = set()
        try:
            target(data, reached.add)
        except RuntimeError:
            return f"crash on run {run} with {data!r}, corpus of {len(corpus)}"
        if guided and not reached <= seen:  # new coverage: keep the input
            seen |= reached
            corpus.append(data)
    return f"no crash in {runs} runs, corpus of {len(corpus)}"


print("blind: ", fuzz(guided=False))
print("guided:", fuzz(guided=True))
```

```output for=guided
blind:  no crash in 200000 runs, corpus of 1
guided: crash on run 21646 with b'FUZZ', corpus of 4
```

The blind fuzzer must guess all four bytes at once. The guided fuzzer guesses one byte at a time, because the corpus keeps each step: `F`, then `FU`, then `FUZ`. AFL's documentation reports the same effect on GNU patch. Blind fuzzing reached 265 edges, guidance by edge coverage reached 2,070, and AFL's own model reached 2,597.[^3]

**The oracle.** Miller counted a crash when the program dumped core and a hang when it ran with no output for a long time.[^1] A coverage-guided fuzzer runs the target in its own process, so it sees a crash as a signal, an uncaught exception or a panic. Go counts a panic, a call to `t.Fail`, an `os.Exit` and a run that takes more than one second as failures.[^5] Many memory errors in C and C++ do not crash at once. libFuzzer is usually combined with AddressSanitizer or UndefinedBehaviorSanitizer, which turn an out-of-bounds read or an overflow into an immediate report.[^4]

**The corpus.** The fuzzer saves the inputs that reach code not reached before. AFL keeps its queue compact: each input in it shows a behaviour that the other inputs do not.[^3] The corpus is also a regression test. libFuzzer re-runs a list of files with no fuzzing when it gets files instead of directories, and the documentation suggests this mode for continuous integration.[^4] Plain `go test` runs each seed corpus entry of a fuzz test once, as a unit test. Only `go test -fuzz=FuzzName` generates inputs.[^5] Go writes each failing input to `testdata/fuzz/FuzzName/`, and from then on plain `go test` runs it.[^5]

**Minimization.** A crashing input from a fuzzer is often long and full of noise. Go minimizes a failing input to the smallest and most readable value that still fails.[^5] AFL trims each input in its queue by removing blocks of data that do not change the coverage, but it keeps crashing inputs exactly as it found them. The tool `afl-tmin` shrinks them on request.[^3] This step is the shrinking of [[property-based-tests|property-based testing]], applied to bytes.

## In languages

The examples run the seed corpus only, with a fixed list of inputs. They show the fixed parser, and they show how each language reacts to the original bug.

| Language | Verdict | Form |
|---|---|---|
| Python | library | Atheris, a coverage-guided fuzzer based on libFuzzer[^8] |
| JavaScript | library | Jazzer.js, a coverage-guided fuzzer for Node.js based on libFuzzer[^10] |
| TypeScript | library | Jazzer.js, as for JavaScript[^10] |
| Java | library | Jazzer, a coverage-guided fuzzer for the JVM, with a regression mode under JUnit[^9] |
| C | library | libFuzzer in Clang: `-fsanitize=fuzzer`, with AddressSanitizer; AFL and AFL++[^4][^3][^12] |
| C++ | library | as for C; the target is `extern "C"`[^4] |
| C# | library | SharpFuzz, which runs .NET code under afl-fuzz[^11] |
| Go | native | `func FuzzXxx(f *testing.F)` in a test file, `f.Add` for seeds, `go test -fuzz`[^5] |
| Rust | library | `cargo fuzz` with `fuzz_target!`; it needs the nightly compiler[^6][^7] |
| Kotlin | library | Jazzer, as for Java[^9] |
| PowerShell | idiom | a loop that pipes generated input to a command, as Miller did with UNIX utilities[^1] |
| Bash | idiom | a pipeline from a generator to the program under test, as `fuzz 100000 -o outfile \| deqn` in Miller's report[^1] |
| SQL | n/a | a query is fuzzed through the database engine that runs it |
| Regex | n/a | a pattern is fuzzed through the engine that runs it |
| HTML/CSS | n/a | markup and style rules are fuzzed through the browser that renders them |

Go is the only tier 1 language with a fuzzer in its standard toolchain. A Go fuzz test lives in a `_test.go` file, takes only the argument types `string`, `[]byte`, the integer and float types, `rune` and `bool`, and needs each seed to have the same types in the same order.[^5] In C and C++, the fuzz target does not depend on libFuzzer, so the same function can run under AFL.[^4] The original authors of libFuzzer have stopped active work on it, but it still receives fixes for important bugs.[^4] AFL is no longer updated, and its home page points to the fork AFL++.[^12]

The languages also differ in what the bug does. Python, Java, C#, Go and Rust check each index, so the missing check is an exception or a panic, and the fuzzer sees it. C and C++ read past the end with no error, so the bug is found only with a sanitizer. JavaScript returns `undefined` for an index past the end of a typed array, so the bug gives a wrong result and no crash at all.

## Variations and related ideas

- **Blind and coverage-guided fuzzing.** Miller's fuzz used no model of the program.[^2] AFL and libFuzzer observe coverage and keep the inputs that reach code not reached before.[^3][^4]
- **Structured inputs.** Miller's later studies of X-Window, Windows NT and Mac OS X applications used valid keyboard and mouse events as well as random streams.[^2]
- **Dictionaries.** AFL combines syntax tokens of a format from a dictionary, and the coverage feedback tells it which combinations reach code not reached before. It can also find tokens in an input by looking for runs of bytes that change the path in the same way when flipped, a sign of a comparison with a constant.[^3]
- **Differential and round-trip oracles.** A harness can check more than "no crash": that two implementations agree, or that an accepted input encodes back to itself. The pitfall "Only crashes are bugs" shows a round-trip oracle.
- **Property-based testing.** A [[property-based-tests|property-based test]] also runs a function on generated inputs, but it starts from typed generators and checks a stated rule. A fuzzer starts from bytes and checks for crashes. Go fuzz tests take typed arguments, so they sit between the two.[^5]

## Pitfalls

### The harness hides the crash

A harness that catches every exception turns each bug into a rejection. The fuzzer then reports nothing.

```python wrong id=swallow
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def parse(data):
    records = []
    i = 0
    while i < len(data):
        tag = data[i]
        length = data[i + 1]  # the same bug as before
        records.append((tag, data[i + 2 : i + 2 + length]))
        i += 2 + length
    return records


state = 2463534242
crashes = 0
for run in range(1000):
    state = xorshift32(state)
    data = bytearray()
    for _ in range(state % 9):
        state = xorshift32(state)
        data.append(state & 0xFF)
    try:
        parse(bytes(data))
    except Exception:
        pass  # hides every crash, not only the documented errors
print("crashes:", crashes)
```

```output for=swallow
crashes: 0
```

Catch only the error that the code documents for bad input, such as `ValueError`. Let every other exception reach the fuzzer. The same holds for a target that calls `exit`: libFuzzer requires that a target must not exit on any input.[^4]

### Only crashes are bugs

A fuzzer with no other oracle finds only crashes and hangs. Miller's report says so: a test succeeded when the program terminated normally, and the correctness of the output was not checked.[^1] The parser below checks the header but not the length of the value. A slice in Python never fails, so a short value is accepted and no crash occurs.

```python wrong id=crash-only
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def parse(data):
    records = []
    i = 0
    while i < len(data):
        if i + 2 > len(data):
            raise ValueError(f"truncated header at offset {i}")
        tag, length = data[i], data[i + 1]
        records.append((tag, data[i + 2 : i + 2 + length]))  # a short value is not detected
        i += 2 + length
    return records


state = 2463534242
for run in range(1, 1001):
    state = xorshift32(state)
    data = bytearray()
    for _ in range(state % 9):
        state = xorshift32(state)
        data.append(state & 0xFF)
    try:
        parse(bytes(data))
    except ValueError:
        pass
print("no crash in 1000 runs")
```

```output for=crash-only
no crash in 1000 runs
```

Add a check to the harness. Here, an accepted input must encode back to the same bytes. The same inputs then show the bug on the second run.

```python run id=round-trip
def xorshift32(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def parse(data):
    records = []
    i = 0
    while i < len(data):
        if i + 2 > len(data):
            raise ValueError(f"truncated header at offset {i}")
        tag, length = data[i], data[i + 1]
        records.append((tag, data[i + 2 : i + 2 + length]))  # a short value is not detected
        i += 2 + length
    return records


def encode(records):
    return b"".join(bytes([tag, len(value)]) + value for tag, value in records)


state = 2463534242
for run in range(1, 1001):
    state = xorshift32(state)
    data = bytearray()
    for _ in range(state % 9):
        state = xorshift32(state)
        data.append(state & 0xFF)
    data = bytes(data)
    try:
        records = parse(data)
    except ValueError:
        continue
    if encode(records) != data:  # the oracle: accepted input must encode back to itself
        print(f"run {run}: {data!r} parses to {records}")
        print(f"which encodes to {encode(records)!r}")
        break
```

```output for=round-trip
run 2: b'~\xe1' parses to [(126, b'')]
which encodes to b'~\x00'
```

### Memory errors that do not crash

In C and C++, a read one byte past the end of a buffer usually returns some byte and goes on. The fuzzer sees no crash. Build the target with AddressSanitizer, as the libFuzzer documentation suggests, so that the read is reported at once.[^4] The C example shows the report. Miller's first guideline, to check every array reference for valid bounds, is the same fix in the code.[^1]

### A target that is not deterministic

A target that uses its own random numbers, the clock or global state gives different coverage for the same input. The fuzzer then keeps inputs that add no coverage and cannot reproduce a failure. libFuzzer asks for a target that is as deterministic as possible.[^4] The Go documentation adds that the fuzz target runs in parallel in several workers, so its state must not persist past one call.[^5]

### The corpus is thrown away

A failure that a fuzzer found once can come back. Keep the failing inputs and run them on each build. Go does this by default with `testdata/fuzz`, and libFuzzer and Jazzer have a regression mode that runs saved inputs with no fuzzing.[^5][^4][^9] The examples on this page do this with the two failing inputs: one byte, `z`, and a header that claims more bytes than follow, `~\xe1`.

## Cost

- **Open-ended run time.** A fuzz run has no natural end. The Go documentation says that the user decides how long to fuzz, and that a run can go on forever if it finds no error.[^5] libFuzzer's option `-runs` sets the number of runs, and its default of -1 runs without end.[^4]
- **Speed per input.** The value of a fuzzer grows with the number of inputs per second. AFL uses a fork server so that the target does `execve`, linking and libc initialization only once. The documentation reports a gain of 1.5 to 2 times for fast targets.[^3] AFL sets the default timeout of a run at five times the calibrated speed, rounded up to 20 ms, and rejects slower paths.[^3]
- **Instrumentation.** Coverage guidance needs a build with instrumentation, and a sanitizer adds more overhead. Go fuzzing needs a platform that supports coverage instrumentation for the corpus to grow.[^5]
- **Harness work.** Someone must write a target for each entry point, decide which errors are documented, and keep the corpus. libFuzzer advises one target for each data format rather than one for all.[^4]

## History

- **1988.** Barton Miller gave the fuzz project as an assignment in his graduate class CS736, Advanced Operating Systems, in the fall of 1988.[^2]
- **1990.** Miller, Fredriksen and So published the study of UNIX utilities in Communications of the ACM. They crashed or hung 24 to 33 percent of the utilities on each of seven versions of UNIX, and grouped the causes: pointer and array errors, unchecked return codes, input functions such as `gets`, and others.[^1][^2]
- **1995 to 2020.** Miller's group repeated the command-line study in 1995, 2006 and 2020, and studied X-Window applications in 1995, Windows NT in 2000 and Mac OS X in 2006.[^2]

## References

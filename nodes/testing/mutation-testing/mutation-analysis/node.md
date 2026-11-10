---
tag: NEW-mutation-analysis
kind: concept
slug: mutation-analysis
name: Mutation analysis
aliases: [mutation testing, program mutation, mutant, mutation score]
area: testing/mutation-testing
level: core
summary: >-
  Mutation analysis plants small faults, called mutants, in copies of a program and runs the tests on each copy. The share of mutants that make a test fail measures how well the tests find faults.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1978
  people: [Richard A. DeMillo, Richard J. Lipton, Frederick G. Sayward]
  note: DeMillo, Lipton and Sayward described program mutation and the coupling effect in IEEE Computer, with a mutation system for Fortran built at Yale University and the Georgia Institute of Technology.
see_also: [unit-testing, property-based-tests, fuzz-testing, test-double, test-driven-development]
same_as:
  wikidata: Q4308623
  wikipedia: https://en.wikipedia.org/wiki/Mutation_testing
sources:
  - paper: doi:10.1109/C-M.1978.218136
    title: Richard A. DeMillo, Richard J. Lipton and Frederick G. Sayward, Hints on Test Data Selection, Help for the Practicing Programmer, IEEE Computer 11(4), April 1978, pages 34 to 41
    locator: page 34 (programs close to being correct); page 35 (the coupling effect); page 36 (program mutation, dead and live mutants, equivalent mutants, adequate test data); page 37 (the MAX example); page 39 (FIND and its equivalent mutants); page 40 (higher-order mutants)
  - paper: doi:10.1109/TSE.2010.62
    title: Yue Jia and Mark Harman, An Analysis and Survey of the Development of Mutation Testing, IEEE Transactions on Software Engineering 37(5), 2011, pages 649 to 678
    locator: I Introduction; II.A Fundamental Hypotheses; II.B The Process of Mutation Analysis; II.C The Problems of Mutation Analysis; III.A.1 Mutant Sampling; III.B Execution Cost Reduction Techniques (strong, weak and firm mutation; mutant schema generation); IV Equivalent Mutant Detection Techniques; the reference list
  - url: https://pitest.org/
    title: PIT Mutation Testing, home page
    locator: What is mutation testing?; What's wrong with line coverage?; How to use it?
  - url: https://pitest.org/quickstart/mutators/
    title: PIT, Mutation Operators
    locator: Overview (bytecode, default operators); Conditionals Boundary Mutator; Negate Conditionals Mutator
  - url: https://mutmut.readthedocs.io/en/latest/
    title: mutmut, Python mutation tester, documentation
    locator: Install and run; Skipping via code comments; Filter generated mutants with type checker; Unstable configs (the timeout); Example mutations
  - url: https://stryker-mutator.io/docs/mutation-testing-elements/mutant-states-and-metrics/
    title: Stryker Mutator, Mutant states and metrics
    locator: Mutant states; Metrics
  - url: https://stryker-mutator.io/
    title: Stryker Mutator, home page
    locator: the list of supported languages (JavaScript and TypeScript, C#, Scala)
  - url: https://mutants.rs/
    title: cargo-mutants, Welcome
    locator: the introduction
  - url: https://mutants.rs/mutants.html
    title: cargo-mutants, Mutation patterns
    locator: Functions that are excluded from mutation; Replace function body with value; Binary operators; Unary operators
  - url: https://github.com/mull-project/mull
    title: Mull, README
    locator: the introduction; Citation
  - url: https://github.com/go-gremlins/gremlins
    title: Gremlins, a mutation testing tool for Go, README
    locator: the introduction; What is Mutation Testing
---

## Idea

A test suite can pass and still miss bugs. Mutation analysis measures how many bugs a suite can find, by planting bugs on purpose. A tool makes many copies of the program. Each copy, called a mutant, differs from the original by one small change, such as `>=` in place of `>`. The tool runs the suite on each mutant. If a test fails, the mutant is killed. If every test passes, the mutant lives: the suite cannot tell that program from the original.[^1][^2]

The idea rests on an observation by DeMillo, Lipton and Sayward. Programmers do not write programs at random. They write programs that are close to being correct, and each pass through the design brings the program closer.[^1] So a real fault is often a small change away from the correct program. The authors added a second claim, the coupling effect: test data that tells apart all programs that differ from a correct one by small errors also tells apart more complex errors. They called it an empirical principle that no one can prove.[^1] Later work calls the first claim the competent programmer hypothesis.[^2]

The program below is the example from the 1978 paper. `argmax` returns the index of the first largest item in a list.[^1] Each mutant is a text replacement in the source, and `exec` compiles the changed text into a second function. The first suite has the three vectors of the paper, and the second adds one more.

```python run id=idea
SOURCE = """
def argmax(xs):
    best = 0
    for i in range(1, len(xs)):
        if xs[i] > xs[best]:
            best = i
    return best
"""

# Each mutant makes one small change to the source text.
MUTANTS = [
    ("> to >=", "xs[i] > xs[best]", "xs[i] >= xs[best]"),
    ("> to <", "xs[i] > xs[best]", "xs[i] < xs[best]"),
    ("xs[i] to xs[best]", "xs[i] > xs[best]", "xs[best] > xs[best]"),
    ("start at 0", "range(1,", "range(0,"),
]

WEAK = [([1, 2, 3], 2), ([1, 3, 2], 1), ([3, 1, 2], 0)]
STRONG = WEAK + [([2, 2, 1], 0)]


def load(source):
    namespace = {}
    exec(source, namespace)
    return namespace["argmax"]


def passes(argmax, suite):
    return all(argmax(xs) == want for xs, want in suite)


assert passes(load(SOURCE), STRONG), "fix the program before mutating it"

print(f"{'mutant':<20}{'3 tests':<8}4 tests")
killed = {"weak": 0, "strong": 0}
for label, old, new in MUTANTS:
    mutant = load(SOURCE.replace(old, new))
    row = []
    for name, suite in (("weak", WEAK), ("strong", STRONG)):
        dead = not passes(mutant, suite)
        killed[name] += dead
        row.append("killed" if dead else "lived")
    print(f"{label:<20}{row[0]:<8}{row[1]}")
total = len(MUTANTS)
print(f"{'killed':<20}{killed['weak']} of {total}  {killed['strong']} of {total}")
```

```output for=idea
mutant              3 tests 4 tests
> to >=             lived   killed
> to <              killed  killed
xs[i] to xs[best]   killed  killed
start at 0          lived   lived
killed              2 of 4  3 of 4
```

The three tests have no repeated items, so `>` and `>=` give the same answer on each of them. The paper found the same gap in its data and added the vector (2, 2, 1).[^1] With the tie, the first of two equal items must win, and the `>=` mutant picks the second. The mutant `start at 0` lives against both suites. Its extra first pass compares `xs[0]` with itself, which is never larger, so the mutant computes the same function as the original. The paper concluded that no test data could tell this mutant from MAX, and that test data which leaves only such mutants alive is adequate.[^1]

A live mutant is a question for the programmer. The paper puts it in the voice of the mutation system: "The test data you've given me says it doesn't matter whether or not this test is for equality or inequality; why is that?"[^1] The answer is either one more test or a proof that the change does not matter.

Mutation analysis measures something that code coverage does not. Line and branch coverage record which code the tests run. They do not check that the tests can detect a fault in that code.[^3] A [[unit-testing|unit test]] that calls a function and checks nothing gives full coverage and kills no mutants. The pitfall "Coverage without checks" shows this.

## How it works

Jia and Harman describe the process in five steps.[^2]

1. **Make the mutants.** A mutation operator is a rule that turns one piece of a program into another, such as `<` into `<=`. The tool applies each operator at each place where it fits, and each application gives one mutant.
2. **Run the original.** The suite must pass on the original program. If it does not, the program has a known bug, and it must be fixed before mutation starts.
3. **Run each mutant.** If the result of any test differs from the result on the original, the mutant is killed. Otherwise it lives. Jia and Harman also say "survived".
4. **Find the equivalent mutants.** Some mutants differ in syntax from the original but compute the same function. No test can kill them. Program equivalence is undecidable, so no tool can find all of them, and a person must check the rest.
5. **Compute the score.** The mutation score is the number of killed mutants divided by the number of mutants that are not equivalent. A score of 1 means that the suite detects every fault that the mutants stand for.

The operators decide which faults the score can see. PIT has a "conditionals boundary" operator, which swaps `<` with `<=` and `>` with `>=`, and a "negate conditionals" operator, which swaps `==` with `!=`, `<=` with `>`, and `>=` with `<`.[^4] mutmut adds 1 to integer literals, changes `<` to `<=`, and swaps `break` with `continue`.[^5] cargo-mutants replaces `>` with `==` or `<`, and it can replace a whole function body with a value of the return type, such as `0` or `1` for an unsigned integer.[^9]

The program below makes mutants from the syntax tree, not from the text. It walks the tree of `argmax` and applies two of PIT's relational rules and the integer rule of mutmut. Each mutant starts from a fresh tree, so that only one change is active at a time.

```python run id=operators
import ast

SOURCE = """
def argmax(xs):
    best = 0
    for i in range(1, len(xs)):
        if xs[i] > xs[best]:
            best = i
    return best
"""

WEAK = [([1, 2, 3], 2), ([1, 3, 2], 1), ([3, 1, 2], 0)]
STRONG = WEAK + [([2, 2, 1], 0)]

# Two relational rules from PIT's default operators.
RELATIONAL = {ast.Gt: [ast.GtE, ast.LtE], ast.Lt: [ast.LtE, ast.GtE]}


def mutations(tree):
    """Lists each (node, change) that the operators allow, in a fixed order."""
    found = []
    for node in ast.walk(tree):
        if isinstance(node, ast.Compare):
            for op in RELATIONAL.get(type(node.ops[0]), ()):
                found.append((node, op))
        elif isinstance(node, ast.Constant) and type(node.value) is int:
            found.append((node, node.value + 1))  # mutmut's rule for integers
    return found


def load(tree):
    namespace = {}
    exec(compile(ast.fix_missing_locations(tree), "<mutant>", "exec"), namespace)
    return namespace["argmax"]


def passes(argmax, suite):
    return all(argmax(xs) == want for xs, want in suite)


assert passes(load(ast.parse(SOURCE)), STRONG)
count = len(mutations(ast.parse(SOURCE)))
for k in range(count):
    tree = ast.parse(SOURCE)  # a fresh tree for each mutant
    node, change = mutations(tree)[k]
    before = ast.unparse(node)
    if isinstance(node, ast.Compare):
        node.ops = [change()]
    else:
        node.value = change
    argmax = load(tree)
    weak = "killed" if not passes(argmax, WEAK) else "lived"
    strong = "killed" if not passes(argmax, STRONG) else "lived"
    print(f"line {node.lineno - 1}: {before} -> {ast.unparse(node)}: 3 tests {weak}, 4 tests {strong}")
```

```output for=operators
line 2: 0 -> 1: 3 tests killed, 4 tests killed
line 3: 1 -> 2: 3 tests killed, 4 tests killed
line 4: xs[i] > xs[best] -> xs[i] >= xs[best]: 3 tests lived, 4 tests killed
line 4: xs[i] > xs[best] -> xs[i] <= xs[best]: 3 tests killed, 4 tests killed
```

The operators made four mutants, and the fourth test kills all of them. They did not make the equivalent mutant `start at 0`, because the integer rule only adds 1. A different set of operators gives a different set of mutants and a different score for the same tests.

**Where the change is made.** A tool can change the source text, the syntax tree, or the compiled code. mutmut changes Python source.[^5] PIT changes the bytecode that the Java compiler made. Its documentation says that this is in general much faster and easier to add to a build, but that a bytecode change can be hard to describe as a change in the source.[^4] Mull is built on LLVM.[^10]

**One build for all mutants.** In a compiled language, a separate build for each mutant costs a compile for each one. The mutant schema technique writes one program that holds every mutant, and a switch picks the active one at run time. The compile happens once.[^2] The examples for the compiled languages on this page use a schema: a global variable or a parameter selects the mutant.

**States beyond killed and lived.** Stryker uses more states. A mutant has "no coverage" when no test runs its code. A run that hits a timeout counts as detected, because an infinite loop would also stop a build. A mutant that ends in a compile error or a runtime error of the test runner is invalid and is not in the score. Stryker computes the score as detected mutants divided by valid mutants, times 100.[^6]

## In languages

Mutation analysis is a tool, not a language feature. No tier 1 language has it in its standard toolchain. The examples write a small mutation harness in each language with the same four mutants of `argmax`, so the tables can be compared.

| Language | Verdict | Form |
|---|---|---|
| Python | library | mutmut, which mutates the source and runs pytest[^5] |
| JavaScript | library | StrykerJS[^7] |
| TypeScript | library | StrykerJS[^7] |
| Java | library | PIT, which mutates bytecode[^3][^4] |
| C | library | Mull, built on LLVM[^10] |
| C++ | library | Mull[^10] |
| C# | library | Stryker.NET[^7] |
| Go | library | Gremlins[^11] |
| Rust | library | cargo-mutants, run as `cargo mutants`[^8][^9] |
| Kotlin | library | PIT works on JVM bytecode; arcmutate, from the same team, extends PIT with support for Kotlin[^3] |
| PowerShell | idiom | a script that edits a copy of the source, runs the tests and checks the result |
| Bash | idiom | a script that edits a copy of the source, runs the tests and checks the exit status |
| SQL | n/a | queries are tested through the program or the database that runs them |
| Regex | n/a | a pattern is tested through the program that uses it |
| HTML/CSS | n/a | markup and style rules have no test suite to measure in this sense |

The languages differ in how a harness can make a mutant. Python and JavaScript can compile changed source text at run time, with `exec` and `new Function`. In the compiled languages, the examples build all mutants into one program.

The languages also differ in which mutants reach the tests. A type checker rejects some mutants before any test runs. Rust has no unary minus for `usize`, so that mutant does not compile, and cargo-mutants calls such a mutant "unviable".[^9] mutmut can use mypy or pyrefly to remove mutants that do not type check.[^5] In C and C++, a mutant can read past the end of an array. The read is undefined behavior, so the mutant may live or die by chance, unless a sanitizer stops it.

## Variations and related ideas

- **Strong, weak and firm mutation.** The method of DeMillo et al. compares the output of the whole program. This is strong mutation. Howden's weak mutation checks the state right after the mutated component runs, which is cheaper but can give a weaker test set. Firm mutation lies between the two.[^2]
- **Higher-order mutants.** A higher-order mutant has more than one change. DeMillo et al. ran more than 22,000 higher-order mutants of Hoare's FIND on their seven test vectors. Only 19 of them passed, and those 19 were equivalent to FIND.[^1] Offutt found that test data which killed all first-order mutants killed over 99 percent of the second-order and third-order mutants.[^2]
- **Fewer mutants.** Mutant sampling runs a random subset. In the studies of Wong and Mathur, a random 10 percent of mutants was only 16 percent less effective than the full set. Selective mutation uses fewer operators. Mathur suggested leaving out two of the 22 Mothra operators, ASR and SVR, which made about 30 to 40 percent of all mutants. Offutt et al. reported that this 2-selective mutation reached a mean score of 99.99 percent with 24 percent fewer mutants.[^2]
- **Generated inputs.** A [[property-based-tests|property-based test]] or a [[fuzz-testing|fuzz test]] makes many inputs, and an input such as the tie `[2, 2, 1]` can come from a generator as well as from a person. Mutation analysis then measures the generator: a mutant that lives shows a kind of input it never makes.
- **Test-driven development.** In [[test-driven-development|test-driven development]], each line of code exists to pass a test, so few mutants should live. The Gremlins README names support for the TDD process as one of its uses.[^11]

## Pitfalls

### Coverage without checks

The suite below runs every line of `argmax`, and every assertion holds. It checks only that the result is a valid index. A coverage tool reports full line coverage, and the suite kills none of the mutants.

```python wrong id=coverage
import sys

SOURCE = """
def argmax(xs):
    best = 0
    for i in range(1, len(xs)):
        if xs[i] > xs[best]:
            best = i
    return best
"""

MUTANTS = [
    ("> to >=", "xs[i] > xs[best]", "xs[i] >= xs[best]"),
    ("> to <", "xs[i] > xs[best]", "xs[i] < xs[best]"),
    ("xs[i] to xs[best]", "xs[i] > xs[best]", "xs[best] > xs[best]"),
    ("start at 0", "range(1,", "range(0,"),
]

INPUTS = [[1, 2, 3], [1, 3, 2], [3, 1, 2], [2, 2, 1]]


def load(source):
    namespace = {}
    exec(compile(source, "<argmax>", "exec"), namespace)
    return namespace["argmax"]


def suite(argmax):
    """Runs argmax on each input but checks only that the index is valid."""
    return all(0 <= argmax(xs) < len(xs) for xs in INPUTS)


lines = set()


def tracer(frame, event, arg):
    if frame.f_code.co_name == "argmax" and event == "line":
        lines.add(frame.f_lineno)
    return tracer


sys.settrace(tracer)
assert suite(load(SOURCE))
sys.settrace(None)
print(f"lines of argmax run by the suite: {sorted(lines)}")

killed = sum(not suite(load(SOURCE.replace(old, new))) for _, old, new in MUTANTS)
print(f"mutants killed: {killed} of {len(MUTANTS)}")
```

```output for=coverage
lines of argmax run by the suite: [3, 4, 5, 6, 7]
mutants killed: 0 of 4
```

The output shows lines 3 to 7, which is the whole body. The PIT documentation calls a test with no assertions the most extreme case of this problem, and it notes that a suite can execute all branches of code that it tests only in part.[^3] The fix is to compare each result with the expected index, as the four tests in "Idea" do.

### Equivalent mutants

A mutant that computes the same function as the original cannot be killed. It stays in the list of live mutants and lowers the score until a person marks it. The program below compares two mutants with `argmax` on every list of up to five items from the values 0 to 3.

```python run id=equivalent
from itertools import product


def argmax(xs):
    best = 0
    for i in range(1, len(xs)):
        if xs[i] > xs[best]:
            best = i
    return best


def start_at_0(xs):  # the mutant: range(1, ...) became range(0, ...)
    best = 0
    for i in range(0, len(xs)):
        if xs[i] > xs[best]:
            best = i
    return best


def boundary(xs):  # the mutant: > became >=
    best = 0
    for i in range(1, len(xs)):
        if xs[i] >= xs[best]:
            best = i
    return best


inputs = [list(t) for n in range(6) for t in product(range(4), repeat=n)]
for mutant in (start_at_0, boundary):
    differ = [xs for xs in inputs if mutant(xs) != argmax(xs)]
    first = differ[0] if differ else None
    print(f"{mutant.__name__}: differs on {len(differ)} of {len(inputs)} lists, first {first}")
```

```output for=equivalent
start_at_0: differs on 0 of 1365 lists, first None
boundary: differs on 672 of 1365 lists, first [0, 0]
```

The search finds no difference for `start_at_0` and finds one at once for `boundary`. A search like this is evidence, not proof. The proof here is short: the extra first pass compares `xs[0]` with `xs[0]`, and `>` is false for equal values. In general no algorithm decides whether two programs are equivalent, so the check needs a person.[^2] For FIND, about 2 percent of the mutants were equivalent. The authors put this down to the long tuning of FIND, and estimated that production programs have well under 1 percent.[^1] Tools let the user skip code that makes equivalent mutants: mutmut reads the comment `# pragma: no mutate`, and cargo-mutants reads the attribute `#[mutants::skip]`.[^5][^9]

### Scores from different tools differ

The definition of the score decides its value. Jia and Harman divide by the mutants that are not equivalent.[^2] Stryker divides by the valid mutants, counts timeouts as detected, and leaves out compile errors and runtime errors.[^6] PIT gauges the quality of tests by the percentage of mutations killed.[^3] The operators also differ between tools. So a score of 80 percent from one tool and 80 percent from another do not say the same thing, and it is safer to compare a score with earlier runs of the same tool on the same code.

### Errors that look like kills

A harness that counts every exception as a failed test can report a mutant as killed when the harness broke it. The JavaScript example shows a mutant that dies of a `ReferenceError` because the harness compiled it outside its module scope. Two rules prevent this. Run the suite on the unchanged program first, through the same harness.[^2] And keep errors apart from failed assertions, as Stryker does with its runtime error state.[^6]

### Mutants that do not stop

A mutant can turn a loop into one that never ends, for example by changing `i += 1` into `i -= 1`. The harness needs a time limit for each run. mutmut sets the limit from the duration of the original tests, a constant and a multiplier.[^5] Stryker counts a timeout as detected.[^6]

## Cost

- **Run time.** The cost is about the number of mutants times the time of the tests that cover each one. Jia and Harman name the cost of running the large number of mutants as a problem that keeps mutation testing from being a practical technique.[^2] The Gremlins README says that a run on a large Go module can take hours.[^11]
- **Ways to cut it.** PIT advises running mutation testing often but only on the code that changed.[^3] mutmut knows which tests to run for each mutant.[^5] A mutant schema saves the compile for each mutant, and sampling runs fewer mutants.[^2]
- **Human time.** Each live mutant needs a decision: one more test or a mark as equivalent. More tests also mean more expected outputs to check by hand, which Jia and Harman call the human oracle problem.[^2]

## History

- **1971.** Richard Lipton wrote a student report at Carnegie Mellon University, Fault Diagnosis of Computer Programs. Jia and Harman trace mutation testing back to it.[^2]
- **1977.** Richard Hamlet published Testing Programs with the Aid of a Compiler in IEEE Transactions on Software Engineering.[^2]
- **1978.** DeMillo, Lipton and Sayward published Hints on Test Data Selection. It described program mutation, dead and live mutants, equivalent mutants and the coupling effect, with the examples MAX, a triangle classifier and Hoare's FIND.[^1]
- **1982.** Budd and Angluin published the work that Jia and Harman cite for the undecidability of mutant equivalence.[^2]
- **1989 and 1992.** Offutt published studies of the coupling effect with first-order and higher-order mutants.[^2]
- **1991.** King and Offutt described the Mothra system for Fortran, which implemented the first formal set of mutation operators.[^2]
- **1993.** Untch, Offutt and Harrold presented mutant schemata at ISSTA.[^2]
- **2011.** Jia and Harman published their survey of the field in IEEE Transactions on Software Engineering.[^2]

## References

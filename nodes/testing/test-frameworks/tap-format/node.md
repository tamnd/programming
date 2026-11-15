---
tag: QS0KQM
kind: format
slug: tap-format
name: Test Anything Protocol
aliases: [TAP, TAP 14, TAP13, TAP version 14]
area: testing/test-frameworks
level: advanced
summary: >-
  TAP is a line-based text format for test results. A producer prints a plan and one ok or not ok line for each test point, and a harness reads the stream and decides if the run passed.
see_also: [test-framework, bats, node-test, junit-xml-format, unit-testing, yaml]
sources:
  - url: https://testanything.org/
    title: Test Anything Protocol, Home
    locator: the introduction; the example stream
    accessed: 2026-11-16
  - url: https://testanything.org/tap-version-14-specification.html
    title: TAP 14 specification
    locator: Changes From TAP13; TAP14 Format; Harness Behavior; Plan; Test Points; Directive; YAML Diagnostics; Pragmas; Bail out!; Escaping; Subtests; Authors
    accessed: 2026-11-16
  - url: https://testanything.org/history.html
    title: Test Anything Protocol, TAP History
    locator: Historical Versions, Version 1 to Version 11
    accessed: 2026-11-16
  - url: https://testanything.org/philosophy.html
    title: Test Anything Protocol, TAP Philosophy
    locator: the whole page
    accessed: 2026-11-16
  - url: https://testanything.org/tap-specification.html
    title: TAP specification
    locator: SYNOPSIS; The version
    accessed: 2026-11-16
  - url: https://testanything.org/tap-version-13-specification.html
    title: TAP 13 specification
    locator: THE TAP13 FORMAT; HARNESS BEHAVIOR; COPYRIGHT
    accessed: 2026-11-16
  - url: https://github.com/TestAnything/Specification
    title: TestAnything/Specification, README
    locator: Guidelines For TAP Specification Changes
    accessed: 2026-11-16
  - url: https://perldoc.perl.org/Test::More
    title: Perldoc, Test::More
    locator: SYNOPSIS; done_testing
    accessed: 2026-11-16
  - url: https://perldoc.perl.org/prove
    title: Perldoc, prove
    locator: NAME; Default Test Directory; Exit Code; --exec
    accessed: 2026-11-16
  - url: https://bats-core.readthedocs.io/en/stable/usage.html
    title: bats-core documentation, Usage
    locator: the options --formatter and --tap
    accessed: 2026-11-16
  - url: https://nodejs.org/api/test.html
    title: Node.js v26.13.1 documentation, Test runner
    locator: Test reporters
    accessed: 2026-11-16
  - url: https://testanything.org/producers.html
    title: Test Anything Protocol, TAP Producers
    locator: Go; JavaScript
    accessed: 2026-11-16
  - url: https://testanything.org/consumers.html
    title: Test Anything Protocol, TAP Consumers
    locator: Python; POSIX shell; Go; Java; Perl
    accessed: 2026-11-16
---

## What it is

The Test Anything Protocol (TAP) "is a simple text-based interface between testing modules in a test harness. It decouples the reporting of errors from the presentation of the reports."[^1] A test program is a producer. It writes TAP to standard output. A harness is a consumer. It reads the TAP, shows the results and decides if the run passed.[^2]

TAP comes from the test harness of Perl. Larry Wall created the basis of the format in the original test script of Perl 1, and the history page dates the protocol from 1988.[^2][^3] Now there are producers and consumers in C, C++, Python, PHP, Perl, Java, JavaScript, Go, Rust and other languages. A producer and a consumer do not have to use the same language.[^1]

This is the example stream on the home page:[^1]

```text
1..4
ok 1 - Input file opened
not ok 2 - First line of the input valid
ok 3 - Read the rest of the file
not ok 4 - Summarized correctly # TODO Not written yet
```

## Syntax or structure

The TAP 14 specification gives this grammar. It says that the grammar is rough guidance for humans, and that it is not strict EBNF.[^2]

```text
TAPDocument := Version Plan Body | Version Body Plan
Version     := "TAP version 14\n"
Plan        := "1.." (Number) (" # " Reason)? "\n"
Body        := (TestPoint | BailOut | Pragma | Comment | Anything | Empty | Subtest)*
TestPoint   := ("not ")? "ok" (" " Number)? ((" -")? (" " Description) )? (" " Directive)? "\n" (YAMLBlock)?
Directive   := " # " ("todo" | "skip") (" " Reason)?
YAMLBlock   := "  ---\n" (YAMLLine)* "  ...\n"
BailOut     := "Bail out!" (" " Reason)? "\n"
Pragma      := "pragma " [+-] PragmaKey "\n"
Subtest     := ("# Subtest" (": " SubtestName)?)? "\n" SubtestDocument TestPoint
Comment     := ^ (" ")* "#" [^\n]* "\n"
```

The parts of a TAP 14 document are these:[^2]

- **Version.** The first line is `TAP version 14`. A harness can also read a stream that starts with `TAP version 13` as TAP 14.
- **Plan.** The line `1..N` says that the stream has N test points. It comes one time, before all the test points or after all of them. A harness must treat a stream with no plan as a failed test. The plan `1..0` means that the whole test set was skipped.
- **Test point.** A line starts with `ok` or `not ok`, which is the only mandatory part. Then come the test point ID, a description after ` - `, and a directive. If there is no number, the harness counts the test points itself.
- **Directive.** The text after the first unescaped `#` with white space before it. Only `SKIP` and `TODO` are defined, and they are not case sensitive. A failed test point with `TODO` or `SKIP` is not a test failure.
- **YAML diagnostics.** A block that starts with `---` and ends with `...`, with 2 spaces of indent, after a test point. It is a YAML 1.2 document with information about that test point, for example the actual and the expected values. The fields are not standardized.
- **Comment.** A line that starts with `#`. A harness can show comments or ignore them.
- **Bail out!** The line `Bail out!` and a reason stop the whole run.
- **Pragma.** A line such as `pragma +strict` turns a harness option on or off. The keys depend on the implementation.
- **Subtest.** A complete TAP document with 4 spaces of indent. The comment `# Subtest: name` can come first. A test point at the parent level ends the subtest and gives its result.

The producer must use UTF-8.[^2] The harness reads TAP from standard output, not from standard error. A harness treats a test program as failed if the TAP shows a failure, if the TAP is invalid in a way that it cannot recover from, or if the exit code of the program is not 0.[^2] The TAP philosophy page asks parsers to work on the stream line by line, to ignore unknown directives, and to ignore lines that they cannot parse.[^4]

In a description or a reason, `\#` is a literal `#` and `\\` is a literal `\`. A producer should escape every `#` and `\` in these fields.[^2]

## Examples

### Write TAP from a shell script

TAP is only lines of text, so a shell function can produce it. This script prints a test point for each check, a YAML block for the failure, a skipped test and a todo test. The plan comes at the end, after the script knows the count.[^2]

```bash run id=shell-producer
# A small TAP 14 producer in shell.
n=0

point() { # point <ok|not ok> <description> [directive]
  n=$((n + 1))
  echo "$1 $n - $2${3:+ # $3}"
}

check() { # check <description> <command...>
  local desc=$1
  shift
  if "$@" >/dev/null 2>&1; then
    point ok "$desc"
  else
    point "not ok" "$desc"
    echo "  ---"
    echo "  message: 'the command failed'"
    echo "  command: '$*'"
    echo "  ..."
  fi
}

echo "TAP version 14"
check "sort is installed" command -v sort
check "sort puts apple first" test "$(printf 'pear\napple\n' | sort | head -n 1)" = apple
check "grep finds zebra" grep -q zebra /dev/null
point ok "read the config file" "SKIP no config file in this example"
point "not ok" "sort by locale" "TODO not written yet"
echo "1..$n"
```

```output for=shell-producer
TAP version 14
ok 1 - sort is installed
ok 2 - sort puts apple first
not ok 3 - grep finds zebra
  ---
  message: 'the command failed'
  command: 'grep -q zebra /dev/null'
  ...
ok 4 - read the config file # SKIP no config file in this example
not ok 5 - sort by locale # TODO not written yet
1..5
```

Test point 3 fails, and test point 5 fails with the directive `TODO`. A harness reports this run as a failure because of test point 3 only.[^2]

### Read TAP in Python

This consumer follows the parsing rules of TAP 14. It reads the plan, the test points and their directives, skips the YAML blocks, removes the escapes, and treats the indented lines of a subtest as part of the subtest. Only the correlated test point at the parent level counts.[^2]

```python run id=python-consumer
import re

POINT = re.compile(r"(not )?ok(?=$| )(?: (\d+))?(?: - | )?(.*)")
DIRECTIVE = re.compile(r"(skip|todo)\S*(?:\s+(.*))?", re.IGNORECASE)


def split_directive(text):
    """Split at the first # that is not escaped and has a space or nothing before it."""
    i = 0
    while i < len(text):
        if text[i] == "\\":
            i += 2
            continue
        if text[i] == "#" and (i == 0 or text[i - 1] == " "):
            return text[:i].rstrip(), text[i + 1 :].strip()
        i += 1
    return text, ""


def unescape(text):
    return re.sub(r"\\([\\#])", r"\1", text)


def parse(tap):
    plan, points, bail = None, [], None
    in_yaml = False
    for line in tap.splitlines():
        if in_yaml:
            in_yaml = line.strip() != "..."
        elif line.startswith("    "):
            continue  # a subtest line: the correlated test point comes later
        elif line.strip() == "---":
            in_yaml = True
        elif m := re.fullmatch(r"1\.\.(\d+)(?: # .*)?", line):
            plan = int(m.group(1))
        elif line.lower().startswith("bail out!"):
            bail = line[9:].strip()
            break
        elif m := POINT.fullmatch(line):
            failed, number, rest = m.groups()
            text, directive = split_directive(rest)
            d = DIRECTIVE.fullmatch(directive) if directive else None
            kind = d.group(1).lower() if d else ("fail" if failed else "pass")
            points.append(
                {
                    "id": int(number) if number else len(points) + 1,
                    "kind": kind,
                    "description": unescape(text),
                    "reason": unescape(d.group(2) or "") if d else "",
                }
            )
    return plan, points, bail


TAP = """\
TAP version 14
ok 1 - input file opened
not ok 2 - first line valid
  ---
  message: 'First line invalid'
  severity: fail
  ...
not ok 3 - summary # TODO not written yet
ok 4 - read the cache # SKIP no cache
ok 5 - issue \\#42 is fixed
# Subtest: parser
    1..1
    ok 1 - parses an empty file
ok 6 - parser
1..6
"""

plan, points, bail = parse(TAP)
for p in points:
    reason = f" ({p['reason']})" if p["reason"] else ""
    print(f"{p['id']}  {p['kind']:<4}  {p['description']}{reason}")
failures = [p["id"] for p in points if p["kind"] == "fail"]
ok = bail is None and plan == len(points) and not failures
print(f"plan {plan}, test points {len(points)}, failed {failures}")
print("result:", "PASS" if ok else "FAIL")
```

```output for=python-consumer
1  pass  input file opened
2  fail  first line valid
3  todo  summary (not written yet)
4  skip  read the cache (no cache)
5  pass  issue #42 is fixed
6  pass  parser
plan 6, test points 6, failed [2]
result: FAIL
```

The run fails because test point 2 failed. The `TODO` test point 3 and the `SKIP` test point 4 do not count as failures, and `\#42` becomes `#42`.[^2]

### Write TAP 14 with a subtest in Go

This producer tests a function `slug` and writes the results as a subtest. The subtest starts with the comment `# Subtest: slug`, its lines have 4 spaces of indent, and the YAML block of a test point in the subtest has 6 spaces, 4 + 2. The test point `not ok 1 - slug` at the parent level ends the subtest and gives its result.[^2]

```go run id=go-subtest
package main

import (
	"fmt"
	"strings"
)

// escape makes text safe for a TAP description: \ becomes \\ and # becomes \#.
func escape(s string) string {
	return strings.NewReplacer(`\`, `\\`, `#`, `\#`).Replace(s)
}

// slug is the function under test. It has a bug: it drops the plus signs.
func slug(s string) string {
	var b strings.Builder
	dash := false
	for _, r := range strings.ToLower(s) {
		if r >= 'a' && r <= 'z' || r >= '0' && r <= '9' {
			b.WriteRune(r)
			dash = false
		} else if !dash && b.Len() > 0 {
			b.WriteByte('-')
			dash = true
		}
	}
	return strings.TrimSuffix(b.String(), "-")
}

// tap writes one TAP document. A subtest is a document with a 4-space indent.
type tap struct {
	indent string
	count  int
	failed bool
}

func (t *tap) point(ok bool, desc string) {
	t.count++
	status := "ok"
	if !ok {
		status = "not ok"
		t.failed = true
	}
	fmt.Printf("%s%s %d - %s\n", t.indent, status, t.count, escape(desc))
}

func (t *tap) check(desc, got, want string) {
	t.point(got == want, desc)
	if got != want {
		fmt.Printf("%s  ---\n", t.indent)
		fmt.Printf("%s  got: %q\n", t.indent, got)
		fmt.Printf("%s  want: %q\n", t.indent, want)
		fmt.Printf("%s  ...\n", t.indent)
	}
}

func (t *tap) plan() {
	fmt.Printf("%s1..%d\n", t.indent, t.count)
}

func main() {
	fmt.Println("TAP version 14")
	root := &tap{}

	fmt.Println("# Subtest: slug")
	sub := &tap{indent: "    "}
	cases := []struct{ in, want string }{
		{"Hello World", "hello-world"},
		{"C# in depth", "c-in-depth"},
		{"  spaces  ", "spaces"},
		{"C++ primer", "c-plus-plus-primer"},
	}
	for _, c := range cases {
		sub.check(fmt.Sprintf("slug(%q)", c.in), slug(c.in), c.want)
	}
	sub.plan()
	root.point(!sub.failed, "slug")

	root.check("escape", escape(`a\b#c`), `a\\b\#c`)
	root.plan()
}
```

```output for=go-subtest
TAP version 14
# Subtest: slug
    ok 1 - slug("Hello World")
    ok 2 - slug("C\# in depth")
    ok 3 - slug("  spaces  ")
    not ok 4 - slug("C++ primer")
      ---
      got: "c-primer"
      want: "c-plus-plus-primer"
      ...
    1..4
not ok 1 - slug
ok 2 - escape
1..2
```

The second description has a `#` with a space after it, so the producer escapes it. Without the escape, the harness reads the text after this `#` as a directive. A TAP 14 harness puts an unknown directive back in the description, but a word such as `todo` or `skip` after the `#` changes the result of the test point.[^2]

## Versions

| Version | Change |
|---|---|
| Version 1 | the `t/TEST` script of Perl 1.0, 1988: the header `1..M`, and `ok #` and `not ok #`[^3] |
| Versions 2 to 11 | Test::Harness in Perl 5, 1996 to 2001: the exit status, comments, `1..0`, the skip directive, `Bail out!`, the `TODO` directive[^3] |
| TAP 12 | the plan can come at the end; a stream with no version line is version 12[^3][^5] |
| TAP 13 | the line `TAP version 13`, and YAML diagnostics after a test point[^3][^6] |
| TAP 14 | the line `TAP version 14`; subtests with 4 spaces of indent; a plan always starts at 1; the escapes of `#` and `\`; pragmas[^2] |

Andy Armstrong wrote the TAP 13 specification, and Isaac Z. Schlueter wrote the TAP 14 specification.[^2] TAP 14 documents the behavior of TAP implementations that are in wide use, and it adds no feature that is not already in wide use across multiple implementations. It is "largely backwards compatible with TAP13".[^2] The specification project accepts a change only when multiple implementations use it.[^7]

## Pitfalls

- **A line that looks like a test point.** A harness reads standard output, so a test program must not print its own lines that start with `ok` or `not ok` there. TAP 13 says that every line on standard output that matches `/^(not )?ok\b/` is a test line.[^6]
- **A missing plan.** If a test program stops early, the plan catches it. A stream with no plan is a failed test. In the example of the specification, the plan is `1..6` but the stream has 5 test points, and Test::Harness reports the missing test point 6 as a failure.[^2]
- **An unescaped `#`.** A `#` with white space before it starts a directive. In the line `ok 3 - parse the # todo list`, the harness reads a `TODO` directive with the reason `list`, not a description. Escape `#` as `\#` in descriptions.[^2]
- **TODO hides failures.** A failed `TODO` test point is not a failure, so a todo test can stay broken for a long time. If a `TODO` test point passes, the harness can report it, so that the developer makes it a normal test point.[^2]
- **Subtests and TAP 13 harnesses.** A TAP 13 harness ignores the indented lines and reads only the correlated test point. Some TAP 13 parsers treat a second version line as an error, even when it is indented, so a subtest should not have a version line.[^2]
- **The exit code.** A harness treats a nonzero exit code as a failure, even if all the test points are `ok`.[^2]

## Tools and libraries

- **Perl.** Test::More writes TAP. `use Test::More tests => 23;` prints the plan first, and `done_testing()` prints it at the end.[^8] `prove` runs tests "through a TAP harness". With no arguments it runs the files `t/*.t`, it exits with a nonzero status if the tests fail, and `--exec` runs tests that are not written in Perl.[^9]
- **Shell.** [[bats]] prints TAP with `--tap`, which is short for `--formatter tap`. The formatter `tap13` prints TAP 13. The formatter `tap` is the default when the output is not a terminal.[^10]
- **Node.js.** The test runner [[node-test]] has a reporter `tap` that "outputs the test results in the TAP format", and the flag `--test-reporter=tap` selects it.[^11] node-tap 14 is a test framework and harness for Node.js.[^12]
- **Consumers.** tappy is a viewer in Python that reads TAP from standard input, and tapview is a viewer in one POSIX shell file. tap4j is a TAP library for Java, and it is part of the Jenkins TAP Plugin. The Go package tap13 parses TAP into structs.[^13]

For a test report that CI services read as a file, see [[junit-xml-format]].

## References

---
tag: BQ0VBY
kind: tool
slug: go-test
name: go test
aliases: [go test command, Go testing package]
area: testing/test-frameworks
level: core
summary: >-
  go test is the test command of Go, and testing is its package. This page shows test functions, subtests, helpers, examples, fuzz tests, benchmarks, coverage and the race detector.
role: [test-framework]
licence: bsd-3-clause
repo: https://github.com/golang/go
homepage: https://pkg.go.dev/testing
see_also: [test-framework, go, unit-testing, fuzz-testing, cargo-test, pytest, node-test]
sources:
  - url: https://pkg.go.dev/testing
    title: Go standard library, Package testing
    locator: Overview; Benchmarks; Examples; Fuzzing; Skipping; Subtests and Sub-benchmarks; Main; T.Error; T.Fail; T.FailNow; T.Fatal; T.Cleanup; T.Helper; T.TempDir
    accessed: 2026-11-15
  - url: https://pkg.go.dev/cmd/go
    title: Go command documentation, Test packages and Testing flags
    locator: Test packages; Testing flags
    accessed: 2026-11-15
  - url: https://go.dev/doc/faq
    title: Go, Frequently Asked Questions
    locator: Why does Go not have assertions?; Where is my favorite helper function for testing?
    accessed: 2026-11-15
  - url: https://github.com/golang/go/blob/master/LICENSE
    title: The Go Authors, LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://go.dev/blog/subtests
    title: Marcel van Lohuizen, Using Subtests and Sub-benchmarks, The Go Blog, 3 October 2016
    locator: Introduction; Table-driven tests basics
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.7
    title: Go 1.7 Release Notes
    locator: Testing
    accessed: 2026-11-15
  - url: https://go.dev/blog/examples
    title: Andrew Gerrand, Testable Examples in Go, The Go Blog, 7 May 2015
    locator: Introduction
    accessed: 2026-11-15
  - url: https://go.dev/doc/security/fuzz/
    title: Go Fuzzing
    locator: Running fuzz tests
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.18
    title: Go 1.18 Release Notes
    locator: Fuzzing
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.24
    title: Go 1.24 Release Notes
    locator: New benchmark function; testing
    accessed: 2026-11-15
  - url: https://go.dev/doc/build-cover
    title: Coverage profiling support for integration tests
    locator: Overview
    accessed: 2026-11-15
  - url: https://go.dev/doc/articles/race_detector
    title: Data Race Detector
    locator: Usage; How To Use; Requirements; Runtime Overhead
    accessed: 2026-11-15
  - url: https://pkg.go.dev/testing/synctest
    title: Go standard library, Package synctest
    locator: Overview; Test; Wait
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.25
    title: Go 1.25 Release Notes
    locator: New testing/synctest package; testing
    accessed: 2026-11-15
  - url: https://github.com/stretchr/testify
    title: testify, README
    locator: assert package; require package
    accessed: 2026-11-15
  - url: https://github.com/onsi/ginkgo
    title: Ginkgo, README
    locator: the introduction
    accessed: 2026-11-15
  - url: https://go.dev/doc/devel/release
    title: Go, Release History
    locator: go1; go1.7; go1.9; go1.14; go1.15; go1.17; go1.18; go1.20; go1.24.0; go1.25.0; go1.26.0; go1.27.0; go1.27.4
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1
    title: Go 1 Release Notes
    locator: The go command; The testing package
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.9
    title: Go 1.9 Release Notes
    locator: Test Helper Functions
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.14
    title: Go 1.14 Release Notes
    locator: testing
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.15
    title: Go 1.15 Release Notes
    locator: testing
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.17
    title: Go 1.17 Release Notes
    locator: testing
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.20
    title: Go 1.20 Release Notes
    locator: Go command
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.26
    title: Go 1.26 Release Notes
    locator: testing
    accessed: 2026-11-15
  - url: https://go.dev/doc/go1.27
    title: Go 1.27 Release Notes
    locator: go test; testing/synctest
    accessed: 2026-11-15
---

## What it is

`go test` is the subcommand of the `go` tool that runs the tests of [[go]] packages, and `testing` is the package of the standard library that the tests use.[^1] Both come with Go, so a Go project needs no test framework from outside.

A test is a function with the form `func TestXxx(*testing.T)` in a file whose name ends in `_test.go`.[^1] `go test` compiles the package together with these files, builds a test binary and runs it.[^2] The same files can hold benchmarks (`BenchmarkXxx`), fuzz tests (`FuzzXxx`) and examples (`ExampleXxx`).[^2]

The `testing` package has no assertion functions. A test compares values with ordinary Go code and reports a failure with methods such as `t.Errorf`. The Go FAQ gives the reason: testing frameworks "tend to develop into mini-languages of their own", and the Go team prefers to "write tests in Go".[^3]

Go is open source under a BSD-style licence with three clauses.[^4]

## Install and first use

Install Go, and `go test` is ready. The package below has two functions. The examples on this page use a module with the path `example`.

```go norun reason=fragment group=first file=cart.go
package shop

import "errors"

// ErrUnknownCode is the error of a discount code that the shop does not know.
var ErrUnknownCode = errors.New("unknown discount code")

// Item is a line of a cart: a price in cents and a quantity.
type Item struct {
	Name     string
	Cents    int
	Quantity int
}

// Total gives the sum of the lines of a cart in cents.
func Total(items []Item) int {
	sum := 0
	for _, it := range items {
		sum += it.Cents * it.Quantity
	}
	return sum
}

// Discount takes a percentage off a total for a known code.
func Discount(total int, code string) (int, error) {
	switch code {
	case "TEN":
		return total - total*10/100, nil
	case "HALF":
		return total / 2, nil
	}
	return total, ErrUnknownCode
}
```

The test file is in the same package, so it can also use names that are not exported.[^1] `TestDiscount` is a table-driven test: each row of the table is a subtest that `t.Run` starts.[^5]

```go test group=first file=cart_test.go id=first-use
package shop

import (
	"errors"
	"testing"
)

func TestTotal(t *testing.T) {
	items := []Item{{"pen", 150, 2}, {"pad", 300, 1}}
	if got := Total(items); got != 600 {
		t.Errorf("Total() = %d, want 600", got)
	}
}

func TestDiscount(t *testing.T) {
	tests := []struct {
		name string
		code string
		want int
		err  error
	}{
		{"ten percent", "TEN", 900, nil},
		{"half", "HALF", 500, nil},
		{"unknown code", "FREE", 1000, ErrUnknownCode},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := Discount(1000, tt.code)
			if !errors.Is(err, tt.err) {
				t.Fatalf("Discount(1000, %q) error = %v, want %v", tt.code, err, tt.err)
			}
			if got != tt.want {
				t.Errorf("Discount(1000, %q) = %d, want %d", tt.code, got, tt.want)
			}
		})
	}
}
```

This site runs the block with `go test -v -count=1`. The option `-v` logs each test as it runs, and `-count=1` turns off the cache of test results.[^2] A space in a subtest name becomes an underscore in the report.

```output
=== RUN   TestTotal
--- PASS: TestTotal ($TIME)
=== RUN   TestDiscount
=== RUN   TestDiscount/ten_percent
=== RUN   TestDiscount/half
=== RUN   TestDiscount/unknown_code
--- PASS: TestDiscount ($TIME)
    --- PASS: TestDiscount/ten_percent ($TIME)
    --- PASS: TestDiscount/half ($TIME)
    --- PASS: TestDiscount/unknown_code ($TIME)
PASS
ok  	example	$TIME
```

Without `-v`, in package list mode such as `go test ./...`, a package whose tests pass prints only one summary line, and a package with a failed test prints the full output of its tests.[^2]

## Concepts of the tool

### Failures: Error and Fatal

`t.Error` and `t.Errorf` log a message and mark the test as failed, and the test goes on. `t.Fatal` and `t.Fatalf` log a message and stop the test at once; the next test then runs.[^1] Use `Errorf` when the next checks still give useful information, and `Fatalf` when the rest of the test cannot run, for example after an error. The FAQ asks for good failure messages, because "the person debugging the failure" may not know the code.[^3]

The test below finds a bug: the function divides integers before it converts the result.

```go test fails expect=test-failure id=failure
package shop

import "testing"

// Average gives the mean of the scores. The bug: integer division.
func Average(scores []int) float64 {
	sum := 0
	for _, s := range scores {
		sum += s
	}
	return float64(sum / len(scores))
}

func TestAverage(t *testing.T) {
	if got := Average([]int{4, 4}); got != 4 {
		t.Errorf("Average([4 4]) = %v, want 4", got)
	}
	if got := Average([]int{2, 3}); got != 2.5 {
		t.Errorf("Average([2 3]) = %v, want 2.5", got)
	}
	if got := Average([]int{1, 2}); got != 1.5 {
		t.Errorf("Average([1 2]) = %v, want 1.5", got)
	}
}
```

Each failed check prints the file name, the line and the message. Because the test uses `Errorf`, it reports both failed checks, not only the first.

```output
=== RUN   TestAverage
    main_test.go:19: Average([2 3]) = 2, want 2.5
    main_test.go:22: Average([1 2]) = 1, want 1.5
--- FAIL: TestAverage ($TIME)
FAIL
FAIL	example	$TIME
FAIL
```

### Subtests and table-driven tests

`t.Run` starts a subtest with a name. The full name of a subtest is the name of the parent test and the names of the subtests, with slashes between them.[^1] The option `-run` takes a regular expression for each part of the name, so `go test -run 'TestDiscount/half'` runs one row of the table.[^1] Subtests came in Go 1.7, and they made table-driven tests and benchmarks easier to write.[^5][^6]

A parent test ends only when all of its subtests end. A subtest that calls `t.Parallel()` runs in parallel with the other parallel subtests, and `-parallel n` sets the largest number of tests that run at the same time.[^1][^2]

### Helpers, cleanup, temporary directories and skips

A helper function that calls `t.Helper()` is left out of the file and line information of a failure, so the report points to the line of the caller.[^1] `t.Cleanup(f)` registers a function that runs when the test and all its subtests end, and the cleanup functions run in the reverse order.[^1] `t.TempDir()` gives a new directory that is removed when the test ends.[^1] `t.Skip` stops a test and marks it as skipped.[^1]

```go test id=helpers
package shop

import (
	"os"
	"path/filepath"
	"testing"
)

// writeFile is a helper. A failure in it reports the line of the caller.
func writeFile(t *testing.T, dir, name, text string) string {
	t.Helper()
	path := filepath.Join(dir, name)
	if err := os.WriteFile(path, []byte(text), 0o644); err != nil {
		t.Fatalf("write %s: %v", name, err)
	}
	return path
}

func TestReadPrices(t *testing.T) {
	dir := t.TempDir() // removed when the test ends
	path := writeFile(t, dir, "prices.txt", "pen 150\npad 300\n")
	t.Cleanup(func() { t.Log("cleanup runs after the test") })
	data, err := os.ReadFile(path)
	if err != nil {
		t.Fatal(err)
	}
	t.Logf("read %d bytes", len(data))
}

func TestPayment(t *testing.T) {
	if os.Getenv("PAYMENT_URL") == "" {
		t.Skip("set PAYMENT_URL to run this test")
	}
}
```

`t.Log` writes to the log of the test. With `-v`, the log is printed also when the test passes.[^2]

```output
=== RUN   TestReadPrices
    main_test.go:27: read 16 bytes
    main_test.go:22: cleanup runs after the test
--- PASS: TestReadPrices ($TIME)
=== RUN   TestPayment
    main_test.go:32: set PAYMENT_URL to run this test
--- SKIP: TestPayment ($TIME)
PASS
ok  	example	$TIME
```

For setup and teardown around all the tests of a package, a test file can declare `func TestMain(m *testing.M)`. The test binary then calls `TestMain` instead of the tests, and `TestMain` calls `m.Run()`. The documentation calls it "a low-level primitive" that ordinary tests do not need.[^1]

### Examples

A function `ExampleXxx` is documentation and a test at the same time. If its body ends with a comment that starts with `Output:`, `go test` runs it and compares its standard output with the comment.[^1] An example with no output comment is compiled, but it does not run.[^1] Documentation sites such as pkg.go.dev show the examples next to the documentation of the function.[^7]

### Fuzz tests

A fuzz test has the form `func FuzzXxx(*testing.F)`. It adds seed inputs with `f.Add` and gives a fuzz target to `f.Fuzz`.[^1] A plain `go test` runs the fuzz target once with each seed input, as subtests. With `-fuzz`, the fuzzing engine makes new inputs from the seeds and runs until it finds a failure, or until the time of `-fuzztime` ends.[^1][^2] When it finds a failure, it writes the input to `testdata/fuzz/<Name>` in the package, and that file then is a seed input for later runs.[^1] Fuzzing came in Go 1.18.[^8][^9]

The block below has an example and a fuzz test. The fuzz target checks a property: to reverse a string two times gives the string back.

```go norun reason=fragment group=words file=words.go
package shop

// Reverse gives the runes of s in the reverse order.
func Reverse(s string) string {
	r := []rune(s)
	for i, j := 0, len(r)-1; i < j; i, j = i+1, j-1 {
		r[i], r[j] = r[j], r[i]
	}
	return string(r)
}
```

```go test group=words file=words_test.go id=examples-and-fuzz
package shop

import (
	"fmt"
	"testing"
	"unicode/utf8"
)

func ExampleReverse() {
	fmt.Println(Reverse("stressed"))
	// Output: desserts
}

func FuzzReverse(f *testing.F) {
	for _, seed := range []string{"hello", "Go", "", "añb"} {
		f.Add(seed)
	}
	f.Fuzz(func(t *testing.T, s string) {
		if !utf8.ValidString(s) {
			t.Skip("not valid UTF-8")
		}
		if got := Reverse(Reverse(s)); got != s {
			t.Errorf("Reverse(Reverse(%q)) = %q", s, got)
		}
	})
}
```

```output
=== RUN   FuzzReverse
=== RUN   FuzzReverse/seed#0
=== RUN   FuzzReverse/seed#1
=== RUN   FuzzReverse/seed#2
=== RUN   FuzzReverse/seed#3
--- PASS: FuzzReverse ($TIME)
    --- PASS: FuzzReverse/seed#0 ($TIME)
    --- PASS: FuzzReverse/seed#1 ($TIME)
    --- PASS: FuzzReverse/seed#2 ($TIME)
    --- PASS: FuzzReverse/seed#3 ($TIME)
=== RUN   ExampleReverse
--- PASS: ExampleReverse ($TIME)
PASS
ok  	example	$TIME
```

To fuzz, run `go test -fuzz=FuzzReverse -fuzztime=30s`. The `-fuzz` pattern must match exactly one fuzz test.[^2] Fuzzing needs a platform with coverage instrumentation, which is AMD64 and ARM64 at the time of writing, so that the corpus can grow.[^8] The page [[fuzz-testing]] explains the technique.

### Benchmarks

A benchmark has the form `func BenchmarkXxx(*testing.B)`, and `go test` runs it only with the option `-bench`.[^1] In the style of Go 1.24 and later, the loop `for b.Loop() { ... }` holds the code to measure, and the setup before the loop is not timed.[^1][^10]

```go norun reason=fragment group=first file=bench_test.go
package shop

import "testing"

func BenchmarkTotal(b *testing.B) {
	items := make([]Item, 1000) // the setup is not timed
	for i := range items {
		items[i] = Item{"pen", 150, i % 5}
	}
	for b.Loop() {
		Total(items)
	}
}
```

The times change from run to run, so this site does not run benchmarks. One run of `go test -run '^$' -bench . -benchmem` on a Linux computer with Go 1.26.5 printed the lines below. The line of the processor model is left out.

```text
goos: linux
goarch: amd64
pkg: example
BenchmarkTotal-8   	  825824	      4900 ns/op	       0 B/op	       0 allocs/op
PASS
ok  	example	4.118s
```

The columns give the number of times the loop ran and the time for each run of the loop.[^1] The option `-benchmem` adds the memory allocations.[^2] The tool `benchstat` of the module `golang.org/x/perf` compares the results of two runs with statistics.[^1]

Before `b.Loop`, a benchmark ran its loop `b.N` times, and the function itself ran several times while the testing package adjusted `b.N`.[^1] Old code still uses this style.

### Coverage

The option `-cover` turns on coverage analysis, and `-coverprofile=cover.out` writes a profile. The command `go tool cover -html=cover.out` shows the profile as HTML.[^2][^11] The flag of the block below adds `-cover` to the command of this site.

```go norun reason=fragment group=cover file=grade.go
package shop

// Grade gives a letter for a score from 0 to 100.
func Grade(score int) string {
	switch {
	case score >= 90:
		return "A"
	case score >= 75:
		return "B"
	case score >= 50:
		return "C"
	}
	return "F"
}
```

```go test group=cover file=grade_test.go flags="-cover" id=coverage
package shop

import "testing"

func TestGrade(t *testing.T) {
	if got := Grade(95); got != "A" {
		t.Errorf("Grade(95) = %q, want A", got)
	}
	if got := Grade(10); got != "F" {
		t.Errorf("Grade(10) = %q, want F", got)
	}
}
```

The test never reaches the cases `B` and `C`, so the coverage is less than 100 percent. The time in the report changes from run to run, so each line of the output below is a regular expression, and `.*` matches the time.

```output match=regex
=== RUN   TestGrade
--- PASS: TestGrade .*
PASS
coverage: 60\.0% of statements
ok\s+example\s+.*coverage: 60\.0% of statements
```

### The race detector

The option `-race` builds the tests with the data race detector. The detector finds a race only when the race happens while the program runs, so it cannot find races in code that the tests do not run.[^12] It needs cgo and, on systems other than macOS, a C compiler. The documentation gives a typical cost of 5 to 10 times the memory and 2 to 20 times the run time.[^12]

The test below has two goroutines that change a counter with no lock.

```go norun reason=fragment
func TestCounter(t *testing.T) {
	var c Counter // Add does c.n++ with no lock
	var wg sync.WaitGroup
	for range 2 {
		wg.Go(func() { c.Add() })
	}
	wg.Wait()
	if c.Value() != 2 {
		t.Errorf("Value() = %d, want 2", c.Value())
	}
}
```

The race report has addresses and goroutine numbers that change in each run, so this site does not run it. A run of `go test -race` with Go 1.26.5 on Linux printed the report below. The stacks of the goroutines are left out, and the project directory is written as `/work/shop`.

```text
==================
WARNING: DATA RACE
Read at 0x00c00011a028 by goroutine 9:
  example.(*Counter).Add()
      /work/shop/counter.go:8 +0x30
  example.TestCounter.func1()
      /work/shop/counter_test.go:12 +0xe

Previous write at 0x00c00011a028 by goroutine 10:
  example.(*Counter).Add()
      /work/shop/counter.go:8 +0x44
  example.TestCounter.func1()
      /work/shop/counter_test.go:12 +0xe
==================
--- FAIL: TestCounter (0.02s)
    testing.go:1712: race detected during execution of test
FAIL
FAIL	example	0.181s
FAIL
```

The test fails because of the race, although its own check passed in this run.

### Tests of concurrent code with synctest

The package `testing/synctest` runs a function in a bubble with a fake clock. In the bubble, the clock moves forward at once when all goroutines of the bubble are blocked, so a test of a timeout of one hour takes no real time.[^13][^14]

```go test id=synctest
package shop

import (
	"errors"
	"testing"
	"testing/synctest"
	"time"
)

var errTimeout = errors.New("timeout")

// waitForPayment waits for the result of a payment, or gives up after the timeout.
func waitForPayment(done <-chan bool, timeout time.Duration) (bool, error) {
	select {
	case ok := <-done:
		return ok, nil
	case <-time.After(timeout):
		return false, errTimeout
	}
}

func TestPaymentTimeout(t *testing.T) {
	synctest.Test(t, func(t *testing.T) {
		start := time.Now()
		done := make(chan bool) // no payment result comes
		if _, err := waitForPayment(done, time.Hour); err != errTimeout {
			t.Fatalf("err = %v, want timeout", err)
		}
		t.Logf("gave up after %v of fake time", time.Since(start))
	})
}
```

```output
=== RUN   TestPaymentTimeout
    main_test.go:29: gave up after 1h0m0s of fake time
--- PASS: TestPaymentTimeout ($TIME)
PASS
ok  	example	$TIME
```

## Common tasks

```text
go test                            # test the package in the current directory
go test ./...                      # test all packages of the module
go test -v -run 'TestDiscount/half' # run one subtest, with the log
go test -skip TestPayment          # run all tests except the ones that match
go test -count=1 ./...             # do not use the cache of test results
go test -short                     # testing.Short() gives true
go test -shuffle=on                # run the tests in a random order
go test -failfast                  # start no new tests after the first failure
go test -json                      # the -v output as JSON
go test -fuzz=FuzzReverse -fuzztime=30s
go test -run '^$' -bench . -benchmem
go test -race ./...
go test -coverprofile=cover.out && go tool cover -html=cover.out
```

In package list mode, such as `go test ./...`, `go test` keeps the results of packages that passed in a cache. It prints `(cached)` when it uses an old result, and `-count=1` is the usual way to turn the cache off.[^2] Before it runs the tests, `go test` runs a part of the `go vet` checks, and a problem that `go vet` finds stops the run. The option `-vet=off` turns this off.[^2]

## Configuration

`go test` has no configuration file, and its options are on the command line.[^2] Tests that need files keep them in a directory named `testdata`, which the `go` tool ignores as a package.[^2]

A test file can be in the same package as the code, or in a package with the suffix `_test`. A test in a `_test` package sees only the exported names, which the documentation calls "black box" testing.[^1]

The default time limit of a test binary is 10 minutes; `-timeout` changes it, and `-timeout 0` turns it off.[^2]

## Alternatives

- testify: a module with the packages `assert` and `require`, which give assertion functions with readable failure messages, and the packages `mock` and `suite`. A `require` function stops the test when it fails.[^15]
- Ginkgo with Gomega: a framework for Go in the style of behaviour-driven development, with the matcher library Gomega. It builds on the `testing` package of Go.[^16]
- [[cargo-test]], [[pytest]] and [[node-test]] are the test tools of other languages that this site describes.

## History and versions

| Version | Change |
|---|---|
| Go 1, 28 March 2012 | the `go` command is new[^17][^18] |
| 1.7, 15 August 2016 | subtests and sub-benchmarks with `Run`[^17][^6] |
| 1.9, 24 August 2017 | `t.Helper()`[^17][^19] |
| 1.14, 25 February 2020 | `t.Cleanup()`[^17][^20] |
| 1.15, 11 August 2020 | `t.TempDir()`[^17][^21] |
| 1.17, 16 August 2021 | the option `-shuffle`, and `t.Setenv()`[^17][^22] |
| 1.18, 15 March 2022 | fuzz tests[^17][^9] |
| 1.20, 1 February 2023 | the option `-skip`, and coverage profiles of whole programs with `go build -cover`[^17][^23][^11] |
| 1.24, 11 February 2025 | `b.Loop()`, `t.Context()`, `t.Chdir()`, and `testing/synctest` as an experiment[^17][^10] |
| 1.25, 12 August 2025 | `testing/synctest` is no longer an experiment; `t.Attr()`[^17][^14] |
| 1.26, 10 February 2026 | `t.ArtifactDir()` and the option `-artifacts`[^17][^24] |
| 1.27, 19 August 2026 | `go test` runs the vet check `stdversion`; `synctest.Sleep`[^17][^25] |

The latest version on 12 November 2026 was Go 1.27.4, from 10 November 2026.[^17] The outputs of the runnable blocks on this page come from the Go version of the site.

## References

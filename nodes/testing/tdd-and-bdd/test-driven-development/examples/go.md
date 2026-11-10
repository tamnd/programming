```go norun reason=fragment group=leap id=leap file=leap.go
package leap

// IsLeap reports whether year is a leap year in the Gregorian calendar.
func IsLeap(year int) bool {
	return year%4 == 0 && (year%100 != 0 || year%400 == 0)
}
```

```go test group=leap id=leap-test file=leap_test.go
package leap

import "testing"

func TestIsLeap(t *testing.T) {
	// The tests in the order of the to-do list. Each one added one row.
	cases := []struct {
		name string
		year int
		want bool
	}{
		{"divisible by 4", 2024, true},
		{"not divisible by 4", 2023, false},
		{"century", 1900, false},
		{"century divisible by 400", 2000, true},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got := IsLeap(c.year); got != c.want {
				t.Errorf("IsLeap(%d) = %v, want %v", c.year, got, c.want)
			}
		})
	}
}
```

```output
=== RUN   TestIsLeap
=== RUN   TestIsLeap/divisible_by_4
=== RUN   TestIsLeap/not_divisible_by_4
=== RUN   TestIsLeap/century
=== RUN   TestIsLeap/century_divisible_by_400
--- PASS: TestIsLeap ($TIME)
    --- PASS: TestIsLeap/divisible_by_4 ($TIME)
    --- PASS: TestIsLeap/not_divisible_by_4 ($TIME)
    --- PASS: TestIsLeap/century ($TIME)
    --- PASS: TestIsLeap/century_divisible_by_400 ($TIME)
PASS
ok  	example	$TIME
```

- Verdict: stdlib. The package `testing` and the command `go test` are part of the Go distribution. `go test` builds the files that end in `_test.go` together with the package and runs each `TestXxx` function.
- The table of cases is the to-do list in code. Each cycle adds one row, sees it fail, and then changes `IsLeap`.
- `t.Errorf` marks the subtest as failed and goes on, so one run reports every row that fails. `t.Fatalf` stops the subtest at the first failure.

The first red in Go is a build error. The test cannot compile, because `IsLeap` does not exist yet.

```go fails expect=compile id=red
package main

import "fmt"

func main() {
	// Red: the test comes first, so isLeap does not exist yet.
	if !isLeap(2024) {
		fmt.Println("FAIL 2024")
	}
}
```

- The compiler reports `undefined: isLeap`. In a test file, `go test` reports the same error and prints `FAIL` with the reason "build failed".
- Go also rejects an unused variable or import. A test written ahead of the code can stop at such an error first, so read the message before you treat the red as the expected one.

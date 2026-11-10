```go norun reason=fragment group=argmax id=argmax file=argmax.go
package argmax

// Mutant picks the active change: "" runs the original code.
var Mutant string

// Argmax returns the index of the first largest item of a non-empty slice.
func Argmax(xs []int) int {
	best := 0
	start := 1
	if Mutant == "start at 0" {
		start = 0
	}
	for i := start; i < len(xs); i++ {
		left := xs[i]
		if Mutant == "xs[i] to xs[best]" {
			left = xs[best]
		}
		var bigger bool
		switch Mutant {
		case "> to >=":
			bigger = left >= xs[best]
		case "> to <":
			bigger = left < xs[best]
		default:
			bigger = left > xs[best]
		}
		if bigger {
			best = i
		}
	}
	return best
}
```

```go test group=argmax id=argmax-test file=argmax_test.go
package argmax

import "testing"

var cases = []struct {
	xs   []int
	want int
}{
	{[]int{1, 2, 3}, 2},
	{[]int{1, 3, 2}, 1},
	{[]int{3, 1, 2}, 0},
	{[]int{2, 2, 1}, 0}, // the tie; without it "> to >=" lives
}

func passes() bool {
	for _, c := range cases {
		if Argmax(c.xs) != c.want {
			return false
		}
	}
	return true
}

func TestMutants(t *testing.T) {
	if !passes() {
		t.Fatal("the original must pass first")
	}
	// A person checked this mutant and found no input that tells it apart.
	equivalent := map[string]bool{"start at 0": true}
	for _, m := range []string{"> to >=", "> to <", "xs[i] to xs[best]", "start at 0"} {
		t.Run(m, func(t *testing.T) {
			Mutant = m
			defer func() { Mutant = "" }()
			switch {
			case !passes():
				t.Log("killed")
			case equivalent[m]:
				t.Log("lived, marked equivalent")
			default:
				t.Error("lived: add a test that kills it")
			}
		})
	}
}
```

```output
=== RUN   TestMutants
=== RUN   TestMutants/>_to_>=
    argmax_test.go:36: killed
=== RUN   TestMutants/>_to_<
    argmax_test.go:36: killed
=== RUN   TestMutants/xs[i]_to_xs[best]
    argmax_test.go:36: killed
=== RUN   TestMutants/start_at_0
    argmax_test.go:38: lived, marked equivalent
--- PASS: TestMutants ($TIME)
    --- PASS: TestMutants/>_to_>= ($TIME)
    --- PASS: TestMutants/>_to_< ($TIME)
    --- PASS: TestMutants/xs[i]_to_xs[best] ($TIME)
    --- PASS: TestMutants/start_at_0 ($TIME)
PASS
ok  	example	$TIME
```

- Verdict: library. `go test` has coverage and fuzzing but no mutation testing. Gremlins is a mutation testing tool for Go.
- The test turns the score into a gate. A mutant that lives and is not on the list of equivalent mutants fails the test with `t.Error`. Delete the tie from `cases` and the subtest `>_to_>=` fails.
- `t.Run` names each subtest after the mutant, and Go replaces the spaces in the name with `_`. The `defer` resets the package variable, so the next test sees the original code.
- The map `equivalent` records a judgment by a person. No tool can decide equivalence in general, so the list needs a review when `Argmax` changes.

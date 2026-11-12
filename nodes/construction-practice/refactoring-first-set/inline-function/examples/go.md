```go run id=put-the-body-back
package main

import "fmt"

type Driver struct{ LateDeliveries int }

// Before: a helper whose name says the same thing as its body.
func moreThanFiveLateDeliveries(d Driver) bool { return d.LateDeliveries > 5 }

func ratingBefore(d Driver) int {
	if moreThanFiveLateDeliveries(d) {
		return 2
	}
	return 1
}

var reads int

func nextReading() int { reads++; return 7 }

func squareBefore(x int) int { return x * x }

func scoreBefore() int { return squareBefore(nextReading()) }

// After: the body at each call. This is the form that gopls produces for an
// argument with a side effect: the parameter becomes a local variable.
func rating(d Driver) int {
	if d.LateDeliveries > 5 {
		return 2
	}
	return 1
}

func score() int {
	var x int = nextReading()
	return x * x
}

func main() {
	for _, late := range []int{0, 5, 6, 12} {
		if rating(Driver{late}) != ratingBefore(Driver{late}) {
			panic("rating")
		}
	}
	reads = 0
	if scoreBefore() != 49 || reads != 1 {
		panic("before")
	}
	reads = 0
	if score() != 49 || reads != 1 {
		panic("after")
	}
	fmt.Println("inline: tests pass")
}
```

```output
inline: tests pass
```

The gopls documentation describes this step: "The inliner will introduce a declaration such as var x int = g() when it cannot prove that it is safe to substitute the argument throughout." Here the argument calls a function that changes `reads`, so the declaration keeps one call.

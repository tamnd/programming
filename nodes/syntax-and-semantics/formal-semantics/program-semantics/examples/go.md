```go run id=loop-invariant
package main

import "fmt"

// Integer division by repeated subtraction, with its Hoare-logic annotations
// checked at run time.
func divide(n, d int) (q, r int) {
	// precondition: n >= 0 && d > 0
	q, r = 0, n
	for r >= d {
		invariant(n, d, q, r)
		q, r = q+1, r-d
	}
	invariant(n, d, q, r)
	// postcondition: the invariant holds and r < d
	if r >= d {
		panic("postcondition fails")
	}
	return q, r
}

func invariant(n, d, q, r int) {
	ok := q*d+r == n && r >= 0
	fmt.Printf("  q=%d r=%-2d  q*d+r == n: %v\n", q, r, ok)
	if !ok {
		panic("invariant fails")
	}
}

func main() {
	q, r := divide(17, 5)
	fmt.Println("17 / 5 =", q, "remainder", r)
}
```

```output for=loop-invariant
  q=0 r=17  q*d+r == n: true
  q=1 r=12  q*d+r == n: true
  q=2 r=7   q*d+r == n: true
  q=3 r=2   q*d+r == n: true
17 / 5 = 3 remainder 2
```

- Verdict: absent. The Go specification is a short document in English, with a separate memory model for concurrent programs. The example writes the annotations of Hoare logic for division by repeated subtraction as checks: the invariant `q*d+r == n && r >= 0` holds before the loop and after each pass, and when the loop exits, `r < d` also holds. Together they prove that `q` and `r` are the quotient and the remainder. Tools such as Gobra check such annotations before the program runs.

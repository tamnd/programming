```go run id=three-part-range-int-and-per-pass-variables
package main

import "fmt"

func main() {
	// The three-part form with an init, a condition and a post statement.
	for i := 0; i < 3; i++ {
		fmt.Println("i =", i)
	}

	// range over an integer counts from 0 to n-1.
	sum := 0
	for i := range 5 {
		sum += i
	}
	fmt.Println("sum of range 5:", sum)

	// Each pass has its own i, so each closure keeps its own value.
	var fs []func() int
	for i := 0; i < 3; i++ {
		fs = append(fs, func() int { return i })
	}
	for _, f := range fs {
		fmt.Print(f())
	}
	fmt.Println()

	// Two variables step together with a parallel assignment.
	for i, j := 0, 4; i < j; i, j = i+1, j-1 {
		fmt.Println(i, j)
	}
}
```

```output for=three-part-range-int-and-per-pass-variables
i = 0
i = 1
i = 2
sum of range 5: 10
012
0 4
1 3
```

- Verdict: native. Go has the three-part `for` with no parentheses and `range` over an integer. The module of the examples declares a Go version after 1.22, so each pass has its own `i` and the closures return 0, 1 and 2. Go has no comma operator, so two counters step with a parallel assignment.

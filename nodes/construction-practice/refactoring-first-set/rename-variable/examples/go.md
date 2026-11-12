```go run id=rename-the-name
package main

import "fmt"

// Before: the inner variable is y. The reference to x reaches the parameter.
func before(x int) int {
	total := 0
	for y := 0; y < 3; y++ {
		total += x * y
	}
	return total
}

// A rename of y to x by text: the inner x shadows the parameter,
// and x * x is not the product that the code meant.
func renamedByText(x int) int {
	total := 0
	for x := 0; x < 3; x++ {
		total += x * x
	}
	return total
}

// A rename by scope to a name that is free: the behaviour stays.
func renamedByScope(x int) int {
	total := 0
	for step := 0; step < 3; step++ {
		total += x * step
	}
	return total
}

func main() {
	fmt.Println(before(10), renamedByText(10), renamedByScope(10))
	if before(10) != renamedByScope(10) || before(10) == renamedByText(10) {
		panic("unexpected results")
	}
	fmt.Println("rename variable: tests pass")
}
```

```output
30 5 30
rename variable: tests pass
```

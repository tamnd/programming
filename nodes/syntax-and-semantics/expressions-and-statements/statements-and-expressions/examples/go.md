```go run id=statements-only
package main

import "fmt"

func label(score int) string {
	switch { // switch and if are statements in Go
	case score >= 90:
		return "A"
	case score >= 50:
		return "pass"
	default:
		return "fail"
	}
}

func main() {
	fmt.Println(label(95), label(70), label(30))

	x := 5
	x++ // a statement, not an expression
	if y := x * 2; y > 10 { // a simple statement before the condition
		fmt.Println("y is", y)
	}
	fmt.Println(x)
}
```

```output for=statements-only
A pass fail
y is 12
6
```

```go fails expect=compile id=increment-has-no-value
package main

import "fmt"

func main() {
	x := 5
	y := x++ // x++ is a statement, so it has no value
	fmt.Println(x, y)
}
```

- Verdict: native. Go keeps expressions small. Assignment, `++` and `--` are statements, there is no conditional operator, and `if` and `switch` are statements, so a choice between two values needs an `if` that assigns or a function that returns. An `if` or `switch` can start with a short statement, such as `y := x * 2`, whose names are visible only inside it.
- The fails block uses `x++` as a value. In Go it is a statement with no value, so the compiler rejects `y := x++`, which removes the C puzzle of the order of `x++` inside larger expressions.

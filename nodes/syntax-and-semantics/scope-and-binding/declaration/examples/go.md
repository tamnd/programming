```go run id=package-declarations-and-iota
package main

import "fmt"

// Package-level declarations can be in any order, so main can use limit.
func main() {
	fmt.Println("limit =", limit, "double(limit) =", double(limit))

	var z int // a var declaration without a value gives the zero value
	fmt.Println("z =", z)

	n := 3 // a short variable declaration
	fmt.Println("n =", n)

	const (
		Low = iota // iota counts the specs in a const declaration
		Mid
		High
	)
	fmt.Println("Low, Mid, High =", Low, Mid, High)

	var c Celsius = 20
	var a Alias = 7
	fmt.Printf("%T %T\n", c, a)
}

func double(x int) int { return 2 * x }

const limit = 10

type Celsius float64 // a type definition: a new type

type Alias = int // an alias declaration: another name for int
```

```output for=package-declarations-and-iota
limit = 10 double(limit) = 20
z = 0
n = 3
Low, Mid, High = 0 1 2
main.Celsius int
```

- Verdict: native. Top-level declarations can come in any order, so `main` uses `limit` and `double` before they appear. A `var` without a value gives the zero value. `iota` numbers the specs of a `const` declaration. A type definition makes the new type `main.Celsius`, and an alias declaration makes another name for `int`.

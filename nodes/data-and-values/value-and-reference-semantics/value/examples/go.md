```go run id=zero-values-and-copies
package main

import "fmt"

type Point struct{ X, Y int }

func main() {
	// A variable with no initializer holds the zero value of its type.
	var i int
	var s string
	var p *Point
	var m map[string]int
	fmt.Printf("%d %q %v %v\n", i, s, p == nil, m == nil)

	// Assignment of a struct copies the value.
	a := Point{1, 2}
	b := a
	b.X = 10
	fmt.Println(a, b, a == Point{1, 2})

	// A slice value refers to an underlying array, so a copy of the slice shares it.
	xs := []int{1, 2, 3}
	ys := xs
	ys[0] = 9
	fmt.Println(xs, ys)

	// A pointer value refers to a variable.
	q := &a
	q.Y = 20
	fmt.Println(a)

	// Functions are values.
	add := func(x, y int) int { return x + y }
	apply := func(f func(int, int) int) int { return f(3, 4) }
	fmt.Println(apply(add))
}
```

```output for=zero-values-and-copies
0 "" true true
{1 2} {10 2} true
[9 2 3] [9 2 3]
{1 20}
7
```

- Verdict: native. A variable without an initializer holds the zero value of its type. Assignment copies a struct, and `==` compares struct values. A slice value refers to an array, so a copy of the slice shares the elements. A pointer value refers to a variable, and a change through it shows in that variable. Functions are values that can be passed as arguments.

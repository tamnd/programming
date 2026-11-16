```go run id=struct-slice-map
package main

import (
	"fmt"
	"slices"
)

type Point struct {
	X, Y int
}

func main() {
	// Arrays and structs are values: assignment copies them, and == compares the parts.
	a := [3]int{1, 2, 3}
	b := a
	b[0] = 9
	fmt.Println(a, b, a == [3]int{1, 2, 3})

	p := Point{1, 2}
	q := p
	q.X = 5
	fmt.Println(p, q, p == Point{1, 2})

	// A slice refers to an array that its copies share.
	s := []int{1, 2, 3}
	t := s
	t[0] = 9
	fmt.Println(s, slices.Equal(s, t))

	m := map[string][]int{"a": {1}}
	m["a"] = append(m["a"], 2)
	fmt.Println(m)

	// A type literal builds a composite type without a name.
	pairs := []struct {
		K string
		V int
	}{{"x", 1}, {"y", 2}}
	fmt.Printf("%v %T\n", pairs[1], pairs[1])
}
```

```output for=struct-slice-map
[1 2 3] [9 2 3] true
{1 2} {5 2} true
[9 2 3] true
map[a:[1 2]]
{y 2} struct { K string; V int }
```

- Verdict: native. An array and a struct are values, so `b` and `q` are copies, and `==` compares them part by part. A slice is a view of an array, so `t` and `s` share the elements. Go has no `==` for slices, so the example uses `slices.Equal`. `[]struct{ K string; V int }` is a slice of an anonymous struct type that a type literal builds.

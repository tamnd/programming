```go run id=multiple-values
package main

import "fmt"

type point struct{ x, y int }

func divmod(a, b int) (int, int) {
	return a / b, a % b
}

func main() {
	q, r := divmod(17, 5)
	fmt.Println(q, r)

	_, r2 := divmod(20, 6)
	fmt.Println(r2)

	a, b := 1, 2
	a, b = b, a
	fmt.Println(a, b)

	p := point{3, 4}
	x, y := p.x, p.y
	fmt.Println(x, y)

	m := map[string]int{"a": 1}
	v, ok := m["b"]
	fmt.Println(v, ok)
}
```

```output for=multiple-values
3 2
2
2 1
3 4
0 false
```

- Verdict: idiom. Go assigns several values at once: the results of a call, or a list of expressions such as `a, b = b, a`. The blank identifier `_` drops a value. Go has no patterns for structs or slices, so each field is read by name.

```go run id=zero-value
package main

import "fmt"

type Point struct {
	X, Y int
}

func main() {
	var i int
	var s string
	var p *int
	var m map[string]int
	var xs []int
	fmt.Printf("%d %q %v %v %d\n", i, s, p == nil, m == nil, len(xs))

	var pt Point
	fmt.Println(pt, m["missing"])

	n, name := 3, "Ada"
	fmt.Println(n, name)
}
```

```output for=zero-value
0 "" true true 0
{0 0} 0
3 Ada
```

- Verdict: native. Each variable without an initializer gets the zero value of its type, and a struct gets the zero value in each field. A nil map can be read, and a read of a missing key gives the zero value, but a write to a nil map panics. `:=` declares and initializes in one step.

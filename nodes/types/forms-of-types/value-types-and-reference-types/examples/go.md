```go run id=copy-and-share
package main

import "fmt"

type Point struct{ X, Y int }

func main() {
	arr := [3]int{1, 2, 3}
	arr2 := arr // copies the whole array
	arr2[0] = 9
	fmt.Println(arr, arr2)

	p := Point{1, 2}
	q := p // copies the struct
	q.X = 9
	fmt.Println(p, q)

	s := []int{1, 2, 3}
	s2 := s // copies the slice header, which points to the same array
	s2[0] = 9
	fmt.Println(s, s2)

	m := map[string]int{"a": 1}
	m2 := m
	m2["a"] = 9
	fmt.Println(m["a"])

	ptr := &p // a pointer shares explicitly
	ptr.X = 7
	fmt.Println(p)
}
```

```output for=copy-and-share
[1 2 3] [9 2 3]
{1 2} {9 2}
[9 2 3] [9 2 3]
9
{7 2}
```

- Verdict: idiom. Every Go type is a value type, so assigning an array or a struct copies it. A slice and a map are small descriptors that hold a pointer to their data, so a copy of one shares the data. A pointer makes sharing explicit.

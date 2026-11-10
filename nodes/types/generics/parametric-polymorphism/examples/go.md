```go run id=max
package main

import (
	"cmp"
	"fmt"
)

func Max[T cmp.Ordered](items []T) T {
	result := items[0]
	for _, item := range items {
		if item > result {
			result = item
		}
	}
	return result
}

type Stack[T any] struct {
	items []T
}

func (s *Stack[T]) Push(item T) {
	s.items = append(s.items, item)
}

func main() {
	fmt.Println(Max([]int{3, 9, 4}))
	fmt.Println(Max([]string{"pear", "apple", "plum"}))
	var s Stack[float64]
	s.Push(2.5)
	s.Push(4)
	fmt.Println(s.items)
}
```

```output for=max
9
plum
[2.5 4]
```

- Verdict: native. Functions and types declare type parameters in square brackets.
- A constraint is an interface. `cmp.Ordered` allows the types that support `<` and `>`, and `any` allows every type. The predeclared `comparable` allows `==` and `!=`.
- A method can declare type parameters of its own, in addition to those of its receiver type. A generic method does not implement a method of an interface (see Pitfalls on the concept page).
- The compiler shares one copy of the code among type arguments with the same GC shape and passes a dictionary for the differences.

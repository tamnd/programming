```go run id=generic-type
package main

import "fmt"

// Stack is a generic type: Stack[int] and Stack[string] are types.
type Stack[T any] struct {
	items []T
}

func (s *Stack[T]) Push(x T) { s.items = append(s.items, x) }

func (s *Stack[T]) Pop() T {
	x := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	return x
}

func main() {
	var s Stack[string]
	s.Push("a")
	s.Push("b")
	fmt.Println(s.Pop(), len(s.items))

	// Built-in constructors: slice, map and pointer types.
	m := map[string][]int{"a": {1, 2}}
	p := &m
	fmt.Printf("%T %T %v\n", m, p, (*p)["a"])
	fmt.Printf("%T\n", s)
}
```

```output for=generic-type
b 1
map[string][]int *map[string][]int [1 2]
main.Stack[string]
```

- Verdict: native. `Stack` has a type parameter, so `Stack[string]` is a type made from it. A generic type in Go must have its type arguments; `Stack` alone is not a type. The slice, map and pointer types are built-in constructors with special syntax: `map[string][]int` applies the map constructor to `string` and to `[]int`.

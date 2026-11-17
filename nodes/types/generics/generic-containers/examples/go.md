```go run id=typed-containers
package main

import (
	"container/list"
	"fmt"
)

// Stack is a generic type: Stack[int] holds only ints.
type Stack[T any] struct{ items []T }

func (s *Stack[T]) Push(v T) { s.items = append(s.items, v) }

func main() {
	scores := []int{90, 85}
	ages := map[string]int{"ann": 31}
	var s Stack[string]
	s.Push("a")
	fmt.Println(len(scores), ages["ann"], s.items)

	// container/list stores items of type any.
	l := list.New()
	l.PushBack(1)
	l.PushBack("two")
	for e := l.Front(); e != nil; e = e.Next() {
		if n, ok := e.Value.(int); ok {
			fmt.Println("int", n)
		} else {
			fmt.Println("other", e.Value)
		}
	}
}
```

```output for=typed-containers
2 31 [a]
int 1
other two
```

- Verdict: native. Slices and maps, such as `[]int` and `map[string]int`, are built-in generic containers, and a program can declare its own generic types. `container/list` is older: its items have the type `any`, so the code uses a type assertion to read them.

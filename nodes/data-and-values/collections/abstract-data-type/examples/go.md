```go run id=stack-adt
package main

import (
	"fmt"
	"strconv"
	"strings"
)

// Stack is the abstract type: only the operations.
type Stack interface {
	Push(v int)
	Pop() int
	IsEmpty() bool
}

// The field names start with a lower-case letter, so code in
// other packages cannot use them.
type ArrayStack struct{ items []int }

func (s *ArrayStack) Push(v int) { s.items = append(s.items, v) }
func (s *ArrayStack) Pop() int {
	v := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	return v
}
func (s *ArrayStack) IsEmpty() bool { return len(s.items) == 0 }

type cell struct {
	v    int
	rest *cell
}

type LinkedStack struct{ top *cell }

func (s *LinkedStack) Push(v int) { s.top = &cell{v, s.top} }
func (s *LinkedStack) Pop() int {
	v := s.top.v
	s.top = s.top.rest
	return v
}
func (s *LinkedStack) IsEmpty() bool { return s.top == nil }

func drain(s Stack) string {
	for v := 1; v <= 3; v++ {
		s.Push(v)
	}
	var out []string
	for !s.IsEmpty() {
		out = append(out, strconv.Itoa(s.Pop()))
	}
	return strings.Join(out, " ")
}

func main() {
	fmt.Println("ArrayStack:", drain(&ArrayStack{}))
	fmt.Println("LinkedStack:", drain(&LinkedStack{}))
}
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
```

- Verdict: native. An interface names the operations, and a type satisfies it by having the methods. Privacy works at the level of the package: a name that starts with a lower-case letter is not exported.
- This example is one `main` package, so the hiding does not show here. In a library, `items` and `top` would be invisible to the code that imports it.

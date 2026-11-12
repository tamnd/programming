```go run id=superclass-to-delegate
package main

import "fmt"

// Stack keeps its slice unexported, so code in other packages can only use the stack methods.
type Stack struct{ storage []int }

func (s *Stack) Push(x int) { s.storage = append(s.storage, x) }

func (s *Stack) Pop() int {
	x := s.storage[len(s.storage)-1]
	s.storage = s.storage[:len(s.storage)-1]
	return x
}

func (s *Stack) Peek() int { return s.storage[len(s.storage)-1] }
func (s *Stack) Len() int  { return len(s.storage) }

func main() {
	var s Stack
	for _, x := range []int{1, 2, 3} {
		s.Push(x)
	}
	if s.Pop() != 3 {
		panic("mismatch")
	}
	fmt.Println(s.Peek(), s.Len())
}
```

```output
2 2
```

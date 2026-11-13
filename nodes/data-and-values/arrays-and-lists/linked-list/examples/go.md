```go run id=container-list
package main

import (
	"container/list"
	"fmt"
)

func values(l *list.List) []any {
	var out []any
	for e := l.Front(); e != nil; e = e.Next() {
		out = append(out, e.Value)
	}
	return out
}

func main() {
	// container/list is a doubly linked list. Each *Element is a node.
	l := list.New()
	a := l.PushBack("a")
	b := l.PushBack("b")
	l.PushBack("c")
	fmt.Println(values(l), l.Len())

	// Moving a node changes only links. A cache moves each used key to the front this way.
	l.MoveToFront(b)
	fmt.Println(values(l))
	l.InsertAfter("a2", a)
	fmt.Println(values(l))

	// To remove while walking, save Next before Remove, because Remove clears the links.
	for e := l.Front(); e != nil; {
		next := e.Next()
		if s := e.Value.(string); len(s) == 2 || s == "c" {
			l.Remove(e)
		}
		e = next
	}
	fmt.Println(values(l), l.Len())

	// Without the saved Next, the loop stops after the first Remove.
	l2 := list.New()
	for _, s := range []string{"x", "y", "z"} {
		l2.PushBack(s)
	}
	n := 0
	for e := l2.Front(); e != nil; e = e.Next() {
		l2.Remove(e)
		n++
	}
	fmt.Println(n, l2.Len())
}
```

```output
[a b c] 3
[b a c]
[b a a2 c]
[b a] 2
1 2
```

- Verdict: stdlib. container/list is a doubly linked list. MoveToFront and InsertAfter change only links. A loop that saves Next before Remove removes every matching element. A loop that calls Next after Remove stops after one element, because Remove clears the links, so 2 of 3 items stay.

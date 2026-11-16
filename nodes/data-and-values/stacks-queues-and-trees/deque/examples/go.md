```go run id=deque
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
	d := list.New()
	d.PushBack("b")
	d.PushBack("c")
	d.PushFront("a")
	d.PushBack("d")
	fmt.Println(values(d), "front", d.Front().Value, "back", d.Back().Value)

	front := d.Remove(d.Front())
	back := d.Remove(d.Back())
	fmt.Println(front, back, values(d))

	// Front and Back return nil on an empty list.
	d.Init()
	fmt.Println("front of empty:", d.Front())
}
```

```output for=deque
[a b c d] front a back d
a d [b c]
front of empty: <nil>
```

- Verdict: stdlib. `container/list` is a doubly linked list, and it works as a deque with `PushFront`, `PushBack`, `Front`, `Back` and `Remove`.
- The values have the type `any`, so a program needs a type assertion to use them.

```go run id=slices-and-container-list
package main

import (
	"container/list"
	"fmt"
	"slices"
)

func main() {
	// Go code uses a slice as its list type.
	xs := []string{"b", "a", "b"}
	xs = append(xs, "c")
	xs = slices.Insert(xs, 1, "z")
	fmt.Println(xs, len(xs), slices.Index(xs, "b"), slices.Contains(xs, "q"))

	// container/list is a doubly linked list of elements with any value.
	l := list.New()
	l.PushBack(1)
	l.PushBack(2)
	l.PushFront(0)
	mid := l.Front().Next()
	l.InsertAfter(15, mid)
	var out []any
	for e := l.Front(); e != nil; e = e.Next() {
		out = append(out, e.Value)
	}
	fmt.Println(out, l.Len())

	// slices.Equal compares two slices item by item.
	fmt.Println(slices.Equal([]int{1, 2}, []int{1, 2}))
}
```

```output
[b z a b c] 5 0 false
[0 1 15 2] 4
true
```

- Verdict: native. A slice is the usual list, and the slices package gives Insert, Index and Contains. container/list is a doubly linked list with PushBack, PushFront and InsertAfter. slices.Equal compares two slices item by item.

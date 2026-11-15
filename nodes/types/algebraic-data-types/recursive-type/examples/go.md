```go run id=list
package main

import (
	"fmt"
	"strings"
)

type Node struct {
	Value int
	Next  *Node
}

func total(n *Node) int {
	if n == nil {
		return 0
	}
	return n.Value + total(n.Next)
}

func length(n *Node) int {
	if n == nil {
		return 0
	}
	return 1 + length(n.Next)
}

func show(n *Node) string {
	var parts []string
	for ; n != nil; n = n.Next {
		parts = append(parts, fmt.Sprint(n.Value))
	}
	return strings.Join(parts, " -> ")
}

func main() {
	xs := &Node{1, &Node{2, &Node{3, nil}}}
	fmt.Println("sum:", total(xs))
	fmt.Println("length:", length(xs))
	fmt.Println("list:", show(xs))
	var empty *Node
	fmt.Println("empty length:", length(empty))
}
```

```output for=list
sum: 6
length: 3
list: 1 -> 2 -> 3
empty length: 0
```

- Verdict: native. A struct cannot contain a field of its own type, but it can contain a pointer to it. A `nil` pointer is the empty list.
- A field `Next Node` (with no `*`) does not compile, because the struct would contain itself.

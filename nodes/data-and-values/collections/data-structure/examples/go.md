```go run id=two-layouts
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type node struct {
	value int
	next  *node
}

func main() {
	array := [5]int{10, 20, 30, 40, 50}
	var head *node
	for i := len(array) - 1; i >= 0; i-- {
		head = &node{array[i], head}
	}

	fmt.Printf("array[3] = %d (0 steps)\n", array[3])
	n, steps := head, 0
	for i := 3; i > 0; i-- {
		n = n.next
		steps++
	}
	fmt.Printf("linked[3] = %d (%d steps)\n", n.value, steps)

	head = &node{5, head}
	var out []string
	for m := head; m != nil; m = m.next {
		out = append(out, strconv.Itoa(m.value))
	}
	fmt.Println("after push_front:", strings.Join(out, " "))
}
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: native. Arrays, slices and maps are in the language. A linked structure is a struct with a pointer field.
- The package `container/list` has a doubly linked list, but most Go code uses a slice.

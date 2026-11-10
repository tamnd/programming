```go run id=tree
package main

import "fmt"

type Tree struct {
	Value    int
	Children []*Tree
}

func total(t *Tree) int {
	sum := t.Value
	for _, child := range t.Children {
		sum += total(child)
	}
	return sum
}

func depth(n int) int {
	if n == 0 {
		return 0
	}
	return 1 + depth(n-1)
}

func main() {
	tree := &Tree{1, []*Tree{{2, []*Tree{{4, nil}, {5, nil}}}, {3, nil}}}
	fmt.Println(total(tree))
	fmt.Println(depth(1_000_000))
}
```

```output for=tree
15
1000000
```

- Verdict: native. A function can call itself, and each call gets a frame on the stack of its goroutine.
- A goroutine starts with a small stack. When the stack is full, the runtime copies it to a larger one, so a depth of a million calls works.
- The stack has a maximum size, 1 GB on 64-bit systems by default. A goroutine that needs more crashes the program. `runtime/debug.SetMaxStack` changes the maximum.

With a maximum of 1 MiB, the same depth crashes the program. The runtime prints `goroutine stack exceeds 1048576-byte limit` and `fatal error: stack overflow`, and the exit status is 2.

```go fails expect=exit:2 id=max-stack
package main

import (
	"fmt"
	"runtime/debug"
)

func depth(n int) int {
	if n == 0 {
		return 0
	}
	return 1 + depth(n-1)
}

func main() {
	debug.SetMaxStack(1 << 20)
	fmt.Println(depth(1_000_000))
}
```

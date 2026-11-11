```go run id=escape-analysis
package main

import (
	"fmt"
	"testing"
)

type point struct{ x, y int }

var sink *point

func byValue() point { return point{1, 2} } // the result is copied out

func byPointer() *point { return &point{1, 2} } // escapes, so it moves to the heap

func down(n int) int {
	if n == 0 {
		return 0
	}
	return 1 + down(n-1)
}

func main() {
	v := testing.AllocsPerRun(100, func() { _ = byValue() })
	p := testing.AllocsPerRun(100, func() { sink = byPointer() })
	fmt.Println(v, p)

	q := byPointer() // safe in Go: the variable lives as long as q refers to it
	q.x = 7
	fmt.Println(*q)

	done := make(chan int)
	go func() { done <- down(1_000_000) }() // goroutine stacks grow as needed
	fmt.Println(<-done)
}
```

```output for=escape-analysis
0 1
{7 2}
1000000
```

- Verdict: native. Go does not let the programmer choose between the stack and the heap. The compiler runs escape analysis: a variable whose address does not outlive its function stays in the frame, and one that does moves to the heap. `testing.AllocsPerRun` shows that returning a struct by value allocates nothing, while returning a pointer that a global keeps costs one heap allocation.
- Returning the address of a local variable is safe in Go, because the variable lives as long as a reference to it exists. Each goroutine starts with a small stack that grows when it fills, so a recursion a million levels deep works.

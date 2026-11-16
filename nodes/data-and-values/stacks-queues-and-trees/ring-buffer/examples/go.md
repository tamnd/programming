```go run id=ring
package main

import "fmt"

type ring struct {
	items []int
	start int
	count int
}

func newRing(capacity int) *ring {
	return &ring{items: make([]int, capacity)}
}

func (r *ring) push(item int) {
	r.items[(r.start+r.count)%len(r.items)] = item
	if r.count < len(r.items) {
		r.count++
	} else {
		r.start = (r.start + 1) % len(r.items)
	}
}

func (r *ring) pop() (int, bool) {
	if r.count == 0 {
		return 0, false
	}
	item := r.items[r.start]
	r.start = (r.start + 1) % len(r.items)
	r.count--
	return item, true
}

func (r *ring) print() {
	for i := 0; i < r.count; i++ {
		fmt.Print(" ", r.items[(r.start+i)%len(r.items)])
	}
	fmt.Println()
}

func main() {
	r := newRing(3)
	for _, reading := range []int{10, 20, 30, 40, 50} {
		r.push(reading)
		fmt.Printf("after %d:", reading)
		r.print()
	}

	if oldest, ok := r.pop(); ok {
		fmt.Printf("read %d, left:", oldest)
		r.print()
	}
}
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: idiom. A slice of fixed length, a start index and a count make a ring buffer.
- `pop` returns `false` as its second value when the buffer is empty.

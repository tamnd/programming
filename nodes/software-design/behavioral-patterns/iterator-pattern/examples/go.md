```go run id=tree
package main

import (
	"fmt"
	"iter"
	"strings"
)

type node struct {
	key         int
	left, right *node
}

type Tree struct{ root *node }

func NewTree(keys ...int) *Tree {
	t := &Tree{}
	for _, key := range keys {
		t.Insert(key)
	}
	return t
}

func (t *Tree) Insert(key int) {
	link := &t.root
	for *link != nil {
		if key < (*link).key {
			link = &(*link).left
		} else {
			link = &(*link).right
		}
	}
	*link = &node{key: key}
}

// All is a push iterator: it calls yield for each key, in order.
func (t *Tree) All() iter.Seq[int] {
	return func(yield func(int) bool) {
		var walk func(n *node) bool
		walk = func(n *node) bool {
			return n == nil || (walk(n.left) && yield(n.key) && walk(n.right))
		}
		walk(t.root)
	}
}

// SameKeys turns the two push iterators into pull iterators and steps them together.
func SameKeys(a, b *Tree) bool {
	nextA, stopA := iter.Pull(a.All())
	defer stopA()
	nextB, stopB := iter.Pull(b.All())
	defer stopB()
	for {
		x, okA := nextA()
		y, okB := nextB()
		if !okA || !okB {
			return okA == okB
		}
		if x != y {
			return false
		}
	}
}

func yesNo(ok bool) string {
	if ok {
		return "yes"
	}
	return "no"
}

func main() {
	a := NewTree(5, 3, 8, 1, 4, 9)
	b := NewTree(1, 3, 4, 5, 8, 9)
	c := NewTree(1, 3, 4, 5, 8)

	var keys []string
	sum := 0
	for key := range a.All() {
		keys = append(keys, fmt.Sprint(key))
		sum += key
	}
	fmt.Println("in order:", strings.Join(keys, " "))
	fmt.Println("sum:", sum)

	var first []string
	for key := range a.All() {
		if len(first) == 2 {
			break // yield returns false, and walk stops
		}
		first = append(first, fmt.Sprint(key))
	}
	fmt.Println("first two:", strings.Join(first, " "))
	fmt.Println("same keys as b:", yesNo(SameKeys(a, b)))
	fmt.Println("same keys as c:", yesNo(SameKeys(a, c)))
}
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: native. A function of type `iter.Seq[int]`, `func(yield func(int) bool)`, is a range expression. The loop body becomes the `yield` function.
- `All` is an internal iterator. The recursive `walk` keeps the traversal state on the call stack, so the code needs no explicit stack.
- A `break` in the loop makes `yield` return `false`. The iterator must then stop and not call `yield` again, which the `&&` chain in `walk` does.
- `iter.Pull` turns a push iterator into a `next` function and a `stop` function. `SameKeys` needs it because it must step two trees together. The `defer stopA()` frees the iterator when the comparison ends early.

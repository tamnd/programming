```go run id=stop-inheriting-the-array
package main

import (
	"fmt"
	"slices"
)

// list is a general list type with methods for any position.
type list struct{ items []int }

func (l *list) Append(x int)      { l.items = append(l.items, x) }
func (l *list) InsertAt(i, x int) { l.items = slices.Insert(l.items, i, x) }
func (l *list) RemoveAt(i int)    { l.items = slices.Delete(l.items, i, i+1) }
func (l *list) Last() int         { return l.items[len(l.items)-1] }
func (l *list) Len() int          { return len(l.items) }
func (l *list) TakeLast() (x int) { x = l.Last(); l.items = l.items[:len(l.items)-1]; return x }
func (l *list) Snapshot() []int   { return slices.Clone(l.items) }

// Before: the stack embeds list, so every list method is promoted to it.
type stackBefore struct{ list }

func (s *stackBefore) Push(x int) { s.Append(x) }
func (s *stackBefore) Pop() int   { return s.TakeLast() }

// After: the stack holds the list in a named field, and offers four methods.
type stack struct{ items list }

func (s *stack) Push(x int) { s.items.Append(x) }
func (s *stack) Pop() int   { return s.items.TakeLast() }
func (s *stack) Peek() int  { return s.items.Last() }
func (s *stack) Len() int   { return s.items.Len() }

type pusher interface{ InsertAt(i, x int) }

func main() {
	before, after := &stackBefore{}, &stack{}
	for _, x := range []int{1, 2, 3} {
		before.Push(x)
		after.Push(x)
	}
	if after.Peek() != before.Last() || after.Pop() != before.Pop() || after.Len() != before.Len() {
		panic("stack operations")
	}

	// The bequest: promoted methods reach the bottom of the first stack.
	before.InsertAt(0, 0)
	before.RemoveAt(1)
	if !slices.Equal(before.Snapshot(), []int{0, 2}) {
		panic("promoted methods")
	}
	var _ pusher = before // the first stack satisfies an interface for inserting anywhere
	if _, ok := any(after).(pusher); ok {
		panic("the second stack must not offer InsertAt")
	}
	fmt.Println("stack: tests pass")
}
```

```output
stack: tests pass
```

- Embedding is not inheritance, but it has the same effect on the method set: each exported method of `list` becomes a method of `stackBefore`, and `stackBefore` satisfies every interface that `list` does.
- A named field, `items list`, keeps the code reuse and drops the promotion. Only the four methods that `stack` declares are in its method set.

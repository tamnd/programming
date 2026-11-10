```go run id=nil
package main

import "fmt"

type Node struct {
	Value int
	Next  *Node
}

func (n *Node) Len() int {
	if n == nil {
		return 0
	}
	return 1 + n.Next.Len()
}

func main() {
	list := &Node{1, &Node{2, nil}}
	var empty *Node
	fmt.Println(list.Len(), empty.Len())

	var m map[string]int
	fmt.Println(m["x"], len(m))

	var s []int
	s = append(s, 7)
	fmt.Println(s, len(s))
}
```

```output
2 0
0 0
[7] 1
```

- Verdict: native. `nil` is the zero value of pointers, functions, interfaces, slices, channels and maps. A variable of one of these types that has no initializer is `nil`.
- `*p` and `p.f` on a `nil` pointer cause a run-time panic. A method with a pointer receiver can still be called on a `nil` pointer, because the call does not read through the pointer. `Len` uses this to treat `nil` as the empty list.
- A `nil` map acts as an empty map for reads, and a `nil` slice works with `len` and `append`. A write to a `nil` map panics (see Pitfalls on the concept page).
- An interface value is `nil` only when both its dynamic type and its value are unset. A `nil` pointer stored in an interface is not equal to `nil` (see Pitfalls).

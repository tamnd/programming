```go run id=nil-and-map-guards
package main

import "fmt"

type node struct {
	val  int
	next *node
}

var calls int

func check(v bool) bool {
	calls++
	return v
}

func main() {
	// && and || evaluate the right operand only when they need it.
	r := check(false) && check(true)
	fmt.Println(r, "calls =", calls)
	calls = 0
	r = check(true) || check(true)
	fmt.Println(r, "calls =", calls)

	// The guard pattern: a nil pointer is never dereferenced.
	list := &node{1, &node{2, nil}}
	for p := list; p != nil; p = p.next {
		fmt.Println(p.val, p.next != nil && p.next.val > p.val)
	}

	// A map lookup with ok, then a test of the value.
	stock := map[string]int{"pen": 4}
	if n, ok := stock["ink"]; ok && n > 0 {
		fmt.Println("ink in stock")
	} else {
		fmt.Println("no ink")
	}
}
```

```output for=nil-and-map-guards
false calls = 1
true calls = 1
1 true
2 false
no ink
```

- Verdict: native. Go's `&&` and `||` evaluate the right operand only when needed, and Go has no other short-circuit operators. The test `p.next != nil && p.next.val > p.val` never reads through a nil pointer. In an `if` with a short statement, `ok && n > 0` tests the value only when the key exists.

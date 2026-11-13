```go run id=arrays-are-values
package main

import "fmt"

func double(a [3]int) [3]int {
	for i := range a {
		a[i] *= 2
	}
	return a
}

func main() {
	// The length is part of the type. Items that the literal does not give are zero.
	var a [5]int
	b := [5]int{1, 2, 3}
	fmt.Println(a, b, len(b))

	// [...] counts the items in the literal.
	c := [...]string{"x", "y", "z"}
	fmt.Printf("%T %v\n", c, c)

	// An array is a value. Assignment and calls copy all the items.
	d := b
	d[0] = 100
	nums := [3]int{1, 2, 3}
	fmt.Println(b[0], d[0], double(nums), nums)

	// Arrays of the same type compare item by item.
	fmt.Println(b == [5]int{1, 2, 3, 0, 0})

	// A constant index out of range is a compile error. A variable index panics.
	defer func() { fmt.Println("recovered:", recover()) }()
	i := 5
	b[i] = 1
}
```

```output
[0 0 0 0 0] [1 2 3 0 0] 5
[3]string [x y z]
1 100 [2 4 6] [1 2 3]
true
recovered: runtime error: index out of range [5] with length 5
```

- Verdict: native. The length is part of the type, and the missing items are zero. An array is a value, so assignment and a call copy all the items, and == compares the items. An index out of range in a variable panics, and the deferred function recovers to print the message.

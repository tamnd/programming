```go run id=stack
package main

import "fmt"

func main() {
	var stack []string
	for _, item := range []string{"a", "b", "c"} {
		stack = append(stack, item)
	}
	fmt.Println(stack, "top", stack[len(stack)-1])

	pop := func() (string, bool) {
		if len(stack) == 0 {
			return "", false
		}
		top := stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		return top, true
	}
	first, _ := pop()
	second, _ := pop()
	fmt.Println(first, second, stack)
	pop()
	if _, ok := pop(); !ok {
		fmt.Println("pop on empty: no value")
	}

	// Without the check, the index is out of range and Go panics.
	defer func() { fmt.Println("recovered:", recover()) }()
	_ = stack[len(stack)-1]
}
```

```output for=stack
[a b c] top c
c b [a]
pop on empty: no value
recovered: runtime error: index out of range [-1]
```

- Verdict: idiom. A slice is the stack. `append` pushes, and `stack[:len(stack)-1]` drops the top item.
- The `pop` function returns a second value that tells whether the stack had an item.
- An index outside the slice panics at run time.

```go run id=reassign-and-shadow
package main

import "fmt"

func main() {
	x := 1
	if x > 0 {
		x := 2
		fmt.Println(x)
	}
	fmt.Println(x)

	x = 3
	fmt.Println(x)
}
```

```output for=reassign-and-shadow
2
1
3
```

- Verdict: absent. Go has no variable that the compiler limits to one assignment. `const` works only for values that the compiler knows. `x := 2` in the inner block makes a new variable that hides the outer one, and the outer `x` keeps 1. The assignment `x = 3` then changes the outer variable.

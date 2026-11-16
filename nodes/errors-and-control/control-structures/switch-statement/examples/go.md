```go run id=switch-go
package main

import "fmt"

func daysIn(month int) int {
	switch month {
	case 2:
		return 28
	case 4, 6, 9, 11:
		return 30
	default:
		return 31
	}
}

func size(n int) string {
	// A switch with no expression tests each case as a condition.
	switch {
	case n < 10:
		return "small"
	case n < 100:
		return "medium"
	default:
		return "large"
	}
}

func main() {
	fmt.Println(daysIn(2), daysIn(4), daysIn(7))
	fmt.Println(size(5), size(50), size(500))

	// A case ends by itself; there is no break.
	switch x := 1; x {
	case 1:
		fmt.Println("one")
	case 2:
		fmt.Println("two")
	}
}
```

```output for=switch-go
28 30 31
small medium large
one
```

- Verdict: native. A Go case lists several values with commas, as in `case 4, 6, 9, 11:`. A case ends by itself, so the `switch` on `x` prints only `one`. A `switch` with no expression compares each case with `true`, so it works like an `if` chain. A `switch` can start with a short statement, such as `x := 1`.

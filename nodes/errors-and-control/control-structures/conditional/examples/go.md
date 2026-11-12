```go run id=if-with-short-statement
package main

import (
	"fmt"
	"strconv"
)

func main() {
	// An if can start with a short statement, whose variables live in the if.
	ages := map[string]int{"ann": 31}
	for _, name := range []string{"ann", "bob"} {
		if age, ok := ages[name]; ok {
			fmt.Println(name, age)
		} else {
			fmt.Println(name, "unknown")
		}
	}

	// The error check is the most common if in Go.
	for _, s := range []string{"12", "x"} {
		n, err := strconv.Atoi(s)
		if err != nil {
			fmt.Println("error:", err)
			continue
		}
		fmt.Println("number", n)
	}

	// A switch with no expression is a chain of if and else if.
	for _, t := range []int{5, 18, 30} {
		switch {
		case t < 10:
			fmt.Println(t, "cold")
		case t < 25:
			fmt.Println(t, "mild")
		default:
			fmt.Println(t, "hot")
		}
	}

	// Go has no conditional operator, so a value choice uses if.
	x, y := 3, 9
	larger := x
	if y > x {
		larger = y
	}
	fmt.Println("larger", larger)
}
```

```output for=if-with-short-statement
ann 31
bob unknown
number 12
error: strconv.Atoi: parsing "x": invalid syntax
5 cold
18 mild
30 hot
larger 9
```

- Verdict: native. An `if` can start with a short statement, so `age, ok := ages[name]` lives only in the `if` and its `else`. The `err != nil` test handles the failed conversion. A `switch` with no expression is a chain of conditions. Go has no conditional operator, so a choice of value uses an `if` statement.

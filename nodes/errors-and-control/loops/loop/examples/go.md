```go run id=one-for-statement-many-forms
package main

import "fmt"

func main() {
	// The three-part form.
	sum := 0
	for i := 1; i <= 5; i++ {
		sum += i
	}
	fmt.Println("three-part:", sum)

	// The condition-only form is a while loop.
	n, steps := 100, 0
	for n > 1 {
		n /= 2
		steps++
	}
	fmt.Println("condition:", steps)

	// The range form loops over a slice, a map, a string or an integer.
	for i, w := range []string{"red", "green"} {
		fmt.Println("range:", i, w)
	}
	count := []int{}
	for i := range 3 {
		count = append(count, i)
	}
	fmt.Println("range 3:", count)

	// A for with no condition loops until break.
	k := 0
	for {
		k++
		if k == 4 {
			break
		}
	}
	fmt.Println("forever stopped at", k)
}
```

```output for=one-for-statement-many-forms
three-part: 15
condition: 6
range: 0 red
range: 1 green
range 3: [0 1 2]
forever stopped at 4
```

- Verdict: native. Go has one loop statement, `for`, in four forms: the three-part loop, a loop with only a condition, a `range` loop and a loop with no condition. `range 3` counts from 0 to 2. The loop with no condition ends only at `break`.

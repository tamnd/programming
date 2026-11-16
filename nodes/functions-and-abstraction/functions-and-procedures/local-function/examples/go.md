```go run id=local-function-go
package main

import "fmt"

func sumOfScaled(values []int, factor int) int {
	scale := func(x int) int {
		return x * factor
	}

	total := 0
	for _, v := range values {
		total += scale(v)
	}
	return total
}

func main() {
	fmt.Println(sumOfScaled([]int{1, 2, 3}, 10))
	fmt.Println(sumOfScaled([]int{4}, 2))
}
```

```output for=local-function-go
60
8
```

- Verdict: idiom. Go declares functions only at the top level. A function literal in a local variable is a closure over `factor`.

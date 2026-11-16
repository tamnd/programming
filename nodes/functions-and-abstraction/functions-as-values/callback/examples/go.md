```go run id=callback-go
package main

import (
	"fmt"
	"sort"
)

func repeat(n int, callback func(int)) {
	for i := 1; i <= n; i++ {
		callback(i)
	}
}

func tick(i int) {
	fmt.Println("tick", i)
}

func main() {
	xs := []int{3, 1, 2}
	sort.Slice(xs, func(i, j int) bool { return xs[i] > xs[j] })
	fmt.Println(xs[0], xs[1], xs[2])
	repeat(2, tick)
}
```

```output for=callback-go
3 2 1
tick 1
tick 2
```

- Verdict: native. `sort.Slice` calls the less function with two indexes. The function literal reads the slice `xs` from the outer function.

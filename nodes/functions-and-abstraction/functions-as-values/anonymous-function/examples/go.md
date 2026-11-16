```go run id=anonymous-function-go
package main

import (
	"fmt"
	"sort"
)

func main() {
	words := []string{"house", "car", "bike"}
	sort.Slice(words, func(i, j int) bool { return len(words[i]) < len(words[j]) })
	fmt.Println(words[0], words[1], words[2])

	nums := []int{1, 2, 3}
	square := func(x int) int { return x * x }
	for i, x := range nums {
		nums[i] = square(x)
	}
	fmt.Println(nums[0], nums[1], nums[2])

	n := 5
	add := func(x int) int { return x + n }
	fmt.Println(add(10))
}
```

```output for=anonymous-function-go
car bike house
1 4 9
15
```

- Verdict: native. Function literals have the full signature with types. The literal `add` refers to `n` from `main`.

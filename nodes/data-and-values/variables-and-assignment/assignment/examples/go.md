```go run id=statements
package main

import "fmt"

func main() {
	a, b := 1, 2
	a, b = b, a
	fmt.Println(a, b)
	x := []int{0, 1}
	i := 0
	i, x[i] = 1, 2
	fmt.Println(x, i)
	_, err := fmt.Sscan("12", &a)
	fmt.Println(a, err)
}
```

```output for=statements
2 1
[2 1] 1
12 <nil>
```

- Verdict: native. Assignment is a statement, so `if x = 0` does not compile. `:=` declares and assigns, and `=` assigns to variables that exist. A tuple assignment computes the indexes and the values first and then assigns, so `i, x[i] = 1, 2` sets `x[0]`. The blank identifier `_` drops a value.

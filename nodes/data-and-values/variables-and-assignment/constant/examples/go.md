```go run id=const
package main

import "fmt"

const Big = 1 << 100

const (
	KB = 1 << (10 * (iota + 1))
	MB
	GB
)

func main() {
	fmt.Println(Big >> 98)
	fmt.Println(KB, MB, GB)
	var f float64 = Big
	fmt.Println(f > 1e30)
}
```

```output for=const
4
1024 1048576 1073741824
true
```

- Verdict: native. A Go constant is a value of the compiler, with exact arithmetic: `1 << 100` is too large for any integer type, but `Big >> 98` is 4, a value that fits. An untyped constant takes the type of its use, so `Big` can be a `float64`. In a `const` block, `iota` counts the lines, and a line with no expression repeats the expression before it.

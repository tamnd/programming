```go run id=shapes
package main

import (
	"cmp"
	"fmt"
)

func Max[T cmp.Ordered](a, b T) T {
	if a > b {
		return a
	}
	return b
}

type Pair[K comparable, V any] struct {
	Key K
	Val V
}

func main() {
	fmt.Println(Max(3, 9), Max("pear", "apple"))
	p := Pair[string, int]{"a", 1}
	fmt.Printf("%T %v\n", p, p)
}
```

```output for=shapes
9 pear
main.Pair[string,int] {a 1}
```

- Verdict: native. The compiler makes code for each GC shape of the type arguments and passes a dictionary that describes them. The type arguments are kept, so `%T` prints `main.Pair[string,int]`.

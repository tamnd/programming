```go run id=function-object-go
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type Multiplier struct {
	factor int
	calls  int
}

func (m *Multiplier) Apply(x int) int {
	m.calls++
	return x * m.factor
}

func mapInts(f func(int) int, xs []int) []string {
	out := make([]string, 0, len(xs))
	for _, x := range xs {
		out = append(out, strconv.Itoa(f(x)))
	}
	return out
}

func main() {
	triple := &Multiplier{factor: 3}
	f := triple.Apply
	fmt.Println(f(5))
	fmt.Println(strings.Join(mapInts(f, []int{1, 2, 3}), " "))
	fmt.Println(triple.calls)
}
```

```output for=function-object-go
15
3 6 9
4
```

- Verdict: idiom. Go has no call operator for structs. The method value `triple.Apply` is a function value that keeps the pointer `triple`, so each call changes the field `calls`.

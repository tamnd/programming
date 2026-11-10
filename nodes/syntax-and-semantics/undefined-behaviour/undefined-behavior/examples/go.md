```go run id=defined
package main

import (
	"fmt"
	"math"
)

func main() {
	x := int32(math.MaxInt32)
	fmt.Println(x + 1)
	fmt.Println(x < x+1)

	s := []int{1, 2, 3}
	i := 3
	defer func() {
		fmt.Println("recovered:", recover())
	}()
	fmt.Println(s[i])
}
```

```output for=defined
-2147483648
false
recovered: runtime error: index out of range [3] with length 3
```

- Verdict: absent. The Go specification defines signed overflow: the value wraps, and there is no panic. A compiler may not assume that `x < x+1` is always true.
- An index out of range causes a run-time panic, which `recover` can stop.
- Two exceptions remain. Package `unsafe` allows operations that violate the type system. A data race on a value of more than one machine word, such as a slice or an interface, can corrupt memory.
- `go build -race` and `go test -race` find data races while the program runs.

```go run id=useful-zero
package main

import (
	"fmt"
	"strings"
)

func main() {
	var sb strings.Builder
	sb.WriteString("ready")
	fmt.Println(sb.String(), sb.Len())

	var counts map[string]int
	fmt.Println(counts["x"], len(counts))

	var list []string
	list = append(list, "a")
	fmt.Println(list, len(list))
}
```

```output for=useful-zero
ready 5
0 0
[a] 1
```

- Verdict: native. Each variable starts at the zero value of its type, and many types work at their zero value: an empty `strings.Builder` is ready, a nil map can be read, and `append` to a nil slice makes a new slice. Go has no default parameters: a function takes a struct of options, and the zero value of each field means "use the default".

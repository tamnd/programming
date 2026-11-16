```go run id=refutable-go
package main

import (
	"fmt"
	"strings"
)

func main() {
	for _, line := range []string{"width=80", "oops", "height=24"} {
		key, value, ok := strings.Cut(line, "=")
		if !ok {
			fmt.Println("skip: " + line)
			continue
		}
		fmt.Println(key + " -> " + value)
	}
}
```

```output for=refutable-go
width -> 80
skip: oops
height -> 24
```

- Verdict: idiom. Go has no patterns. A function that can fail returns an extra boolean, and the program tests it: here `strings.Cut` returns `ok`. The assignment of three results never fails.

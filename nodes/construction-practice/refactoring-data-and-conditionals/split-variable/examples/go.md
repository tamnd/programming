```go run id=one-job-each
package main

import (
	"fmt"
	"strings"
)

func handle(rawInput string) string {
	trimmed := strings.TrimSpace(rawInput)
	lower := strings.ToLower(trimmed)
	slug := strings.Join(strings.Fields(lower), "-")
	clipped := clip(slug, 12)
	return clipped
}

// clip returns at most the first n bytes of s.
func clip(s string, n int) string {
	if len(s) > n {
		return s[:n]
	}
	return s
}

func main() {
	if got := handle("  Ana Pires "); got != "ana-pires" {
		panic(got)
	}
	if got := handle("Very Long Display Name"); got != "very-long-di" {
		panic(got)
	}
	fmt.Println("split variable: tests pass")
}
```

```output
split variable: tests pass
```

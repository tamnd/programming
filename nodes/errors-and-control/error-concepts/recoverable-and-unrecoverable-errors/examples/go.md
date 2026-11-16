```go run id=recoverable-go
package main

import (
	"fmt"
	"strconv"
)

func portOrDefault(text string, fallback int) (int, string) {
	// A bad fallback is a bug in the caller: panic.
	if fallback < 1 || fallback > 65535 {
		panic("fallback out of range")
	}
	// Bad text is an expected error: recover.
	n, err := strconv.Atoi(text)
	if err != nil {
		return fallback, " (not a number)"
	}
	return n, ""
}

func main() {
	for _, text := range []string{"8080", "x"} {
		port, note := portOrDefault(text, 80)
		fmt.Printf("%s -> %d%s\n", text, port, note)
	}
}
```

```output for=recoverable-go
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: native. Go reports an expected error as an `error` value, and uses `panic` for a bug. A panic runs the deferred functions and then crashes the program, unless a deferred call to `recover` stops it.

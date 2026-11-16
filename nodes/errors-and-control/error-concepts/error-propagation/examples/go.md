```go run id=propagate-go
package main

import (
	"fmt"
	"strconv"
	"strings"
)

func parsePort(text string) (int, error) {
	n, err := strconv.Atoi(text)
	if err != nil || n < 0 {
		return 0, fmt.Errorf("not a number: %s", text)
	}
	return n, nil
}

func loadConfig(line string) (int, error) {
	_, value, _ := strings.Cut(line, "=")
	port, err := parsePort(value)
	if err != nil {
		// Add context, and wrap the original error with %w.
		return 0, fmt.Errorf("config line '%s': %w", line, err)
	}
	return port, nil
}

func main() {
	for _, line := range []string{"port=8080", "port=x"} {
		port, err := loadConfig(line)
		if err != nil {
			fmt.Println("error:", err)
			continue
		}
		fmt.Println("port", port)
	}
}
```

```output for=propagate-go
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. Each function returns its error, and each caller tests it and returns it up with `return`. The verb `%w` in `fmt.Errorf` wraps the error, so `errors.Is` and `errors.As` can still find the original in the chain.

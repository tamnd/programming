```go run id=ports
package main

import (
	"errors"
	"fmt"
	"strconv"
)

var ErrRange = errors.New("out of range")

func parsePort(text string) (int, error) {
	port, err := strconv.Atoi(text)
	if err != nil {
		return 0, fmt.Errorf("not a number: %w", err)
	}
	if port < 1 || port > 65535 {
		return 0, fmt.Errorf("%w: %d", ErrRange, port)
	}
	return port, nil
}

func element(items []int, i int) (n int, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("recovered: %v", r)
		}
	}()
	return items[i], nil
}

func main() {
	for _, text := range []string{"8080", "70000", "abc"} {
		port, err := parsePort(text)
		if err != nil {
			fmt.Printf("error: %v (range: %t)\n", err, errors.Is(err, ErrRange))
			continue
		}
		fmt.Println("port", port)
	}
	_, err := element([]int{1, 2, 3}, 5)
	fmt.Println(err)
}
```

```output
port 8080
error: out of range: 70000 (range: true)
error: not a number: strconv.Atoi: parsing "abc": invalid syntax (range: false)
recovered: runtime error: index out of range [5] with length 3
```

- Verdict: native, for a panic only. A panic runs the deferred calls up the stack and ends the program, unless a deferred function calls `recover`.
- Ordinary errors are values of type `error`, returned as the last result and tested with `if err != nil`.
- `%w` in `fmt.Errorf` wraps an error, and `errors.Is` looks through the chain of wrapped errors.
- An index out of range is a run-time panic. `element` turns it into an error value.

```go run id=port
package main

import (
	"errors"
	"fmt"
	"strconv"
)

var ErrMissing = errors.New("missing key")

type PortError struct {
	Text string
}

func (e *PortError) Error() string {
	return fmt.Sprintf("invalid port %q", e.Text)
}

func lookup(config map[string]string, key string) (string, error) {
	value, ok := config[key]
	if !ok {
		return "", fmt.Errorf("%w %q", ErrMissing, key)
	}
	return value, nil
}

func parsePort(text string) (int, error) {
	n, err := strconv.Atoi(text)
	if err != nil || n < 1 || n > 65535 {
		return 0, &PortError{Text: text}
	}
	return n, nil
}

func portSetting(config map[string]string) (int, error) {
	text, err := lookup(config, "port")
	if err != nil {
		return 0, fmt.Errorf("port setting: %w", err)
	}
	port, err := parsePort(text)
	if err != nil {
		return 0, fmt.Errorf("port setting: %w", err)
	}
	return port, nil
}

func main() {
	configs := []map[string]string{{"port": "8080"}, {"port": "x"}, {}}
	for _, config := range configs {
		port, err := portSetting(config)
		var portErr *PortError
		switch {
		case err == nil:
			fmt.Println("port", port)
		case errors.As(err, &portErr):
			fmt.Printf("bad text %q: %v\n", portErr.Text, err)
		case errors.Is(err, ErrMissing):
			fmt.Println("use the default 80:", err)
		}
	}
}
```

```output
port 8080
bad text "x": port setting: invalid port "x"
use the default 80: port setting: missing key "port"
```

- Verdict: native. `error` is a predeclared interface with one method, `Error() string`. A function returns it as its last result, and `nil` means success.
- `fmt.Errorf` with the verb `%w` wraps an error: the new error adds context to the message and keeps the old error inside. `errors.Is` finds a sentinel value such as `ErrMissing` anywhere in the chain, and `errors.As` finds an error of a given type, such as `*PortError`.
- The compiler does not force the caller to test the error. The blank identifier `_` discards it, and a call statement that ignores all the results compiles.

A wrapped error is a new value, so the operator `==` no longer finds the sentinel. Here the comparison is false, and the code takes the wrong branch.

```go wrong id=compare
package main

import (
	"errors"
	"fmt"
)

var ErrMissing = errors.New("missing key")

func lookup(config map[string]string, key string) (string, error) {
	value, ok := config[key]
	if !ok {
		return "", fmt.Errorf("lookup %q: %w", key, ErrMissing)
	}
	return value, nil
}

func main() {
	_, err := lookup(map[string]string{}, "port")
	fmt.Println("err == ErrMissing:", err == ErrMissing)
	fmt.Println("errors.Is:", errors.Is(err, ErrMissing))
}
```

```output
err == ErrMissing: false
errors.Is: true
```

An interface value is `nil` only when both its type and its value are unset. A function that returns a typed `nil` pointer as an `error` returns an interface that holds the type `*PortError`, so the caller sees an error.

```go wrong id=typed-nil
package main

import "fmt"

type PortError struct{ Text string }

func (e *PortError) Error() string { return "invalid port " + e.Text }

func check(text string) error {
	var result *PortError
	if text == "" {
		result = &PortError{Text: text}
	}
	return result
}

func main() {
	err := check("8080")
	fmt.Println("err != nil:", err != nil)
	fmt.Printf("dynamic type: %T\n", err)
}
```

```output
err != nil: true
dynamic type: *main.PortError
```

The fix is to return the literal `nil` on the success path, and to give the function the result type `error`, not `*PortError`.

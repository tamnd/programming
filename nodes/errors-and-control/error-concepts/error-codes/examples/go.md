```go run id=codes-go
package main

import (
	"errors"
	"fmt"
	"strconv"
)

const (
	OK = iota
	Empty
	NotANumber
	OutOfRange
)

var names = []string{"ok", "empty", "not a number", "out of range"}

// parsePort returns a port and a code. The port is valid only when the code is OK.
func parsePort(text string) (int, int) {
	if text == "" {
		return 0, Empty
	}
	port, err := strconv.Atoi(text)
	if errors.Is(err, strconv.ErrRange) {
		return 0, OutOfRange
	}
	if err != nil {
		return 0, NotANumber
	}
	if port < 0 || port > 65535 {
		return 0, OutOfRange
	}
	return port, OK
}

func main() {
	for _, text := range []string{"8080", "", "x", "99999"} {
		port, code := parsePort(text)
		if code != OK {
			fmt.Printf("'%s' -> error code %d (%s)\n", text, code, names[code])
		} else {
			fmt.Printf("'%s' -> port %d\n", text, port)
		}
	}
}
```

```output for=codes-go
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: idiom. Go returns errors as values of the `error` interface, not as integers. An operating system code is still a number: "An Errno is an unsigned number describing an error condition. It implements the error interface."

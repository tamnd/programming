```go run id=error-go
package main

import (
	"errors"
	"fmt"
	"strconv"
)

func main() {
	for _, text := range []string{"42", "x42"} {
		n, err := strconv.Atoi(text)
		if err != nil {
			fmt.Printf("%s -> error (%v)\n", text, errors.Unwrap(err))
			continue
		}
		fmt.Printf("%s -> %d\n", text, n)
	}
}
```

```output for=error-go
42 -> 42
x42 -> error (invalid syntax)
```

- Verdict: native. Go reports an error as a value of the predeclared interface type `error`, returned as the last result. `nil` means no error. `strconv.Atoi` returns a `*strconv.NumError` that wraps `strconv.ErrSyntax`, and `errors.Unwrap` gets that cause.

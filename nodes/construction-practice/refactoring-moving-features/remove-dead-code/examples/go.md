```go run id=gone
package main

import (
	"errors"
	"fmt"
)

// After: the always-off flag, the helper that only its branch called,
// and the unused rate function are deleted. The compiler would reject
// an unused import or an unused local variable, which leaves fewer
// kinds of dead code to look for by hand.
var errCurrency = errors.New("unsupported currency")

func price(cents int, currency string) (int, error) {
	if currency == "EUR" {
		return cents, nil
	}
	return 0, fmt.Errorf("%w %s", errCurrency, currency)
}

func main() {
	if c, err := price(1999, "EUR"); err != nil || c != 1999 {
		panic("EUR")
	}
	if _, err := price(100, "USD"); !errors.Is(err, errCurrency) {
		panic("USD")
	}
	fmt.Println("remove dead code: tests pass")
}
```

```output
remove dead code: tests pass
```

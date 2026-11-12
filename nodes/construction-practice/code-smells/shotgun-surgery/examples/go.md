```go run id=gather-the-codes
package main

import (
	"fmt"
	"math"
	"slices"
)

// Before: three functions each know the codes.
func priceBefore(cents int, code string) int {
	switch code {
	case "SPRING10":
		return int(math.Round(float64(cents) * 0.9))
	case "VIP25":
		return int(math.Round(float64(cents) * 0.75))
	}
	return cents
}

func receiptBefore(code string) string {
	switch code {
	case "SPRING10":
		return "SPRING10 (10% off)"
	case "VIP25":
		return "VIP25 (25% off)"
	}
	return "no discount"
}

func isValidBefore(code string) bool {
	return slices.Contains([]string{"SPRING10", "VIP25", "SUMMER20"}, code)
}

// SUMMER20 was added to the validation, but not to the price or the receipt.

// After: one table owns the codes, and the three functions read it.
var discounts = map[string]int{"SPRING10": 10, "VIP25": 25, "SUMMER20": 20}

func isValid(code string) bool { _, ok := discounts[code]; return ok }

func price(cents int, code string) int {
	return int(math.Round(float64(cents*(100-discounts[code])) / 100))
}

func receipt(code string) string {
	if !isValid(code) {
		return "no discount"
	}
	return fmt.Sprintf("%s (%d%% off)", code, discounts[code])
}

func main() {
	if !isValidBefore("SUMMER20") || priceBefore(1000, "SUMMER20") != 1000 {
		panic("before")
	}
	fmt.Println("before: one of three edits was made")

	if price(1000, "SPRING10") != 900 || price(1000, "SUMMER20") != 800 ||
		receipt("SUMMER20") != "SUMMER20 (20% off)" ||
		price(1000, "BOGUS") != 1000 || receipt("BOGUS") != "no discount" {
		panic("after")
	}
	fmt.Println("after: tests pass")
}
```

```output
before: one of three edits was made
after: tests pass
```

- A read of a missing key from a Go map returns the zero value, so `discounts["BOGUS"]` is 0 and the price is unchanged. `isValid` uses the two-value form to tell a missing code from a code of 0%.
- `%%` in the format string prints one percent sign.

```go run id=say-it-by-name
package main

import (
	"fmt"
	"slices"
	"strings"
)

func main() {
	states := []string{"NY", "MA", "CA"}
	coupon := "SAVE-10"

	// Before: a loop and a manual prefix check.
	appliesBefore := false
	for _, s := range states {
		if s == "MA" {
			appliesBefore = true
		}
	}
	saveBefore := len(coupon) >= 5 && coupon[:5] == "SAVE-"

	// After: the standard library says it by name.
	applies := slices.Contains(states, "MA")
	save := strings.HasPrefix(coupon, "SAVE-")

	if applies != appliesBefore || save != saveBefore {
		panic("differ")
	}
	if slices.Contains([]string(nil), "MA") || strings.HasPrefix("", "SAVE-") {
		panic("edge cases")
	}
	fmt.Println("replace inline code with function call: tests pass")
}
```

```output
replace inline code with function call: tests pass
```

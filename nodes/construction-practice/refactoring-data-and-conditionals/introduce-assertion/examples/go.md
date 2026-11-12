```go run id=say-the-assumption
package main

import (
	"fmt"
	"math"
)

// refundCents panics if daysUsed is out of range: that is a bug in the
// caller, not an input error. Input from users is checked before this.
func refundCents(paidCents, daysPaid, daysUsed int) int {
	if daysUsed < 0 || daysUsed > daysPaid {
		panic(fmt.Sprintf("days used %d must be between 0 and days paid %d", daysUsed, daysPaid))
	}
	return int(math.Round(float64(paidCents*(daysPaid-daysUsed)) / float64(daysPaid)))
}

func main() {
	if refundCents(3000, 30, 10) != 2000 {
		panic("refund")
	}
	func() {
		defer func() {
			if recover() == nil {
				panic("expected a panic")
			}
		}()
		refundCents(3000, 30, 31)
	}()
	fmt.Println("introduce assertion: tests pass")
}
```

```output
introduce assertion: tests pass
```

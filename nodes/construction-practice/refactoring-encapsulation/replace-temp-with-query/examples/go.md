```go run id=ask-for-it
package main

import "fmt"

type Rental struct {
	Days      int
	DailyRate int
}

// Methods are the queries. Each method of the type can ask them.
func (r Rental) Subtotal() int { return r.Days * r.DailyRate }

func (r Rental) Fee() int {
	if r.Subtotal() > 500 {
		return 0
	}
	return 25
}

func (r Rental) Total() int { return r.Subtotal() + r.Fee() }

func (r Rental) Receipt() string { return fmt.Sprintf("%d + %d", r.Subtotal(), r.Fee()) }

func main() {
	for _, r := range []Rental{{2, 100}, {6, 100}} {
		fmt.Println(r.Receipt(), "=", r.Total())
	}
	if (Rental{5, 100}).Total() != 525 {
		panic("unexpected total")
	}
	fmt.Println("replace temp with query: tests pass")
}
```

```output
200 + 25 = 225
600 + 0 = 600
replace temp with query: tests pass
```

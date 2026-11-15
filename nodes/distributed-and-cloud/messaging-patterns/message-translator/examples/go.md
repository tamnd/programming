```go run id=translate-orders
package main

import (
	"errors"
	"fmt"
	"regexp"
	"strconv"
	"strings"
)

// Order is the format of the shop.
type Order struct {
	ID         int
	Customer   string
	TotalCents int
	Date       string
}

var amountRe = regexp.MustCompile(`^(\d+)(?:\.(\d\d))?$`)

// translate turns one partner line (id;customer;amount;day/month/year) into an Order.
func translate(line string) (Order, error) {
	f := strings.Split(line, ";")
	if len(f) != 4 {
		return Order{}, errors.New("want 4 fields")
	}
	m := amountRe.FindStringSubmatch(f[2])
	if m == nil {
		return Order{}, errors.New("bad amount " + f[2])
	}
	whole, _ := strconv.Atoi(m[1])
	frac := 0
	if m[2] != "" {
		frac, _ = strconv.Atoi(m[2])
	}
	id, err := strconv.Atoi(strings.TrimPrefix(f[0], "P-"))
	if err != nil {
		return Order{}, errors.New("bad id " + f[0])
	}
	d := strings.Split(f[3], "/")
	if len(d) != 3 {
		return Order{}, errors.New("bad date " + f[3])
	}
	return Order{ID: id, Customer: f[1], TotalCents: whole*100 + frac, Date: d[2] + "-" + d[1] + "-" + d[0]}, nil
}

func main() {
	partner := []string{
		"P-17;Lan Pham;12.50;03/11/2026",
		"P-18;Hoa Le;8;05/11/2026",
		"P-19;Minh Tran;7,25;04/11/2026",
	}
	for _, line := range partner {
		o, err := translate(line)
		if err != nil {
			fmt.Printf("invalid: %s (%v)\n", line, err)
			continue
		}
		fmt.Printf("order %d: customer=%s total_cents=%d date=%s\n", o.ID, o.Customer, o.TotalCents, o.Date)
	}
}
```

```output
order 17: customer=Lan Pham total_cents=1250 date=2026-11-03
order 18: customer=Hoa Le total_cents=800 date=2026-11-05
invalid: P-19;Minh Tran;7,25;04/11/2026 (bad amount 7,25)
```

- Verdict: idiom. The function `translate` returns an `Order` and an `error`, so each failure has a reason, and the caller decides where the bad line goes.
- The amount is changed to an integer number of cents and not to a `float64`, so that the value is exact.
- The struct `Order` is the format of the target; a change of the partner format changes only `translate`.

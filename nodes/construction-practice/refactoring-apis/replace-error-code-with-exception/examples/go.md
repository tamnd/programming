```go run id=code-to-exception
package main

import (
	"errors"
	"fmt"
)

type ShippingRules struct{ Flat int }

var shippingRules = map[string]ShippingRules{"PT": {Flat: 5}}

// Before: a special value in the normal result.
func localShippingRulesBefore(country string) (ShippingRules, int) {
	if r, ok := shippingRules[country]; ok {
		return r, 0
	}
	return ShippingRules{}, -23
}

// In Go, the code moves into a value of type error, not into a panic.
type OrderProcessingError struct{ Code int }

func (e *OrderProcessingError) Error() string {
	return fmt.Sprintf("order processing error %d", e.Code)
}

func localShippingRules(country string) (ShippingRules, error) {
	if r, ok := shippingRules[country]; ok {
		return r, nil
	}
	return ShippingRules{}, &OrderProcessingError{Code: -23}
}

func shippingCost(country string) (int, error) {
	r, err := localShippingRules(country)
	if err != nil {
		return 0, fmt.Errorf("shipping cost for %s: %w", country, err)
	}
	return r.Flat, nil
}

func main() {
	if _, code := localShippingRulesBefore("XX"); code != -23 {
		panic("mismatch")
	}
	var status []int
	for _, country := range []string{"PT", "XX"} {
		cost, err := shippingCost(country)
		var ope *OrderProcessingError
		if errors.As(err, &ope) {
			status = append(status, ope.Code)
			continue
		}
		status = append(status, cost)
	}
	fmt.Println(status)
}
```

```output
[5 -23]
```

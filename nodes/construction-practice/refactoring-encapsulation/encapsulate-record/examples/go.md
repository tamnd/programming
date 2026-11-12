```go run id=hide-the-storage
package main

import (
	"encoding/json"
	"fmt"
)

// After: unexported fields; the methods are the interface.
type Product struct {
	sku        string
	priceCents int
	currency   string
}

func NewProduct(sku string, priceCents int, currency string) (*Product, error) {
	if priceCents < 0 {
		return nil, fmt.Errorf("bad price %d", priceCents)
	}
	return &Product{sku, priceCents, currency}, nil
}

func (p *Product) SKU() string { return p.sku }

func (p *Product) PriceText() string { // derived, not stored
	return fmt.Sprintf("%d.%02d %s", p.priceCents/100, p.priceCents%100, p.currency)
}

// MarshalJSON is the one place that knows the record form.
func (p *Product) MarshalJSON() ([]byte, error) {
	return json.Marshal(map[string]any{"sku": p.sku, "priceCents": p.priceCents, "currency": p.currency})
}

func main() {
	text := `{"sku":"T-100","priceCents":1999,"currency":"EUR"}`

	// Before: a map, with a type assertion at each use.
	var raw map[string]any
	if err := json.Unmarshal([]byte(text), &raw); err != nil {
		panic(err)
	}
	cents := int(raw["priceCents"].(float64)) // JSON numbers decode as float64
	before := fmt.Sprintf("%d.%02d %s", cents/100, cents%100, raw["currency"])

	p, err := NewProduct(raw["sku"].(string), cents, raw["currency"].(string))
	if err != nil || p.PriceText() != before {
		panic("behaviour changed")
	}
	out, _ := json.Marshal(p)
	fmt.Println(p.PriceText(), string(out))
	fmt.Println("encapsulate record: tests pass")
}
```

```output
19.99 EUR {"currency":"EUR","priceCents":1999,"sku":"T-100"}
encapsulate record: tests pass
```

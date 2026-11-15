```go run id=money-allocate
package main

import (
	"fmt"
	"strings"
)

var fractionDigits = map[string]int{"EUR": 2, "USD": 2, "JPY": 0} // from ISO 4217

// Money is a value: Units is in the minor unit of the currency, such as cents.
type Money struct {
	Units    int64
	Currency string
}

func (m Money) Plus(o Money) (Money, error) {
	if m.Currency != o.Currency {
		return Money{}, fmt.Errorf("cannot add %s and %s", m.Currency, o.Currency)
	}
	return Money{m.Units + o.Units, m.Currency}, nil
}

// Allocate splits by ratios. The sum of the parts is always the whole amount.
func (m Money) Allocate(ratios ...int64) []Money {
	var total int64
	for _, r := range ratios {
		total += r
	}
	parts := make([]Money, len(ratios))
	left := m.Units
	for i, r := range ratios {
		parts[i] = Money{m.Units * r / total, m.Currency}
		left -= parts[i].Units
	}
	for i := 0; left > 0; i, left = i+1, left-1 { // the units that are left, one each
		parts[i].Units++
	}
	return parts
}

func (m Money) String() string {
	d := fractionDigits[m.Currency]
	if d == 0 {
		return fmt.Sprintf("%d %s", m.Units, m.Currency)
	}
	scale := int64(1)
	for i := 0; i < d; i++ {
		scale *= 10
	}
	return fmt.Sprintf("%d.%0*d %s", m.Units/scale, d, m.Units%scale, m.Currency)
}

func show(parts []Money) string {
	s := make([]string, len(parts))
	for i, p := range parts {
		s[i] = p.String()
	}
	return strings.Join(s, ", ")
}

func main() {
	a := Money{1000, "EUR"}
	b := Money{50, "EUR"}
	fmt.Println("a:", a)
	sum, _ := a.Plus(b)
	fmt.Println("a + b:", sum)
	if _, err := a.Plus(Money{500, "JPY"}); err != nil {
		fmt.Println("error:", err)
	}
	fmt.Println("fee:", Money{500, "JPY"})
	fmt.Println("5 cents 70/30:", show(Money{5, "EUR"}.Allocate(70, 30)))
	fmt.Println("100 EUR in 3:", show(Money{10000, "EUR"}.Allocate(1, 1, 1)))
	eq := "no"
	if (Money{1050, "EUR"}) == sum {
		eq = "yes"
	}
	fmt.Println("equal:", eq)
}
```

```output
a: 10.00 EUR
a + b: 10.50 EUR
error: cannot add EUR and JPY
fee: 500 JPY
5 cents 70/30: 0.04 EUR, 0.01 EUR
100 EUR in 3: 33.34 EUR, 33.33 EUR, 33.33 EUR
equal: yes
```

- Verdict: idiom. `Money` is a struct of an `int64` amount and a currency code, so `==` compares two amounts. The standard library has no money or decimal type.
- `%0*d` takes the width from an argument, so the format works for any number of fraction digits.

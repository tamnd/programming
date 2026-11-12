```go run id=extract-the-promotion
package main

import "fmt"

type item struct{ cents, qty int }

func sumCents(items []item) int {
	t := 0
	for _, i := range items {
		t += i.cents * i.qty
	}
	return t
}

// Before: the promotion fields are zero values for most orders.
type orderBefore struct {
	items        []item
	promoCode    string
	promoPercent int
	promoUntil   string
}

func (o *orderBefore) applyPromo(code string, percent int, until string) {
	o.promoCode, o.promoPercent, o.promoUntil = code, percent, until
}

func (o *orderBefore) total(day string) int {
	sum := sumCents(o.items)
	if o.promoCode != "" && day <= o.promoUntil {
		return sum - (sum*o.promoPercent+50)/100
	}
	return sum
}

func (o *orderBefore) label() string {
	if o.promoCode == "" {
		return "no promotion"
	}
	return "promotion " + o.promoCode
}

// After: an interface, with a type for each case.
type promotion interface {
	apply(sum int, day string) int
	label() string
}

type discount struct {
	code    string
	percent int
	until   string
}

func (d discount) apply(sum int, day string) int {
	if day <= d.until {
		return sum - (sum*d.percent+50)/100
	}
	return sum
}
func (d discount) label() string { return "promotion " + d.code }

type noPromotion struct{}

func (noPromotion) apply(sum int, _ string) int { return sum }
func (noPromotion) label() string               { return "no promotion" }

type order struct {
	items     []item
	promotion promotion
}

func newOrder(items []item) order { return order{items, noPromotion{}} }

func (o order) total(day string) int { return o.promotion.apply(sumCents(o.items), day) }
func (o order) label() string        { return o.promotion.label() }

func main() {
	items := []item{{1250, 2}, {499, 1}}
	plainBefore := &orderBefore{items: items}
	promoBefore := &orderBefore{items: items}
	promoBefore.applyPromo("SPRING", 10, "2026-04-30")
	plain := newOrder(items)
	promo := order{items, discount{"SPRING", 10, "2026-04-30"}}

	for _, day := range []string{"2026-04-01", "2026-05-01"} {
		if plain.total(day) != plainBefore.total(day) || promo.total(day) != promoBefore.total(day) {
			panic("total " + day)
		}
	}
	if promo.total("2026-04-01") != 2699 || promo.label() != promoBefore.label() || plain.label() != "no promotion" {
		panic("discount or label")
	}
	fmt.Println("promotion: tests pass")
}
```

```output
promotion: tests pass
```

- In Go the empty string and zero are the zero values, so the first version uses `promoCode != ""` as its check. A real code that is empty would be read as no promotion, which is one more risk of the temporary field.
- `noPromotion` is an empty struct. It takes no memory, and each value of it is the same.
- `newOrder` sets the empty case, so the `promotion` field of an order made with it is never a nil interface.

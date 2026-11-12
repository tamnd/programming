```go run id=split-the-steps
package main

import (
	"fmt"
	"math"
)

type line struct{ price, qty int }

type order struct {
	customer string
	lines    []line
}

// Before: one function, with comments for the steps.
func invoiceBefore(o order) string {
	// add up the lines
	subtotal := 0
	for _, l := range o.lines {
		subtotal += l.price * l.qty
	}
	// discount for large orders
	discount := 0
	if subtotal >= 10000 {
		discount = int(math.Round(float64(subtotal) / 10))
	}
	// tax on the discounted amount
	tax := int(math.Round(float64(subtotal-discount) / 5))
	// format
	total := subtotal - discount + tax
	return fmt.Sprintf("%s: %.2f", o.customer, float64(total)/100)
}

// After: each comment became the name of a function.
func subtotal(lines []line) int {
	sum := 0
	for _, l := range lines {
		sum += l.price * l.qty
	}
	return sum
}

func discount(cents int) int {
	if cents < 10000 {
		return 0
	}
	return int(math.Round(float64(cents) / 10))
}

func tax(cents int) int { return int(math.Round(float64(cents) / 5)) }

func formatCents(cents int) string { return fmt.Sprintf("%.2f", float64(cents)/100) }

func invoice(o order) string {
	gross := subtotal(o.lines)
	net := gross - discount(gross)
	return o.customer + ": " + formatCents(net+tax(net))
}

func main() {
	large := order{"Ada", []line{{4000, 2}, {2500, 1}}}
	small := order{"Bob", []line{{1500, 2}}}
	for _, v := range []struct {
		name string
		f    func(order) string
	}{{"invoiceBefore", invoiceBefore}, {"invoice", invoice}} {
		if v.f(large) != "Ada: 113.40" || v.f(small) != "Bob: 36.00" || discount(9999) != 0 {
			panic(v.name)
		}
		fmt.Println(v.name + ": tests pass")
	}
}
```

```output
invoiceBefore: tests pass
invoice: tests pass
```

- In the "before" version, the local variables `subtotal`, `discount` and `tax` hide the package functions with the same names. Go allows this, but after the declaration the code in the function cannot call the function `tax`, because the name means the variable. The extract removes the clash.
- `discount` uses an early return for the small case. Each extracted function is short enough that its control flow fits in a few lines.

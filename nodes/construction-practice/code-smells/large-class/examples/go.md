```go run id=split-the-checkout
package main

import (
	"fmt"
	"strings"
)

// Before: one type, two jobs, and fields with prefixes.
type checkoutBefore struct {
	cartItems                                   []int
	cartCoupon                                  int // 0 means no coupon
	shipName, shipStreet, shipCity, shipCountry string
}

func (c *checkoutBefore) addItem(name string, cents int) { c.cartItems = append(c.cartItems, cents) }
func (c *checkoutBefore) applyCoupon(percent int)        { c.cartCoupon = percent }
func (c *checkoutBefore) cartTotal() int {
	sum := 0
	for _, x := range c.cartItems {
		sum += x
	}
	return sum - (sum*c.cartCoupon+50)/100
}
func (c *checkoutBefore) shippingLabel() string {
	return strings.Join([]string{c.shipName, c.shipStreet, c.shipCity + ", " + c.shipCountry}, "\n")
}
func (c *checkoutBefore) isDomestic() bool { return c.shipCountry == "NL" }

// After: each group of fields has a type, and the checkout uses both.
type cart struct {
	items  []int
	coupon int
}

func (c *cart) add(name string, cents int) { c.items = append(c.items, cents) }
func (c *cart) applyCoupon(percent int)    { c.coupon = percent }
func (c *cart) total() int {
	sum := 0
	for _, x := range c.items {
		sum += x
	}
	return sum - (sum*c.coupon+50)/100
}

type address struct{ name, street, city, country string }

func (a address) label() string {
	return strings.Join([]string{a.name, a.street, a.city + ", " + a.country}, "\n")
}

type checkout struct {
	cart    *cart
	address address
}

func (c checkout) isDomestic() bool { return c.address.country == "NL" }

func main() {
	before := &checkoutBefore{}
	before.addItem("lamp", 3999)
	before.addItem("bulb", 499)
	before.applyCoupon(10)
	before.shipName, before.shipStreet, before.shipCity, before.shipCountry = "Ada", "Keizersgracht 1", "Amsterdam", "NL"

	c := &cart{}
	c.add("lamp", 3999)
	c.add("bulb", 499)
	c.applyCoupon(10)
	after := checkout{c, address{"Ada", "Keizersgracht 1", "Amsterdam", "NL"}}

	if after.cart.total() != before.cartTotal() || after.cart.total() != 4048 {
		panic("total")
	}
	if after.address.label() != before.shippingLabel() || after.isDomestic() != before.isDomestic() {
		panic("address")
	}
	fmt.Println("checkout: tests pass")
}
```

```output
checkout: tests pass
```

- `address` is a small value type with no pointer, so a copy of it is a full, independent address.
- `cart` has state that changes, so the checkout holds a pointer to it, and its methods have pointer receivers.

```go run id=stop-the-trade
package main

import "fmt"

// Before: billing reads and writes the account's fields.
type accountBefore struct {
	Payments []int
	Balance  int
}

func payBefore(a *accountBefore, cents int) {
	a.Payments = append(a.Payments, cents)
	a.Balance -= cents
}

type bill struct{ charged, paid, due int }

func invoiceBefore(a *accountBefore, charged int) bill {
	a.Balance += charged
	paid := 0
	for _, p := range a.Payments {
		paid += p
	}
	return bill{charged, paid, a.Balance}
}

// After: unexported fields, exported methods. Outside this package only the methods are visible.
type Account struct {
	payments []int
	charges  []int
}

func sum(xs []int) int {
	t := 0
	for _, x := range xs {
		t += x
	}
	return t
}

func (a *Account) Pay(cents int)    { a.payments = append(a.payments, cents) }
func (a *Account) Charge(cents int) { a.charges = append(a.charges, cents) }
func (a *Account) Paid() int        { return sum(a.payments) }
func (a *Account) Due() int         { return sum(a.charges) - a.Paid() }

func invoice(a *Account, charged int) bill {
	a.Charge(charged)
	return bill{charged, a.Paid(), a.Due()}
}

func main() {
	before := &accountBefore{}
	after := &Account{}
	payBefore(before, 1500)
	after.Pay(1500)
	if invoice(after, 4000) != invoiceBefore(before, 4000) {
		panic("first invoice")
	}
	if got := invoice(after, 1000); got != (bill{1000, 1500, 3500}) {
		panic(fmt.Sprint("second invoice ", got))
	}
	fmt.Println("invoice: tests pass")
}
```

```output
invoice: tests pass
```

- Go controls visibility by the case of the first letter, and the unit of visibility is the package, not the type. In one file, as here, `main` could still read `a.payments`. The protection is real when `Account` is in its own package.
- A struct value of type `bill` can be compared with `==` because all its fields are comparable, so the test compares whole invoices.

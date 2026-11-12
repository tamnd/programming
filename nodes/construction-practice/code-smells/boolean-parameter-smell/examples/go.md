```go run id=name-the-choice
package main

import "fmt"

type Booking struct {
	Customer, Seat string
	Fee            int
}

// Before: a flag argument chooses the operation.
func bookBefore(customer string, isPremium bool) Booking {
	if isPremium {
		return Booking{customer, "front", 40}
	}
	return Booking{customer, "general", 0}
}

// After, the callers pass literals: a function for each operation.
func PremiumBook(customer string) Booking { return Booking{customer, "front", 40} }
func RegularBook(customer string) Booking { return Booking{customer, "general", 0} }

// After, the callers pass a value they read: a named type with constants.
type Tier int

const (
	Regular Tier = iota + 1
	Premium
)

func ParseTier(s string) (Tier, error) {
	switch s {
	case "premium":
		return Premium, nil
	case "regular":
		return Regular, nil
	}
	return 0, fmt.Errorf("unknown tier %q", s)
}

func Book(customer string, tier Tier) Booking {
	if tier == Premium {
		return PremiumBook(customer)
	}
	return RegularBook(customer)
}

func main() {
	if PremiumBook("ada") != bookBefore("ada", true) || RegularBook("ada") != bookBefore("ada", false) {
		panic("explicit functions")
	}
	t, err := ParseTier("premium")
	if err != nil || Book("ada", t) != bookBefore("ada", true) {
		panic("tier")
	}
	if _, err := ParseTier("gold"); err == nil {
		panic("an unknown tier should be refused")
	}
	fmt.Println("booking: tests pass")
}
```

```output
booking: tests pass
```

A call `Book("ada", Premium)` names the choice, and the compiler refuses a `bool` where a `Tier` is expected.

```go run id=money-type
package main

import (
	"fmt"
	"regexp"
)

type line struct {
	cents    int64
	currency string
}

// Before: money is a number of cents and a separate string.
func totalBefore(lines []line) int64 {
	var sum int64
	for _, l := range lines {
		sum += l.cents
	}
	return sum
}

func formatBefore(cents int64, currency string) string {
	return fmt.Sprintf("%d.%02d %s", cents/100, cents%100, currency)
}

// After: a defined type for the amount, and Money for the amount and its currency.
type Cents int64

type Money struct {
	cents    Cents
	currency string
}

var currencyCode = regexp.MustCompile(`^[A-Z]{3}$`)

func NewMoney(cents Cents, currency string) (Money, error) {
	if !currencyCode.MatchString(currency) {
		return Money{}, fmt.Errorf("currency code %q", currency)
	}
	return Money{cents, currency}, nil
}

func (m Money) Add(other Money) (Money, error) {
	if other.currency != m.currency {
		return Money{}, fmt.Errorf("cannot add %s to %s", other.currency, m.currency)
	}
	return Money{m.cents + other.cents, m.currency}, nil
}

func (m Money) String() string {
	return fmt.Sprintf("%d.%02d %s", m.cents/100, m.cents%100, m.currency)
}

func total(prices []Money) (Money, error) {
	sum := prices[0]
	for _, p := range prices[1:] {
		var err error
		if sum, err = sum.Add(p); err != nil {
			return Money{}, err
		}
	}
	return sum, nil
}

func must(m Money, err error) Money {
	if err != nil {
		panic(err)
	}
	return m
}

func check(ok bool, what string) {
	if !ok {
		panic(what)
	}
}

func main() {
	lines := []line{{1250, "EUR"}, {800, "EUR"}, {500, "USD"}}
	check(formatBefore(totalBefore(lines), "EUR") == "25.50 EUR", "before") // 5 dollars as 5 euros
	fmt.Println("before: the total mixes two currencies")

	euros := []Money{must(NewMoney(1250, "EUR")), must(NewMoney(800, "EUR"))}
	sum, err := total(euros)
	check(err == nil && sum.String() == "20.50 EUR", "total")
	_, err = total(append(euros, must(NewMoney(500, "USD"))))
	check(err != nil, "mixed currencies")
	_, err = NewMoney(100, "euro")
	check(err != nil, "bad code")
	check(must(NewMoney(100, "EUR")) == must(NewMoney(100, "EUR")), "equal by value")
	fmt.Println("after: tests pass")
}
```

```output
before: the total mixes two currencies
after: tests pass
```

- `Cents` is a defined type, so an `int64` variable must be converted with `Cents(n)` before it can be passed where `Cents` is expected. An untyped constant, such as `1250`, converts without one.
- Because `Money` has a `String` method, `fmt.Println(m)` and `%v` print it in the format of the type.

```go run id=kinds
package main

import "fmt"

// Go has no inheritance, so the "class" form embeds a step interface.
type Step interface{ Discount(amount int) int }

type Checkout struct{ step Step }

func (c Checkout) Total(prices []int) int {
	sum := 0
	for _, p := range prices {
		sum += p
	}
	return c.step.Discount(sum)
}

type Sale struct{}

func (Sale) Discount(amount int) int { return amount * 9 / 10 }

type Till struct{ discount func(int) int }

func (t Till) Total(prices []int) int {
	sum := 0
	for _, p := range prices {
		sum += p
	}
	return t.discount(sum)
}

func main() {
	prices := []int{40, 60}
	fmt.Println("class pattern:", Checkout{Sale{}}.Total(prices))
	till := Till{func(amount int) int { return amount * 9 / 10 }}
	fmt.Println("object pattern:", till.Total(prices))
	till.discount = func(amount int) int { return amount - 20 }
	fmt.Println("changed at run time:", till.Total(prices))
}
```

```output
class pattern: 90
object pattern: 90
changed at run time: 80
```

- Verdict: idiom. Go has no subclasses, so the step of the first form is an interface value that the checkout holds. The strategy is a function value.

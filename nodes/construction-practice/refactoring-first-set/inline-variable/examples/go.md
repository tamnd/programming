```go run id=use-the-expression
package main

import "fmt"

type order struct{ basePrice int }

// Before: a variable that adds no meaning.
func isLargeBefore(o order) bool {
	basePrice := o.basePrice
	return basePrice > 1000
}

// After: the expression in place.
func isLarge(o order) bool {
	return o.basePrice > 1000
}

func sum(xs []int) int {
	t := 0
	for _, x := range xs {
		t += x
	}
	return t
}

// A variable that must stay: the slice changes between the declaration and the use.
func totalWithFee(items []int) int {
	subtotal := sum(items)
	items = append(items, 5) // the fee is an item too
	return subtotal + 5
}

func totalWithFeeWrong(items []int) int {
	items = append(items, 5)
	return sum(items) + 5 // inlined after the append
}

// Shadowing: inside the block, x names another variable, so s cannot
// be replaced by its initializer there. gopls refuses this inline.
func shadow(x int) (string, string) {
	s := fmt.Sprintf("+%d", x)
	{
		x := 123
		return s, fmt.Sprintf("+%d", x)
	}
}

func main() {
	for _, p := range []int{0, 1000, 1001} {
		if isLarge(order{p}) != isLargeBefore(order{p}) {
			panic("behaviour changed")
		}
	}
	if totalWithFee([]int{10, 20}) != 35 || totalWithFeeWrong([]int{10, 20}) != 40 {
		panic("unexpected totals")
	}
	kept, inlined := shadow(7)
	fmt.Println(kept, inlined)
	fmt.Println("inline variable: tests pass")
}
```

```output
+7 +123
inline variable: tests pass
```

```go run id=two-copies
package main

import "fmt"

// The rule "orders of 50 or more ship free" is written twice.
func bannerText(total int) string {
	if total >= 50 {
		return "Free shipping"
	}
	return "Shipping: 5"
}

// The business changes the limit to 40. Only one copy is updated.
func shippingCost(total int) int {
	if total >= 40 {
		return 0
	}
	return 5
}

func main() {
	fmt.Println(shippingCost(45), bannerText(45))
}
```

```output
0 Shipping: 5
```

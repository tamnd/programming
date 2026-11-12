```go run id=four-rules
package main

import "fmt"

const memberDiscount = 0.1

func priceAfterDiscount(amount float64, isMember bool) float64 {
	if isMember || amount > 100 {
		return amount * (1 - memberDiscount)
	}
	return amount
}

func main() {
	for _, c := range []struct {
		amount float64
		member bool
		want   float64
	}{{50, false, 50}, {50, true, 45}, {200, false, 180}} {
		if got := priceAfterDiscount(c.amount, c.member); got != c.want {
			fmt.Println("fail", c, got)
		}
	}
	fmt.Println(priceAfterDiscount(50, false), priceAfterDiscount(50, true), priceAfterDiscount(200, false))
}
```

```output
50 45 180
```

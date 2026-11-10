```go wrong id=wrap
package main

import "fmt"

// orderFits reports whether the order costs no more than the budget.
func orderFits(price, quantity, budget int32) bool {
	total := price * quantity // wraps on overflow
	return total <= budget
}

func main() {
	fmt.Println("3 items:", orderFits(100, 3, 1000))
	fmt.Println("42949673 items:", orderFits(100, 42949673, 1000))
}
```

```output for=wrap
3 items: true
42949673 items: true
```

The product wraps to 4, so the large order fits. The Go specification says that signed overflow does not cause a run-time panic.

```go run id=check
package main

import (
	"fmt"
	"math"
)

// orderFits reports whether the order costs no more than the budget.
func orderFits(price, quantity, budget int32) bool {
	if price < 0 || quantity < 0 {
		return false
	}
	if quantity != 0 && price > math.MaxInt32/quantity {
		return false // the total does not fit in an int32
	}
	return price*quantity <= budget
}

func main() {
	fmt.Println("3 items:", orderFits(100, 3, 1000))
	fmt.Println("42949673 items:", orderFits(100, 42949673, 1000))
}
```

```output for=check
3 items: true
42949673 items: false
```

- Verdict: absent. Go defines the result of overflow: unsigned values wrap modulo 2ⁿ, and signed values wrap in two's complement. There is no run-time check.
- The specification says that a compiler may not assume that `x < x + 1` is always true, so a check written in Go is not removed by the optimizer.
- Check the operands before the operation, with the constants of the package `math`. `math/bits.Mul64` and `bits.Add64` return the high word or the carry for unsigned values.
- A conversion between integer types, such as `int32(n)`, truncates silently. Check the range first.

```go wrong id=atoi
package main

import (
	"fmt"
	"strconv"
)

var prices = map[string]int{"pen": 150, "book": 1200} // in cents

func orderTotal(product, quantity string) (int, error) {
	n, err := strconv.Atoi(quantity)
	if err != nil {
		return 0, err
	}
	return prices[product] * n, nil
}

func main() {
	for _, q := range []string{"2", "-3", "+2"} {
		total, err := orderTotal("book", q)
		fmt.Println(q, total, err)
	}
	total, err := orderTotal("boat", "1")
	fmt.Println("boat", total, err)
}
```

```output for=atoi
2 2400 <nil>
-3 -3600 <nil>
+2 2400 <nil>
boat 0 <nil>
```

```go run id=parse
package main

import (
	"errors"
	"fmt"
	"strconv"
)

var prices = map[string]int{"pen": 150, "book": 1200} // in cents

// Order has unexported fields. Code in other packages can only get one
// from ParseOrder, so every Order it sees was checked.
type Order struct {
	product  string
	quantity int
}

func ParseOrder(product, quantity string) (Order, error) {
	if _, ok := prices[product]; !ok {
		return Order{}, errors.New("unknown product")
	}
	if len(quantity) < 1 || len(quantity) > 3 {
		return Order{}, errors.New("quantity must be 1 to 3 ASCII digits")
	}
	for _, c := range []byte(quantity) {
		if c < '0' || c > '9' {
			return Order{}, errors.New("quantity must be 1 to 3 ASCII digits")
		}
	}
	n, err := strconv.Atoi(quantity)
	if err != nil {
		return Order{}, err
	}
	if n < 1 || n > 100 {
		return Order{}, errors.New("quantity must be from 1 to 100")
	}
	return Order{product, n}, nil
}

func (o Order) Total() int { return prices[o.product] * o.quantity }

func main() {
	inputs := [][2]string{{"book", "2"}, {"book", "-3"}, {"book", "+2"}, {"boat", "1"}}
	for _, in := range inputs {
		order, err := ParseOrder(in[0], in[1])
		if err != nil {
			fmt.Println("rejected:", err)
			continue
		}
		fmt.Println(order.Total())
	}
}
```

```output for=parse
2400
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: unknown product
```

- Verdict: stdlib. `strconv` converts and reports errors; the parser adds the checks of format and range.
- `strconv.Atoi` accepts a leading `+` or `-`. It rejects white space and non-ASCII digits. It returns an error when the value does not fit in an `int`.
- A missing key in a Go map gives the zero value. In the first program, the unknown product `boat` costs `0`, and no error is reported. Use the two-value form `v, ok := m[k]`.
- Unexported fields make the parsed type a real guarantee across packages: other packages can only get an `Order` from `ParseOrder`. Inside the same package, code can still write `Order{}`, so keep the type in a small package of its own.
- `go test -fuzz` runs a fuzz target in the standard toolchain. A target that calls `ParseOrder` and checks that every accepted order has a quantity from 1 to 100 tests the rule directly.

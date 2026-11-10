```go norun reason=fragment id=cart file=cart.go
package cart

import "fmt"

// Item is one line of a cart: a price in cents and a quantity.
type Item struct {
	Price    int
	Quantity int
}

// Total returns the total of the items in cents.
func Total(items []Item) (int, error) {
	total := 0
	for _, item := range items {
		if item.Quantity < 0 {
			return 0, fmt.Errorf("negative quantity: %d", item.Quantity)
		}
		total += item.Price * item.Quantity
	}
	return total, nil
}
```

```go norun reason=fragment id=cart-test file=cart_test.go
package cart

import "testing"

func TestTotal(t *testing.T) {
	tests := []struct {
		name  string
		items []Item
		want  int
	}{
		{"empty cart is zero", nil, 0},
		{"sums price times quantity", []Item{{250, 2}, {100, 1}}, 600},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := Total(tt.items)
			if err != nil {
				t.Fatalf("Total(%v) returned error %v", tt.items, err)
			}
			if got != tt.want {
				t.Errorf("Total(%v) = %d, want %d", tt.items, got, tt.want)
			}
		})
	}
}

func TestTotalRejectsNegativeQuantity(t *testing.T) {
	if _, err := Total([]Item{{250, -1}}); err == nil {
		t.Error("Total with quantity -1 returned no error")
	}
}
```

The command `go test -v` in the directory of the two files prints:

```text
=== RUN   TestTotal
=== RUN   TestTotal/empty_cart_is_zero
=== RUN   TestTotal/sums_price_times_quantity
--- PASS: TestTotal (0.00s)
    --- PASS: TestTotal/empty_cart_is_zero (0.00s)
    --- PASS: TestTotal/sums_price_times_quantity (0.00s)
=== RUN   TestTotalRejectsNegativeQuantity
--- PASS: TestTotalRejectsNegativeQuantity (0.00s)
PASS
ok  	example.com/cart	0.173s
```

- Verdict: stdlib. The `testing` package and the `go test` command are part of the Go distribution. A file whose name ends in `_test.go` holds the tests, and `go build` leaves it out.
- A test is a function `TestXxx(t *testing.T)`. `t.Errorf` records a failure and goes on, and `t.Fatalf` records a failure and stops the test.
- Go has no assertion functions in the standard library. The test compares the values with `if` and writes its own message. Libraries such as testify add assertions.
- The table of cases with `t.Run` for each case is the usual form of a Go test. Each case is a subtest with its own name in the report.
- These blocks do not run on this page, because `go test` builds the test binary and its `main` function, and the runner of this site runs `go build` on one `main` package.

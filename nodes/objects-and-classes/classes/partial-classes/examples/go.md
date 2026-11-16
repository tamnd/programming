```go run id=methods-in-any-file
package main

import "fmt"

// Each part can be in its own file of the package, such as order.go and total.go.

func (o Order) Total() int { return o.Count * o.Price } // declared before the type

type Order struct {
	Count, Price int
}

func (o Order) String() string { return fmt.Sprintf("Order(%d, %d)", o.Count, o.Price) }

func main() {
	o := Order{Count: 3, Price: 40}
	fmt.Println(o.Total(), o)
}
```

```output for=methods-in-any-file
120 Order(3, 40)
```

- Verdict: idiom. Go has no partial types, but a method can be in any file of the package of its type, and the order of declarations does not matter. `Total` is declared before `Order` here.
- The fields cannot be split: the `struct` type is one declaration. Code in another package cannot add a method to `Order`.

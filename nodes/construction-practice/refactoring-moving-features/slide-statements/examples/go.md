```go run id=keep-together
package main

import "fmt"

type Booking struct{ Nights, RateCents, Stays int }

func price(b Booking) int {
	base := b.Nights * b.RateCents
	tax := base / 10

	// The discount lines slid together; they can now be extracted.
	loyal := b.Stays >= 5
	discount := 0
	if loyal {
		discount = base * 5 / 100
	}

	return base + tax - discount // slid out of both branches
}

func main() {
	if got := price(Booking{3, 9000, 1}); got != 29700 {
		panic(got)
	}
	if got := price(Booking{2, 12000, 7}); got != 25200 {
		panic(got)
	}
	fmt.Println("slide statements: tests pass")
}
```

```output
slide statements: tests pass
```

```go run id=one-job
package main

import "fmt"

type Order struct{ ID, Cents int }

func revenueOf(orders []Order) int {
	total := 0
	for _, o := range orders {
		total += o.Cents
	}
	return total
}

// largestOf returns the ID of the largest order, or 0 if there are none.
func largestOf(orders []Order) int {
	id, most := 0, -1
	for _, o := range orders {
		if o.Cents > most {
			id, most = o.ID, o.Cents
		}
	}
	return id
}

func main() {
	orders := []Order{{1, 1200}, {2, 5400}, {3, 800}}
	if revenueOf(orders) != 7400 || largestOf(orders) != 2 {
		panic("split loop")
	}
	if revenueOf(nil) != 0 || largestOf(nil) != 0 {
		panic("empty")
	}
	fmt.Println("split loop: tests pass")
}
```

```output
split loop: tests pass
```

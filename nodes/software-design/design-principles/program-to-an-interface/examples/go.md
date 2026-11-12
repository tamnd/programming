```go run id=sales-store
package main

import "fmt"

// SalesStore is the interface that the report depends on.
type SalesStore interface {
	SalesOn(day string) []int
}

func dailyReport(s SalesStore, day string) string {
	sales := s.SalesOn(day)
	total := 0
	for _, x := range sales {
		total += x
	}
	return fmt.Sprintf("%s: %d sales, total %d", day, len(sales), total)
}

type row struct {
	day    string
	amount int
}

// sliceStore implements SalesStore with no implements clause.
type sliceStore []row

func (s sliceStore) SalesOn(day string) []int {
	var out []int
	for _, r := range s {
		if r.day == day {
			out = append(out, r.amount)
		}
	}
	return out
}

type fakeStore struct{}

func (fakeStore) SalesOn(string) []int { return []int{5, 7} }

func main() {
	s := sliceStore{{"mon", 5}, {"mon", 7}, {"tue", 1}}
	if dailyReport(s, "mon") != dailyReport(fakeStore{}, "mon") {
		fmt.Println("mismatch")
	}
	fmt.Println(dailyReport(s, "mon"))
}
```

```output
mon: 2 sales, total 12
```

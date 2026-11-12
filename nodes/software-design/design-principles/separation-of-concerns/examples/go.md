```go run id=three-concerns
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type Item struct {
	Name  string
	Qty   int
	Price float64
}

type Row struct {
	Name string
	Cost float64
}

func parse(text string) ([]Item, error) {
	var items []Item
	for _, line := range strings.Split(text, "\n") {
		f := strings.Split(line, ",")
		if len(f) != 3 {
			return nil, fmt.Errorf("bad line %q", line)
		}
		qty, err := strconv.Atoi(f[1])
		if err != nil {
			return nil, err
		}
		price, err := strconv.ParseFloat(f[2], 64)
		if err != nil {
			return nil, err
		}
		items = append(items, Item{f[0], qty, price})
	}
	return items, nil
}

func priced(items []Item) []Row {
	rows := make([]Row, 0, len(items))
	for _, i := range items {
		rows = append(rows, Row{i.Name, float64(i.Qty) * i.Price})
	}
	return rows
}

func format(rows []Row) string {
	var b strings.Builder
	total := 0.0
	for _, r := range rows {
		fmt.Fprintf(&b, "%s: %.2f\n", r.Name, r.Cost)
		total += r.Cost
	}
	fmt.Fprintf(&b, "total: %.2f", total)
	return b.String()
}

func main() {
	items, err := parse("apples,3,0.5\npears,2,0.75")
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(format(priced(items)))
}
```

```output
apples: 1.50
pears: 1.50
total: 3.00
```

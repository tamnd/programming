```go run id=one-rule
package main

import "fmt"

type Item struct{ Backordered bool }

type Order struct {
	Items   []Item
	Country string
}

func (o Order) hasBackorder() bool {
	for _, i := range o.Items {
		if i.Backordered {
			return true
		}
	}
	return false
}

func (o Order) shipsTodayBefore(hour int) bool {
	if hour >= 15 {
		return false
	}
	if o.hasBackorder() {
		return false
	}
	if o.Country != "NO" {
		return false
	}
	return true
}

func (o Order) missesTodaysDispatch(hour int) bool {
	return hour >= 15 || o.hasBackorder() || o.Country != "NO"
}

func (o Order) shipsToday(hour int) bool { return !o.missesTodaysDispatch(hour) }

func main() {
	for _, hour := range []int{9, 15} {
		for _, items := range [][]Item{nil, {{true}}} {
			for _, c := range []string{"NO", "SE"} {
				o := Order{items, c}
				if o.shipsToday(hour) != o.shipsTodayBefore(hour) {
					panic(fmt.Sprint("differs: ", o, hour))
				}
			}
		}
	}
	fmt.Println("consolidate conditional expression: tests pass")
}
```

```output
consolidate conditional expression: tests pass
```

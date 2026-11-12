```go run id=fold-it-back
package main

import (
	"fmt"
	"net/url"
)

// Before: Order embeds a Delivery that holds one field.
type Delivery struct{ TrackingID string }

func (d Delivery) TrackingURL() string {
	return "https://track.example/" + url.PathEscape(d.TrackingID)
}

type OrderBefore struct {
	ID int
	Delivery
}

// After: the field moves up into Order, and Delivery is deleted.
type Order struct {
	ID         int
	TrackingID string
}

func (o Order) TrackingURL() string {
	return "https://track.example/" + url.PathEscape(o.TrackingID)
}

func main() {
	for _, t := range []string{"AB12", "x y/z"} {
		before := OrderBefore{1, Delivery{t}}
		after := Order{1, t}
		if before.TrackingURL() != after.TrackingURL() {
			panic(t)
		}
	}
	if got := (Order{1, "x y/z"}).TrackingURL(); got != "https://track.example/x%20y%2Fz" {
		panic(got)
	}
	fmt.Println("inline class: tests pass")
}
```

```output
inline class: tests pass
```

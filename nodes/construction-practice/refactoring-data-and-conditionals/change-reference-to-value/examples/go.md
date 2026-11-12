```go run id=replace-not-change
package main

import "fmt"

// DeliveryWindow is a value: assignment copies it, and == compares the fields.
type DeliveryWindow struct {
	Day, StartHour, EndHour int
}

func (w DeliveryWindow) MovedBy(days int) DeliveryWindow {
	w.Day += days // w is a copy
	return w
}

type Order struct {
	ID     string
	Window DeliveryWindow
}

func main() {
	morning := DeliveryWindow{3, 9, 12}
	c := Order{"c", morning}
	d := Order{"d", morning}
	c.Window = c.Window.MovedBy(1)
	if d.Window.Day != 3 || morning.Day != 3 {
		panic("moved")
	}
	if c.Window != (DeliveryWindow{4, 9, 12}) {
		panic("equality")
	}
	seen := map[DeliveryWindow]bool{c.Window: true}
	if !seen[DeliveryWindow{4, 9, 12}] {
		panic("map key")
	}
	fmt.Println("change reference to value: tests pass")
}
```

```output
change reference to value: tests pass
```

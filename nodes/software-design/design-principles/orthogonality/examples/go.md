```go run id=orthogonal-total
package main

import "fmt"

type Line struct {
	Qty, Price int
}

func invoiceTotal(lines []Line) int {
	total := 0
	for _, l := range lines {
		total += l.Qty * l.Price
	}
	return total
}

func main() {
	fmt.Println(invoiceTotal([]Line{{2, 3}, {1, 4}}))
}
```

```output
10
```

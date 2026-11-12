```go run id=counter
package main

import "fmt"

type Counter struct{ n int }

// Increment is a command: it changes state and returns nothing.
func (c *Counter) Increment() { c.n++ }

// Value is a query: it returns state and changes nothing.
func (c *Counter) Value() int { return c.n }

func main() {
	var c Counter
	c.Increment()
	fmt.Println(c.Value(), c.Value())
}
```

```output
1 1
```

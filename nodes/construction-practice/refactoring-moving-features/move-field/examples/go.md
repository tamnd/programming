```go run id=one-place
package main

import "fmt"

type Section struct {
	Name       string
	PriceCents int // moved here from Seat
}

type Seat struct {
	Row     string
	Number  int
	section *Section
}

func (s Seat) PriceCents() int { return s.section.PriceCents }

func main() {
	stalls := &Section{"stalls", 4500}
	seats := []Seat{{"A", 1, stalls}, {"A", 2, stalls}, {"B", 7, stalls}}
	stalls.PriceCents = 4900
	for _, s := range seats {
		if s.PriceCents() != 4900 {
			panic(s)
		}
	}
	fmt.Println("move field: tests pass")
}
```

```output
move field: tests pass
```

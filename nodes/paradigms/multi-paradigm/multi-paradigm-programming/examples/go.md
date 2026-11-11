```go run id=orders
package main

import (
	"fmt"
	"sort"
	"strconv"
	"strings"
)

type Order struct { // a value type
	Customer string
	Amount   int
}

func parse(line string) Order { // a pure function
	name, amount, _ := strings.Cut(line, ",")
	n, _ := strconv.Atoi(strings.TrimSpace(amount))
	return Order{strings.TrimSpace(name), n}
}

type Report interface{ Line(name string, total int) string } // satisfied implicitly

type Plain struct{}
type Csv struct{}

func (Plain) Line(n string, t int) string { return fmt.Sprintf("%s %d", n, t) }
func (Csv) Line(n string, t int) string   { return fmt.Sprintf("%s,%d", n, t) }

func main() {
	text := "ana, 30\nbo, 70\nana, 25\ncy, 10\nbo, 5"
	totals := map[string]int{}
	for _, line := range strings.Split(text, "\n") { // an imperative loop
		o := parse(line)
		totals[o.Customer] += o.Amount
	}
	var big []string
	for name, t := range totals {
		if t > 50 {
			big = append(big, name)
		}
	}
	sort.Slice(big, func(i, j int) bool { return totals[big[i]] > totals[big[j]] }) // a closure
	for _, r := range []Report{Plain{}, Csv{}} {
		for _, name := range big {
			fmt.Println(r.Line(name, totals[name]))
		}
	}
}
```

```output for=orders
bo 75
ana 55
bo,75
ana,55
```

- Verdict: native. Go is imperative first, with structs, methods, interfaces that types satisfy without declaring it, closures, and goroutines with channels for concurrency. It leaves out inheritance and has few functional helpers, so filtering is a loop, while sorting takes a closure. The order in which Go iterates over a map is not fixed, so the program sorts before it prints.

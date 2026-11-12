```go run id=precheck
package main

import "fmt"

var values = []float64{12.5, 8, 3.25}

// A read past the end of a slice panics in Go, so the check comes first.
func getValueForPeriod(period int) float64 {
	if period >= len(values) {
		return 0
	}
	return values[period]
}

func main() {
	var out []float64
	for p := 0; p < 4; p++ {
		out = append(out, getValueForPeriod(p))
	}
	fmt.Println(out)
}
```

```output
[12.5 8 3.25 0]
```

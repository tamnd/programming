```go run id=one-rate
package main

import "fmt"

const vatRate = 0.2

func taxOnly(net float64) float64 { return net * vatRate }

func priceWithTax(net float64) float64 { return net + taxOnly(net) }

// Two facts that only look the same stay apart.
const (
	maxUsernameLength = 20
	maxLoginAttempts  = 20
)

func main() {
	fmt.Println(priceWithTax(50), taxOnly(50), maxUsernameLength == maxLoginAttempts)
}
```

```output
60 10 true
```

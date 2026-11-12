```go run id=one-way-in
package main

import (
	"fmt"
	"sync"
)

type TaxSettings struct {
	Rate   float64
	Region string
}

// The variable is unexported, so code in other packages must use the functions.
var (
	mu       sync.Mutex
	settings = TaxSettings{Rate: 0.2, Region: "EU"}
)

// Settings returns a copy; a struct value is copied when it is returned.
func Settings() TaxSettings {
	mu.Lock()
	defer mu.Unlock()
	return settings
}

func SetSettings(s TaxSettings) error {
	if s.Rate < 0 || s.Rate >= 1 {
		return fmt.Errorf("bad tax rate %v", s.Rate)
	}
	mu.Lock()
	defer mu.Unlock()
	settings = s
	return nil
}

func priceWithTax(net float64) float64 {
	return net * (1 + Settings().Rate)
}

func main() {
	mine := Settings()
	mine.Rate = 0.9
	if Settings().Rate != 0.2 {
		panic("a reader changed the settings")
	}
	if SetSettings(TaxSettings{Rate: 1.5, Region: "EU"}) == nil {
		panic("the check did not run")
	}
	var wg sync.WaitGroup
	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func() { defer wg.Done(); _ = SetSettings(TaxSettings{Rate: 0.1, Region: "EU"}) }()
	}
	wg.Wait()
	fmt.Printf("%.0f\n", priceWithTax(100))
	fmt.Println("encapsulate variable: tests pass")
}
```

```output
110
encapsulate variable: tests pass
```

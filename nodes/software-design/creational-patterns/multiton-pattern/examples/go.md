```go run id=multiton
package main

import (
	"fmt"
	"sync"
)

type Currency struct {
	Code     string
	Decimals int
}

var (
	decimals  = map[string]int{"USD": 2, "EUR": 2, "JPY": 0} // the valid keys
	mu        sync.Mutex
	instances = map[string]*Currency{}
)

// Of returns the one *Currency for code.
func Of(code string) (*Currency, error) {
	d, ok := decimals[code]
	if !ok {
		return nil, fmt.Errorf("%s: unknown currency", code)
	}
	mu.Lock()
	defer mu.Unlock()
	c, ok := instances[code]
	if !ok {
		c = &Currency{Code: code, Decimals: d}
		instances[code] = c // one instance per key
	}
	return c, nil
}

func count() int {
	mu.Lock()
	defer mu.Unlock()
	return len(instances)
}

func main() {
	usd, _ := Of("USD")
	jpy, _ := Of("JPY")
	fmt.Println("USD has", usd.Decimals, "decimals")
	fmt.Println("JPY has", jpy.Decimals, "decimals")
	again, _ := Of("USD")
	fmt.Println("same USD object:", usd == again)
	fmt.Println("instances:", count())
	if _, err := Of("XYZ"); err != nil {
		fmt.Println(err)
	}

	var wg sync.WaitGroup
	var seenMu sync.Mutex
	seen := map[*Currency]bool{}
	for range 8 {
		wg.Add(1)
		go func() {
			defer wg.Done()
			c, _ := Of("EUR")
			seenMu.Lock()
			seen[c] = true
			seenMu.Unlock()
		}()
	}
	wg.Wait()
	fmt.Println("8 goroutines ask for EUR:", len(seen), "object")
	fmt.Println("instances:", count())
}
```

```output
USD has 2 decimals
JPY has 0 decimals
same USD object: true
instances: 2
XYZ: unknown currency
8 goroutines ask for EUR: 1 object
instances: 3
```

- Verdict: idiom. A package-level map behind a `sync.Mutex` is the registry, and a pointer is the identity. Go has no private constructor, so other packages could still build a `Currency` value; an unexported type with an exported interface closes that gap.
- Go reports an unknown key with an `error` value in place of an exception.

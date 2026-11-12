```go run id=extract-shared-weight
package main

import (
	"fmt"
	"math"
)

type item struct {
	kg  float64
	qty int
}

// Before: the weight rule is written twice.
func domesticBefore(items []item) int {
	weight := 0.0
	for _, it := range items {
		weight += it.kg * float64(it.qty)
	}
	return 5 + int(math.Ceil(weight))*2
}

func internationalBefore(items []item) int {
	weight := 0.0
	for _, it := range items {
		weight += it.kg * float64(it.qty)
	}
	return 20 + int(math.Ceil(weight))*6
}

// After: the weight rule has one home.
func billableKg(items []item) int {
	weight := 0.0
	for _, it := range items {
		weight += it.kg * float64(it.qty)
	}
	return int(math.Ceil(weight))
}

func domestic(items []item) int      { return 5 + billableKg(items)*2 }
func international(items []item) int { return 20 + billableKg(items)*6 }

func main() {
	items := []item{{0.4, 3}, {1.1, 1}}
	for _, v := range []struct {
		name      string
		dom, intl func([]item) int
	}{
		{"before", domesticBefore, internationalBefore},
		{"after", domestic, international},
	} {
		if v.dom(items) != 11 || v.intl(items) != 38 || v.dom(nil) != 5 {
			panic(v.name)
		}
		fmt.Println(v.name + ": tests pass")
	}
}
```

```output
before: tests pass
after: tests pass
```

- Go has no implicit conversion from `int` to `float64`, so `float64(it.qty)` appears in each copy of the loop. The extract puts the conversion in one place.
- `billableKg` starts with a lowercase letter, so it is not exported. The extract does not change the API of the package.

```go run id=loop-to-pipeline
package main

import (
	"fmt"
	"slices"
	"strings"
)

type order struct {
	email, country string
	paid           bool
}

// Before: one loop body chooses, changes and collects.
func emailsBefore(orders []order, country string) []string {
	var result []string
	for _, o := range orders {
		if o.country == country {
			if o.paid {
				email := strings.ToLower(o.email)
				if !slices.Contains(result, email) {
					result = append(result, email)
				}
			}
		}
	}
	slices.Sort(result)
	return result
}

// After: the loop only chooses and changes; the library does the rest.
func emails(orders []order, country string) []string {
	var result []string
	for _, o := range orders {
		if o.country == country && o.paid {
			result = append(result, strings.ToLower(o.email))
		}
	}
	slices.Sort(result)
	return slices.Compact(result)
}

func main() {
	orders := []order{
		{"Ana@example.com", "PT", true},
		{"bo@example.com", "SE", true},
		{"ana@example.com", "PT", true},
		{"cy@example.com", "PT", false},
		{"Dee@example.com", "PT", true},
	}
	want := []string{"ana@example.com", "dee@example.com"}
	if !slices.Equal(emailsBefore(orders, "PT"), want) {
		panic("before")
	}
	if !slices.Equal(emails(orders, "PT"), want) {
		panic("after")
	}
	if len(emails(orders, "FR")) != 0 {
		panic("empty")
	}
	fmt.Println("emails: tests pass")
}
```

```output
emails: tests pass
```

- `slices.Compact` removes only the duplicates that are next to each other, so it must come after `slices.Sort`.
- The second version keeps a loop, which is the usual Go form, but the loop has one job. The nested `if` statements are one condition, and the duplicate check is a library call.

```go run id=status-list
package main

import (
	"fmt"
	"slices"
)

// Two copies of the same knowledge.
var menuStatuses = []string{"open", "closed", "archived"}

func isValidCopy(s string) bool { return slices.Contains([]string{"open", "closed"}, s) } // not updated

// One source of truth.
var statuses = []string{"open", "closed", "archived"}
var menu = statuses

func isValid(s string) bool { return slices.Contains(statuses, s) }

func main() {
	fmt.Printf("copies: menu %d, archived valid %t\n", len(menuStatuses), isValidCopy("archived"))
	fmt.Printf("single source: menu %d, archived valid %t\n", len(menu), isValid("archived"))
}
```

```output
copies: menu 3, archived valid false
single source: menu 3, archived valid true
```

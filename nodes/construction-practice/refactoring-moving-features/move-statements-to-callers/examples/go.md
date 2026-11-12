```go run id=let-callers-vary
package main

import (
	"fmt"
	"io"
	"strings"
)

type Item struct {
	Name  string
	Cents int
}

// After: WriteReceipt writes only what all callers share.
func WriteReceipt(w io.Writer, items []Item) {
	for _, it := range items {
		fmt.Fprintf(w, "%s %d.%02d\n", it.Name, it.Cents/100, it.Cents%100)
	}
}

func main() {
	items := []Item{{"Tea", 350}, {"Scone", 275}}

	var till strings.Builder
	WriteReceipt(&till, items)
	till.WriteString("Thank you!\n") // moved to the caller

	var email strings.Builder
	WriteReceipt(&email, items)
	email.WriteString("Thank you!\nUnsubscribe: example.com/u\n") // this caller varies it

	if till.String() != "Tea 3.50\nScone 2.75\nThank you!\n" {
		panic(till.String())
	}
	if !strings.HasPrefix(email.String(), till.String()) {
		panic(email.String())
	}
	fmt.Println("move statements to callers: tests pass")
}
```

```output
move statements to callers: tests pass
```

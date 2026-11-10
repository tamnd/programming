```go run id=invoice
package main

import (
	"fmt"
	"strings"
)

type Item struct {
	Name     string
	Price    int
	Quantity int
}

func invoiceBefore(customer string, items []Item) string {
	var b strings.Builder
	fmt.Fprintf(&b, "Invoice for %s\n", customer)
	for _, item := range items {
		fmt.Fprintf(&b, "  %s: %d x %d.%02d\n", item.Name, item.Quantity, item.Price/100, item.Price%100)
	}
	// compute the total
	total := 0
	for _, item := range items {
		total += item.Price * item.Quantity
	}
	fmt.Fprintf(&b, "Total: %d.%02d", total/100, total%100)
	return b.String()
}

func formatCents(cents int) string {
	return fmt.Sprintf("%d.%02d", cents/100, cents%100)
}

func totalCents(items []Item) int {
	total := 0
	for _, item := range items {
		total += item.Price * item.Quantity
	}
	return total
}

func invoiceAfter(customer string, items []Item) string {
	var b strings.Builder
	fmt.Fprintf(&b, "Invoice for %s\n", customer)
	for _, item := range items {
		fmt.Fprintf(&b, "  %s: %d x %s\n", item.Name, item.Quantity, formatCents(item.Price))
	}
	fmt.Fprintf(&b, "Total: %s", formatCents(totalCents(items)))
	return b.String()
}

func main() {
	items := []Item{{"pen", 250, 2}, {"pad", 1205, 1}}
	expected := "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05"
	versions := []struct {
		name    string
		invoice func(string, []Item) string
	}{{"invoiceBefore", invoiceBefore}, {"invoiceAfter", invoiceAfter}}
	for _, v := range versions {
		if v.invoice("Ada", items) != expected || v.invoice("Bob", nil) != "Invoice for Bob\nTotal: 0.00" {
			panic(v.name + " failed")
		}
		fmt.Println(v.name + ": tests pass")
	}
	fmt.Println(invoiceAfter("Ada", items))
}
```

```output for=invoice
invoiceBefore: tests pass
invoiceAfter: tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

- Verdict: idiom. The refactoring makes a function, or a method when the fragment uses a receiver.
- Go functions return several values, so a fragment that assigns two variables can return both: `count, total := addUp(items)`.
- The usual test is a `_test.go` file that runs under `go test`. This page checks both versions in `main` so that the block runs on its own.
- gopls offers the code actions `refactor.extract.function` and `refactor.extract.method` for a selection of statements.

```go run id=remove-the-hooks
package main

import (
	"fmt"
	"strings"
)

// Before: an options struct whose fields no caller sets.
type ExportOptions struct {
	Delimiter string
	LineEnd   string
	Header    bool
}

func exportBefore(rows [][]string, opts ExportOptions) string {
	d, le := opts.Delimiter, opts.LineEnd
	if d == "" {
		d = ","
	}
	if le == "" {
		le = "\n"
	}
	start := 0
	if !opts.Header && len(rows) > 0 && rows[0][0] == "#" {
		start = 1 // a rule for a header format that no input has
	}
	lines := make([]string, 0, len(rows))
	for _, r := range rows[start:] {
		lines = append(lines, strings.Join(r, d))
	}
	return strings.Join(lines, le)
}

// After: the one form that the program uses.
func toCSV(rows [][]string) string {
	lines := make([]string, 0, len(rows))
	for _, r := range rows {
		lines = append(lines, strings.Join(r, ","))
	}
	return strings.Join(lines, "\n")
}

func main() {
	rows := [][]string{{"id", "name"}, {"1", "Ada"}, {"2", "Grace"}}
	if exportBefore(rows, ExportOptions{}) != "id,name\n1,Ada\n2,Grace" {
		panic("before")
	}
	if toCSV(rows) != exportBefore(rows, ExportOptions{}) {
		panic("after")
	}
	fmt.Println("toCSV: tests pass")
}
```

```output
toCSV: tests pass
```

- Each caller writes `ExportOptions{}`, so every field has its zero value, and the code for the other values never runs. A search for `ExportOptions{` with fields set finds no caller.
- The `Header` rule is the kind of special case that *Refactoring* names: code for an input that the program does not have.

```go run id=export
package main

import (
	"fmt"
	"strings"
)

type Writer interface {
	Header() string
	Row(name string, count int) string
}

type csvWriter struct{}

func (csvWriter) Header() string                    { return "name,count" }
func (csvWriter) Row(name string, count int) string { return fmt.Sprintf("%s,%d", name, count) }

type textWriter struct{}

func (textWriter) Header() string                    { return "Inventory" }
func (textWriter) Row(name string, count int) string { return fmt.Sprintf("%s: %d", name, count) }

// Exporter is the creator. Go has no inheritance, so the factory method is
// an interface method, and the template method is a function that takes it.
type Exporter interface {
	NewWriter() Writer
}

type Row struct {
	Name  string
	Count int
}

func Export(e Exporter, rows []Row) string {
	w := e.NewWriter() // the factory method
	lines := []string{w.Header()}
	for _, r := range rows {
		lines = append(lines, w.Row(r.Name, r.Count))
	}
	return strings.Join(lines, "\n")
}

type CSVExporter struct{}

func (CSVExporter) NewWriter() Writer { return csvWriter{} }

type TextExporter struct{}

func (TextExporter) NewWriter() Writer { return textWriter{} }

func main() {
	rows := []Row{{"pen", 3}, {"book", 1}}
	for _, e := range []Exporter{CSVExporter{}, TextExporter{}} {
		fmt.Println(Export(e, rows))
	}
}
```

```output
name,count
pen,3
book,1
Inventory
pen: 3
book: 1
```

- Verdict: idiom. Go has no inheritance, so a base type cannot call a method that a "subclass" overrides. The factory method becomes a method of an interface, and the shared algorithm is a function over that interface.
- Embedding a struct does not help. A method of the embedded struct calls its own methods, not the methods of the outer type.
- The usual Go form is simpler: pass a `func() Writer`, or call a constructor function that returns the interface, as `crc32.NewIEEE` returns a `hash.Hash32`.

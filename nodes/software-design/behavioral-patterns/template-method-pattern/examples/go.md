```go run id=loader
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type Item struct {
	Name  string
	Count int
}

// Parser is the step that each format must give.
type Parser interface {
	Parse(line string) Item
}

// Accepter is an optional hook. A parser that has no Accept method keeps every record.
type Accepter interface {
	Accept(Item) bool
}

// Load is the template: a function, not a method of a base type.
func Load(p Parser, lines []string) []Item {
	accept := func(Item) bool { return true }
	if a, ok := p.(Accepter); ok {
		accept = a.Accept
	}
	var records []Item
	for _, raw := range lines {
		line := strings.TrimSpace(raw)
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}
		if record := p.Parse(line); accept(record) {
			records = append(records, record)
		}
	}
	return records
}

func split(line, sep string) Item {
	name, count, _ := strings.Cut(line, sep)
	n, _ := strconv.Atoi(count)
	return Item{name, n}
}

type CSV struct{}

func (CSV) Parse(line string) Item { return split(line, ",") }

type KeyValue struct{}

func (KeyValue) Parse(line string) Item { return split(line, "=") }
func (KeyValue) Accept(r Item) bool     { return r.Count > 0 }

func show(label string, records []Item) {
	parts := make([]string, len(records))
	for i, r := range records {
		parts[i] = fmt.Sprintf("%s:%d", r.Name, r.Count)
	}
	fmt.Printf("%s: %s\n", label, strings.Join(parts, " "))
}

func main() {
	show("csv", Load(CSV{}, []string{"# stock", "pen,3", "", "book,1"}))
	show("kv", Load(KeyValue{}, []string{"pen=3", "# sold out", "cup=0"}))
}
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: idiom. Go has no inheritance, so the template is a function that takes an interface. The steps are the interface's methods. This is the form of `sort.Sort`, which runs a sort algorithm over the `Len`, `Less` and `Swap` methods of its argument.
- The hook is a second, optional interface. `Load` checks for it with a type assertion and uses a default when it is absent.
- A function cannot be overridden, so the order of the steps is fixed, as with a `final` method in Java.

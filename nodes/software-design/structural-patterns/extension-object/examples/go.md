```go run id=extensions
package main

import (
	"fmt"
	"strings"
)

type Document struct {
	Text       string
	extensions []any
}

func (d *Document) AddExtension(e any) { d.extensions = append(d.extensions, e) }

// Extension returns the first extension of type T, and false if there is none.
func Extension[T any](d *Document) (T, bool) {
	for _, e := range d.extensions {
		if t, ok := e.(T); ok {
			return t, true
		}
	}
	var zero T
	return zero, false
}

type WordCount struct{ doc *Document }

func (w *WordCount) Count() int { return len(strings.Fields(w.doc.Text)) }

type Upper struct{ doc *Document }

func (u *Upper) Text() string { return strings.ToUpper(u.doc.Text) }

func showUpper(d *Document) {
	if u, ok := Extension[*Upper](d); ok {
		fmt.Println("upper:", u.Text())
	} else {
		fmt.Println("upper: not supported")
	}
}

func main() {
	doc := &Document{Text: "the quick brown fox"}
	doc.AddExtension(&WordCount{doc})
	w, _ := Extension[*WordCount](doc)
	fmt.Println("words:", w.Count())
	showUpper(doc)
	doc.AddExtension(&Upper{doc})
	showUpper(doc)
}
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. A type assertion `e.(T)` checks the type of each extension, and the comma-ok form reports a missing one. `T` can also be an interface type, so a client can ask for a behaviour instead of a concrete type.

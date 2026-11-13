```go run id=rect-area
package main

import "fmt"

// Clients use Area() for both versions, so the storage can change.
type Areaer interface{ Area() int }

type StoredRect struct{ Width, Height, area int }

func NewStoredRect(w, h int) *StoredRect { return &StoredRect{w, h, w * h} } // stored once

func (r *StoredRect) Area() int { return r.area }

type ComputedRect struct{ Width, Height int }

func (r *ComputedRect) Area() int { return r.Width * r.Height } // computed on each call

func report(name string, r Areaer) { fmt.Printf("%s: %d\n", name, r.Area()) }

func main() {
	a := NewStoredRect(2, 3)
	b := &ComputedRect{2, 3}
	report("stored", a)
	report("computed", b)

	a.Width = 4
	b.Width = 4
	report("stored", a)
	report("computed", b)
}
```

```output
stored: 6
computed: 6
stored: 6
computed: 12
```

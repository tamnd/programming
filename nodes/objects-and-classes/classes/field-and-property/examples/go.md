```go run id=struct-fields
package main

import (
	"errors"
	"fmt"
)

// Size has exported fields: other packages read and write them directly.
type Size struct {
	Width, Height int
}

// Rectangle hides its fields; the lowercase names are visible only in the package.
type Rectangle struct {
	width, height int
}

func (r *Rectangle) Width() int { return r.width } // a getter is named Width, not GetWidth

func (r *Rectangle) SetWidth(w int) error {
	if w < 0 {
		return errors.New("negative width")
	}
	r.width = w
	return nil
}

func (r *Rectangle) Area() int { return r.width * r.height }

func main() {
	s := Size{Width: 3, Height: 4}
	s.Width = 5
	fmt.Println(s.Width, s.Height)

	r := &Rectangle{width: 3, height: 4}
	fmt.Println(r.Width(), r.Area())
	fmt.Println(r.SetWidth(5), r.Area())
	fmt.Println(r.SetWidth(-1))
}
```

```output for=struct-fields
5 4
3 12
<nil> 20
negative width
```

- Verdict: idiom. Go has struct fields and no properties. A field whose name starts with an upper-case letter is exported, and a field with a lower-case name is visible only inside its package.
- Go code uses exported fields for plain data and methods for checked access. The convention names a getter after the field, `Width`, without a `Get` prefix, and a setter `SetWidth`.
- A setter that can fail returns an `error`, because Go has no exceptions for this.

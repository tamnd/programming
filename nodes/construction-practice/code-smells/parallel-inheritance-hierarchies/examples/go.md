```go run id=merge-the-twins
package main

import (
	"fmt"
	"reflect"
)

// Before: shape types and renderer types that mirror them.
type circleBefore struct{ r int }
type squareBefore struct{ side int }
type triangleBefore struct{ base, height int }

type renderer interface{ svg(shape any) string }
type circleRenderer struct{}
type squareRenderer struct{}

func (circleRenderer) svg(s any) string { return fmt.Sprintf(`<circle r="%d"/>`, s.(circleBefore).r) }
func (squareRenderer) svg(s any) string {
	side := s.(squareBefore).side
	return fmt.Sprintf(`<rect width="%d" height="%d"/>`, side, side)
}

var renderers = map[reflect.Type]renderer{
	reflect.TypeFor[circleBefore](): circleRenderer{},
	reflect.TypeFor[squareBefore](): squareRenderer{},
}

func svgBefore(s any) (string, error) {
	r, ok := renderers[reflect.TypeOf(s)]
	if !ok {
		return "", fmt.Errorf("no renderer for %T", s)
	}
	return r.svg(s), nil
}

// After: each shape draws itself, and the interface names the method.
type shape interface{ svg() string }

type circle struct{ r int }
type square struct{ side int }
type triangle struct{ base, height int }

func (c circle) svg() string { return fmt.Sprintf(`<circle r="%d"/>`, c.r) }
func (s square) svg() string { return fmt.Sprintf(`<rect width="%d" height="%d"/>`, s.side, s.side) }
func (t triangle) svg() string {
	return fmt.Sprintf(`<polygon points="0,%d %d,%d %d,0"/>`, t.height, t.base, t.height, t.base/2)
}

// The compiler checks each shape here.
var _ = []shape{circle{}, square{}, triangle{}}

func main() {
	for _, pair := range []struct {
		after  shape
		before any
	}{{circle{5}, circleBefore{5}}, {square{4}, squareBefore{4}}} {
		got, err := svgBefore(pair.before)
		if err != nil || got != pair.after.svg() {
			panic(fmt.Sprint("mismatch: ", got, err))
		}
	}
	if _, err := svgBefore(triangleBefore{6, 3}); err == nil {
		panic("the missing twin must fail")
	}
	if (triangle{6, 3}).svg() != `<polygon points="0,3 6,3 3,0"/>` {
		panic("triangle")
	}
	fmt.Println("svg: tests pass")
}
```

```output
svg: tests pass
```

- The table of the first version is keyed by `reflect.Type`, and each renderer asserts the type of its argument. Both are the price of linking two sets of types by convention.
- The line `var _ = []shape{circle{}, square{}, triangle{}}` makes the compiler check that each shape type has an `svg` method. It is a common Go idiom for such checks.
- `reflect.TypeFor` needs Go 1.22 or later.

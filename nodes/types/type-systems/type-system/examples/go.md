```go run id=structural-interfaces-and-assertions
package main

import (
	"fmt"
	"strings"
)

// Shape is satisfied by any type with an Area method: no declaration is needed.
type Shape interface{ Area() float64 }

type Square struct{ Side float64 }

func (s Square) Area() float64 { return s.Side * s.Side }

type Seconds int64
type Meters int64

func main() {
	var s Shape = Square{2}
	fmt.Println(s.Area())

	// A type assertion checks the dynamic type at run time.
	var x any = "text"
	if n, ok := x.(int); !ok {
		fmt.Println("not an int", n)
	}
	str, ok := x.(fmt.Stringer)
	fmt.Println(str, ok)

	// Go has no implicit numeric conversions, even between types with the same representation.
	var t Seconds = 90
	var d Meters = 400
	fmt.Println(int64(t)+int64(d), Meters(t) == d)

	// Untyped constants have arbitrary precision until they get a type.
	const big = 1 << 100
	fmt.Println(big>>98, 1.0/3*3 == 1)

	// A failed assertion without ok panics.
	defer func() { fmt.Println("recovered:", strings.SplitN(fmt.Sprint(recover()), ":", 2)[0]) }()
	_ = x.(int)
}
```

```output for=structural-interfaces-and-assertions
4
not an int 0
<nil> false
490 false
4 true
recovered: interface conversion
```

- Verdict: native. `Square` satisfies `Shape` because it has an `Area` method, with no declaration. A type assertion with `ok` tests the dynamic type, and one without `ok` panics when it fails. `Seconds` and `Meters` have the same representation but do not mix without a conversion. Untyped constants keep full precision, so `1 << 100` is valid as a constant.

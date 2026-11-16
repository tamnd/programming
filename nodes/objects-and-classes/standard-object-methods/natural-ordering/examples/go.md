```go run id=compare-method-and-sortfunc
package main

import (
	"cmp"
	"fmt"
	"math"
	"slices"
)

type Version struct{ Major, Minor int }

// Compare gives the natural order of Version by convention.
func (v Version) Compare(o Version) int {
	return cmp.Or(cmp.Compare(v.Major, o.Major), cmp.Compare(v.Minor, o.Minor))
}

func main() {
	vs := []Version{{1, 10}, {1, 2}, {0, 9}}
	slices.SortFunc(vs, Version.Compare)
	fmt.Println(vs)

	xs := []float64{2, math.NaN(), -1}
	slices.Sort(xs)
	fmt.Println(xs, cmp.Compare(math.NaN(), math.NaN()), math.NaN() < 0)
}
```

```output for=compare-method-and-sortfunc
[{0 9} {1 2} {1 10}]
[NaN -1 2] 0 false
```

- Verdict: idiom. The `<` operator works only on ordered types, such as numbers and strings; a struct has no order. The convention is a `Compare` method, which `slices.SortFunc` takes as a function.
- `cmp.Or` returns its first argument that is not zero, so it chains the comparisons of the fields.
- `cmp.Compare` and `slices.Sort` treat `NaN` as less than any other number and equal to itself, which gives floats a total order; the operator `<` still gives false for `NaN`.

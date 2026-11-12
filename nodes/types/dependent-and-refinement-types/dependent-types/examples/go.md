```go run id=checked-constructors-and-array-lengths
package main

import (
	"errors"
	"fmt"
)

// Port holds a TCP port number from 1 to 65535. The field is unexported,
// so code in other packages can get a Port only from NewPort.
type Port struct{ n uint16 }

func NewPort(n int) (Port, error) {
	if n < 1 || n > 65535 {
		return Port{}, errors.New(fmt.Sprint(n, " is not a port number"))
	}
	return Port{uint16(n)}, nil
}

func (p Port) Number() int { return int(p.n) }

// The length of an array is part of its type: [3]float64 is not [2]float64.
func dot(a, b [3]float64) float64 {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]
}

func main() {
	for _, n := range []int{8080, 0, 70000} {
		if p, err := NewPort(n); err != nil {
			fmt.Println("error:", err)
		} else {
			fmt.Println("port", p.Number())
		}
	}
	fmt.Println(dot([3]float64{1, 2, 3}, [3]float64{4, 5, 6}))
	var pair [2]float64
	fmt.Printf("%T and %T are different types\n", pair, [3]float64{})
}
```

```output for=checked-constructors-and-array-lengths
port 8080
error: 0 is not a port number
error: 70000 is not a port number
32
[2]float64 and [3]float64 are different types
```

- Verdict: idiom. Go has no refinement types. A struct with an unexported field and a constructor that returns an error keeps the predicate for all code outside the package. The length of a Go array is part of its type, so `dot` accepts only arrays of length 3, but the length must be a constant and generic code cannot take it as a parameter.

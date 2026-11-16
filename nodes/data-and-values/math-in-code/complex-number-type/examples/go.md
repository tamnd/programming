```go run id=complex128
package main

import (
	"fmt"
	"math/cmplx"
	"unsafe"
)

func main() {
	z := complex(3, 4)
	w := 1 - 2i
	fmt.Println(z*w, z/w)
	fmt.Println(real(z), imag(z), cmplx.Abs(z), cmplx.Phase(1i))
	fmt.Println(cmplx.Sqrt(-4), cmplx.Conj(z))
	fmt.Println(unsafe.Sizeof(z), unsafe.Sizeof(complex64(z)))
	fmt.Printf("%.1f\n", z)
}
```

```output for=complex128
(11-2i) (-1+2i)
3 4 5 1.5707963267948966
(0+2i) (3-4i)
16 8
(3.0+4.0i)
```

- Verdict: native. `complex64` and `complex128` are built-in types, `2i` is an imaginary literal, and the built-in functions `complex`, `real` and `imag` build a value and take it apart. The package `math/cmplx` has `Abs`, `Phase`, `Sqrt` and the other functions. A `complex128` takes 16 bytes and a `complex64` takes 8. `fmt` prints the form `(re+imi)`, and a verb such as `%.1f` applies to both parts.

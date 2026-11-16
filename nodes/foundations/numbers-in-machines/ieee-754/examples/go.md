```go run id=fields
package main

import (
	"fmt"
	"math"
)

func main() {
	for _, x := range []float64{1.0, -2.5, 0.1} {
		fmt.Printf("%4.1f binary32 %08x binary64 %016x\n", x, math.Float32bits(float32(x)), math.Float64bits(x))
	}

	b := math.Float64bits(-2.5)
	fmt.Printf("-2.5: sign %d, exponent %d, fraction %x\n", b>>63, int(b>>52&0x7ff)-1023, b&(1<<52-1))

	fmt.Println("MaxFloat32:", math.MaxFloat32)
	fmt.Println("MaxFloat64:", math.MaxFloat64)
	fmt.Println("FMA(0.1, 10, -1):", math.FMA(0.1, 10, -1))
}
```

```output for=fields
 1.0 binary32 3f800000 binary64 3ff0000000000000
-2.5 binary32 c0200000 binary64 c004000000000000
 0.1 binary32 3dcccccd binary64 3fb999999999999a
-2.5: sign 1, exponent 1, fraction 4000000000000
MaxFloat32: 3.4028234663852886e+38
MaxFloat64: 1.7976931348623157e+308
FMA(0.1, 10, -1): 5.551115123125783e-17
```

- Verdict: native. Go `float32` and `float64` are binary32 and binary64.
- `math.Float32bits` and `math.Float64bits` give the bits, and `math.Float32frombits` and `math.Float64frombits` do the reverse.
- `math.FMA` is the fused multiply-add of the standard. It rounds `0.1*10 - 1` one time, so the result shows the error of the stored 0.1, which is about 5.55e-17.

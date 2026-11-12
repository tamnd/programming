```go run id=defined-wraparound
package main

import (
	"encoding/binary"
	"fmt"
	"math"
)

func main() {
	x := 200
	fmt.Println(int8(x), uint8(x+100)) // a conversion keeps the low bits

	var i int32 = math.MaxInt32
	i++ // Go defines signed overflow: it wraps
	fmt.Println(i)

	m := int64(math.MinInt64)
	fmt.Println(m/-1, -m == m) // the spec says x / -1 equals x for the most negative value

	var u uint8 = 0
	u--
	fmt.Println(u, ^uint32(0))

	var k int16 = -1000
	b := binary.BigEndian.AppendUint16(nil, uint16(k))
	fmt.Printf("% x % x\n", b, binary.LittleEndian.AppendUint16(nil, uint16(k)))
	fmt.Println(int16(binary.BigEndian.Uint16(b)))
	fmt.Println(-16>>2, uint32(0xF0000000)>>28)
}
```

```output for=defined-wraparound
-56 44
-2147483648
-9223372036854775808 true
255 4294967295
fc 18 18 fc
-1000
-4 15
```

- Verdict: native. Go has `int8` to `int64` and `uint8` to `uint64`, and its specification defines overflow: signed and unsigned arithmetic wrap, with no panic. The most negative `int64` divided by −1 is itself, as the specification says. A conversion such as `int8(x)` keeps the low bits. `encoding/binary` writes and reads integers in big-endian and little-endian order.

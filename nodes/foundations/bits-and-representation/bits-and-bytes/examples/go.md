```go run id=and-not-and-binary-order
package main

import (
	"encoding/binary"
	"fmt"
	"math/bits"
)

const (
	Read = 1 << iota
	Write
	Exec
)

func main() {
	perm := Read | Write
	perm &^= Write // AND NOT clears the bits of Write
	perm ^= Exec
	fmt.Printf("%03b %d\n", perm, bits.OnesCount(uint(perm)))

	var s uint = 40
	var u uint32 = 1
	fmt.Println(-8>>1, u<<s, bits.LeadingZeros32(u), bits.RotateLeft8(0b1001_0110, 1))

	buf := make([]byte, 4)
	binary.LittleEndian.PutUint32(buf, 0x12345678)
	fmt.Printf("% x %x\n", buf, binary.BigEndian.Uint32(buf))
	fmt.Printf("%#x\n", bits.ReverseBytes32(0x12345678))
}
```

```output for=and-not-and-binary-order
101 2
-4 0 31 45
78 56 34 12 78563412
0x78563412
```

- Verdict: native. Go has `&`, `|`, `^`, `<<`, `>>` and the AND NOT operator `&^`, which clears the bits of its right operand. Unary `^` is bitwise NOT. A right shift is arithmetic for signed integers and logical for unsigned ones, and a shift count at or above the width gives 0, not an error. `iota` numbers the constants of a set of flags.
- `math/bits` counts, rotates and reverses bits, and `encoding/binary` reads and writes integers in big-endian or little-endian order.

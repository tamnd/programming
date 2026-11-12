```go run id=risc-v-emulator
package main

import (
	"encoding/binary"
	"fmt"
)

// The machine code of a RISC-V function that returns 6 * 7, as little-endian bytes.
var code = []byte{
	0x13, 0x05, 0x60, 0x00, // addi a0, zero, 6
	0x93, 0x05, 0x70, 0x00, // addi a1, zero, 7
	0x33, 0x05, 0xb5, 0x02, // mul a0, a0, a1
	0x67, 0x80, 0x00, 0x00, // ret (jalr zero, 0(ra))
}

func main() {
	var x [32]int64 // x10 is a0, x11 is a1
	for pc := 0; ; pc += 4 {
		w := binary.LittleEndian.Uint32(code[pc:])
		// Each field is at a fixed place in the 32-bit word.
		op, rd, f3 := w&0x7f, w>>7&31, w>>12&7
		rs1, rs2, f7 := w>>15&31, w>>20&31, w>>25
		switch {
		case op == 0x13 && f3 == 0:
			imm := int64(int32(w) >> 20) // sign-extend the 12-bit immediate
			fmt.Printf("%08x  addi x%d, x%d, %d\n", w, rd, rs1, imm)
			x[rd] = x[rs1] + imm
		case op == 0x33 && f3 == 0 && f7 == 1:
			fmt.Printf("%08x  mul  x%d, x%d, x%d\n", w, rd, rs1, rs2)
			x[rd] = x[rs1] * x[rs2]
		case op == 0x67:
			fmt.Printf("%08x  ret\n", w)
			fmt.Println("a0 =", x[10])
			return
		default:
			panic(fmt.Sprintf("unknown instruction %08x", w))
		}
		x[0] = 0 // register x0 is always zero
	}
}
```

```output for=risc-v-emulator
00600513  addi x10, x0, 6
00700593  addi x11, x0, 7
02b50533  mul  x10, x10, x11
00008067  ret
a0 = 42
```

- Verdict: native. The Go compiler writes machine code, and Go has assembly files in its own syntax but no inline assembly. The example runs RISC-V machine code in a small emulator: each instruction is a 32-bit word with the register numbers and the operation at fixed bit positions, and `addi` keeps a 12-bit signed constant in the top bits. The words are the ones that an assembler gives for the same lines.

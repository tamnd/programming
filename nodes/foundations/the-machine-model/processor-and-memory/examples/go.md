```go run id=machine-in-slice
package main

import (
	"fmt"
	"unsafe"
)

const (
	HALT = iota
	LOAD
	ADD
	SUB
	STORE
	JZ
	JMP
)

func run(mem []int) int {
	pc, acc, steps := 0, 0, 0
	for {
		op, addr := mem[pc], mem[pc+1]
		pc += 2
		steps++
		switch op {
		case HALT:
			return steps
		case LOAD:
			acc = mem[addr]
		case ADD:
			acc += mem[addr]
		case SUB:
			acc -= mem[addr]
		case STORE:
			mem[addr] = acc
		case JZ:
			if acc == 0 {
				pc = addr
			}
		case JMP:
			pc = addr
		}
	}
}

func main() {
	mem := []int{LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
		LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0, 5, 0, 1}
	steps := run(mem)
	fmt.Printf("sum=%d steps=%d\n", mem[21], steps)
	var p *int
	fmt.Println(unsafe.Sizeof(p), unsafe.Sizeof(mem[0]), unsafe.Sizeof(mem))
}
```

```output for=machine-in-slice
sum=15 steps=48
8 8 24
```

- Verdict: native. Go compiles to machine code and lets a program see sizes with `unsafe.Sizeof`: a pointer and an `int` are 8 bytes on a 64-bit processor, and a slice header is 24 bytes, a pointer, a length and a capacity. Every index into a slice is checked against its length, and `unsafe.Pointer` converts between pointers and addresses.

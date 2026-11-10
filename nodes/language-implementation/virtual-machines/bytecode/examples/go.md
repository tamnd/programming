```go run id=vm
package main

import "fmt"

type Op byte

// iota numbers the opcodes from 0: PUSH = 0, LOAD = 1, ... HALT = 10.
const (
	PUSH Op = iota
	LOAD
	STORE
	ADD
	SUB
	MUL
	LT
	JUMP_IF_FALSE
	JUMP
	PRINT
	HALT
)

var names = [...]string{"PUSH", "LOAD", "STORE", "ADD", "SUB", "MUL", "LT", "JUMP_IF_FALSE", "JUMP", "PRINT", "HALT"}

func (op Op) hasArg() bool {
	switch op {
	case PUSH, LOAD, STORE, JUMP_IF_FALSE, JUMP:
		return true
	}
	return false
}

// let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
// Slots: n = 0, acc = 1, step = 2.
var code = []byte{
	0, 5, 2, 0, 0, 1, 2, 1,
	0, 0, 1, 0, 6, 7, 35,
	1, 0, 2, 2,
	1, 1, 1, 2, 5, 2, 1,
	1, 0, 0, 1, 4, 2, 0,
	8, 8,
	1, 1, 9, 10,
}

func disassemble(code []byte) {
	for pc := 0; pc < len(code); {
		op := Op(code[pc])
		if op.hasArg() {
			fmt.Printf("%4d %-13s %d\n", pc, names[op], code[pc+1])
			pc += 2
		} else {
			fmt.Printf("%4d %s\n", pc, names[op])
			pc++
		}
	}
}

func run(code []byte, nslots int) (executed, maxDepth int) {
	stack := make([]int, 0, 16)
	slots := make([]int, nslots)
	pop := func() int {
		v := stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		return v
	}
	pc := 0
	for {
		op := Op(code[pc])
		executed++
		arg := 0
		if op.hasArg() {
			arg = int(code[pc+1])
			pc += 2
		} else {
			pc++
		}
		switch op {
		case PUSH:
			stack = append(stack, arg)
		case LOAD:
			stack = append(stack, slots[arg])
		case STORE:
			slots[arg] = pop()
		case ADD, SUB, MUL, LT:
			b := pop() // the right operand is on top
			a := pop()
			var r int
			switch op {
			case ADD:
				r = a + b
			case SUB:
				r = a - b
			case MUL:
				r = a * b
			case LT:
				if a < b {
					r = 1
				}
			}
			stack = append(stack, r)
		case JUMP_IF_FALSE:
			if pop() == 0 {
				pc = arg
			}
		case JUMP:
			pc = arg
		case PRINT:
			fmt.Println(pop())
		case HALT:
			return executed, maxDepth
		default:
			panic(fmt.Sprintf("bad opcode %d at %d", op, pc))
		}
		maxDepth = max(maxDepth, len(stack))
	}
}

func main() {
	disassemble(code)
	executed, maxDepth := run(code, 3)
	fmt.Printf("bytes of code: %d\n", len(code))
	fmt.Printf("instructions executed: %d, maximum stack depth: %d\n", executed, maxDepth)
}
```

```output for=vm
   0 PUSH          5
   2 STORE         0
   4 PUSH          1
   6 STORE         1
   8 PUSH          0
  10 LOAD          0
  12 LT
  13 JUMP_IF_FALSE 35
  15 LOAD          0
  17 STORE         2
  19 LOAD          1
  21 LOAD          2
  23 MUL
  24 STORE         1
  26 LOAD          0
  28 PUSH          1
  30 SUB
  31 STORE         0
  33 JUMP          8
  35 LOAD          1
  37 PRINT
  38 HALT
120
bytes of code: 39
instructions executed: 86, maximum stack depth: 2
```

- Verdict: idiom. Go compiles ahead of time to machine code and has no bytecode, so the program builds its own.
- A defined type `Op byte` with `iota` constants gives the opcodes names and keeps them one byte wide.
- The stack is a slice. `append` pushes, and slicing off the last element pops. An empty stack makes `pop` panic with an index out of range.

```javascript run id=vm
const Op = Object.freeze({
  PUSH: 0, LOAD: 1, STORE: 2, ADD: 3, SUB: 4, MUL: 5, LT: 6,
  JUMP_IF_FALSE: 7, JUMP: 8, PRINT: 9, HALT: 10,
});
const NAMES = Object.keys(Op);
const HAS_ARG = new Set([Op.PUSH, Op.LOAD, Op.STORE, Op.JUMP_IF_FALSE, Op.JUMP]);

// let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
// Slots: n = 0, acc = 1, step = 2.
const code = Uint8Array.of(
  Op.PUSH, 5, Op.STORE, 0, Op.PUSH, 1, Op.STORE, 1,
  Op.PUSH, 0, Op.LOAD, 0, Op.LT, Op.JUMP_IF_FALSE, 35,
  Op.LOAD, 0, Op.STORE, 2,
  Op.LOAD, 1, Op.LOAD, 2, Op.MUL, Op.STORE, 1,
  Op.LOAD, 0, Op.PUSH, 1, Op.SUB, Op.STORE, 0,
  Op.JUMP, 8,
  Op.LOAD, 1, Op.PRINT, Op.HALT,
);

function disassemble(code) {
  for (let pc = 0; pc < code.length; ) {
    const name = NAMES[code[pc]];
    const at = String(pc).padStart(4);
    if (HAS_ARG.has(code[pc])) {
      console.log(`${at} ${name.padEnd(13)} ${code[pc + 1]}`);
      pc += 2;
    } else {
      console.log(`${at} ${name}`);
      pc += 1;
    }
  }
}

function run(code, nslots) {
  const stack = [];
  const slots = new Array(nslots).fill(0);
  let pc = 0, executed = 0, maxDepth = 0;
  for (;;) {
    const op = code[pc];
    executed++;
    let arg = 0;
    if (HAS_ARG.has(op)) {
      arg = code[pc + 1];
      pc += 2;
    } else {
      pc += 1;
    }
    switch (op) {
      case Op.PUSH: stack.push(arg); break;
      case Op.LOAD: stack.push(slots[arg]); break;
      case Op.STORE: slots[arg] = stack.pop(); break;
      case Op.ADD: case Op.SUB: case Op.MUL: case Op.LT: {
        const b = stack.pop(); // the right operand is on top
        const a = stack.pop();
        stack.push(op === Op.ADD ? a + b : op === Op.SUB ? a - b : op === Op.MUL ? a * b : a < b ? 1 : 0);
        break;
      }
      case Op.JUMP_IF_FALSE: if (stack.pop() === 0) pc = arg; break;
      case Op.JUMP: pc = arg; break;
      case Op.PRINT: console.log(stack.pop()); break;
      case Op.HALT: return { executed, maxDepth };
      default: throw new Error(`bad opcode ${op} at ${pc}`);
    }
    maxDepth = Math.max(maxDepth, stack.length);
  }
}

disassemble(code);
const { executed, maxDepth } = run(code, 3);
console.log(`bytes of code: ${code.length}`);
console.log(`instructions executed: ${executed}, maximum stack depth: ${maxDepth}`);
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

- Verdict: idiom. JavaScript defines no bytecode of its own. V8 compiles functions to an internal register bytecode, but a program cannot read it through the language.
- A `Uint8Array` holds the code, so each opcode and each operand is one byte. An operand above 255 would need two bytes.
- `Object.freeze` makes the opcode table read-only. `Object.keys(Op)` gives the names in the order of definition, which is also the order of the opcode numbers.

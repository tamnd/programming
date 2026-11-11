```javascript run id=machine-in-typed-array
const [HALT, LOAD, ADD, SUB, STORE, JZ, JMP] = [0, 1, 2, 3, 4, 5, 6];

function run(mem) {
  let pc = 0, acc = 0, steps = 0;
  for (;;) {
    const op = mem[pc], addr = mem[pc + 1];
    pc += 2;
    steps++;
    switch (op) {
      case HALT: return steps;
      case LOAD: acc = mem[addr]; break;
      case ADD: acc += mem[addr]; break;
      case SUB: acc -= mem[addr]; break;
      case STORE: mem[addr] = acc; break;
      case JZ: if (acc === 0) pc = addr; break;
      case JMP: pc = addr; break;
    }
  }
}

const mem = new Int32Array([LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
  LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0, 5, 0, 1]);
const steps = run(mem);
console.log(`sum=${mem[21]} steps=${steps}`);
console.log(mem.BYTES_PER_ELEMENT, mem.byteLength);
```

```output for=machine-in-typed-array
sum=15 steps=48
4 92
```

```javascript wrong id=numbers-are-not-words
const word = 2 ** 53;  // a counter that has run for a long time
console.log(word + 1, word + 1 === word);
console.log(Number.MAX_SAFE_INTEGER, 2n ** 53n + 1n);
```

```output for=numbers-are-not-words
9007199254740992 true
9007199254740991 9007199254740993n
```

- Verdict: idiom. JavaScript has no access to registers or addresses. An `Int32Array` is the closest thing to raw memory: a buffer of bytes read as 32-bit words, here 23 words in 92 bytes.
- The wrong block treats a JavaScript number as a machine word. A number is a 64-bit floating-point value, so integers above 2 to the power 53 lose their low bits, and adding 1 changes nothing. Use `BigInt` or a typed array of 64-bit integers for values that must hold every bit of a word.

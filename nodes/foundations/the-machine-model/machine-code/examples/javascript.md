```javascript run id=webassembly-by-hand
// A WebAssembly module, written byte by byte: one function that adds two 32-bit integers.
const bytes = new Uint8Array([
  0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, // magic "\0asm" and version 1
  0x01, 0x07, 0x01, 0x60, 0x02, 0x7f, 0x7f, 0x01, 0x7f, // type: (i32, i32) -> i32
  0x03, 0x02, 0x01, 0x00, // function 0 has type 0
  0x07, 0x07, 0x01, 0x03, 0x61, 0x64, 0x64, 0x00, 0x00, // export function 0 as "add"
  0x0a, 0x09, 0x01, 0x07, 0x00, // code: one body of 7 bytes, no locals
  0x20, 0x00, // local.get 0
  0x20, 0x01, // local.get 1
  0x6a, // i32.add
  0x0b, // end
]);

console.log("valid:", WebAssembly.validate(bytes), "size:", bytes.length, "bytes");
const { instance } = await WebAssembly.instantiate(bytes);
console.log("add(40, 2) =", instance.exports.add(40, 2));
console.log("add(2147483647, 1) =", instance.exports.add(2147483647, 1)); // i32 wraps around

// Change one byte of the code: 0x6b is i32.sub.
const sub = bytes.slice();
sub[sub.length - 2] = 0x6b;
const m2 = await WebAssembly.instantiate(sub);
console.log("after one byte changes: add(40, 2) =", m2.instance.exports.add(40, 2));
```

```output for=webassembly-by-hand
valid: true size: 41 bytes
add(40, 2) = 42
add(2147483647, 1) = -2147483648
after one byte changes: add(40, 2) = 38
```

- Verdict: absent. The engine compiles JavaScript to machine code, but the program cannot see it. WebAssembly is the closest form: the program writes a module of 41 bytes, with the magic number, a function type, an export and a body of four instructions. `WebAssembly.instantiate` checks the bytes and compiles them, and the 32-bit add wraps around. Changing the one byte `6a` to `6b` makes the function subtract.

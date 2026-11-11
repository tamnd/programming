```rust run id=machine-in-array
use std::mem::{align_of, size_of};

const HALT: i64 = 0;
const LOAD: i64 = 1;
const ADD: i64 = 2;
const SUB: i64 = 3;
const STORE: i64 = 4;
const JZ: i64 = 5;
const JMP: i64 = 6;

fn run(mem: &mut [i64]) -> u32 {
    let (mut pc, mut acc, mut steps) = (0usize, 0i64, 0u32);
    loop {
        let (op, addr) = (mem[pc], mem[pc + 1] as usize);
        pc += 2;
        steps += 1;
        match op {
            HALT => return steps,
            LOAD => acc = mem[addr],
            ADD => acc += mem[addr],
            SUB => acc -= mem[addr],
            STORE => mem[addr] = acc,
            JZ => {
                if acc == 0 {
                    pc = addr
                }
            }
            JMP => pc = addr,
            _ => panic!("bad opcode {op}"),
        }
    }
}

#[allow(dead_code)]
struct Cell {
    tag: u8,
    value: u64,
}

fn main() {
    let mut mem = [LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
                   LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0, 5, 0, 1];
    let steps = run(&mut mem);
    println!("sum={} steps={steps}", mem[21]);
    println!("{} {} {}", size_of::<usize>(), size_of::<Cell>(), align_of::<Cell>());
}
```

```output for=machine-in-array
sum=15 steps=48
8 16 8
```

```rust fails expect=panic id=address-out-of-range
fn main() {
    let mem = [0i64; 23]; // addresses 0 to 22
    let addr = std::env::args().count() + 22; // one past the end
    println!("{}", mem[addr]);
}
```

- Verdict: native. Rust compiles to machine code and exposes the layout of types: `size_of` and `align_of` give the size and alignment, and the struct `Cell` takes 16 bytes for 9 bytes of fields, because the compiler aligns the `u64` to 8 bytes. `usize` is the width of an address, and raw pointers in `unsafe` code read and write memory directly.
- The fails block reads one cell past the end of the memory. A real processor would read whatever is at that address; Rust checks the index and panics.

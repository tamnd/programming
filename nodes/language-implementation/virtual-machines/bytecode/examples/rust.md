```rust run id=vm
#[derive(Clone, Copy, Debug, PartialEq)]
#[repr(u8)]
enum Op {
    Push, Load, Store, Add, Sub, Mul, Lt, JumpIfFalse, Jump, Print, Halt,
}

const OPS: [Op; 11] = [
    Op::Push, Op::Load, Op::Store, Op::Add, Op::Sub, Op::Mul, Op::Lt,
    Op::JumpIfFalse, Op::Jump, Op::Print, Op::Halt,
];
const NAMES: [&str; 11] = [
    "PUSH", "LOAD", "STORE", "ADD", "SUB", "MUL", "LT", "JUMP_IF_FALSE", "JUMP", "PRINT", "HALT",
];

impl Op {
    fn decode(byte: u8) -> Result<Op, String> {
        OPS.get(byte as usize).copied().ok_or(format!("bad opcode {byte}"))
    }
    fn has_arg(self) -> bool {
        matches!(self, Op::Push | Op::Load | Op::Store | Op::JumpIfFalse | Op::Jump)
    }
    fn name(self) -> &'static str {
        NAMES[self as usize]
    }
}

// let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
// Slots: n = 0, acc = 1, step = 2.
const CODE: [u8; 39] = [
    0, 5, 2, 0, 0, 1, 2, 1,
    0, 0, 1, 0, 6, 7, 35,
    1, 0, 2, 2,
    1, 1, 1, 2, 5, 2, 1,
    1, 0, 0, 1, 4, 2, 0,
    8, 8,
    1, 1, 9, 10,
];

fn disassemble(code: &[u8]) -> Result<(), String> {
    let mut pc = 0;
    while pc < code.len() {
        let op = Op::decode(code[pc])?;
        if op.has_arg() {
            println!("{pc:4} {:<13} {}", op.name(), code[pc + 1]);
            pc += 2;
        } else {
            println!("{pc:4} {}", op.name());
            pc += 1;
        }
    }
    Ok(())
}

fn run(code: &[u8], nslots: usize) -> Result<(usize, usize), String> {
    let mut stack: Vec<i64> = Vec::new();
    let mut slots = vec![0i64; nslots];
    let (mut pc, mut executed, mut max_depth) = (0usize, 0usize, 0usize);
    loop {
        let op = Op::decode(code[pc])?;
        executed += 1;
        let arg = if op.has_arg() { code[pc + 1] as usize } else { 0 };
        pc += if op.has_arg() { 2 } else { 1 };
        let mut pop = || stack.pop().ok_or(format!("stack underflow at {pc}"));
        match op {
            Op::Push => stack.push(arg as i64),
            Op::Load => stack.push(slots[arg]),
            Op::Store => slots[arg] = pop()?,
            Op::Add | Op::Sub | Op::Mul | Op::Lt => {
                let b = pop()?; // the right operand is on top
                let a = pop()?;
                stack.push(match op {
                    Op::Add => a + b,
                    Op::Sub => a - b,
                    Op::Mul => a * b,
                    _ => (a < b) as i64,
                });
            }
            Op::JumpIfFalse => {
                if pop()? == 0 {
                    pc = arg;
                }
            }
            Op::Jump => pc = arg,
            Op::Print => println!("{}", pop()?),
            Op::Halt => return Ok((executed, max_depth)),
        }
        max_depth = max_depth.max(stack.len());
    }
}

fn main() -> Result<(), String> {
    disassemble(&CODE)?;
    let (executed, max_depth) = run(&CODE, 3)?;
    println!("bytes of code: {}", CODE.len());
    println!("instructions executed: {executed}, maximum stack depth: {max_depth}");
    Ok(())
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

- Verdict: idiom. Rust compiles ahead of time to machine code and has no bytecode, so the program builds its own.
- `Op::decode` returns a `Result`, so a byte that is not an opcode becomes an error value instead of an invalid enum. The `match` on `Op` has no default arm, and the compiler rejects it if a variant is missing.
- `pop` returns an error on an empty stack, and `?` passes the error up to `main`.

```rust run id=op-enum
use Op::*;

#[derive(Clone, Copy, Debug)]
enum Op {
    Push(i64),
    Add,
    Mul,
    Dup,
    Swap,
    Call(&'static [Op]),
}

const SQUARE: &[Op] = &[Dup, Mul];
const SUM_OF_SQUARES: &[Op] = &[Call(SQUARE), Swap, Call(SQUARE), Add];

fn run(prog: &[Op], stack: &mut Vec<i64>) {
    for op in prog {
        match *op {
            Push(n) => stack.push(n),
            Add | Mul => {
                let b = stack.pop().expect("stack underflow");
                let a = stack.pop().expect("stack underflow");
                stack.push(if matches!(op, Add) { a + b } else { a * b });
            }
            Dup => {
                let top = *stack.last().expect("stack underflow");
                stack.push(top);
            }
            Swap => {
                let n = stack.len();
                stack.swap(n - 2, n - 1);
            }
            Call(body) => run(body, stack),
        }
    }
}

fn main() {
    let mut s = vec![];
    run(&[Push(3), Push(4), Call(SUM_OF_SQUARES)], &mut s);
    println!("{s:?}");

    let p = [Push(2), Push(5)];
    let q = [Mul, Dup, Add];
    let joined: Vec<Op> = [&p[..], &q[..]].concat();
    let (mut one, mut two) = (vec![], vec![]);
    run(&joined, &mut one);
    run(&p, &mut two);
    run(&q, &mut two);
    println!("{one:?} {two:?}");
}
```

```output for=op-enum
[25]
[20] [20]
```

- Verdict: absent. Rust applies functions to arguments. An enum of operations and a `match` loop give a stack machine whose programs are slices, and `concat` joins two programs into one that does the work of both in turn. Iterator chains read left to right like a concatenative program, but each adaptor names its function and takes one value at a time.

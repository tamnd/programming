```rust run id=macro-rules
macro_rules! square {
    ($x:expr) => {
        $x * $x
    };
}

macro_rules! maximum {
    ($x:expr) => { $x };
    ($x:expr, $($rest:expr),+) => {{
        let a = $x;
        let b = maximum!($($rest),+);
        if a > b { a } else { b }
    }};
}

fn main() {
    println!("square!(1 + 2) = {}", square!(1 + 2));
    println!("maximum!(3, 9, 4, 7) = {}", maximum!(3, 9, 4, 7));
}
```

```output for=macro-rules
square!(1 + 2) = 9
maximum!(3, 9, 4, 7) = 9
```

`$x:expr` matches a whole expression, and the transcriber puts back that expression as one syntax element. So `square!(1 + 2)` is `(1 + 2) * (1 + 2)`, with no extra parentheses in the macro. A macro can also take any number of arguments and call itself, as `maximum!` does.

```rust wrong id=double-evaluation
macro_rules! square {
    ($x:expr) => {
        $x * $x
    };
}

fn next(counter: &mut i32) -> i32 {
    *counter += 1;
    *counter
}

fn main() {
    let mut counter = 1;
    let s = square!(next(&mut counter));
    println!("s = {s}, counter = {counter}");
}
```

```output for=double-evaluation
s = 6, counter = 3
```

Hygiene and syntax fragments fix the precedence trap, but not double evaluation. The expansion calls `next` twice, so the result is 2 × 3 = 6, not 4. The fix binds the argument once: `{ let v = $x; v * v }`.

```rust run id=hygiene
macro_rules! swap {
    ($a:ident, $b:ident) => {
        let tmp = $a;
        $a = $b;
        $b = tmp;
    };
}

fn main() {
    let mut x = 1;
    let mut tmp = 2;
    swap!(x, tmp);
    println!("x = {x}, tmp = {tmp}");
}
```

```output for=hygiene
x = 2, tmp = 1
```

The `tmp` that the macro declares and the `tmp` of the caller are different variables, although they have the same name. This is the C `SWAP` of the C example, and here it works.

```rust fails expect=compile id=no-capture
macro_rules! show_x {
    () => {
        println!("{}", x);
    };
}

fn main() {
    let x = 5;
    show_x!();
}
```

Local variables in a macro body are looked up where the macro is defined. No `x` exists there, so the compiler reports error E0425, "cannot find value `x` in this scope". To use the caller's `x`, pass it as an argument.

```rust run id=derive
#[derive(Debug, Clone, PartialEq)]
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let p = Point { x: 1, y: 2 };
    let q = p.clone();
    println!("{:?}", q);
    println!("{}", p == q);
}
```

```output for=derive
Point { x: 1, y: 2 }
true
```

- Verdict: native. `macro_rules!` defines macros by example: each rule has a matcher and a transcriber, and the matcher binds typed syntax fragments such as `expr`, `ident` and `ty`.
- Macros by example have mixed-site hygiene. Local variables and labels are looked up at the definition, other names at the call.
- Procedural macros are Rust functions from a token stream to a token stream. They come in three kinds: function-like, derive (as `#[derive(Debug)]`) and attribute. They must live in a separate crate of type `proc-macro`, so this page cannot run one in a single file. They are not hygienic.
- Use a function or a generic function when it can do the job. A macro is right for a variable number of arguments, for items such as an `impl` block, or for syntax that a function cannot take.

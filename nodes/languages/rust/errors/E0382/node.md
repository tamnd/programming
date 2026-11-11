---
tag: GPC59W
kind: diagnostic
owner: rust
slug: E0382
code: E0382
name: "E0382: use of a moved value"
aliases: [borrow of moved value, use of moved value, use of partially moved value]
area: languages/rust/errors
level: core
summary: >-
  rustc reports E0382 when a program uses or borrows a variable, or assigns to a part of it, after its value has moved out, and the type of the value does not implement Copy.
since: "1.2.0"
severity: error
message_templates:
  - "use of moved value: `{}`"
  - "borrow of moved value: `{}`"
  - "use of partially moved value: `{}`"
  - "borrow of partially moved value: `{}`"
  - "assign to part of moved value: `{}`"
concepts: [ownership-and-borrowing]
features: [rust/ownership]
see_also: [rust/ownership]
sources:
  - url: https://doc.rust-lang.org/1.99.1/error_codes/E0382.html
    title: Rust Compiler Error Index, E0382, Rust 1.99.1
    locator: E0382
    accessed: 2026-11-09
  - url: https://github.com/rust-lang/rust/blob/1.98.0/compiler/rustc_borrowck/src/borrowck_errors.rs#L321-L343
    title: rustc 1.98.0 source, compiler/rustc_borrowck/src/borrowck_errors.rs, cannot_act_on_moved_value
    locator: lines 321 to 343
    accessed: 2026-11-09
  - url: https://github.com/rust-lang/rust/blob/1.98.0/compiler/rustc_borrowck/src/lib.rs#L1145-L1154
    title: rustc 1.98.0 source, compiler/rustc_borrowck/src/lib.rs, InitializationRequiringAction::as_noun
    locator: lines 1145 to 1154
    accessed: 2026-11-09
  - url: https://github.com/rust-lang/rust/blob/1.2.0/src/librustc_borrowck/borrowck/mod.rs#L649-L652
    title: rustc 1.2.0 source, src/librustc_borrowck/borrowck/mod.rs, report_use_of_moved_value
    locator: lines 649 to 652 (first release with the code E0382; rustc 1.1.0 gives the same message with no code)
    accessed: 2026-11-09
  - url: https://github.com/rust-lang/rust/blob/1.98.0/compiler/rustc_feature/src/unstable.rs#L513-L514
    title: rustc 1.98.0 source, compiler/rustc_feature/src/unstable.rs, feature gate diagnostic_on_move
    locator: lines 513 to 514
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/reference/expressions.html#moved-and-copied-types
    title: The Rust Reference, Expressions, Moved and copied types, Rust 1.99.1
    locator: expr.move
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/reference/expressions/loop-expr.html#r-expr.loop.for.desugar
    title: The Rust Reference, Loops, Iterator loops, Rust 1.99.1
    locator: expr.loop.for.desugar
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/book/ch04-01-what-is-ownership.html#ownership-and-functions
    title: The Rust Programming Language, 4.1 What Is Ownership?, Rust 1.99.1, 2024 edition
    locator: Ownership and Functions; Return Values and Scope
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/std/marker/trait.Copy.html#whats-the-difference-between-copy-and-clone
    title: Rust standard library, trait std::marker::Copy, Rust 1.99.1
    locator: What's the difference between Copy and Clone?; When can my type be Copy?
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/std/rc/struct.Rc.html
    title: Rust standard library, struct std::rc::Rc, Rust 1.99.1
    locator: Rc, Rc::clone, Rc::strong_count
    accessed: 2026-11-09
  - url: https://rustc-dev-guide.rust-lang.org/borrow-check.html
    title: Rust Compiler Development Guide, MIR borrow check
    locator: Major phases of the borrow checker
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/error_codes/error-index.html
    title: Rust Compiler Error Index, Rust 1.99.1
    locator: E0373, E0381, E0499, E0502, E0505, E0506, E0507, E0508, E0509, E0597, E0716
    accessed: 2026-11-09
---

## Message

The headline of E0382 has the form ``<action> of [partially ]moved value: `<place>` ``.[^2]

- The action is `use` when the program moves the value or matches on it. It is `borrow` when the program borrows the value, also with an implicit borrow such as the receiver of a method with `&self`. It is `assign to part` when the program assigns to a field.[^3]
- `partially` appears when only some fields of the value moved out.[^2]
- The place is the variable or the field path, as written in the source. When the place has no such name, the colon and the place are left out.[^2]

These are the five forms that rustc gives:

- ``use of moved value: `{}` ``
- ``borrow of moved value: `{}` ``
- ``use of partially moved value: `{}` ``
- ``borrow of partially moved value: `{}` ``
- ``assign to part of moved value: `{}` ``

The program below gives four of them. The labels under the source lines mark three points. The first is where the value was declared, with its type, which is not `Copy`. The second is where the value moved, and the third is where the program used it after the move. The last form, `borrow of moved value`, is in the next section.

```rust fails expect=error:E0382 id=e-messages
struct Pair {
    left: String,
    right: String,
}

fn pair() -> Pair {
    Pair {
        left: String::from("l"),
        right: String::from("r"),
    }
}

fn main() {
    let a = String::from("a");
    drop(a);
    drop(a); // use of moved value

    let p = pair();
    let left = p.left;
    let whole = p; // use of partially moved value

    let p2 = pair();
    let right = p2.right;
    let view = &p2; // borrow of partially moved value

    let mut p3 = pair();
    let moved = p3;
    p3.left = String::new(); // assign to part of moved value

    println!("{left} {right} {}", whole.right);
    println!("{} {}", view.left, moved.left);
}
```

```output for=e-messages stream=stderr
error[E0382]: use of moved value: `a`
  --> main.rs:16:10
   |
14 |     let a = String::from("a");
   |         - move occurs because `a` has type `String`, which does not implement the `Copy` trait
15 |     drop(a);
   |          - value moved here
16 |     drop(a); // use of moved value
   |          ^ value used here after move
   |
help: consider cloning the value if the performance cost is acceptable
   |
15 |     drop(a.clone());
   |           ++++++++

error[E0382]: use of partially moved value: `p`
  --> main.rs:20:17
   |
19 |     let left = p.left;
   |                ------ value partially moved here
20 |     let whole = p; // use of partially moved value
   |                 ^ value used here after partial move
   |
   = note: partial move occurs because `p.left` has type `String`, which does not implement the `Copy` trait

error[E0382]: borrow of partially moved value: `p2`
  --> main.rs:24:16
   |
23 |     let right = p2.right;
   |                 -------- value partially moved here
24 |     let view = &p2; // borrow of partially moved value
   |                ^^^ value borrowed here after partial move
   |
   = note: partial move occurs because `p2.right` has type `String`, which does not implement the `Copy` trait

error[E0382]: assign to part of moved value: `p3`
  --> main.rs:28:5
   |
26 |     let mut p3 = pair();
   |         ------ move occurs because `p3` has type `Pair`, which does not implement the `Copy` trait
27 |     let moved = p3;
   |                 -- value moved here
28 |     p3.left = String::new(); // assign to part of moved value
   |     ^^^^^^^ value partially assigned here after move
   |
note: if `Pair` implemented `Clone`, you could clone the value
  --> main.rs:1:1
   |
 1 | struct Pair {
   | ^^^^^^^^^^^ consider implementing `Clone` for this type
...
27 |     let moved = p3;
   |                 -- you could clone this value

error: aborting due to 4 previous errors

For more information about this error, try `rustc --explain E0382`.
```

> [!history]
> rustc 1.0 and 1.1 gave the same headline with no error code. The code E0382 is in the source of rustc 1.2.0.[^4]

The `help` and `note` lines below the labels change with the case. They suggest a clone, a borrow, or an implementation of `Clone`. They are suggestions, and the right fix depends on the program (see Fix).

On the nightly toolchain, the unstable attribute `#[diagnostic::on_move]` (feature gate `diagnostic_on_move`, tracking issue 154181) lets a type replace the headline with its own text. The code stays E0382.[^5][^2]

## Cause

A value of a type that does not implement `Copy` has exactly one owner. Assignment, a call argument, a return and a `for` loop move the value to a new owner. After the move, the old variable is uninitialized.[^6][^8] The compiler rejects each later read, borrow or partial assignment of it, until a full assignment gives the variable a value again.[^6]

The rule protects the resources that a value owns. A `String` holds a pointer to a heap buffer. If two variables could use the same `String` after a move, both would own the buffer, and both would free it when they go out of scope: a double free.[^9] A `Copy` type has no such resource, so a copy leaves both values usable.[^9]

The check is done at compile time and follows each path of the control flow. rustc computes which places hold a moved value at each point of the MIR of the function, and then reports each use of such a place.[^11] So a move in one branch of an `if`, or in an earlier iteration of a loop, is enough. The error then says that the value may have moved on a path that reaches the use.

A partial move takes a field out of a struct. The other fields stay usable, but the struct as a whole is not.[^6] Only a field of a type that does not implement `Drop` can move out this way.[^6]

## Example

`print_report` takes the vector by value, so the call moves `lines` into the function. The function drops the vector when it returns. The next line borrows `lines` to call `len`.

```rust fails expect=error:E0382 id=e-example
fn print_report(lines: Vec<String>) {
    for line in lines {
        println!("{line}");
    }
}

fn main() {
    let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
    print_report(lines);
    println!("{} lines", lines.len());
}
```

```output for=e-example stream=stderr
error[E0382]: borrow of moved value: `lines`
  --> main.rs:10:26
   |
 8 |     let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
   |         ----- move occurs because `lines` has type `Vec<String>`, which does not implement the `Copy` trait
 9 |     print_report(lines);
   |                  ----- value moved here
10 |     println!("{} lines", lines.len());
   |                          ^^^^^ value borrowed here after move
   |
note: consider changing this parameter type in function `print_report` to borrow instead if owning the value isn't necessary
  --> main.rs:1:24
   |
 1 | fn print_report(lines: Vec<String>) {
   |    ------------        ^^^^^^^^^^^ this parameter takes ownership of the value
   |    |
   |    in this function
help: consider cloning the value if the performance cost is acceptable
   |
 9 |     print_report(lines.clone());
   |                       ++++++++

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0382`.
```

## Fix

Each fix below compiles. Choose the one that matches who must own the value.

### Borrow instead of moving

If the function only reads the value, make the parameter a reference. A `&[String]` accepts a borrow of a `Vec<String>`. This is the usual fix.

```rust run id=e-fix-borrow
fn print_report(lines: &[String]) {
    for line in lines {
        println!("{line}");
    }
}

fn main() {
    let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
    print_report(&lines);
    println!("{} lines", lines.len());
}
```

```output for=e-fix-borrow
orders: 3
refunds: 1
2 lines
```

### Clone the value

If the function must own a value and the caller also needs one, give the function a clone. `clone` on a `Vec<String>` allocates a second vector and copies each string, so use it when that cost is acceptable.[^9]

```rust run id=e-fix-clone
fn print_report(lines: Vec<String>) {
    for line in lines {
        println!("{line}");
    }
}

fn main() {
    let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
    print_report(lines.clone());
    println!("{} lines", lines.len());
}
```

```output for=e-fix-clone
orders: 3
refunds: 1
2 lines
```

### Use the value before the move

If the caller needs only a result computed from the value, compute it before the move.

```rust run id=e-fix-reorder
fn print_report(lines: Vec<String>) {
    for line in lines {
        println!("{line}");
    }
}

fn main() {
    let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
    let count = lines.len();
    print_report(lines);
    println!("{count} lines");
}
```

```output for=e-fix-reorder
orders: 3
refunds: 1
2 lines
```

### Give ownership back

A function that takes ownership can return the value to the caller. Bind the result again, here with the same name.[^8]

```rust run id=e-fix-return
fn add_total(mut lines: Vec<String>) -> Vec<String> {
    let total = lines.len();
    lines.push(format!("total: {total}"));
    lines
}

fn main() {
    let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
    let lines = add_total(lines);
    println!("{lines:?}");
}
```

```output for=e-fix-return
["orders: 3", "refunds: 1", "total: 2"]
```

### Make the value in each iteration of a loop

A move inside a loop body moves the value in the first iteration. The second iteration has no value to move.

```rust fails expect=error:E0382 id=e-loop
fn send(host: &str, body: String) {
    println!("{host}: {body}");
}

fn main() {
    let body = String::from("ping");
    for host in ["alpha", "beta"] {
        send(host, body);
    }
}
```

```output for=e-loop stream=stderr
error[E0382]: use of moved value: `body`
 --> main.rs:8:20
  |
6 |     let body = String::from("ping");
  |         ---- move occurs because `body` has type `String`, which does not implement the `Copy` trait
7 |     for host in ["alpha", "beta"] {
  |     ----------------------------- inside of this loop
8 |         send(host, body);
  |                    ^^^^ value moved here, in previous iteration of loop
  |
note: consider changing this parameter type in function `send` to borrow instead if owning the value isn't necessary
 --> main.rs:1:27
  |
1 | fn send(host: &str, body: String) {
  |    ---- in this function  ^^^^^^ this parameter takes ownership of the value
help: consider cloning the value if the performance cost is acceptable
  |
8 |         send(host, body.clone());
  |                        ++++++++

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0382`.
```

If each iteration needs its own value, make the value inside the loop body. Each iteration then moves a different value. If the function only reads the value, a borrow is better, as in the first fix.

```rust run id=e-fix-loop
fn send(host: &str, body: String) {
    println!("{host}: {body}");
}

fn main() {
    for host in ["alpha", "beta"] {
        let body = String::from("ping");
        send(host, body);
    }
}
```

```output for=e-fix-loop
alpha: ping
beta: ping
```

### Iterate over a reference

A `for` loop calls `IntoIterator::into_iter` on its operand.[^7] For a `Vec<T>`, this takes the vector by value, so `for line in lines` moves `lines`.

```rust fails expect=error:E0382 id=e-for
fn main() {
    let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
    for line in lines {
        println!("{line}");
    }
    println!("{} lines", lines.len());
}
```

```output for=e-for stream=stderr version=1.98.0
error[E0382]: borrow of moved value: `lines`
 --> main.rs:6:26
  |
2 |     let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
  |         ----- move occurs because `lines` has type `Vec<String>`, which does not implement the `Copy` trait
3 |     for line in lines {
  |                 ----- `lines` moved due to this implicit call to `.into_iter()`
...
6 |     println!("{} lines", lines.len());
  |                          ^^^^^ value borrowed here after move
  |
note: `into_iter` takes ownership of the receiver `self`, which moves `lines`
 --> /rustc/$RUSTC/library/core/src/iter/traits/collect.rs:312:17
help: consider iterating over a slice of the `Vec<String>`'s content to avoid moving into the `for` loop
  |
3 |     for line in &lines {
  |                 +

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0382`.
```

Iterate over `&lines` to borrow the vector. Each `line` is then a `&String`.

```rust run id=e-fix-for
fn main() {
    let lines = vec![String::from("orders: 3"), String::from("refunds: 1")];
    for line in &lines {
        println!("{line}");
    }
    println!("{} lines", lines.len());
}
```

```output for=e-fix-for
orders: 3
refunds: 1
2 lines
```

### Derive `Copy` for a small type

A type whose fields are all `Copy`, and which does not implement `Drop`, can derive `Copy`. Its values are then copied, never moved, and E0382 cannot occur for them.[^6][^9]

```rust run id=e-fix-copy
#[derive(Clone, Copy, Debug)]
struct Point {
    x: i32,
    y: i32,
}

fn shift(p: Point) -> Point {
    Point { x: p.x + 1, y: p.y }
}

fn main() {
    let start = Point { x: 0, y: 5 };
    let next = shift(start);
    println!("{start:?} {next:?}");
}
```

```output for=e-fix-copy
Point { x: 0, y: 5 } Point { x: 1, y: 5 }
```

### Share ownership with `Rc`

If two parts of a program must both own one value, put it in an `Rc<T>`. `Rc::clone` copies the pointer and increments a reference count. It does not copy the value. The value is dropped when the last `Rc` is dropped.[^10] Use `Arc<T>` when the owners are on different threads.[^10] Use `Rc<RefCell<T>>` when the owners must change the value.[^1]

```rust run id=e-fix-rc
use std::rc::Rc;

struct Service {
    name: &'static str,
    config: Rc<String>,
}

fn main() {
    let config = Rc::new(String::from("retries=3"));
    let api = Service {
        name: "api",
        config: Rc::clone(&config),
    };
    let worker = Service {
        name: "worker",
        config,
    };
    println!("{} {}", api.name, api.config);
    println!("{} {}", worker.name, worker.config);
    println!("owners: {}", Rc::strong_count(&api.config));
}
```

```output for=e-fix-rc
api retries=3
worker retries=3
owners: 2
```

## Related diagnostics

Each line gives the meaning of the code in the error index.[^12]

- E0381: a variable is used or captured before it has a value. E0382 is for a variable whose value moved out.
- E0505: a value moves out while a borrow of it is still live.
- E0507: a value moves out of a borrowed place, for example through a reference or an index.
- E0508: a value moves out of an element of an array whose element type is not `Copy`.
- E0509: a field moves out of a value whose type implements `Drop`.
- E0499: a variable is borrowed as mutable more than once at the same time.
- E0502: a variable is borrowed as mutable while it is borrowed as shared, or the reverse.
- E0506: a program assigns to a value while it is borrowed.
- E0597: a value is dropped while it is still borrowed.
- E0716: a temporary value is dropped while a borrow of it is still in use.
- E0373: a closure borrows a variable but can outlive the function that owns the variable.

The feature page [Ownership and borrowing](/rust/ownership/) gives the rules behind all of these errors.

## References

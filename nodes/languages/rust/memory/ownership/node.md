---
tag: NEW-rust-ownership
kind: feature
owner: rust
slug: ownership
name: Ownership and borrowing
aliases: [move semantics, borrowing, borrow checker]
area: languages/rust/memory
level: core
summary: >-
  Each value in Rust has one owner, and the value is dropped when its owner goes out of scope. Assignment moves a value unless its type is Copy, and a reference borrows a value without taking ownership.
grammar: "BorrowExpression, The Rust Reference, 8.2.4 Operator expressions"
spec:
  url: https://doc.rust-lang.org/1.99.1/reference/expressions.html#moved-and-copied-types
  locator: expr.move
proposals: [rfc-2094, rfc-2025, rfc-3606]
compat: compat.yaml
since: "1.0.0"
see_also: [rust/errors/E0382]
sources:
  - url: https://doc.rust-lang.org/1.99.1/book/ch04-01-what-is-ownership.html#ownership-rules
    title: The Rust Programming Language, 4.1 What Is Ownership?, Rust 1.99.1, 2024 edition
    locator: Ownership Rules
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/book/ch04-02-references-and-borrowing.html#the-rules-of-references
    title: The Rust Programming Language, 4.2 References and Borrowing, Rust 1.99.1, 2024 edition
    locator: The Rules of References
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/reference/expressions.html#moved-and-copied-types
    title: The Rust Reference, Expressions, Moved and copied types, Rust 1.99.1
    locator: expr.move
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/reference/special-types-and-traits.html#copy
    title: The Rust Reference, Special types and traits, Copy and Clone, Rust 1.99.1
    locator: lang-types.copy, lang-types.clone
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/reference/destructors.html
    title: The Rust Reference, Destructors, Rust 1.99.1
    locator: destructors.intro, destructors.operation, destructors.scope, destructors.forget, destructors.process-termination
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/reference/expressions/operator-expr.html#borrow-operators
    title: The Rust Reference, Operator expressions, Borrow operators, Rust 1.99.1
    locator: expr.operator.borrow
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/reference/expressions/closure-expr.html#r-expr.closure.capture-inference
    title: The Rust Reference, Closure expressions, Rust 1.99.1
    locator: expr.closure.capture-inference, expr.closure.capture-move
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/std/marker/trait.Copy.html#whats-the-difference-between-copy-and-clone
    title: Rust standard library, trait std::marker::Copy, Rust 1.99.1
    locator: What's the difference between Copy and Clone?; Implementors
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/std/ops/trait.Drop.html#tymethod.drop
    title: Rust standard library, trait std::ops::Drop, Rust 1.99.1
    locator: Required methods, fn drop
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/std/mem/fn.forget.html#safety
    title: Rust standard library, function std::mem::forget, Rust 1.99.1
    locator: Safety
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/std/thread/fn.spawn.html
    title: Rust standard library, function std::thread::spawn, Rust 1.99.1
    locator: the 'static constraint
    accessed: 2026-11-09
  - url: https://rust-lang.github.io/rfcs/2094-nll.html
    title: "RFC 2094: Non-lexical lifetimes"
    locator: Motivation, Problem case #3; Detailed design
    accessed: 2026-11-09
  - url: https://rust-lang.github.io/rfcs/2025-nested-method-calls.html#detailed-design
    title: "RFC 2025: Enable nested method calls (two-phase borrows)"
    locator: Detailed design
    accessed: 2026-11-09
  - url: https://rustc-dev-guide.rust-lang.org/borrow-check/two-phase-borrows.html
    title: Rust Compiler Development Guide, Two-phase borrows
    locator: the three cases of two-phase borrows
    accessed: 2026-11-09
  - url: https://rustc-dev-guide.rust-lang.org/borrow-check.html
    title: Rust Compiler Development Guide, MIR borrow check
    locator: Major phases of the borrow checker
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/1.99.1/nomicon/drop-flags.html
    title: The Rustonomicon, Drop Flags
    locator: Drop Flags
    accessed: 2026-11-09
  - url: https://doc.rust-lang.org/edition-guide/rust-2024/temporary-tail-expr-scope.html
    title: The Rust Edition Guide, Rust 2024, Tail expression temporary scope
    locator: Details; Migration
    accessed: 2026-11-09
  - url: https://blog.rust-lang.org/2026/08/04/enabling-polonius-alpha-on-nightly/
    title: Enabling the next iteration of the borrow checker on nightly, Rust Blog, 2026-08-04
    locator: whole post
    accessed: 2026-11-09
  - url: https://eel.is/c++draft/forward
    title: Working Draft, Programming Languages C++, [forward] Forward/move helpers
    locator: "[forward] std::move"
    accessed: 2026-11-09
  - url: https://eel.is/c++draft/lib.types.movedfrom
    title: Working Draft, Programming Languages C++, [lib.types.movedfrom] Moved-from state of library types
    locator: "[lib.types.movedfrom]"
    accessed: 2026-11-09
  - url: https://eel.is/c++draft/unique.ptr.single.ctor
    title: Working Draft, Programming Languages C++, [unique.ptr.single.ctor] unique_ptr constructors
    locator: "[unique.ptr.single.ctor], move constructor, postconditions"
    accessed: 2026-11-09
  - url: https://eel.is/c++draft/class.dtor
    title: Working Draft, Programming Languages C++, [class.dtor] Destructors
    locator: "[class.dtor]/14.3"
    accessed: 2026-11-09
  - url: https://github.com/swiftlang/swift-evolution/blob/main/proposals/0366-move-function.md
    title: "SE-0366: consume operator to end the lifetime of a variable binding"
    locator: Proposed solution; Detailed design
    accessed: 2026-11-09
  - url: https://github.com/swiftlang/swift-evolution/blob/main/proposals/0377-parameter-ownership-modifiers.md
    title: "SE-0377: borrowing and consuming parameter ownership modifiers"
    locator: Detailed design
    accessed: 2026-11-09
  - url: https://github.com/swiftlang/swift-evolution/blob/main/proposals/0390-noncopyable-structs-and-enums.md
    title: "SE-0390: Noncopyable structs and enums"
    locator: Detailed design
    accessed: 2026-11-09
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.3.1
    title: The Java Language Specification, Java SE 25 Edition, 4.3.1 Objects
    locator: "4.3.1"
    accessed: 2026-11-09
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.6
    title: The Java Language Specification, Java SE 25 Edition, 12.6 Finalization of Class Instances
    locator: "12.6"
    accessed: 2026-11-09
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.20.3
    title: The Java Language Specification, Java SE 25 Edition, 14.20.3 try-with-resources
    locator: "14.20.3"
    accessed: 2026-11-09
---

## Synopsis

Ownership has no syntax of its own. It is a set of rules that the compiler applies to each binding, assignment, call, return and scope. The program below uses each form once.

```rust run id=synopsis
#[derive(Clone, Copy)]
struct Point {
    x: i32,
    y: i32,
}

struct Logger;

impl Drop for Logger {
    fn drop(&mut self) {
        println!("logger dropped");
    }
}

fn take(text: String) -> usize {
    text.len()
}

fn read(text: &str) -> usize {
    text.len()
}

fn append(text: &mut String) {
    text.push('!');
}

fn main() {
    let a = String::from("hi");
    let b = a; // move: `a` cannot be used after this line
    let mut c = b.clone(); // clone: a second, independent String
    append(&mut c); // mutable borrow for the length of the call
    let n = read(&c); // shared borrow for the length of the call
    let m = take(c); // move into the parameter; `take` drops it
    let p = Point { x: 1, y: 2 };
    let q = p; // copy: Point is Copy, so `p` stays usable
    let log = Logger;
    drop(log); // run the destructor now
    println!("{b} {n} {m} {} {}", p.x + q.x, p.y + q.y);
}
```

```output for=synopsis
logger dropped
hi 3 3 2 4
```

- `let b = a;` moves the value of `a` into `b`. If the type implements `Copy`, it copies the value, and `a` stays usable.
- `a.clone()` makes an independent value through the `Clone` trait.
- `&a` borrows `a` through a shared reference of type `&T`.
- `&mut a` borrows `a` through a mutable reference of type `&mut T`.
- `move || ...` makes a closure that takes ownership of the variables that it uses.
- `impl Drop for T` gives the type `T` a destructor. The call `drop(a)` drops `a` before the end of its scope.

## Semantics

r1. Each value has one owner at a time. The owner is a variable, a field or element of another value, or a temporary. When the owner goes out of scope, the value is dropped.[^1] {rule:rust/destructors.intro}

r2. When a program uses a place expression (a variable, a field, an element) as a value, the value moves out of the place, unless its type implements `Copy`. {rule:rust/expr.move.intro} {rule:rust/expr.move.copy} After a move out of a local variable, the variable is uninitialized, and the compiler rejects each read of it until an assignment gives it a value again ([E0382](/rust/errors/E0382/)). {rule:rust/expr.move.deinitialization}[^3] An argument of a call and a return value move in the same way.[^1]

r3. A program can move out of these places: a variable that is not borrowed, a temporary, the target of a `Box<T>`, and a field of a movable place whose type does not implement `Drop`. A move out of any other place is an error. Examples are an element behind an index or a reference (E0507), and a field of a type that implements `Drop` (E0509). {rule:rust/expr.move.movable-place} {rule:rust/expr.move.place-invalid}

r4. A type can implement `Copy` only if it does not implement `Drop` and all its fields are `Copy`. A use of a `Copy` value copies it, and the source place stays usable. {rule:rust/lang-types.copy.constraint} {rule:rust/lang-types.copy.behavior}[^4] The integer and floating-point types, `bool`, `char`, shared references `&T`, and tuples and arrays of `Copy` types are `Copy`. `String`, `Vec<T>`, `Box<T>` and `&mut T` are not.[^8]

r5. `Clone` is the trait for an explicit copy with the method `clone`. A copy through `Copy` is always a bitwise copy. An implementation of `clone` can do the work that the type needs, for example copy the heap buffer of a `String`.[^8] `Clone` is a supertrait of `Copy`, so each `Copy` type is also `Clone`. {rule:rust/lang-types.clone.intro}

r6. To drop a value, Rust first calls `Drop::drop` if the type implements `Drop`, and then drops each field. The fields of a struct drop in declaration order, the fields of a tuple in order, and the elements of an array from first to last. {rule:rust/destructors.operation}[^5] When control leaves a scope, its variables drop in reverse order of declaration. {rule:rust/destructors.scope.intro} Function parameters drop after the local variables of the body. {rule:rust/destructors.scope.params} Only initialized values drop, so a variable whose value moved out is not dropped. {rule:rust/destructors.intro}

r7. A program cannot call `Drop::drop` directly (E0040). The function `std::mem::drop` takes its argument by value, so the value moves into the call and is dropped when the call returns.[^9]

r8. Rust does not guarantee that a destructor runs. `std::mem::forget`, `ManuallyDrop`, a reference cycle of `Rc` values, and `std::process::exit` all leave values that are never dropped, and each of them is safe code. {rule:rust/destructors.manually-suppressing} {rule:rust/destructors.process-termination}[^10]

r9. The expression `&place` makes a shared reference, and `&mut place` makes a mutable reference. While a shared borrow is live, the place can be read and borrowed as shared again, but it cannot change. While a mutable borrow is live, the program cannot use the place in any way except through that reference. {rule:rust/expr.operator.borrow.lifetime}[^6] Thus at each point a value has any number of `&T` or exactly one `&mut T`: aliasing and mutation never occur together.[^2] A conflict is error E0499 (two mutable borrows) or E0502 (a shared and a mutable borrow).

r10. A reference must not outlive the value that it points to. If the owner drops or moves the value while a borrow of it is live, the compiler rejects the program (E0597, E0505).[^2]

r11. A borrow is live from the point where the program makes it to each later point where the program can still use the reference. This is true along each path of the control flow. The end of the enclosing block is not a limit. This is the model of non-lexical lifetimes (NLL).[^12][^2] When a value whose type implements `Drop` holds a reference, its drop at the end of the scope is a use, so the borrow lasts until that point (see Pitfalls).[^12]

r12. Some implicit mutable borrows have two phases. First the compiler reserves the borrow, and during the reservation it acts as a shared borrow. The borrow activates as a full mutable borrow at the call. Three borrows are two-phase: the automatic borrow of a method receiver of type `&mut Self`, a mutable reborrow passed as a function argument, and the implicit borrow of an overloaded compound assignment such as `+=`. A `&mut` that is written in the source is never two-phase.[^13][^14]

r13. A closure without `move` captures each variable by shared reference if the body permits it, else by mutable reference, else by move or copy. A `move` closure moves each captured variable into the closure, or copies it when the type is `Copy`. {rule:rust/expr.closure.capture-inference} {rule:rust/expr.closure.capture-move}[^7]

r14. A temporary is dropped at the end of the smallest enclosing statement, function, or other temporary scope that the Reference lists. {rule:rust/destructors.scope.temporary.enclosing} In edition 2024, the temporaries of the tail expression of a block are dropped before the local variables of the block. {rule:rust/destructors.scope.temporary.edition2024}[^17] The pattern `_` does not bind, copy, move or borrow the value. {rule:rust/patterns.wildcard.no-binding}

## Examples

An `i32` is `Copy`, so both names stay usable. A `String` moves. An assignment makes the moved-from variable usable again.

```rust run id=move-copy
fn main() {
    let count = 3;
    let copy_of_count = count; // i32 is Copy: both names hold a value
    println!("{count} {copy_of_count}");

    let mut name = String::from("Ada");
    let moved = name; // the String moves; `name` is now uninitialized
    let cloned = moved.clone(); // a second heap buffer with the same text
    name = String::from("Grace"); // assignment makes `name` usable again
    println!("{moved} {cloned} {name}");
}
```

```output for=move-copy
3 3
Ada Ada Grace
```

Variables drop in reverse order of declaration, and an inner block drops its variables when it ends. The fields of `pair` drop in declaration order. The call `drop(c)` ends `c` early, so `c` is not dropped again at the end of `main`.

```rust run id=drop-order
struct Noisy(&'static str);

impl Drop for Noisy {
    fn drop(&mut self) {
        println!("drop {}", self.0);
    }
}

struct Pair {
    first: Noisy,
    second: Noisy,
}

fn main() {
    let _a = Noisy("a");
    let pair = Pair {
        first: Noisy("pair.first"),
        second: Noisy("pair.second"),
    };
    println!("made {} and {}", pair.first.0, pair.second.0);
    let c = Noisy("c");
    {
        let _inner = Noisy("inner");
    }
    drop(c);
    let _b = Noisy("b");
    println!("end of main");
}
```

```output for=drop-order
made pair.first and pair.second
drop inner
drop c
end of main
drop b
drop pair.first
drop pair.second
drop a
```

Two shared borrows can be live together. The mutable borrow for `add_tax` is allowed because the program does not use `a` or `b` after it.

```rust run id=borrow
fn total(prices: &[u32]) -> u32 {
    prices.iter().sum()
}

fn add_tax(prices: &mut [u32]) {
    for price in prices.iter_mut() {
        *price += *price / 10;
    }
}

fn main() {
    let mut prices = vec![100, 250];
    let a = &prices;
    let b = &prices; // two shared borrows at the same time
    println!("{} {}", total(a), total(b));
    add_tax(&mut prices); // `a` and `b` are not used again, so this is allowed
    println!("{prices:?} {}", total(&prices));
}
```

```output for=borrow
350 350
[110, 275] 385
```

Here the shared borrow `first` is still live at the call to `push`, because the last line uses it. The compiler rejects the mutable borrow.

```rust fails expect=error:E0502 id=aliasing
fn main() {
    let mut names = vec![String::from("Ada")];
    let first = &names[0];
    names.push(String::from("Grace"));
    println!("{first}");
}
```

```output for=aliasing stream=stderr
error[E0502]: cannot borrow `names` as mutable because it is also borrowed as immutable
 --> main.rs:4:5
  |
3 |     let first = &names[0];
  |                  ----- immutable borrow occurs here
4 |     names.push(String::from("Grace"));
  |     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ mutable borrow occurs here
5 |     println!("{first}");
  |                ----- immutable borrow later used here

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0502`.
```

A borrow ends at the last use of the reference, not at the end of the block.

```rust run id=nll
fn main() {
    let mut scores = vec![7, 3, 9];
    let best = scores.iter().max().unwrap(); // `best` borrows `scores`
    println!("best: {best}"); // the last use of `best`
    scores.push(10); // allowed: the borrow ended at the line above
    println!("{scores:?}");
}
```

```output for=nll
best: 9
[7, 3, 9, 10]
```

In `sizes.push(sizes.len())`, the method call borrows `sizes` mutably for `push`, and the argument reads `sizes`. The two-phase rule reserves the mutable borrow, lets `len` read the vector, and then activates the borrow.

```rust run id=two-phase
fn main() {
    let mut sizes = vec![1, 2];
    sizes.push(sizes.len()); // `&mut sizes` is reserved, then `len` reads it
    let r = &mut sizes;
    r.push(r.len()); // the same rule for an implicit reborrow of `r`
    println!("{sizes:?}");
}
```

```output for=two-phase
[1, 2, 2, 3]
```

The explicit form has a `&mut` in the source, so it is not two-phase. The compiler rejects it.

```rust fails expect=error:E0502 id=two-phase-explicit
fn main() {
    let mut sizes = vec![1, 2];
    Vec::push(&mut sizes, sizes.len());
    println!("{sizes:?}");
}
```

```output for=two-phase-explicit stream=stderr
error[E0502]: cannot borrow `sizes` as immutable because it is also borrowed as mutable
 --> main.rs:3:27
  |
3 |     Vec::push(&mut sizes, sizes.len());
  |     --------- ----------  ^^^^^ immutable borrow occurs here
  |     |         |
  |     |         mutable borrow occurs here
  |     mutable borrow later used by call

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0502`.
```

A spawned thread can outlive the function that started it, so `thread::spawn` needs a closure that borrows nothing from the stack.[^11] The `move` closure owns `log`, and the thread gives the vector back as the result of `join`.

```rust run id=thread-move
use std::thread;

fn main() {
    let mut log = vec![String::from("start")];
    log.push(String::from("queued"));
    let worker = thread::spawn(move || {
        log.push(String::from("done")); // the closure owns `log`
        log
    });
    let log = worker.join().unwrap(); // ownership comes back through `join`
    println!("{log:?}");
}
```

```output for=thread-move
["start", "queued", "done"]
```

## Versions

In edition 2024 the temporaries of a tail expression are dropped before the local variables of the block. Below, the temporary `Ref` from `name.borrow()` is dropped before `name`, so the program compiles.

```rust run id=tail-2024
use std::cell::RefCell;

fn name_length() -> usize {
    let name = RefCell::new(String::from("Ada"));
    name.borrow().len()
}

fn main() {
    println!("{}", name_length());
}
```

```output for=tail-2024
3
```

In edition 2021 and before, the same temporary lives until the end of the function body, after `name` is dropped, so the compiler rejects the program.

```rust fails expect=error:E0597 flags="--edition 2021" id=tail-2021
use std::cell::RefCell;

fn name_length() -> usize {
    let name = RefCell::new(String::from("Ada"));
    name.borrow().len()
}

fn main() {
    println!("{}", name_length());
}
```

```output for=tail-2021 stream=stderr
error[E0597]: `name` does not live long enough
 --> main.rs:5:5
  |
4 |     let name = RefCell::new(String::from("Ada"));
  |         ---- binding `name` declared here
5 |     name.borrow().len()
  |     ^^^^---------
  |     |
  |     borrowed value does not live long enough
  |     a temporary with access to the borrow is created here ...
6 | }
  | -
  | |
  | `name` dropped here while still borrowed
  | ... and the borrow might be used here, when that temporary is dropped and runs the destructor for type `Ref<'_, String>`
  |
  = note: the temporary is part of an expression at the end of a block;
          consider forcing this temporary to be dropped sooner, before the block's local variables are dropped
help: for example, you could save the expression's value in a new local variable `x` and then make `x` be the expression at the end of the block
  |
5 |     let x = name.borrow().len(); x
  |     +++++++                    +++

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0597`.
```

The change can also make a destructor run earlier, before a local variable drops. `cargo fix --edition` runs the lint `tail_expr_drop_order`, which reports each tail expression where a temporary with a destructor changes its drop order. It does not change the code.[^17]

Code written for the lexical borrow checker that came before NLL often has an extra block `{ ... }` whose only purpose is to end a borrow. Under NLL these blocks are not necessary. The compat table gives the version and the edition where NLL and two-phase borrows started.

## Pitfalls

### Move inside a loop

A call that takes a `String` by value moves it in the first iteration. The second iteration has no value to move.

```rust fails expect=error:E0382 id=loop-move
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

```output for=loop-move stream=stderr
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

If the function only reads the text, make the parameter a reference. The page [E0382](/rust/errors/E0382/) gives the other fixes.

```rust run id=loop-move-fix
fn send(host: &str, body: &str) {
    println!("{host}: {body}");
}

fn main() {
    let body = String::from("ping");
    for host in ["alpha", "beta"] {
        send(host, &body);
    }
}
```

```output for=loop-move-fix
alpha: ping
beta: ping
```

### Move out of an index

`names[0]` is a place inside the vector. A `let` that uses it as a value tries to move the `String` out, and only a `Copy` type allows that.

```rust fails expect=error:E0507 id=index-move
fn main() {
    let names = vec![String::from("Ada"), String::from("Grace")];
    let first = names[0];
    println!("{first}");
}
```

```output for=index-move stream=stderr
error[E0507]: cannot move out of index of `Vec<String>`
 --> main.rs:3:17
  |
3 |     let first = names[0];
  |                 ^^^^^^^^ move occurs because value has type `String`, which does not implement the `Copy` trait
  |
help: consider borrowing here
  |
3 |     let first = &names[0];
  |                 +
help: consider cloning the value if the performance cost is acceptable
  |
3 |     let first = names[0].clone();
  |                         ++++++++

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0507`.
```

Borrow the element, clone it, or replace it with `std::mem::take` (or `std::mem::replace`), which leaves a value in its place.

```rust run id=index-move-fix
fn main() {
    let mut names = vec![String::from("Ada"), String::from("Grace")];
    let first = &names[0]; // borrow the element
    println!("{first}");
    let copy = names[1].clone(); // or clone it
    let taken = std::mem::take(&mut names[0]); // or swap in an empty String
    println!("{copy} {taken} {names:?}");
}
```

```output for=index-move-fix
Ada
Grace Ada ["", "Grace"]
```

### `let _` drops at once

`let _ = value;` does not bind the value. If the value is a temporary, such as a lock guard that a function returns, it is dropped at the end of the statement. A named binding, even one that starts with `_`, keeps it until the end of the scope.

```rust run id=let-underscore
struct Guard(&'static str);

impl Drop for Guard {
    fn drop(&mut self) {
        println!("release {}", self.0);
    }
}

fn main() {
    let _ = Guard("a"); // no binding: the temporary drops at the end of this statement
    let _held = Guard("b"); // a binding: it drops at the end of `main`
    println!("work");
}
```

```output for=let-underscore
release a
work
release b
```

### A `move` closure copies a `Copy` value

A `move` closure gets its own copy of a captured `i32`. The closure changes its copy, and the variable outside does not change.

```rust wrong id=move-closure
fn main() {
    let mut count = 0;
    let mut bump = move || {
        count += 1;
        count
    };
    bump();
    println!("closure saw {}", bump());
    println!("count is still {count}");
}
```

```output for=move-closure
closure saw 2
count is still 0
```

To change the outer variable, capture it by mutable reference: remove `move`. The borrow ends after the last call of `bump`.

```rust run id=move-closure-fix
fn main() {
    let mut count = 0;
    let mut bump = || {
        count += 1;
        count
    };
    bump();
    println!("closure saw {}", bump());
    println!("count is {count}");
}
```

```output for=move-closure-fix
closure saw 2
count is 2
```

### A destructor keeps a borrow live

The drop of a value whose type implements `Drop` is a use of the references inside it. So a borrow inside such a value stays live until the end of the scope, after the last use that the source shows.

```rust fails expect=error:E0502 id=drop-borrow
struct Cursor<'a> {
    items: &'a Vec<i32>,
}

impl Drop for Cursor<'_> {
    fn drop(&mut self) {
        println!("cursor over {} items closed", self.items.len());
    }
}

fn main() {
    let mut items = vec![1, 2];
    let cursor = Cursor { items: &items };
    println!("{}", cursor.items[0]);
    items.push(3);
}
```

```output for=drop-borrow stream=stderr
error[E0502]: cannot borrow `items` as mutable because it is also borrowed as immutable
  --> main.rs:15:5
   |
13 |     let cursor = Cursor { items: &items };
   |                                  ------ immutable borrow occurs here
14 |     println!("{}", cursor.items[0]);
15 |     items.push(3);
   |     ^^^^^^^^^^^^^ mutable borrow occurs here
16 | }
   | - immutable borrow might be used here, when `cursor` is dropped and runs the `Drop` code for type `Cursor`

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0502`.
```

Drop the value before the conflicting borrow.

```rust run id=drop-borrow-fix
struct Cursor<'a> {
    items: &'a Vec<i32>,
}

impl Drop for Cursor<'_> {
    fn drop(&mut self) {
        println!("cursor over {} items closed", self.items.len());
    }
}

fn main() {
    let mut items = vec![1, 2];
    let cursor = Cursor { items: &items };
    println!("{}", cursor.items[0]);
    drop(cursor); // end the borrow before the mutation
    items.push(3);
    println!("{items:?}");
}
```

```output for=drop-borrow-fix
1
cursor over 2 items closed
[1, 2, 3]
```

### `process::exit` does not run destructors

`std::process::exit` ends the process without unwinding the stack, so no destructor runs.

```rust wrong id=exit
struct TempFile(&'static str);

impl Drop for TempFile {
    fn drop(&mut self) {
        println!("delete {}", self.0);
    }
}

fn main() {
    let _scratch = TempFile("scratch.tmp");
    println!("exiting");
    std::process::exit(0);
}
```

```output for=exit
exiting
```

Put the work in a function that returns, and call `exit` after the values are dropped.

```rust run id=exit-fix
struct TempFile(&'static str);

impl Drop for TempFile {
    fn drop(&mut self) {
        println!("delete {}", self.0);
    }
}

fn run() -> i32 {
    let _scratch = TempFile("scratch.tmp");
    println!("exiting");
    0
}

fn main() {
    let code = run(); // `_scratch` is dropped when `run` returns
    std::process::exit(code);
}
```

```output for=exit-fix
exiting
delete scratch.tmp
```

### Return a borrow from one branch, then mutate

NLL rejects a function that returns a borrow from one branch and mutates the same value in another branch. On the path that returns `found`, the borrow must last as long as the result. The NLL checker does not separate the paths, so it also treats the borrow as live on the path that calls `insert`. This is problem case #3 of RFC 2094.[^12]

```rust fails expect=error:E0502 id=get-or-insert
use std::collections::HashMap;

fn label(cache: &mut HashMap<u32, String>, id: u32) -> &String {
    if let Some(found) = cache.get(&id) {
        return found;
    }
    cache.insert(id, format!("item-{id}"));
    &cache[&id]
}

fn main() {
    let mut cache = HashMap::new();
    println!("{}", label(&mut cache, 7));
}
```

```output for=get-or-insert stream=stderr
error[E0502]: cannot borrow `*cache` as mutable because it is also borrowed as immutable
 --> main.rs:7:5
  |
3 | fn label(cache: &mut HashMap<u32, String>, id: u32) -> &String {
  |                 - let's call the lifetime of this reference `'1`
4 |     if let Some(found) = cache.get(&id) {
  |                          ----- immutable borrow occurs here
5 |         return found;
  |                ----- returning this value requires that `*cache` is borrowed for `'1`
6 |     }
7 |     cache.insert(id, format!("item-{id}"));
  |     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ mutable borrow occurs here

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0502`.
```

Use an API that does the lookup and the insertion in one call, such as the `entry` API of `HashMap`.

```rust run id=get-or-insert-fix
use std::collections::HashMap;

fn label(cache: &mut HashMap<u32, String>, id: u32) -> &String {
    cache.entry(id).or_insert_with(|| format!("item-{id}"))
}

fn main() {
    let mut cache = HashMap::new();
    println!("{}", label(&mut cache, 7));
}
```

```output for=get-or-insert-fix
item-7
```

Polonius, the next borrow checker, accepts the first program. The Rust project enabled its alpha version by default on the nightly toolchain (announced on 2026-08-04). Stable Rust does not use it.[^18] The nightly toolchain 1.100.0 of 2026-09-06 compiles the first program, and it prints `item-7`.

## Comparison

**C++.** `std::move` does not move anything. It is a cast to an rvalue reference, and the move constructor or move assignment of the type does the transfer.[^19] The source object stays alive, and its destructor runs at the end of its block, as for every automatic object (RAII).[^22] Standard library types are left in a valid but unspecified state[^20], and a moved-from `std::unique_ptr` holds a null pointer.[^21] A use after a move compiles. Rust instead makes the moved-from variable unusable, and no destructor runs for it.

**Swift.** Swift copies values implicitly by default. The `consume` operator ends the lifetime of a local binding, and a later use is a compile-time error.[^23] The parameter modifiers `borrowing` and `consuming` choose between a borrow and a transfer of ownership, and such parameters are not copied implicitly.[^24] A noncopyable type (`~Copyable`) is always moved and can have a `deinit`, which is close to a Rust type without `Copy` that implements `Drop`.[^25]

**A garbage-collected language (Java).** Many references can point to the same object, and each of them can change its state.[^26] The collector reclaims an object some time after no reference reaches it. The language does not say when a finalizer runs, and Java SE 25 allows an implementation to disable finalization.[^27] So Java ties the release of a resource to a block with `try`-with-resources, which closes the resources in reverse order of initialization.[^28] Rust releases memory and other resources at the same point, the drop at the end of the owner's scope, with no collector at run time.[^1]

## Implementation notes

rustc runs the borrow checker on MIR, the mid-level intermediate representation of each function body. The checker finds where each value moves and computes the points of the control-flow graph where each borrow is live. Then it walks the MIR and reports each action that conflicts with a live borrow.[^15] Because MIR is a control-flow graph, the checker can end a borrow at its last use, which is the basis of NLL.[^15]

A drop at the end of a scope is a statement in MIR.[^12] When the compiler cannot know at compile time if a variable still holds a value, it keeps a drop flag on the stack and tests it at run time.[^16] Below, `token` moves on one path only, so its drop at the end of `main` depends on the flag.

```rust run id=drop-flag
struct Noisy(&'static str);

impl Drop for Noisy {
    fn drop(&mut self) {
        println!("drop {}", self.0);
    }
}

fn main() {
    let token = Noisy("token");
    if std::env::args().count() > 3 {
        drop(token); // moved on this path only
    }
    println!("end of main");
}
```

```output for=drop-flag
end of main
drop token
```

A move and a copy both copy the bytes of the value. The difference is only that the source of a move is no longer usable. The optimizer often removes the copy.[^8]

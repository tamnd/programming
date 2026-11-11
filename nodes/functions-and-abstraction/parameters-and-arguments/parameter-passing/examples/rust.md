```rust run id=modes
fn consume(s: String) -> usize {
    s.len()
}

fn read(s: &str) -> usize {
    s.len()
}

fn append(s: &mut String) {
    s.push_str(" world");
}

fn main() {
    let mut text = String::from("hello");
    println!("{}", read(&text));
    append(&mut text);
    println!("{text}");
    let n = consume(text);
    println!("{n}");
}
```

```output for=modes
5
hello world
11
```

- Verdict: native. Passing a value to a function moves it or copies it, as an assignment does. A type that implements `Copy`, such as `i32`, is copied. Other types, such as `String`, move, and the caller cannot use the variable after the call.
- A shared reference `&T` lends the value for reading. A mutable reference `&mut T` lends it for change, and the call must write `&mut` too.
- Rust has no default values, no named arguments and no variadic functions, except for C functions. Builder types, `Option` parameters and macros fill those roles.

```rust fails expect=compile id=moved
fn consume(s: String) -> usize {
    s.len()
}

fn main() {
    let text = String::from("hello");
    let n = consume(text);
    println!("{n} {text}");
}
```

The call moves `text` into `consume`. The compiler rejects the later use with error E0382. Pass `&text` if the function only reads the string.

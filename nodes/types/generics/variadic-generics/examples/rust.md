```rust run id=macro-per-size
trait Arity {
    fn arity() -> usize;
}

// One impl for each tuple size, written by a macro.
macro_rules! impl_arity {
    ($($name:ident),+) => {
        impl<$($name),+> Arity for ($($name,)+) {
            fn arity() -> usize {
                [$(stringify!($name)),+].len()
            }
        }
    };
}

impl_arity!(A);
impl_arity!(A, B);
impl_arity!(A, B, C);

fn main() {
    println!(
        "{} {} {}",
        <(i32,)>::arity(),
        <(i32, &str)>::arity(),
        <(u8, u8, u8)>::arity()
    );
}
```

```output for=macro-per-size
1 2 3
```

```rust fails expect=compile id=thirteen
fn main() {
    let t = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
    println!("{:?}", t);
}
```

- Verdict: absent. Rust has no type parameter packs. The macro writes one `impl` for each size, and the trait has no `impl` for sizes that the macro did not write. The standard library does the same for `Debug`, so a tuple with 13 elements cannot be printed with `{:?}`.

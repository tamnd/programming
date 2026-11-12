```rust run id=nested-and-doc-comments
//! An inner doc comment documents the item that contains it, here the crate.

/// Returns the area of a circle.
/// An outer doc comment is the same as the attribute #[doc = "..."].
fn area(r: f64) -> f64 {
    std::f64::consts::PI * r * r
}

#[doc = "Returns twice x. This attribute is a doc comment written in the long form."]
fn double(x: i32) -> i32 {
    x * 2
}

fn main() {
    println!("{:.4} {}", area(1.0), double(21));

    /* Block comments nest in Rust:
       /* this inner comment */
       and this line is still inside the outer one. */
    println!("after a nested comment");

    // Comments are not tokens, so a macro never sees them.
    println!("{}", stringify!(a /* hidden */ + b // also hidden
    ));

    // In a string, the comment markers are plain text.
    println!("{}", r"// not a comment /* nor this */".len());
}
```

```output for=nested-and-doc-comments
3.1416 42
after a nested comment
a + b
31
```

- Verdict: native. Rust block comments nest, so a region that is commented out can hold other block comments. `///` and `//!` are doc comments, the same as a `#[doc]` attribute, and rustdoc reads them. Other comments are whitespace, so `stringify!` sees `a + b` with no comment text.

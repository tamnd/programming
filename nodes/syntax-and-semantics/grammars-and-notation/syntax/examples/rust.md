```rust run id=macros-and-desugaring
// A macro by example matches a pattern of syntax and writes new syntax.
macro_rules! square_all {
    ($($x:expr),*) => { vec![$($x * $x),*] };
}

struct Counter { n: u32 }

impl Counter {
    fn bump(&mut self) -> u32 { self.n += 1; self.n }
}

fn main() {
    println!("{:?}", square_all!(1, 2 + 1, 4));

    // Method call syntax is sugar for a call of the associated function.
    let mut c = Counter { n: 0 };
    let a = c.bump();
    let b = Counter::bump(&mut c);
    println!("{a} {b}");

    // A for loop is sugar for a loop over IntoIterator::into_iter and next.
    let mut total = 0;
    for v in [1, 2, 3] { total += v; }
    let mut it = IntoIterator::into_iter([1, 2, 3]);
    let mut total2 = 0;
    while let Some(v) = it.next() { total2 += v; }
    println!("{total} {total2}");

    // stringify! gives the text of the tokens that it gets.
    println!("{}", stringify!(a + b * 2));
}
```

```output for=macros-and-desugaring
[1, 9, 16]
1 2
6 6
a + b * 2
```

- Verdict: native. `macro_rules!` matches a pattern of syntax and writes new code. Method call syntax is sugar for a call of the associated function with `&mut c`. A `for` loop is sugar for `IntoIterator::into_iter` and `next`. `stringify!` gives the text of its tokens.

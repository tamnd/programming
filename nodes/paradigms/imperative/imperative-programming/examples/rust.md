```rust run id=mut
fn reverse_in_place(xs: &mut [i32]) {
    let (mut i, mut j) = (0, xs.len() - 1);
    while i < j {
        xs.swap(i, j);
        i += 1;
        j -= 1;
    }
}

fn add_to(total: &mut i32, amount: i32) {
    *total += amount;
}

fn main() {
    let mut data = [1, 2, 3, 4, 5];
    reverse_in_place(&mut data);
    println!("{data:?}");

    let mut total = 0;
    add_to(&mut total, 5);
    add_to(&mut total, 7);
    println!("{total}");

    let mut count = 0;
    for ch in "imperative".chars() {
        if "aeiou".contains(ch) {
            count += 1;
        }
    }
    println!("{count}");
}
```

```output for=mut
[5, 4, 3, 2, 1]
12
5
```

```rust fails expect=compile id=immutable
fn main() {
    let count = 0;
    count += 1;
    println!("{count}");
}
```

- Verdict: native. The Rust book says that "by default, variables are immutable", and `let mut` opts in to change. A function that changes a value of its caller takes a `&mut` reference, and the caller writes `&mut` too.
- The failing block assigns to a variable that is not `mut`, and the compiler rejects it with error E0384.

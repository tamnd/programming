```rust run id=moves
fn total(items: &[u32]) -> u32 {
    items.iter().sum()
}

fn add_tax(items: &mut Vec<u32>) {
    for item in items.iter_mut() {
        *item += *item / 10;
    }
}

fn consume(items: Vec<u32>) -> usize {
    items.len()
}

fn main() {
    let mut prices = vec![100, 250];
    println!("{}", total(&prices));
    add_tax(&mut prices);
    println!("{:?}", prices);

    let moved = prices;
    println!("{}", consume(moved));
}
```

```output for=moves
350
[110, 275]
2
```

- Verdict: native. Each value has one owner. Assignment and a call by value move the value to a new owner, and the old name can no longer be used. When the owner goes out of scope, the value is dropped.
- `&T` is a shared borrow and `&mut T` is a unique borrow. The borrow checker accepts many shared borrows or one unique borrow at a time, and no borrow can live longer than the owner.

```rust fails expect=compile id=use-after-move
fn main() {
    let names = vec![String::from("a")];
    let other = names;
    println!("{} {}", names.len(), other.len());
}
```

The compiler rejects the program with error E0382, borrow of moved value. After `let other = names`, the vector belongs to `other`. To keep both, clone the vector, or borrow it with `&names`.

```rust fails expect=compile id=borrow-conflict
fn main() {
    let mut scores = vec![1, 2, 3];
    let first = &scores[0];
    scores.push(4);
    println!("{first}");
}
```

The compiler rejects the program with error E0502. `push` can move the elements to a new buffer, and `first` would then point to freed memory. In C++, the same code compiles and has undefined behavior.

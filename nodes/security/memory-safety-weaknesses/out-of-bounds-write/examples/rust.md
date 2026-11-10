```rust fails expect=panic id=index
fn set_score(scores: &mut [i32], player: usize, score: i32) {
    scores[player] = score;
}

fn main() {
    let mut scores = vec![0; 4];
    set_score(&mut scores, 2, 10);
    println!("player 2: {}", scores[2]);
    set_score(&mut scores, 4, 99);
}
```

The index is checked at run time. The program stops with a panic before it writes, so no memory outside the vector changes.

```rust run id=get-mut
fn set_score(scores: &mut [i32], player: usize, score: i32) -> Result<(), String> {
    match scores.get_mut(player) {
        Some(slot) => {
            *slot = score;
            Ok(())
        }
        None => Err(format!("player {player} is out of range, the size is {}", scores.len())),
    }
}

fn main() {
    let mut scores = vec![0; 4];
    set_score(&mut scores, 2, 10).unwrap();
    println!("player 2: {}", scores[2]);
    if let Err(error) = set_score(&mut scores, 4, 99) {
        println!("refused: {error}");
    }
}
```

```output for=get-mut
player 2: 10
refused: player 4 is out of range, the size is 4
```

- Verdict: native. Each index into an array, a slice or a `Vec` is checked. A failed check is a panic, and the reference says that a constant index can be checked at compile time.
- A panic is a denial of service, not memory corruption. Use `get` and `get_mut` where the index comes from input, and handle `None`.
- Iterators such as `iter_mut` and `chunks_mut` remove most index arithmetic.
- `unsafe` code can skip the check, for example with `get_unchecked_mut` or raw pointers. The weakness can come back there and in C code that the program calls through FFI.

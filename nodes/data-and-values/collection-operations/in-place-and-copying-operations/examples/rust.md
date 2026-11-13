```rust run id=mut-self-and-copies
fn main() {
    // sort takes &mut self: it changes the vector, and the binding must be mut.
    let mut nums = vec![3, 1, 2];
    nums.sort();
    println!("{:?}", nums);

    // To keep the original, sort a clone.
    let src = vec![3, 1, 2];
    let mut copy = src.clone();
    copy.sort();
    println!("{:?} {:?}", src, copy);

    // retain filters in place; filter and collect build a new vector.
    let mut evens = vec![1, 2, 3, 4];
    evens.retain(|n| n % 2 == 0);
    let odds: Vec<i32> = src.iter().copied().filter(|n| n % 2 == 1).collect();
    println!("{:?} {:?}", evens, odds);

    // An iterator adapter is lazy and does not change the vector.
    let letters = vec!['a', 'b', 'c'];
    let back: String = letters.iter().rev().collect();
    println!("{:?} {}", letters, back);

    // reverse is the in-place form.
    let mut word = vec!['a', 'b', 'c'];
    word.reverse();
    println!("{:?}", word);

    // sort returns (), the unit value, so there is no sorted vector to chain.
    let mut marks = vec![3, 1, 2];
    let result = marks.sort();
    println!("{:?} {:?}", result, marks);
}
```

```output
[1, 2, 3]
[3, 1, 2] [1, 2, 3]
[2, 4] [3, 1]
['a', 'b', 'c'] cba
['c', 'b', 'a']
() [1, 2, 3]
```

- Verdict: native. `sort` takes `&mut self`, so the binding must be `mut`, and it returns `()`. `clone` gives a copy to sort. `retain` filters in place, and `filter` with `collect` builds a new vector. `iter().rev()` is lazy and does not change the vector, and `reverse` is the in-place form.

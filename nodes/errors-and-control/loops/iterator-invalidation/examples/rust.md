```rust run id=retain-collect-then-apply-and-iter-mut
use std::collections::HashMap;

fn main() {
    let mut nums = vec![1, 2, 2, 3, 4];
    // for n in &nums { if n % 2 == 0 { nums.remove(0); } }
    // does not compile: error[E0502], nums is borrowed by the loop.
    nums.retain(|n| n % 2 != 0);
    println!("{:?}", nums);

    // Changing the values in place is allowed with iter_mut.
    for n in nums.iter_mut() {
        *n *= 10;
    }
    println!("{:?}", nums);

    // Collect the changes first, then apply them.
    let mut stock: HashMap<&str, i32> = HashMap::from([("pen", 0), ("ink", 5), ("pad", 0)]);
    let empty: Vec<&str> = stock.iter().filter(|(_, v)| **v == 0).map(|(k, _)| *k).collect();
    for k in empty {
        stock.remove(k);
    }
    let mut left: Vec<_> = stock.keys().collect();
    left.sort();
    println!("{:?}", left);

    // An index loop that adds elements reads len() on each pass.
    let mut v = vec![1, 2, 3];
    let mut i = 0;
    while i < v.len() && v.len() < 6 {
        let x = v[i];
        v.push(x * 10);
        i += 1;
    }
    println!("{:?}", v);
}
```

```output for=retain-collect-then-apply-and-iter-mut
[1, 3]
[10, 30]
["ink"]
[1, 2, 3, 10, 20, 30]
```

- Verdict: native. A `for` loop over `&nums` borrows the vector, so a call to `remove` or `push` in the body is a compile error, not a run-time bug. `retain` removes the elements in one call, and `iter_mut` changes the values without changing the structure. For the map, the code collects the keys in one pass and removes them in a second. A `while` loop with an index copies each value out before it pushes, so it holds no borrow across the change.

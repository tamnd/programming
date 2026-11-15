```rust run id=move-closures-and-threads
use std::thread;

fn main() {
    // Each pass binds a new i, and move copies it into the closure.
    let mut fs: Vec<Box<dyn Fn() -> i32>> = Vec::new();
    for i in 0..3 {
        fs.push(Box::new(move || i * 10));
    }
    let vals: Vec<i32> = fs.iter().map(|f| f()).collect();
    println!("{:?}", vals);

    // Threads get owned values, one in each pass.
    let names = vec!["ann".to_string(), "bob".to_string()];
    let mut handles = Vec::new();
    for (i, name) in names.into_iter().enumerate() {
        handles.push(thread::spawn(move || format!("{i}:{name}")));
    }
    for h in handles {
        println!("{}", h.join().unwrap());
    }

    // A closure that borrows a variable cannot outlive it.
    let mut total = 0;
    let mut add = |x: i32| total += x;
    for x in [1, 2, 3] {
        add(x);
    }
    println!("total {}", total);
}
```

```output for=move-closures-and-threads
[0, 10, 20]
0:ann
1:bob
total 6
```

- Verdict: native. The pattern of a `for` loop binds a new `i` in each pass, and a `move` closure copies the value of that pass. Without `move`, the closure would borrow `i`, and the compiler would reject the code, because `i` does not live as long as the vector. The threads take ownership of `i` and `name`. The last closure borrows `total` mutably, and the borrow ends before `println!` reads it.

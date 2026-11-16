```rust run id=unit-value
use std::collections::HashMap;
use std::mem::size_of;

fn greet(name: &str) {
    println!("hello {name}");
}

fn check_positive(s: &str) -> Result<(), String> {
    let n: i32 = s.parse().map_err(|e| format!("{e}"))?;
    if n > 0 {
        Ok(())
    } else {
        Err(format!("{n} is not positive"))
    }
}

fn main() {
    let r: () = greet("Ann");
    println!("{:?} {} {}", r, size_of::<()>(), size_of::<((), ())>());

    // A semicolon at the end of a block makes the block evaluate to ().
    let a = {
        let x = 1 + 2;
        x
    };
    let b = {
        let _x = 1 + 2;
    };
    println!("{a} {b:?}");

    let mut seen: HashMap<&str, ()> = HashMap::new();
    for w in ["b", "a", "b"] {
        seen.insert(w, ());
    }
    println!("{} {}", seen.len(), seen.contains_key("a"));

    println!("{:?} {:?}", check_positive("5"), check_positive("-1"));
}
```

```output for=unit-value
hello Ann
() 0 0
3 ()
2 true
Ok(()) Err("-1 is not positive")
```

- Verdict: native. `greet` has no `->`, so it returns `()`, and the program can store that value. `()` and a tuple of units have the size 0. The block for `a` ends with `x`, so its value is 3, and the block for `b` ends with a statement, so its value is `()`. `HashMap<&str, ()>` keeps only the keys, which is how `HashSet` is built. `Result<(), String>` is the result of a check that can fail but has no value when it succeeds.

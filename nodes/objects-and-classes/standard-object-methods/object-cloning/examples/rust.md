```rust run id=derive-clone-and-rc
use std::cell::RefCell;
use std::rc::Rc;

#[derive(Clone, Debug)]
struct Team {
    name: String,
    members: Vec<String>,
}

#[derive(Clone, Debug)]
struct SharedTeam {
    members: Rc<RefCell<Vec<String>>>,
}

fn main() {
    let a = Team {
        name: "red".to_string(),
        members: vec!["Ann".to_string(), "Bo".to_string()],
    };
    let mut b = a.clone();
    b.members.push("Cy".to_string());
    println!("{:?}", a.members);
    println!("{:?}", b.members);

    let s = SharedTeam {
        members: Rc::new(RefCell::new(vec!["Ann".to_string()])),
    };
    let t = s.clone();
    t.members.borrow_mut().push("Cy".to_string());
    println!("{:?}", s.members.borrow());
    println!("{}", Rc::strong_count(&s.members));
}
```

```output for=derive-clone-and-rc
["Ann", "Bo"]
["Ann", "Bo", "Cy"]
["Ann", "Cy"]
2
```

- Verdict: stdlib. `#[derive(Clone)]` calls `clone` on each field. `String` and `Vec` clone their data, so the clone of `Team` is deep.
- `Rc::clone` makes a new pointer to the same data and adds 1 to the count, so the clone of `SharedTeam` shares the list.
- The copy is always visible in the code as a call to `clone`. Assignment `let b = a;` moves the value, and `a` can no longer be used.

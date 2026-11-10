```rust run id=countdown
struct Countdown {
    current: u32,
}

impl Iterator for Countdown {
    type Item = u32;

    fn next(&mut self) -> Option<u32> {
        if self.current == 0 {
            return None;
        }
        self.current -= 1;
        Some(self.current + 1)
    }
}

fn main() {
    let countdown = Countdown { current: 3 };
    for n in countdown {
        println!("{n}");
    }

    let tens: Vec<u32> = Countdown { current: 5 }
        .filter(|n| n % 2 == 1)
        .map(|n| n * 10)
        .collect();
    println!("{tens:?}");

    let mut it = vec!["a", "b"].into_iter();
    println!("{:?} {:?} {:?}", it.next(), it.next(), it.next());
}
```

```output for=countdown
3
2
1
[50, 30, 10]
Some("a") Some("b") None
```

- Verdict: native. A type implements the trait `Iterator` with one required method, `next(&mut self) -> Option<Self::Item>`. `None` means the end.
- A `for` loop calls `IntoIterator::into_iter` on its value. So it takes a `Vec` by value, and `for x in &v` borrows it.
- Adapters such as `filter` and `map` are lazy. They return other iterators, and nothing runs until a consumer such as `collect` or `for` pulls values.
- The borrow checker rejects a change to a collection while an iterator borrows it (see Pitfalls on the concept page).

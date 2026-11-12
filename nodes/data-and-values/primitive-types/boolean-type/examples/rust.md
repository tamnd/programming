```rust run id=as-and-then-some
fn main() {
    // bool is its own type: if needs a bool, and no other type converts to it.
    let x = 5;
    let non_zero = x != 0;
    println!("{} {} {}", non_zero, x > 3 && x < 10, !non_zero);

    // as converts a bool to an integer, but not the other way.
    println!("{} {}", true as u8, false as i32);
    let count = [true, false, true].iter().filter(|&&b| b).count();
    println!("{count}");

    // & and | evaluate both operands; && and || stop early. ^ is exclusive or.
    println!("{} {} {}", true & false, true | false, true ^ true);

    // then and then_some turn a bool into an Option.
    let age = 20;
    println!("{:?} {:?}", (age >= 18).then_some("adult"), (age < 13).then(|| "child"));

    // Parsing accepts only "true" and "false".
    println!("{:?} {}", "true".parse::<bool>(), "1".parse::<bool>().is_err());

    // A bool takes 1 byte, and match must cover both values.
    let label = match non_zero {
        true => "yes",
        false => "no",
    };
    println!("{} {}", std::mem::size_of::<bool>(), label);
}
```

```output for=as-and-then-some
true true false
1 0
2
false true false
Some("adult") None
Ok(true) true
1 yes
```

- Verdict: native. A condition must be a `bool`. `as` converts `true` to `1`, but no cast goes from an integer to `bool`. `&` and `|` evaluate both operands. `then_some` and `then` turn a condition into an `Option`. `"true".parse::<bool>()` succeeds and `"1"` fails. A `bool` takes 1 byte, and a `match` on it must cover `true` and `false`.

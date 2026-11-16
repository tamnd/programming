```rust run id=never-type
use std::convert::Infallible;
use std::mem::size_of;
use std::panic;

// A function with the result type ! never returns.
fn fail(msg: &str) -> ! {
    panic!("{msg}")
}

fn port(s: &str) -> u16 {
    match s.parse::<u16>() {
        Ok(n) => n,
        // fail has the type !, which fits where a u16 is expected.
        Err(_) => fail(&format!("bad port: {s}")),
    }
}

fn sum_until_zero(xs: &[i32]) -> i32 {
    let mut total = 0;
    for &x in xs {
        // return and continue have the type !, so the match still has the type i32.
        let v = match x {
            n if n > 0 => n,
            0 => return total,
            _ => continue,
        };
        total += v;
    }
    total
}

fn shout(s: &str) -> Result<String, Infallible> {
    Ok(s.to_uppercase())
}

fn main() {
    panic::set_hook(Box::new(|_| {}));
    println!("{}", port("8080"));
    let err = panic::catch_unwind(|| port("x")).unwrap_err();
    println!("{}", err.downcast_ref::<String>().unwrap());

    println!("{} {}", sum_until_zero(&[3, -1, 4]), sum_until_zero(&[2, 0, 5]));

    // The Err case cannot occur, so the pattern needs only Ok.
    let Ok(name) = shout("ann");
    println!("{name} {} {}", size_of::<Result<u32, Infallible>>(), size_of::<u32>());
}
```

```output for=never-type
8080
bad port: x
7 2
ANN 4 4
```

- Verdict: native. `fail` has the result type `!`, so a call to it fits in a `match` arm whose other arm gives a `u16`. `return` and `continue` also have the type `!`. `Infallible` is the standard type with no values, so `Result<u32, Infallible>` has the same size as `u32`, and `let Ok(name) = ...` needs no `Err` case. The panic hook is replaced only to keep the panic message off the error output.

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

/// A boxed function from a type to the same type.
pub type Func<T> = Box<dyn Fn(T) -> T>;

/// Returns the composition of the functions. The last one runs first.
pub fn compose<T>(fs: Vec<Func<T>>) -> impl Fn(T) -> T {
    move |x| fs.iter().rev().fold(x, |acc, f| f(acc))
}

/// Returns the function that name names.
fn op(name: &str) -> Func<i32> {
    match name {
        "inc" => Box::new(|x: i32| x + 1),
        "dec" => Box::new(|x: i32| x - 1),
        "double" => Box::new(|x: i32| 2 * x),
        "negate" => Box::new(|x: i32| -x),
        _ => panic!("unknown function: {name}"),
    }
}

/// Composes the functions that ops names and applies the result to x.
pub fn compose_all(ops: &[&str], x: i32) -> i32 {
    compose(ops.iter().copied().map(op).collect())(x)
}

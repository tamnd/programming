```rust run id=infinite
fn is_prime(n: u64) -> bool {
    (2..).take_while(|d| d * d <= n).all(|d| n % d != 0)
}

fn main() {
    let primes: Vec<u64> = (2..).filter(|&n| is_prime(n)).take(10).collect();
    println!("{:?}", primes);

    let powers: Vec<u64> = std::iter::successors(Some(1u64), |x| Some(2 * x))
        .take(8)
        .collect();
    println!("{:?}", powers);

    let cycled: Vec<char> = "abc".chars().cycle().take(7).collect();
    println!("{:?}", cycled);

    let first_square = (1u64..).map(|n| n * n).find(|&s| s > 50);
    println!("first square over 50: {:?}", first_square);
}
```

```output for=infinite
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
[1, 2, 4, 8, 16, 32, 64, 128]
['a', 'b', 'c', 'a', 'b', 'c', 'a']
first square over 50: Some(64)
```

- Verdict: stdlib. The open range `2..` is an infinite iterator, and `successors`, `repeat`, `from_fn` and `cycle` make others. Adapters such as `filter` and `map` are lazy, and `take`, `take_while` and `find` stop.
- `is_prime` also uses an infinite range, and `take_while` cuts it at the square root of `n`.

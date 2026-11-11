```rust run id=backoff
use std::fmt;

#[derive(Debug)]
enum CallError {
    Unavailable,
    BadRequest,
}

impl CallError {
    fn is_transient(&self) -> bool {
        matches!(self, CallError::Unavailable)
    }
}

impl fmt::Display for CallError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            CallError::Unavailable => write!(f, "503 Service Unavailable"),
            CallError::BadRequest => write!(f, "400 Bad Request"),
        }
    }
}

struct XorShift(u64);

impl XorShift {
    fn below(&mut self, bound: u64) -> u64 {
        self.0 ^= self.0 << 13;
        self.0 ^= self.0 >> 7;
        self.0 ^= self.0 << 17;
        self.0 % bound
    }
}

fn retry<T>(
    mut op: impl FnMut() -> Result<T, CallError>,
    rng: &mut XorShift,
) -> Result<T, CallError> {
    let (attempts, base, cap) = (5u32, 100u64, 1000u64);
    let mut now = 0u64;
    let mut attempt = 0;
    loop {
        match op() {
            Ok(value) => return Ok(value),
            Err(e) if !e.is_transient() || attempt + 1 == attempts => return Err(e),
            Err(e) => {
                let delay = rng.below(cap.min(base << attempt) + 1);
                println!("  t={now:4} ms: attempt {} failed ({e}); wait {delay} ms", attempt + 1);
                now += delay;
                attempt += 1;
            }
        }
    }
}

fn service(failures: u32, error: fn() -> CallError) -> impl FnMut() -> Result<String, CallError> {
    let mut calls = 0;
    move || {
        calls += 1;
        if calls <= failures {
            Err(error())
        } else {
            Ok(format!("200 OK after {calls} calls"))
        }
    }
}

fn main() {
    let mut rng = XorShift(0x2545_F491_4F6C_DD1D);
    for (name, failures, error) in [
        ("flaky", 3, (|| CallError::Unavailable) as fn() -> CallError),
        ("bad request", 1, || CallError::BadRequest),
        ("down", 100, || CallError::Unavailable),
    ] {
        println!("{name}");
        match retry(service(failures, error), &mut rng) {
            Ok(v) => println!("  result: {v}"),
            Err(e) => println!("  gave up: {e}"),
        }
    }
}
```

```output
flaky
  t=   0 ms: attempt 1 failed (503 Service Unavailable); wait 54 ms
  t=  54 ms: attempt 2 failed (503 Service Unavailable); wait 81 ms
  t= 135 ms: attempt 3 failed (503 Service Unavailable); wait 211 ms
  result: 200 OK after 4 calls
bad request
  gave up: 400 Bad Request
down
  t=   0 ms: attempt 1 failed (503 Service Unavailable); wait 75 ms
  t=  75 ms: attempt 2 failed (503 Service Unavailable); wait 144 ms
  t= 219 ms: attempt 3 failed (503 Service Unavailable); wait 103 ms
  t= 322 ms: attempt 4 failed (503 Service Unavailable); wait 380 ms
  gave up: 503 Service Unavailable
```

- Verdict: stdlib. Rust's standard library has no retry function and no random number generator. The example uses a xorshift generator of a few lines, with a fixed seed.
- The error type says which errors are transient. The match arm with a guard returns a permanent error at once, and it returns a transient one when the attempts run out.
- The closure that `service` returns keeps its own count of calls, so each case starts from zero.

Exponential growth without a cap gives absurd delays after a few attempts. After 20 failed attempts, a base of 100 ms has grown to more than a day.

```rust wrong id=no-cap
fn main() {
    let base: u64 = 100;
    for attempt in [0u32, 5, 10, 15, 20] {
        let delay = base * 2u64.pow(attempt);
        let seconds = delay as f64 / 1000.0;
        println!("attempt {attempt:2}: wait {seconds:>9.1} s = {:>4.1} hours", seconds / 3600.0);
    }
}
```

```output
attempt  0: wait       0.1 s =  0.0 hours
attempt  5: wait       3.2 s =  0.0 hours
attempt 10: wait     102.4 s =  0.0 hours
attempt 15: wait    3276.8 s =  0.9 hours
attempt 20: wait  104857.6 s = 29.1 hours
```

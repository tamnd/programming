---
tag: 77BBH6
kind: task
slug: loop-with-a-step
name: Loop with a step or downward
aliases: [count down, countdown loop, downward for loop, loop with a step, step by 2, count by n, negative step, range with step, step_by, Loops/Downward for, Loops/For with a specified step]
area: errors-and-control/loops
level: intro
summary: >-
  Count from a start to a stop by a step that can be negative, include the stop when the loop reaches it, and keep the counter from overflowing near the largest integer.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loops-Downward-for/00-TASK.txt
  title: Loops/Downward for, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loops-For-with-a-specified-step/00-TASK.txt
  title: Loops/For with a specified step, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Ranges
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: Iterator in std::iter, The Rust Standard Library
  locator: step_by
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 3.5.1 Brace Expansion
- url: https://doc.rust-lang.org/std/ops/struct.RangeInclusive.html
  title: RangeInclusive in std::ops, The Rust Standard Library
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.18.2 Additive Operators (+ and -) for Numeric Types
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators
  title: Arithmetic operators, C# language reference
  locator: Integer arithmetic overflow
---

## Task

Count from `start` to `stop` by `step`. The function `count_by(start, stop, step)` returns `start`, `start + step`, `start + 2 * step` and so on, while the value has not passed `stop`. The value `stop` is in the result when the loop reaches it. For example, `count_by(10, 0, -1)` is the countdown `[10, 9, ..., 0]`, and `count_by(1, 10, 3)` is `[1, 4, 7, 10]`.

`start`, `stop` and `step` are 32-bit signed integers, `step` is not 0, and the result has at most 10000 values. A positive step counts up, and a negative step counts down. The task joins two Rosetta Code tasks: a countdown from 10 to 0[^1], and a loop whose step is greater than one[^2].

## Solutions

| Language | Up | Down |
|---|---|---|
| Python | `range(start, stop + 1, step)` | `range(start, stop - 1, step)` |
| Rust | `(start..=stop).step_by(k)` | `(stop..=start).rev().step_by(k)` |
| JavaScript, TypeScript, Java, C#, C, C++, Go | `i <= stop; i += step` | `i >= stop; i += step` |
| Bash | `((i <= stop))` | `((i >= stop))` |

- **Python.** For a positive step, a range holds `start + step*i` while it is `< stop`, and for a negative step, while it is `> stop`[^3]. So the solution moves the end one past `stop`, in the direction of the step.
- **Rust.** `step_by` makes an iterator "starting at the same point, but stepping by the given amount at each iteration"[^4]. Its step is a `usize`, so it cannot be negative. To count down, the solution reverses the range first.
- **JavaScript, TypeScript, Java, C#, C, C++ and Go.** The test depends on the sign of the step: `i <= stop` when it counts up, `i >= stop` when it counts down. The counter is a 64-bit integer, so `i + step` cannot overflow.
- **Bash.** The arithmetic `for` uses the same tests. Brace expansion also takes a step, `{x..y..incr}`, where "incr, an optional increment, is an integer"[^5], but it does not work with variables.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `uneven`, `past-stop` and `down-by-three`: the step does not always land on `stop`. The loop stops at the last value before it passes `stop`.
- `empty-up` and `empty-down`: the step goes away from `stop`, so the result is empty. A loop with the wrong test for the direction runs forever or not at all.
- `single`: `start` equals `stop`, so the result has one value, for any step.
- `negative`: the values cross 0.
- `limit-up` and `limit-down`: `stop` is near the largest or the smallest 32-bit integer. The next value after the last one does not fit in 32 bits. With a 32-bit counter, it wraps around, and the test `i <= stop` stays true.
- `big-step`: the step is the largest 32-bit integer, so the second value is past `stop`.

## Pitfalls

- **Python `range` stops one before `stop`, also when it counts down.** For a negative step, "the constraints are i >= 0 and r[i] > stop"[^3], so `range(10, 0, -1)` ends at 1. A step of 0 is an error: "If step is zero, ValueError is raised"[^3].

```python run id=python-range-down
print(list(range(10, 0, -1)))
print(list(range(10, -1, -1)))
try:
    range(1, 10, 0)
except ValueError as e:
    print("ValueError:", e)
```

```output for=python-range-down
[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
[10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0]
ValueError: range() arg 3 must not be zero
```

- **Rust `10..=0` is empty.** A `RangeInclusive` "is empty unless start <= end"[^6], so it cannot count down. Reverse an upward range. `step_by` "will panic if the given step is 0"[^4].

```rust run id=rust-range-down
fn main() {
    let empty: Vec<i32> = (10..=0).collect();
    let down: Vec<i32> = (1..=10).rev().step_by(3).collect();
    let up: Vec<i32> = (1..=10).step_by(3).collect();
    println!("{empty:?} {down:?} {up:?}");
}
```

```output for=rust-range-down
[] [10, 7, 4, 1] [1, 4, 7, 10]
```

- **A 32-bit counter wraps near the limit.** In Java, "If an integer addition overflows, then the result is the low-order bits of the mathematical sum"[^7]. In C#, outside a checked context, "the result is truncated by discarding any high-order bits that don't fit in the destination type"[^8]. So a loop with `int i` and `i <= stop` near the largest `int` wraps to a negative number and does not end. The program below stops it after four passes:

```java run id=java-step-overflow
public class Main {
  public static void main(String[] args) {
    int passes = 0;
    StringBuilder out = new StringBuilder();
    for (int i = 2147483640; i <= 2147483647; i += 5) {
      out.append(i).append(passes < 3 ? " " : "");
      if (++passes == 4) {
        break;
      }
    }
    System.out.println(out);
    StringBuilder ok = new StringBuilder();
    for (long i = 2147483640; i <= 2147483647; i += 5) {
      ok.append(i).append(" ");
    }
    System.out.println(ok.toString().strip());
  }
}
```

```output for=java-step-overflow
2147483640 2147483645 -2147483646 -2147483641
2147483640 2147483645
```

- **The test must match the direction.** With a negative step, `i <= stop` is false at once when `start > stop`, and true forever when `start < stop`. See [infinite loops](/infinite-loops/) and [off-by-one errors](/off-by-one-errors/).

## Related tasks

- [[write-a-counted-loop]]: count up by 1.
- [[write-a-while-loop]]: a loop that stops on a condition.
- [[reverse-a-list]]: walk a list from the end.

## References

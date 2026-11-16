---
tag: 447HCD
kind: task
slug: take-the-first-n-items
name: Take the first n items
aliases: [take n items, first n elements, head of a list, limit a sequence, take from an infinite sequence, prefix of a list]
area: functions-and-abstraction/lazy-evaluation
level: intro
summary: Copy the first n items of a list, where a shorter list gives all its items, and take the first n items of an infinite lazy sequence.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3.14/library/itertools.html
  title: itertools, Python 3.14 documentation
  locator: count; islice
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator/take
  title: Iterator.prototype.take(), MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Interface Stream, Java SE 25 API
  locator: limit(long)
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: Trait Iterator, The Rust Standard Library
  locator: take
- url: https://en.cppreference.com/w/cpp/ranges/take_view.html
  title: std::ranges::take_view, std::ranges::views::take, cppreference.com
  locator: description
- url: https://pkg.go.dev/iter@go1.26.5
  title: Package iter, Go 1.26.5 standard library
  locator: Seq; Iterators
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.take?view=net-10.0
  title: Enumerable.Take, .NET API reference
  locator: Take(IEnumerable, Int32); Remarks
- url: https://www.gnu.org/software/coreutils/manual/coreutils.html
  title: GNU Coreutils manual
  locator: 5.1 head
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/slice
  title: Array.prototype.slice(), MDN Web Docs
  locator: Description; Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
  title: Class Arrays, Java SE 25 API
  locator: copyOf(int[], int)
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 3.2.3 Pipelines; 3.7.5 Exit Status
- url: https://man7.org/linux/man-pages/man7/signal.7.html
  title: signal(7), Linux manual page
  locator: Signal numbering for standard signals
---

## Task

Take the first `n` items of a list of integers. The function `take(items, n)` returns them as a new list, in input order. If the list has fewer than `n` items, the result is the whole list, and `n = 0` gives an empty list. For example, `take([1, 2, 3, 4, 5], 3)` is `[1, 2, 3]` and `take([1, 2], 5)` is `[1, 2]`.

The second function, `take_naturals(start, n)`, takes the first `n` items of the infinite sequence `start`, `start + 1`, `start + 2`, and so on. A program cannot make this sequence as a list, because the list has no end. Each solution makes the sequence lazily, so that it gives one value each time the program asks for one, and then takes `n` values from it. For example, `take_naturals(10, 3)` is `[10, 11, 12]`.

## Solutions

| Language | `take` | `take_naturals` |
|---|---|---|
| Python | `items[:n]` | `list(islice(count(start), n))` |
| JavaScript | `items.slice(0, n)` | a generator with `take(n)` and `toArray()` |
| TypeScript | `items.slice(0, n)` | a generator and a `for` loop with `break` |
| Java | `Arrays.copyOf(items, Math.min(n, items.length))` | `IntStream.iterate(start, i -> i + 1).limit(n)` |
| C# | `items.Take(n).ToArray()` | a `yield return` method with `Take(n)` |
| Go | `slices.Clone(items[:min(n, len(items))])` | an `iter.Seq[int]` and a `range` loop with `break` |
| Rust | `items.iter().take(n).copied().collect()` | `(start..).take(n).collect()` |
| C | `memcpy` of the smaller of `n` and the length | a struct that holds the next value |
| C++ | `items | std::views::take(n)` | `std::views::iota(start) | std::views::take(n)` |
| Bash | `"${@:1:n}"` | a loop without end, piped to `head -n "$n"` |

- **The infinite sequence.** Python `count` makes "an iterator that returns evenly spaced values beginning with start"[^1], and `islice` reads only `n` values from it. JavaScript `take` "returns a new iterator helper object that yields the given number of elements in this iterator and then terminates."[^2] Java `limit` gives "the elements of this stream, truncated to be no longer than maxSize in length"[^3]. Rust says that "take() is often used with an infinite iterator, to make it finite"[^4]. The C++ `take` view represents a "view of the elements from an underlying sequence, starting at the beginning and ending at a given bound."[^5]
- **Go.** An `iter.Seq` is a function that calls `yield` for each value. The function "stops either when the sequence is finished or when yield returns false, indicating to stop the iteration early."[^6] A `break` in the `range` loop makes `yield` return false.
- **C#.** `Take` "Returns a specified number of contiguous elements from the start of a sequence."[^7] The iterator method with `yield return` runs only up to the value that `Take` asks for.
- **C.** C has no lazy sequence in its library. The struct `naturals` holds only the next value, and the function `naturals_next` returns it and adds 1. The caller asks for `n` values and no more.
- **Bash.** `head -n` prints "the first num lines"[^8] and then exits. The loop of `naturals` then writes to a closed pipe, so it stops. A function that prints the items one on each line is the Bash form of a list.
- **A copy.** Each `take` returns a new list. A change to the result does not change the input, and the tests check this.

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/infinite.jsonl`.

- `zero`: `n = 0` gives an empty list, also from the infinite sequence (`none`).
- `empty`: an empty list gives an empty list for each `n`.
- `more-than-length`: `take([1, 2], 5)` is `[1, 2]`. The function does not fail and does not add values.
- `equal-length`: `take([4, 5, 6], 3)` is the whole list.
- `negative` and `extremes`: negative items, -2147483648 and 2147483647 come through unchanged.
- `large-start`: `take_naturals(999998, 2)` is `[999998, 999999]`. The contract keeps `start + n` at 1000000 or less, so no value overflows a 32-bit integer.

## Pitfalls

- **Python a second take from the same iterator gives other items.** `islice` reads items from the iterator, and they are then gone. "If the input is an iterator, then fully consuming the islice advances the input iterator by max(start, stop) steps regardless of the step value."[^1] A slice of a list does not change the list:

```python run id=python-islice-consumes
from itertools import count, islice

numbers = count(1)
print(list(islice(numbers, 3)))
print(list(islice(numbers, 3)))

items = [1, 2, 3, 4, 5, 6]
print(items[:3], items[:3])
```

```output for=python-islice-consumes
[1, 2, 3]
[4, 5, 6]
[1, 2, 3] [1, 2, 3]
```

- **JavaScript a negative n takes from the end.** For the end of `slice`, MDN says: "Negative index counts back from the end of the array"[^9]. A count that a program computes, and that goes below 0 by mistake, gives a wrong list and no error. Check that `n` is at least 0:

```javascript run id=javascript-slice-negative
const items = [1, 2, 3, 4, 5];
console.log(items.slice(0, 2));
console.log(items.slice(0, -2));
```

```output for=javascript-slice-negative
[ 1, 2 ]
[ 1, 2, 3 ]
```

- **Java copyOf pads a short array.** `Arrays.copyOf` copies "the specified array, truncating or padding with zeros (if necessary) so the copy has the specified length."[^10] With `n` larger than the length, the result has zeros at the end. Use the smaller of `n` and the length:

```java run id=java-copyof-pads
import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        int[] items = {1, 2};
        System.out.println(Arrays.toString(Arrays.copyOf(items, 5)));
        System.out.println(Arrays.toString(Arrays.copyOf(items, Math.min(5, items.length))));
    }
}
```

```output for=java-copyof-pads
[1, 2, 0, 0, 0]
[1, 2]
```

- **C# Take is deferred.** "This method is implemented by using deferred execution."[^7] The query reads the list when the program enumerates it, not when the program calls `Take`. A change to the list between the two changes the result. Call `ToArray` or `ToList` to take the items at once:

```csharp run id=csharp-take-deferred
var items = new List<int> { 1, 2, 3 };
var deferred = items.Take(2);
var now = items.Take(2).ToArray();
items.Insert(0, 99);
Console.WriteLine(string.Join(", ", deferred));
Console.WriteLine(string.Join(", ", now));
```

```output for=csharp-take-deferred
99, 1
1, 2
```

- **Bash head and pipefail.** When `head` exits, the command before it in the pipeline gets a write error or a signal, and it exits with a status that is not zero. "If pipefail is enabled, the pipeline’s return status is the value of the last (rightmost) command to exit with a non-zero status"[^11]. A script with `set -o pipefail` then sees a failure, although the result is correct:

```bash run id=bash-head-pipefail
#!/usr/bin/env bash
set -o pipefail
seq 1 100000 | head -n 2
echo "status $?"
```

```output for=bash-head-pipefail
1
2
status 141
```

Bash says: "When a command terminates on a fatal signal whose number is N, Bash uses the value 128+N as the exit status."[^11] On Linux, `SIGPIPE` is signal 13[^12], so the status is 141. Do not use `set -o pipefail` around a pipeline that ends in `head`, or accept the status 141 from it.

## Related tasks

- [[use-lazy-sequences]]: build a pipeline of lazy steps over an infinite sequence.
- [[slice-a-list]]: take the items between two positions.
- [[iterate-in-chunks]]: split a list into parts of a fixed size.
- [[filter-a-list]]: keep the items that pass a test.

## References

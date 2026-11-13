---
tag: CFF71D
kind: task
slug: map-over-a-list
name: Map over a list
aliases: [apply a function to each item, transform a list, select, collect, list comprehension, apply a callback to an array]
area: data-and-values/collection-operations
level: intro
summary: Make a new list with the result of a function for each item of a list, in the same order, and know when the map runs.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/functions.html#map
  title: The Python Standard Library, Built-in Functions
  locator: map()
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.map
  title: Rust standard library, trait Iterator
  locator: map
- url: https://en.cppreference.com/w/cpp/algorithm/transform
  title: cppreference.com, std::transform
  locator: Notes
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map
  title: MDN Web Docs, Array.prototype.map()
  locator: Using parseInt() with map()
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Java SE 25 API, Interface Stream
  locator: Interface description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.select?view=net-10.0
  title: .NET API reference, Enumerable.Select method
  locator: Remarks
- url: https://go.dev/ref/spec#For_range
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://en.cppreference.com/w/c/language/array
  title: cppreference.com, C array declaration
  locator: Array to pointer conversion
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Looping-Constructs
  title: Bash Reference Manual
  locator: 3.2.5.1 Looping Constructs
---

## Task

Make a new list with the result of a function for each item of a list. The function `square_all(items)` returns the square of each integer, in the same order. For example, `square_all([1, 2, 3])` is `[1, 4, 9]`, and `square_all([-3, -1])` is `[9, 1]`.

The result has the same length as the input, and the input list does not change. Each item is from -46340 to 46340, so each square fits in a 32-bit signed integer.

## Solutions

| Language | Square each item |
|---|---|
| Python | `[x * x for x in items]` |
| JavaScript, TypeScript | `items.map((x) => x * x)` |
| Java | `Arrays.stream(items).map(x -> x * x).toArray()` |
| C# | `items.Select(x => x * x).ToArray()` |
| Go | `make` a slice of the same length, and a loop |
| Rust | `items.iter().map(\|x\| x * x).collect()` |
| C | a loop that writes `out[i]` |
| C++ | `std::ranges::transform` with `std::back_inserter` |
| Bash | a loop that prints `$((x * x))` |

- **Python.** `map` returns "an iterator that applies function to every item of iterable, yielding the results"[^1]. The solution uses a list comprehension, which makes a list.
- **Java.** `toArray` is the terminal operation, and it makes the new array.
- **C#.** `ToArray` runs the query at once and keeps the result.
- **Rust.** "as map() is lazy, it is best used when"[^2] the code already works with other iterators. `collect` runs the map and makes the vector.
- **C.** The caller gives an `out` array with room for `n` items.
- **C++.** `std::back_inserter` adds each result to the end of the vector. "std::transform does not guarantee in-order application of unary_op or binary_op"[^3], so the function must not depend on the order of the calls.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the map of an empty list is an empty list.
- `single`: one item gives one result.
- `negative` and `zero`: the square of a negative item is positive, and the square of 0 is 0.
- `limit`: 46340 and -46340 give 2147395600, which fits in a 32-bit signed integer. The square of 46341 does not fit.
- `order` and `duplicates`: result `i` comes from item `i`, and equal items give equal results.

## Pitfalls

- **Python map gives a one-pass iterator.** The second `list(squares)` in the example is empty, because the first one used all the items. Make a list if you need the result more than once:

```python run id=python-map-once
squares = map(lambda x: x * x, [1, 2, 3])
print(list(squares))
print(list(squares))
```

```output for=python-map-once
[1, 4, 9]
[]
```

- **JavaScript map(parseInt) gives NaN.** `map` gives the callback the item, the index and the array. "parseInt is often used with one argument, but takes two"[^4], and the second one is the radix. "While one might expect [1, 2, 3], the actual result is [1, NaN, NaN]"[^4]:

```javascript run id=javascript-map-parseint
console.log(["1", "2", "3"].map(parseInt));
console.log(["1", "2", "3"].map((s) => parseInt(s, 10)));
```

```output for=javascript-map-parseint
[ 1, NaN, NaN ]
[ 1, 2, 3 ]
```

- **Java streams run once.** "A stream should be operated on (invoking an intermediate or terminal stream operation) only once"[^5]. "A stream implementation may throw IllegalStateException if it detects that the stream is being reused"[^5]:

```java run id=java-stream-reuse
import java.util.Arrays;
import java.util.stream.IntStream;

public class Main {
  public static void main(String[] args) {
    IntStream squares = IntStream.of(1, 2, 3).map(x -> x * x);
    System.out.println(Arrays.toString(squares.toArray()));
    try {
      squares.toArray();
    } catch (IllegalStateException e) {
      System.out.println("IllegalStateException");
    }
  }
}
```

```output for=java-stream-reuse
[1, 4, 9]
IllegalStateException
```

- **C# Select runs again each time.** "This method is implemented by using deferred execution"[^6]. In the example, `Sum` and `Max` each run the function on the 3 items, so it runs 6 times. `ToList` runs it 3 times:

```csharp run id=csharp-select-deferred
int calls = 0;
var squares = new[] { 1, 2, 3 }.Select(x => { calls++; return x * x; });
Console.WriteLine($"{squares.Sum()} {squares.Max()} calls={calls}");
calls = 0;
var list = new[] { 1, 2, 3 }.Select(x => { calls++; return x * x; }).ToList();
Console.WriteLine($"{list.Sum()} {list.Max()} calls={calls}");
```

```output for=csharp-select-deferred
14 9 calls=6
14 9 calls=3
```

- **Go range gives a copy of the item.** "the iteration values are assigned to the respective variables as in an assignment statement"[^7]. An assignment to `x` does not change the slice. Write to `items[i]`:

```go run id=go-range-copy
package main

import "fmt"

func main() {
	items := []int{1, 2, 3}
	for _, x := range items {
		x = x * x
	}
	fmt.Println(items)
	for i, x := range items {
		items[i] = x * x
	}
	fmt.Println(items)
}
```

```output for=go-range-copy
[1 2 3]
[1 4 9]
```

- **Rust map does nothing until a consumer runs it.** In the example, the map that is dropped does not call the closure. `collect` calls it once for each item:

```rust run id=rust-map-lazy
fn main() {
    let items = [1, 2, 3];
    let mut calls = 0;
    let squares = items.iter().map(|x| {
        calls += 1;
        x * x
    });
    drop(squares);
    println!("calls={calls}");
    let v: Vec<i32> = items
        .iter()
        .map(|x| {
            calls += 1;
            x * x
        })
        .collect();
    println!("{v:?} calls={calls}");
}
```

```output for=rust-map-lazy
calls=0
[1, 4, 9] calls=3
```

- **C map in place changes the array of the caller.** An array argument "undergoes an implicit conversion to the pointer to its first element"[^8]. So the function gets a pointer, and it writes to the array of the caller. Write to a separate `out` array if the caller needs the input:

```c run id=c-array-pointer
#include <stdio.h>

// The parameter a is a pointer to the first item of the array of the caller.
static void square_in_place(int *a, size_t n) {
    for (size_t i = 0; i < n; i++) {
        a[i] = a[i] * a[i];
    }
}

int main(void) {
    int items[] = {1, 2, 3};
    square_in_place(items, 3);
    printf("%d %d %d\n", items[0], items[1], items[2]);
    return 0;
}
```

```output for=c-array-pointer
1 4 9
```

- **C++ views::transform runs the function each time.** A view does not keep the results. In the example, two loops over the view call the function 6 times, and the copy to a vector calls it 3 more times:

```cpp run id=cpp-view-calls
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    int calls = 0;
    std::vector<int> v{1, 2, 3};
    auto squares = v | std::views::transform([&calls](int x) {
                       ++calls;
                       return x * x;
                   });
    int sum = 0;
    for (int x : squares) {
        sum += x;
    }
    for (int x : squares) {
        sum += x;
    }
    std::cout << sum << " calls=" << calls << "\n";
    std::vector<int> out(squares.begin(), squares.end());
    std::cout << out.size() << " calls=" << calls << "\n";
}
```

```output for=cpp-view-calls
28 calls=6
3 calls=9
```

- **Bash for gives a copy of each word.** The loop runs "with name bound to the current word"[^9]. An assignment to `x` does not change the array. Loop over the indices `${!a[@]}`:

```bash run id=bash-for-copy
a=(1 2 3)
for x in "${a[@]}"; do
  x=$((x * x))
done
echo "${a[@]}"
for i in "${!a[@]}"; do
  a[i]=$((a[i] * a[i]))
done
echo "${a[@]}"
```

```output for=bash-for-copy
1 2 3
1 4 9
```

## Related tasks

- [[filter-a-list]]: keep the items that pass a test.
- [[reduce-a-list-to-a-value]]: combine the items of a list into one value.
- [[zip-two-lists]]: map over two lists at the same time.
- [[sort-a-list]]: put the items in order.

## References

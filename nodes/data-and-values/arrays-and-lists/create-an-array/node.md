---
tag: 6GQ962
kind: task
slug: create-an-array
name: Create an array
aliases: [make an array, initialize an array, allocate an array, fill an array, new array, array of n copies, declare an array]
area: data-and-values/arrays-and-lists
level: intro
summary: Make an array of a given length, with listed values or with one value in each slot, as a fixed array or as a growable one.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 2
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/Array
  title: MDN Web Docs, Array() constructor
  locator: Parameters, arrayLength
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.12.5
  title: The Java Language Specification, Java SE 25 Edition
  locator: 4.12.5 Initial Values of Variables
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/arrays
  title: C# language reference, Arrays
  locator: Single-dimensional arrays
- url: https://go.dev/ref/spec#Making_slices_maps_and_channels
  title: The Go Programming Language Specification
  locator: Making slices, maps and channels
- url: https://doc.rust-lang.org/std/macro.vec.html
  title: Rust standard library, macro vec
  locator: Description
- url: https://en.cppreference.com/w/cpp/container/vector/vector
  title: cppreference.com, std::vector constructors
  locator: "4)"
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
- url: https://en.cppreference.com/w/c/memory/malloc
  title: cppreference.com, malloc
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html#asList(T...)
  title: Java SE 25 API, Class Arrays
  locator: asList
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.-ctor?view=net-10.0
  title: .NET API reference, List<T> constructors
  locator: List<T>(Int32)
- url: https://en.cppreference.com/w/c/language/array_initialization
  title: cppreference.com, C array initialization
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/list_initialization
  title: cppreference.com, List-initialization
  locator: Explanation
---

## Task

Make an array of `n` items in which each item is `value`. The function `filled(n, value)` returns the new array. For example, `filled(3, 7)` is `[7, 7, 7]`, and `filled(0, 7)` is the empty array `[]`.

The count `n` is from 0 to 1000, and `value` is a 32-bit signed integer. The task also shows the two other common ways to make an array: from a list of values, and with the default value of the item type.

## Solutions

| Language | From values | n copies of a value | Growable |
|---|---|---|---|
| Python | `[1, 2, 3]` | `[value] * n` | a list grows with `append` |
| JavaScript, TypeScript | `[1, 2, 3]` | `new Array(n).fill(value)` | an array grows with `push` |
| Java | `new int[] {1, 2, 3}` | `new int[n]` and `Arrays.fill` | `new ArrayList<>(List.of(1, 2, 3))` |
| C# | `[1, 2, 3]` or `new[] { 1, 2, 3 }` | `new int[n]` and `Array.Fill` | `new List<int> { 1, 2, 3 }` |
| Go | `[3]int{1, 2, 3}` | `make([]int, n)` and a loop | a slice grows with `append` |
| Rust | `[1, 2, 3]` | `vec![value; n]` | `vec![1, 2, 3]` |
| C | `int a[] = {1, 2, 3};` | `malloc` and a loop | `realloc` |
| C++ | `std::array{1, 2, 3}` | `std::vector<int>(n, value)` | `std::vector<int>{1, 2, 3}` |
| Bash | `a=(1 2 3)` | a loop with `a+=("$value")` | an array grows with `+=` |

- **Python.** `[value] * n` repeats the list. "Values of n less than 0 are treated as 0"[^1], so a negative count gives an empty list and no error.
- **JavaScript.** `new Array(n)` with one integer argument "returns a new JavaScript array with its length property set to that number"[^2], but the slots are empty. `fill` puts a value in each slot.
- **Java.** "Each class variable, instance variable, or array component is initialized with a default value when it is created"[^3]. So `new int[n]` holds `n` zeros, and `Arrays.fill` then sets each item.
- **C#.** "Elements that you don't initialize are set to the default value"[^4]. So `new int[n]` holds `n` zeros.
- **Go.** `make(T, n)` gives a "slice of type T with length n and capacity n"[^5], and each item is the zero value of the type.
- **Rust.** `vec![value; n]` "supports all elements which implement Clone and the number of elements doesn’t have to be a constant"[^6]. An array `[value; N]` needs a constant `N`.
- **C.** The function returns memory from `malloc`, and the caller must `free` it.
- **C++.** `std::vector<int>(n, value)` "Constructs a vector with count copies of elements with value value"[^7]. Use parentheses here, not braces (see the pitfalls).
- **Bash.** "Arrays are assigned using compound assignments of the form name=(value1 value2 ... )"[^8]. A Bash array holds strings, so the solution prints one item on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: a count of 0 gives an empty array. In C, "If size is zero, the behavior of malloc is implementation-defined"[^9], so the solution asks for room for one item. Then a null pointer always means that the allocation failed.
- `single`: a count of 1 gives one item.
- `zero`: an array of zeros is the same as an array with the default value in Java, C#, Go and C.
- `negative` and `limit`: the value can be negative, and it can be the largest or the smallest 32-bit signed integer.
- `many`: a count of 10 gives 10 equal items.

## Pitfalls

- **Python does not reject a negative count.** `[7] * -2` is an empty list. Check `n` first when a negative count is a bug:

```python run id=python-negative-count
print([7] * 3)
print([7] * 0)
print([7] * -2)
```

```output for=python-negative-count
[7, 7, 7]
[]
[]
```

- **JavaScript new Array(n) has empty slots.** "This implies an array of arrayLength empty slots, not slots with actual undefined values"[^2]. In the example, `map` leaves the empty slots empty. With a string argument, `new Array("3")` is an array of one item. Use `fill` or `Array.from`:

```javascript run id=javascript-empty-slots
const a = new Array(3);
console.log(a.length, 0 in a);
console.log(a.map(() => 7));
console.log(new Array(3).fill(7));
console.log(Array.from({ length: 3 }, (_, i) => i * 10));
console.log(new Array("3"));
```

```output for=javascript-empty-slots
3 false
[ <3 empty items> ]
[ 7, 7, 7 ]
[ 0, 10, 20 ]
[ '3' ]
```

- **Java Arrays.asList has a fixed size.** It "Returns a fixed-size list backed by the specified array"[^10]. The list "implements the optional Collection methods, except those that would change the size of the returned list"[^10], so `set` works and `add` throws. Copy it into an `ArrayList` to get a list that grows:

```java run id=java-aslist-fixed
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    int[] zeros = new int[3];
    String[] names = new String[2];
    System.out.println(Arrays.toString(zeros) + " " + Arrays.toString(names));
    List<Integer> fixed = Arrays.asList(1, 2, 3);
    fixed.set(0, 9);
    System.out.println(fixed);
    try {
      fixed.add(4);
    } catch (UnsupportedOperationException e) {
      System.out.println("UnsupportedOperationException");
    }
    List<Integer> growable = new ArrayList<>(Arrays.asList(1, 2, 3));
    growable.add(4);
    System.out.println(growable);
  }
}
```

```output for=java-aslist-fixed
[0, 0, 0] [null, null]
[9, 2, 3]
UnsupportedOperationException
[1, 2, 3, 4]
```

- **C# new List(n) is empty.** The constructor makes a list "that is empty and has the specified initial capacity"[^11]. The capacity is room, not items, so `list[0] = 7` throws. Use an array, or add the items:

```csharp run id=csharp-list-capacity
var list = new List<int>(3);
Console.WriteLine($"Count={list.Count} Capacity={list.Capacity}");
try
{
    list[0] = 7;
}
catch (ArgumentOutOfRangeException)
{
    Console.WriteLine("ArgumentOutOfRangeException");
}
int[] a = new int[3];
Array.Fill(a, 7);
Console.WriteLine(string.Join(",", a));
Console.WriteLine(string.Join(",", Enumerable.Repeat(7, 3).ToList()));
```

```output for=csharp-list-capacity
Count=0 Capacity=3
ArgumentOutOfRangeException
7,7,7
7,7,7
```

- **Go append after make(n) adds after the zeros.** `make([]int, 3)` already has 3 items, so `append` makes the fourth one. To fill a slice with `append`, make it with length 0 and a capacity, `make([]int, 0, n)`:

```go run id=go-make-append
package main

import "fmt"

func main() {
	s := make([]int, 3)
	s = append(s, 7)
	fmt.Println(s, len(s))
	t := make([]int, 0, 3)
	t = append(t, 7)
	fmt.Println(t, len(t), cap(t))
}
```

```output for=go-make-append
[0 0 0 7] 4
[7] 1 3
```

- **Rust vec! clones one value.** "vec![Rc::new(1); 5] will create a vector of five references to the same boxed integer value, not five references pointing to independently boxed integers"[^6]. In the example, a change to the first item shows in all three. Make each item with its own call:

```rust run id=rust-vec-clone
use std::cell::RefCell;
use std::rc::Rc;

fn main() {
    let shared = vec![Rc::new(RefCell::new(0)); 3];
    *shared[0].borrow_mut() += 1;
    let values: Vec<i32> = shared.iter().map(|c| *c.borrow()).collect();
    println!("{values:?} owners={}", Rc::strong_count(&shared[0]));
    let own: Vec<Rc<RefCell<i32>>> = (0..3).map(|_| Rc::new(RefCell::new(0))).collect();
    *own[0].borrow_mut() += 1;
    let values: Vec<i32> = own.iter().map(|c| *c.borrow()).collect();
    println!("{values:?} owners={}", Rc::strong_count(&own[0]));
}
```

```output for=rust-vec-clone
[1, 1, 1] owners=3
[1, 0, 0] owners=1
```

- **C {1} sets only the first item.** "All array elements that are not initialized explicitly are empty-initialized"[^12], so `int a[5] = {1}` holds one 1 and four zeros. To put 1 in each item, use a loop:

```c run id=c-partial-initializer
#include <stdio.h>

static void show(const int *a, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%d%s", a[i], i + 1 < n ? " " : "\n");
    }
}

int main(void) {
    int a[5] = {1};
    show(a, 5);
    int b[5];
    for (size_t i = 0; i < 5; i++) {
        b[i] = 1;
    }
    show(b, 5);
    return 0;
}
```

```output for=c-partial-initializer
1 0 0 0 0
1 1 1 1 1
```

- **C++ braces pick the initializer_list constructor.** For list-initialization, "All constructors that take std::initializer_list as the only argument, or as the first argument if the remaining arguments have default values, are examined"[^13] first. So `std::vector<int>{3, 7}` holds the two items 3 and 7, and `std::vector<int>(3, 7)` holds three sevens:

```cpp run id=cpp-braces-or-parens
#include <iostream>
#include <vector>

static void show(const char *name, const std::vector<int> &v) {
    std::cout << name << " size=" << v.size() << ":";
    for (int x : v) {
        std::cout << " " << x;
    }
    std::cout << "\n";
}

int main() {
    show("parens", std::vector<int>(3, 7));
    show("braces", std::vector<int>{3, 7});
}
```

```output for=cpp-braces-or-parens
parens size=3: 7 7 7
braces size=2: 3 7
```

- **Bash splits an unquoted value into words.** "Each value in the list undergoes the shell expansions"[^8], and word splitting is one of them. In the example, `s` holds `a b`. The assignment `a=($s)` makes two items, and `b` gets one item, because its value is in double quotes. To read the lines of a command into an array, use `mapfile -t`:

```bash run id=bash-array-words
s="a b"
a=($s)
b=("$s")
echo "${#a[@]} ${#b[@]}"
mapfile -t c < <(printf '%s\n' x y z)
echo "${#c[@]} ${c[1]}"
```

```output for=bash-array-words
2 1
3 y
```

## Related tasks

- [[get-the-length-of-an-array]]: count the items of an array.
- [[map-over-a-list]]: make a new list from the items of a list.
- [[filter-a-list]]: keep the items that pass a test.
- [[sort-a-list]]: put the items of a list in order.

## References

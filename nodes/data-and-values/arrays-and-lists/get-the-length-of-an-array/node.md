---
tag: Y9P099
kind: task
slug: get-the-length-of-an-array
name: Get the length of an array
aliases: [array length, array size, count the items of an array, number of elements, len, length of a list, size of a vector]
area: data-and-values/arrays-and-lists
level: intro
summary: Get the number of items in an array or list with len, length, size or Count, and know why sizeof and a pointer give a wrong count in C.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/functions.html#len
  title: The Python Standard Library, Built-in Functions
  locator: len()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/length
  title: MDN Web Docs, Array length
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html#jls-10.7
  title: The Java Language Specification, Java SE 25 Edition
  locator: 10.7 Array Members
- url: https://learn.microsoft.com/en-us/dotnet/api/system.array.length?view=net-10.0
  title: .NET API reference, Array.Length property
  locator: Property Value
- url: https://go.dev/ref/spec#Length_and_capacity
  title: The Go Programming Language Specification
  locator: Length and capacity
- url: https://en.cppreference.com/w/c/language/main_function
  title: cppreference.com, C main function
  locator: Parameters
- url: https://en.cppreference.com/w/cpp/iterator/size
  title: cppreference.com, std::size, std::ssize
  locator: Return value
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
- url: https://docs.oracle.com/javase/tutorial/java/generics/restrictions.html
  title: The Java Tutorials, Restrictions on Generics
  locator: Cannot Instantiate Generic Types with Primitive Types
- url: https://go.dev/ref/spec#Slice_expressions
  title: The Go Programming Language Specification
  locator: Slice expressions
- url: https://doc.rust-lang.org/std/vec/struct.Vec.html#method.with_capacity
  title: Rust standard library, struct Vec
  locator: with_capacity
- url: https://en.cppreference.com/w/c/language/sizeof
  title: cppreference.com, C sizeof operator
  locator: Notes
- url: https://en.cppreference.com/w/cpp/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators
  locator: Overflows
---

## Task

Get the number of items in an array of strings. The function `length(items)` returns the count. For example, `length(["a", "b", "c"])` is 3, and `length([])` is 0.

The function counts items, not characters or bytes. So `length(["", ""])` is 2, and `length(["é", "日本語"])` is 2.

## Solutions

| Language | Array | List or growable array |
|---|---|---|
| Python | `len(items)` | `len(items)` |
| JavaScript, TypeScript | `items.length` | `items.length` |
| Java | `items.length` (a field) | `list.size()` |
| C# | `items.Length` | `list.Count` |
| Go | `len(items)` | `len(items)` |
| Rust | `items.len()` | `v.len()` |
| C | `sizeof a / sizeof a[0]` in the scope of the array | a count that the code keeps |
| C++ | `std::size(a)` | `v.size()` or `std::ssize(v)` |
| Bash | `${#a[@]}` | `${#a[@]}` |

- **Python.** `len` returns "the length (the number of items) of an object"[^1]. It works on any sequence or collection.
- **JavaScript.** "The length data property of an Array instance represents the number of slots in that array"[^2]. A slot can be empty, so the length is the number of items only for an array with no holes (see the pitfalls).
- **Java.** An array has "The public final field length, which contains the number of components of the array"[^3]. It is a field, so it has no parentheses. A `String` has the method `length()`, and a collection has `size()`.
- **C#.** For an array, `Length` is "The total number of elements in all the dimensions of the Array"[^4]. A `List<T>` has `Count`.
- **Go.** `len` works on arrays, slices, strings, maps and channels. "The length of a nil slice, map or channel is 0"[^5].
- **C.** A function gets a pointer, not an array, so it cannot find the length with `sizeof`. The array must come with a count, or it must end with a marker. The solution uses a null pointer as the marker, like `argv`, where "the last element, argv[argc], is guaranteed to be a null pointer"[^6].
- **C++.** `std::size` works on containers and on built-in arrays. For an array of `N` items, it "Returns N"[^7].
- **Bash.** "If subscript is ‘@’ or ‘*’, the expansion is the number of elements in the array"[^8]. The solution takes the items as arguments, so `$#` gives the count.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty array has length 0.
- `single`: one item gives 1.
- `empty-strings`: an empty string is an item, so `["", ""]` has length 2. In C, an empty string is not a null pointer, so it does not end the array.
- `non-ascii`: the length counts items, not the characters or the bytes of the items.
- `duplicates` and `ten`: equal items count once each.

## Pitfalls

- **Python len does not work on a generator.** `len` takes "a sequence (such as a string, bytes, tuple, list, or range) or a collection"[^1]. A generator is neither, so `len` raises `TypeError`. To count its items, use the generator up, for example with `sum`:

```python run id=python-len-generator
squares = (x * x for x in range(3))
try:
    len(squares)
except TypeError as e:
    print(type(e).__name__)
print(sum(1 for _ in squares))
print(len([x * x for x in range(3)]))
```

```output for=python-len-generator
TypeError
3
3
```

- **JavaScript length is not the number of items in a sparse array.** The length "is always numerically greater than the highest index in the array. It may be greater than the number of elements if the array is sparse"[^2]. An assignment to a larger index "extends the array"[^2], and a smaller length "truncates the array"[^2]:

```javascript run id=javascript-sparse-length
const a = [];
a[9] = "x";
console.log(a.length, Object.keys(a).length);
a.length = 2;
console.log(a);
```

```output for=javascript-sparse-length
10 1
[ <2 empty items> ]
```

- **Java Arrays.asList of an int array has one item.** For generics, "you cannot substitute a primitive type for the type parameter"[^9]. So `Arrays.asList(nums)` with an `int[]` gives a list with one item, the array itself. An `Integer[]` gives a list of 3 items:

```java run id=java-aslist-primitive
import java.util.Arrays;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    int[] nums = {1, 2, 3};
    System.out.println(nums.length);
    System.out.println(Arrays.asList(nums).size());
    Integer[] boxed = {1, 2, 3};
    System.out.println(Arrays.asList(boxed).size());
    System.out.println("abc".length() + " " + List.of(1, 2).size());
  }
}
```

```output for=java-aslist-primitive
3
1
3
3 2
```

- **C# Length of a multidimensional array counts all the items.** For a 3 by 4 array, `Length` is 12. Use `GetLength(0)` for the number of rows. A jagged array is an array of arrays, so its `Length` is the number of rows:

```csharp run id=csharp-length-dimensions
int[,] grid = new int[3, 4];
Console.WriteLine($"{grid.Length} {grid.GetLength(0)} {grid.GetLength(1)}");
int[][] jagged = [new int[4], new int[2]];
Console.WriteLine($"{jagged.Length} {jagged[0].Length} {jagged[1].Length}");
```

```output for=csharp-length-dimensions
12 3 4
2 4 2
```

- **Go len is not cap.** `len` is the number of items, and `cap` is the room. "For slices, the upper index bound is the slice capacity cap(a) rather than the length"[^10], so a slice expression can make a slice longer than the slice it starts from:

```go run id=go-len-cap
package main

import "fmt"

func main() {
	var s []int
	fmt.Println(len(s), s == nil)
	t := make([]int, 2, 10)
	fmt.Println(len(t), cap(t))
	u := t[:5]
	fmt.Println(len(u), cap(u))
}
```

```output for=go-len-cap
0 true
2 10
5 10
```

- **Rust with_capacity gives an empty vector.** "It is important to note that although the returned vector has the minimum capacity specified, the vector will have a zero length"[^11]. `len` counts the items, and `capacity` gives the room:

```rust run id=rust-len-capacity
fn main() {
    let mut v: Vec<i32> = Vec::with_capacity(10);
    println!("{} {}", v.len(), v.capacity() >= 10);
    v.push(1);
    println!("{} {}", v.len(), v.is_empty());
    let a = [0u8; 4];
    println!("{}", a.len());
}
```

```output for=rust-len-capacity
0 true
1 false
4
```

- **C sizeof of a pointer is not the length.** `sizeof a / sizeof a[0]` gives the number of items of an array. "Note that if a has pointer type (such as after array-to-pointer conversion of function parameter type adjustment), this expression would simply divide the number of bytes in a pointer type by the number of bytes in the pointed type"[^12]. In the example, the function gets a pointer, and `sizeof p` is the size of the pointer. The output is from a 64-bit machine with 4-byte `int`:

```c run id=c-sizeof-pointer
#include <stdio.h>

static size_t bytes(const int *p) {
    return sizeof p;
}

int main(void) {
    int a[5] = {0};
    printf("%zu %zu\n", sizeof a, sizeof a / sizeof a[0]);
    printf("%zu\n", bytes(a));
    return 0;
}
```

```output for=c-sizeof-pointer
20 5
8
```

- **C++ size() is unsigned.** Unsigned integer arithmetic wraps around: "subtracting one from 0 gives UINT_MAX"[^13] for `unsigned int`, and the same rule holds for `std::size_t`. So `v.size() - 1` on an empty vector is a very large number. `std::ssize` returns a signed type[^7], so `std::ssize(v) - 1` is -1:

```cpp run id=cpp-size-unsigned
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> v;
    std::cout << v.size() - 1 << "\n";
    std::cout << std::ssize(v) - 1 << "\n";
    int a[4]{};
    std::cout << std::size(a) << "\n";
}
```

```output for=cpp-size-unsigned
18446744073709551615
-1
4
```

- **Bash `${#a}` is the length of the first item.** "Referencing an array variable without a subscript is equivalent to referencing with a subscript of 0"[^8], so `${#a}` is the number of characters in `${a[0]}`. Bash arrays can have gaps, so the count is not always the largest index plus one:

```bash run id=bash-array-count
a=(apple banana cherry)
echo "${#a} ${#a[@]}"
b=()
b[5]=x
echo "${#b[@]} ${!b[@]}"
```

```output for=bash-array-count
5 3
1 5
```

## Related tasks

- [[create-an-array]]: make an array of a given length.
- [[map-over-a-list]]: make a new list with the same length.
- [[filter-a-list]]: keep the items that pass a test.
- [[count-the-items-of-each-kind]]: count the items by value.

## References

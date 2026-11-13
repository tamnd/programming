---
tag: 8R8Q8H
kind: task
slug: append-to-a-list
name: Append to a list
aliases: [add an item to a list, push to an array, insert into a list, add to the end of a list, prepend to a list, insert at an index, grow an array]
area: data-and-values/arrays-and-lists
level: intro
summary: Add one item to a list, at the end with append, push or Add, or at a given position with insert, and know what each call returns and when it fails.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#mutable-sequence-types
  title: The Python Standard Library, Built-in Types
  locator: Mutable Sequence Types, sequence.append and sequence.insert
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/splice
  title: MDN Web Docs, Array.prototype.splice()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ArrayList.html#add(int,E)
  title: Java SE 25 API, Class ArrayList
  locator: add(int index, E element)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.insert?view=net-10.0
  title: .NET API reference, List<T>.Insert method
  locator: Remarks and Exceptions
- url: https://pkg.go.dev/slices#Insert
  title: Go standard library, package slices
  locator: func Insert
- url: https://doc.rust-lang.org/std/vec/struct.Vec.html#method.insert
  title: Rust standard library, struct Vec
  locator: insert
- url: https://en.cppreference.com/w/c/memory/realloc
  title: cppreference.com, realloc
  locator: Description and Return value
- url: https://en.cppreference.com/w/cpp/container/vector/insert
  title: cppreference.com, std::vector::insert
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 3.4 Shell Parameters and 6.7 Arrays
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 4
- url: https://docs.python.org/3/tutorial/datastructures.html#more-on-lists
  title: The Python Tutorial, Data Structures
  locator: 5.1 More on Lists
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/push
  title: MDN Web Docs, Array.prototype.push()
  locator: Return value
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.getenumerator?view=net-10.0
  title: .NET API reference, List<T>.GetEnumerator method
  locator: Remarks
- url: https://go.dev/ref/spec#Appending_and_copying_slices
  title: The Go Programming Language Specification
  locator: Appending to and copying slices
- url: https://en.cppreference.com/w/cpp/container/vector/push_back
  title: cppreference.com, std::vector::push_back
  locator: Description
---

## Task

Add one item to a list of integers. The function `insert_at(items, index, value)` returns a new list with `value` at position `index`, and `items` does not change. An index of `len(items)` adds the value at the end, and an index of 0 adds it at the front. For example, `insert_at([1, 2], 2, 3)` is `[1, 2, 3]`, and `insert_at([2, 3], 0, 1)` is `[1, 2, 3]`.

Each item is a 32-bit signed integer. The index must be from 0 to `len(items)`. The solutions show the in-place calls too, because most code adds to a list in place.

## Solutions

| Language | At the end, in place | At a position, in place |
|---|---|---|
| Python | `items.append(value)` | `items.insert(index, value)` |
| JavaScript, TypeScript | `items.push(value)` | `items.splice(index, 0, value)` |
| Java | `list.add(value)` | `list.add(index, value)` |
| C# | `list.Add(value)` | `list.Insert(index, value)` |
| Go | `items = append(items, value)` | `items = slices.Insert(items, index, value)` |
| Rust | `v.push(value)` | `v.insert(index, value)` |
| C | `realloc` to one more item, then `a[n] = value` | `realloc`, then `memmove` the tail |
| C++ | `v.push_back(value)` | `v.insert(v.begin() + index, value)` |
| Bash | `a+=("$value")` | `a=("${a[@]:0:i}" "$value" "${a[@]:i}")` |

- **Python.** `append` does "Append value to the end of the sequence"[^1]. `insert` does "Insert value into sequence at the given index"[^1]. The solution copies the list with `list(items)` and then calls `insert`, so the input does not change.
- **JavaScript.** `push` adds at the end. `splice` "changes the contents of an array by removing or replacing existing elements and/or adding new elements in place"[^2], so `splice(index, 0, value)` inserts. The solution uses `slice` and spread syntax to make a new array.
- **Java.** An array cannot grow, so the solution makes an array with one more slot and copies the two parts with `System.arraycopy`. For an `ArrayList`, `add(index, value)` "Shifts the element currently at that position (if any) and any subsequent elements to the right (adds one to their indices)"[^3].
- **C#.** For `Insert`, "If index is equal to Count, item is added to the end of List<T>"[^4]. The solution uses a collection expression with two ranges, `[.. items[..index], value, .. items[index..]]`.
- **Go.** `slices.Insert` "modifies s in place by inserting the values v... at index i, and returns the modified slice"[^5]. So the solution clones `items` first. Without the clone, the call can write into the array of the caller.
- **Rust.** `insert` "Inserts an element at position index within the vector, shifting all elements after it to the right"[^6]. `push` adds at the end.
- **C.** The solution uses `malloc` for `n + 1` items and copies the two parts with `memcpy`. To grow an array in place, use `realloc`. "If ptr is NULL, the behavior is the same as calling malloc(new_size)"[^7], so the first call can start from a null pointer.
- **C++.** `insert` with one value "Inserts a copy of value before pos"[^8], so `v.end()` as the position appends.
- **Bash.** With `+=` and parentheses, "new values are appended to the array beginning at one greater than the array’s maximum index"[^9]. The solution takes the items before and after the index with `${a[@]:offset:length}`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the only valid index of an empty list is 0, and the result has one item.
- `single-front` and `single-end`: a list of one item has two valid positions, 0 and 1.
- `front-of-many` and `end-of-many`: the index 0 and the index `len(items)` are the two ends. The index `len(items)` is valid, but it is not a valid index for reading an item.
- `duplicates`: the value can be equal to items in the list. The result keeps all of them.
- `limit`: the items and the value can be the largest and the smallest 32-bit signed integers.
- An index past the end is out of range. Each solution reports it: an exception, a panic, `NULL` in C, or status 1 in Bash. Python `insert` does not fail, because it works like a slice assignment, and for a slice, "If i or j is greater than len(s), use len(s)"[^10]. So the Python solution checks the index first.

## Pitfalls

- **Python append returns None.** "methods like insert, remove or sort that only modify the list have no return value printed"[^11], because "they return the default None"[^11]. So `items = items.append(3)` sets `items` to `None`. Also, `append` adds one item, so the append of a list adds the list as one item. Use `extend` to add each item:

```python run id=python-append-none
items = [1, 2]
result = items.append(3)
print(result, items)
items.append([4, 5])
print(items, len(items))
items.extend([6, 7])
print(items, len(items))
```

```output for=python-append-none
None [1, 2, 3]
[1, 2, 3, [4, 5]] 4
[1, 2, 3, [4, 5], 6, 7] 6
```

- **JavaScript push returns the new length.** The return value is "The new length property of the object upon which the method was called"[^12]. So `items = items.push(3)` sets `items` to a number:

```javascript run id=javascript-push-length
let items = [1, 2];
items = items.push(3);
console.log(items, typeof items);
const list = [1, 3];
list.splice(1, 0, 2);
console.log(list);
```

```output for=javascript-push-length
3 number
[ 1, 2, 3 ]
```

- **Java add at a position past the end throws.** `add(index, value)` throws `IndexOutOfBoundsException` "if the index is out of range (index < 0 || index > size())"[^3]. The index `size()` is valid and appends:

```java run id=java-add-index
import java.util.ArrayList;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Integer> list = new ArrayList<>(List.of(1, 2));
    list.add(2, 3);
    System.out.println(list);
    try {
      list.add(5, 9);
    } catch (IndexOutOfBoundsException e) {
      System.out.println("IndexOutOfBoundsException");
    }
    System.out.println(list);
  }
}
```

```output for=java-add-index
[1, 2, 3]
IndexOutOfBoundsException
[1, 2, 3]
```

- **C# Add in a foreach loop over the same list throws.** "If changes are made to the collection, such as adding, modifying, or deleting elements, the enumerator is irrecoverably invalidated and the next call to MoveNext or IEnumerator.Reset throws an InvalidOperationException"[^13]. In the example, the first `Add` works, and the loop stops at the next step. Loop over a copy, or loop with an index up to the old count:

```csharp run id=csharp-add-in-foreach
var list = new List<int> { 1, 2 };
try
{
    foreach (var x in list)
    {
        list.Add(x * 10);
    }
}
catch (InvalidOperationException)
{
    Console.WriteLine("InvalidOperationException");
}
Console.WriteLine(string.Join(",", list));
foreach (var x in list.ToList())
{
    list.Add(x * 10);
}
Console.WriteLine(string.Join(",", list));
```

```output for=csharp-add-in-foreach
InvalidOperationException
1,2,10
1,2,10,10,20,100
```

- **Go append in a function does not change the length of the caller.** `append` "returns the resulting slice"[^14], and a function gets a copy of the slice header. In the example, `add` writes 1 into the shared array, but `s` in `main` still has length 0. Return the new slice, and assign it:

```go run id=go-append-in-func
package main

import "fmt"

func add(s []int, v int) {
	s = append(s, v)
}

func addAndReturn(s []int, v int) []int {
	return append(s, v)
}

func main() {
	s := make([]int, 0, 4)
	add(s, 1)
	fmt.Println(len(s), s, s[:1])
	s = addAndReturn(s, 2)
	fmt.Println(len(s), s)
}
```

```output for=go-append-in-func
0 [] [1]
1 [2]
```

- **Rust insert past the end panics.** For `insert`, the documentation says "Panics if index > len"[^6]. The index `len` is valid and appends. The example sets an empty panic hook, so the panic message does not go to the standard error stream:

```rust run id=rust-insert-panic
use std::panic;

fn main() {
    let mut v = vec![1, 2];
    v.insert(2, 3);
    println!("{v:?}");
    panic::set_hook(Box::new(|_| {}));
    let r = panic::catch_unwind(|| {
        let mut w = vec![1, 2];
        w.insert(5, 9);
    });
    println!("{}", if r.is_err() { "panic" } else { "no panic" });
}
```

```output for=rust-insert-panic
[1, 2, 3]
panic
```

- **C p = realloc(p, size) loses the array when realloc fails.** "On failure, returns a null pointer. The original pointer ptr remains valid and may need to be deallocated with free or realloc"[^7]. If the code assigns the result to `p` at once, a failure sets `p` to `NULL`, and the old array leaks. Keep the result in a second pointer first. The example grows the capacity by doubling:

```c run id=c-realloc-append
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *a = NULL;
    size_t len = 0;
    size_t cap = 0;
    for (int x = 1; x <= 5; x++) {
        if (len == cap) {
            size_t ncap = cap > 0 ? cap * 2 : 2;
            int *tmp = realloc(a, ncap * sizeof *a);
            if (tmp == NULL) {
                free(a);
                return 1;
            }
            a = tmp;
            cap = ncap;
        }
        a[len++] = x;
    }
    for (size_t i = 0; i < len; i++) {
        printf("%d ", a[i]);
    }
    printf("len=%zu cap=%zu\n", len, cap);
    free(a);
    return 0;
}
```

```output for=c-realloc-append
1 2 3 4 5 len=5 cap=8
```

- **C++ push_back can move all the items.** "If after the operation the new size() is greater than old capacity() a reallocation takes place, in which case all iterators (including the end() iterator) and all references to the elements are invalidated"[^15]. So a pointer or a reference to an item is not safe after `push_back`. Keep an index instead. The example fills the vector to its capacity, then adds one more item:

```cpp run id=cpp-push-back-moves
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2};
    while (v.size() < v.capacity()) {
        v.push_back(0);
    }
    const auto before = reinterpret_cast<std::uintptr_t>(v.data());
    const std::size_t cap = v.capacity();
    v.push_back(3);
    const auto after = reinterpret_cast<std::uintptr_t>(v.data());
    std::cout << std::boolalpha << "capacity grew: " << (v.capacity() > cap) << ", data moved: " << (before != after) << "\n";
    const std::size_t first = 0;
    std::cout << v[first] << "\n";
}
```

```output for=cpp-push-back-moves
capacity grew: true, data moved: true
1
```

- **Bash a+=x without parentheses changes the first item.** "Referencing an array variable without a subscript is equivalent to referencing with a subscript of 0"[^9]. And for `+=`, "When applied to a string-valued variable, value is expanded and appended to the variable’s value"[^9]. So `a+=three` adds text to `a[0]`, and the array keeps 2 items. Use `a+=(three)`:

```bash run id=bash-append-parens
a=(one two)
a+=three
echo "${#a[@]} ${a[0]}"
b=(one two)
b+=(three)
echo "${#b[@]} ${b[2]}"
```

```output for=bash-append-parens
2 onethree
3 three
```

## Related tasks

- [[concatenate-two-lists]]: add all the items of a second list.
- [[create-an-array]]: make an array of a given length.
- [[get-the-length-of-an-array]]: get the number of items.
- [[filter-a-list]]: keep the items that pass a test.

## References

---
tag: 9WX5NJ
kind: task
slug: create-a-two-dimensional-array
name: Create a two-dimensional array
aliases: [2d array, two-dimensional list, matrix of zeros, grid of values, list of lists, nested array, jagged array, array of arrays]
area: data-and-values/arrays-and-lists
level: intro
summary: Make a grid of rows and columns filled with one value, with a separate list for each row, and avoid the rows that share one list.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/faq/programming.html#how-do-i-create-a-multidimensional-list
  title: Python Programming FAQ
  locator: How do I create a multidimensional list?
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/from
  title: MDN Web Docs, Array.from()
  locator: Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html#fill(int%5B%5D,int)
  title: Java SE 25 API, Class Arrays
  locator: fill(int[] a, int val)
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/arrays
  title: C# language reference, Arrays
  locator: Array overview and Jagged arrays
- url: https://go.dev/ref/spec#Slice_types
  title: The Go Programming Language Specification
  locator: Slice types and Slice expressions
- url: https://doc.rust-lang.org/std/macro.vec.html
  title: Rust standard library, macro vec
  locator: Description
- url: https://en.cppreference.com/w/cpp/container/vector/vector
  title: cppreference.com, std::vector constructor
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 2
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/fill
  title: MDN Web Docs, Array.prototype.fill()
  locator: Parameters
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html#jls-10.7
  title: The Java Language Specification, Java SE 25 Edition
  locator: 10.7 Array Members
- url: https://en.cppreference.com/w/c/language/array
  title: cppreference.com, C array declaration
  locator: Multidimensional arrays
---

## Task

Make a grid with `rows` rows and `cols` columns, with `value` in each cell. The function `grid(rows, cols, value)` returns a list of rows, and each row is a list of `cols` items. For example, `grid(2, 3, 0)` is `[[0, 0, 0], [0, 0, 0]]`.

Each row must be a separate list. After `g[0][0] = 1`, the cell `g[1][0]` must keep its value. Each value is a 32-bit signed integer, and `rows` and `cols` are from 0 to 100.

## Solutions

| Language | A grid with a separate list for each row |
|---|---|
| Python | `[[value] * cols for _ in range(rows)]` |
| JavaScript, TypeScript | `Array.from({ length: rows }, () => new Array(cols).fill(value))` |
| Java | `new int[rows][cols]`, then `Arrays.fill(row, value)` for each row |
| C# | `new int[rows][]` and a loop that makes each row, or `new int[rows, cols]` |
| Go | `make([][]int, rows)` and a loop that makes each row |
| Rust | `vec![vec![value; cols]; rows]` |
| C | one block of `rows * cols` items, with the index `i * cols + j` |
| C++ | `std::vector<std::vector<int>>(rows, std::vector<int>(cols, value))` |
| Bash | one line for each row, or one array with the index `i * cols + j` |

- **Python.** The FAQ gives the comprehension `A = [[None] * w for i in range(h)]`[^1]. The expression `[value] * cols` runs once for each row, so each row is a new list.
- **JavaScript.** For `Array.from`, `mapFn` is "A function to call on every element of the array"[^2]. The function returns a new row each time.
- **Java.** `new int[rows][cols]` makes the outer array and one array for each row, with 0 in each cell. `Arrays.fill` "Assigns the specified int value to each element of the specified array of ints"[^3].
- **C#.** "An array can be single-dimensional, multidimensional, or jagged"[^4]. A multidimensional array, `int[,]`, is one block with one length for each dimension. A jagged array, `int[][]`, is an array of rows, and the rows can have different lengths. The solution makes a jagged array, so that each row is a list as in the contract.
- **Go.** "Like arrays, slices are always one-dimensional but may be composed to construct higher-dimensional objects"[^5]. For a slice of slices, "the inner slices must be initialized individually"[^5], so the loop makes each row.
- **Rust.** `vec![elem; n]` "will use clone to duplicate an expression"[^6]. The clone of a `Vec` is a new vector, so the rows do not share storage.
- **C.** A function cannot return an array, and the column count of `int a[rows][cols]` must be known to index it. The solution returns one block with the rows in order, which is the row-major layout of a C array. The cell at row `i` and column `j` is at the index `i * cols + j`.
- **C++.** The constructor with a count and a value "Constructs a vector with count copies of elements with value value"[^7]. The value is a row, so each copy is a separate vector.
- **Bash.** "Bash provides one-dimensional indexed and associative array variables"[^8]. The solution prints one line for each row. A program can also keep the grid in one array with the index `i * cols + j`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `no-rows`: zero rows give an empty list, and the column count does not matter.
- `no-cols`: two rows with zero columns give two empty rows, `[[], []]`. This is not the same as an empty list. In C, the block has no items, and the row count is lost, so the caller must keep `rows` and `cols`.
- `one-cell`, `one-row` and `one-col`: a grid can have one row or one column. The order of the arguments matters: `grid(1, 4, 2)` has 1 row of 4 items, and `grid(4, 1, 2)` has 4 rows of 1 item.
- `negative`, `min-int` and `max-int`: the value can be any 32-bit signed integer.
- Each test also sets `g[0][0]` to a different value and checks that `g[1][0]` does not change. This test finds rows that share one list. The concept page [multidimensional array](/multidimensional-array/) compares the layouts.

## Pitfalls

- **Python `[[0] * 3] * 2` repeats one row.** For `s * n`, "items in the sequence s are not copied; they are referenced multiple times"[^9]. The FAQ says that "replicating a list with * doesn’t create copies, it only creates references to the existing objects"[^1]. So a change to one row shows in each row. Use a comprehension:

```python run id=python-shared-rows
g = [[0] * 3] * 2
g[0][0] = 1
print(g)
h = [[0] * 3 for _ in range(2)]
h[0][0] = 1
print(h)
```

```output for=python-shared-rows
[[1, 0, 0], [1, 0, 0]]
[[1, 0, 0], [0, 0, 0]]
```

- **JavaScript fill with an array repeats one row.** For `fill`, "if value is an object, each slot in the array will reference that object"[^10]. An array is an object, so `new Array(2).fill(new Array(3).fill(0))` has one row in two slots. `Array.from` with a function makes a new row for each slot:

```javascript run id=javascript-fill-shared
const g = new Array(2).fill(new Array(3).fill(0));
g[0][0] = 1;
console.log(g, g[0] === g[1]);
const h = Array.from({ length: 2 }, () => new Array(3).fill(0));
h[0][0] = 1;
console.log(h, h[0] === h[1]);
```

```output for=javascript-fill-shared
[ [ 1, 0, 0 ], [ 1, 0, 0 ] ] true
[ [ 1, 0, 0 ], [ 0, 0, 0 ] ] false
```

- **Java clone of a two-dimensional array copies only the outer array.** "A clone of a multidimensional array is shallow, which is to say that it creates only a single new array. Subarrays are shared"[^11]. So a change to a cell of the clone changes the original. Clone each row to make a deep copy:

```java run id=java-clone-shallow
import java.util.Arrays;

public class Main {
  public static void main(String[] args) {
    int[][] g = new int[2][3];
    int[][] shallow = g.clone();
    shallow[0][0] = 1;
    System.out.println(Arrays.deepToString(g));
    int[][] deep = new int[g.length][];
    for (int i = 0; i < g.length; i++) {
      deep[i] = g[i].clone();
    }
    deep[1][0] = 2;
    System.out.println(Arrays.deepToString(g) + " " + Arrays.deepToString(deep));
  }
}
```

```output for=java-clone-shallow
[[1, 0, 0], [0, 0, 0]]
[[1, 0, 0], [0, 0, 0]] [[1, 0, 0], [2, 0, 0]]
```

- **C# `new int[2][]` has no rows.** "A jagged array is an array of arrays, and each member array has the default value of null"[^4]. So `g[0][0]` throws `NullReferenceException` until the code makes each row. A multidimensional array, `new int[2, 3]`, has all its cells at once:

```csharp run id=csharp-jagged-null
int[][] g = new int[2][];
try
{
    g[0][0] = 1;
}
catch (NullReferenceException)
{
    Console.WriteLine("NullReferenceException");
}
for (var i = 0; i < g.Length; i++)
{
    g[i] = new int[3];
}
g[0][0] = 1;
Console.WriteLine(string.Join(" | ", g.Select(row => string.Join(",", row))));
int[,] m = new int[2, 3];
m[0, 0] = 1;
Console.WriteLine($"{m.GetLength(0)} {m.GetLength(1)} {m[0, 0]}");
```

```output for=csharp-jagged-null
NullReferenceException
1,0,0 | 0,0,0
2 3 1
```

- **Go rows cut from one slice can write into the next row.** A program can make all the rows from one slice of `rows * cols` items, so that the grid is one block. But a row from `cells[0:3]` has the capacity of the rest of the block, so `append` to the row writes into the next row. The full slice expression `a[low : high : max]` "controls the resulting slice's capacity by setting it to max - low"[^5], so `cells[0:3:3]` makes `append` copy the row:

```go run id=go-rows-share-block
package main

import "fmt"

func main() {
	cells := make([]int, 6)
	g := [][]int{cells[0:3], cells[3:6]}
	g[0] = append(g[0], 9)
	fmt.Println(g[1])
	cells = make([]int, 6)
	h := [][]int{cells[0:3:3], cells[3:6:6]}
	h[0] = append(h[0], 9)
	fmt.Println(h[1], h[0])
}
```

```output for=go-rows-share-block
[9 0 0]
[0 0 0] [0 0 0 9]
```

- **Rust `vec!` puts the count after the value.** The macro can "Create a Vec from a given element and size"[^6], as `vec![elem; n]`. In `vec![vec![0; 3]; 2]`, the inner 3 is the number of columns and the outer 2 is the number of rows, so the grid has 2 rows of 3. Check the lengths when the grid is not square:

```rust run id=rust-vec-order
fn main() {
    let g = vec![vec![0; 3]; 2];
    println!("{} rows, {} columns", g.len(), g[0].len());
    let mut h = vec![vec![0; 3]; 2];
    h[0][0] = 1;
    println!("{h:?}");
}
```

```output for=rust-vec-order
2 rows, 3 columns
[[1, 0, 0], [0, 0, 0]]
```

- **C `int **` is not a two-dimensional array.** An `int a[2][3]` is one block of 6 `int` items, and "a multidimensional array is converted to a pointer to its first element, e.g., pointer to the first row"[^12]. That pointer has the type `int (*)[3]`, not `int **`. A function for the array takes `int (*a)[3]` or `int a[][3]`, so it knows the length of a row. The output is from a machine with 4-byte `int`:

```c run id=c-pointer-to-row
#include <stddef.h>
#include <stdio.h>

static int sum(size_t rows, int (*a)[3]) {
    int s = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < 3; j++) {
            s += a[i][j];
        }
    }
    return s;
}

int main(void) {
    int a[2][3] = {{1, 2, 3}, {4, 5, 6}};
    printf("%zu %zu\n", sizeof a, sizeof a[0]);
    printf("%td\n", (char *)&a[1][0] - (char *)&a[0][0]);
    printf("%d\n", sum(2, a));
    return 0;
}
```

```output for=c-pointer-to-row
24 12
12
21
```

- **C++ a vector of rows with a count only has empty rows.** The constructor with only a count "Constructs a vector with count default-inserted objects of T"[^7], and a default-inserted vector is empty. So `std::vector<std::vector<int>> g(2)` has 2 rows with no items, and `g[0][0]` is undefined behavior. Pass the row too:

```cpp run id=cpp-empty-rows
#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> g(2);
    std::cout << g.size() << " " << g[0].size() << "\n";
    std::vector<std::vector<int>> h(2, std::vector<int>(3, 0));
    h[0][0] = 1;
    std::cout << h.size() << " " << h[0].size() << " " << h[0][0] << " " << h[1][0] << "\n";
}
```

```output for=cpp-empty-rows
2 0
2 3 1 0
```

- **Bash `a[0,1]` is not a cell of a grid.** "Indexed arrays are referenced using arithmetic expressions that must expand to an integer"[^8]. In an arithmetic expression, the comma is an operator, and `0,1` has the value 1. So `a[0,1]` and `a[1,1]` are the same item, `a[1]`. In an associative array, the key `0,1` is a string, so each cell has its own key:

```bash run id=bash-comma-index
a=()
a[0,1]=x
a[1,1]=y
echo "${#a[@]} ${!a[@]} ${a[1]}"
declare -A g
g[0,1]=x
g[1,1]=y
echo "${#g[@]} ${g[0,1]} ${g[1,1]}"
```

```output for=bash-comma-index
1 1 y
2 x y
```

## Related tasks

- [[create-an-array]]: make an array of one dimension with a given length.
- [[get-the-length-of-an-array]]: get the number of rows and the number of columns.
- [[iterate-in-chunks]]: split a list into rows of a fixed size.
- [[flatten-a-nested-list]]: join the rows into one list.

## References

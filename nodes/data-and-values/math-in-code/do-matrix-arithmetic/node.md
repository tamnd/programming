---
tag: R3P151
kind: task
slug: do-matrix-arithmetic
name: Do matrix arithmetic
aliases: [matrix multiplication, multiply two matrices, matrix product, matrix transpose, transpose a matrix, 2D array, two-dimensional array, row-major order, column-major order, "@ operator", matmul, zip(*m)]
area: data-and-values/math-in-code
level: core
summary: >-
  Multiply two matrices and transpose a matrix, with a list of rows. The columns of the first matrix must match the rows of the second, and a times b is not b times a.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Matrix-multiplication/00-TASK.txt
  title: Rosetta Code, Matrix multiplication (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Matrix-transposition/00-TASK.txt
  title: Rosetta Code, Matrix transposition (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Matrix_multiplication
  title: Wikipedia, Matrix multiplication
  locator: Introduction; Fundamental properties; Computational complexity
- url: https://docs.python.org/3/library/functions.html
  title: The Python Standard Library, Built-in Functions
  locator: zip
- url: https://peps.python.org/pep-0465/
  title: PEP 465, A dedicated infix operator for matrix multiplication
  locator: Abstract; Specification
- url: https://en.wikipedia.org/wiki/Row-_and_column-major_order
  title: Wikipedia, Row- and column-major order
  locator: Introduction; Programming languages and libraries
- url: https://docs.python.org/3/faq/programming.html
  title: Python Programming FAQ
  locator: How do I create a multidimensional list?
---

## Task

Multiply two matrices, and transpose a matrix. The Rosetta Code task says: "Multiply two matrices together"[^1]. The matrices "can be of any dimensions, so long as the number of columns of the first matrix is equal to the number of rows of the second matrix"[^1]. The second task is: "Transpose an arbitrarily sized rectangular Matrix"[^2].

A matrix is a list of rows, and each row is a list of numbers. The function `matrix_multiply(a, b)` returns the product of `a` and `b`. If `a` has `n` rows and `m` columns, and `b` has `m` rows and `p` columns, the product has `n` rows and `p` columns. The item in row `i` and column `j` of the product is the sum of `a[i][k] * b[k][j]` for each `k`: row `i` of `a` times column `j` of `b`. For example:

```text
[1 2]   [5 6]   [1*5 + 2*7   1*6 + 2*8]   [19 22]
[3 4] x [7 8] = [3*5 + 4*7   3*6 + 4*8] = [43 50]
```

"For matrix multiplication, the number of columns in the first matrix must be equal to the number of rows in the second matrix"[^3]. The order is important: "the product is not commutative in general"[^3], so `a` times `b` is often not `b` times `a`.

The transpose of a matrix swaps the rows and the columns: row `i` of the transpose is column `i` of the matrix. Each solution also has a function `transpose`.

## Solutions

| Language | Matrix | Transpose |
|---|---|---|
| Python | a list of lists, and `zip(*b)` for the columns | `zip(*m)` |
| JavaScript, TypeScript | an array of arrays, with `map` and `reduce` | `map` over the columns |
| Java | `long[][]`, with three loops | two loops |
| C# | a jagged array `long[][]`, with three loops | two loops |
| Go | `[][]int64`, with three loops | two loops |
| Rust | slices of rows, and a `Vec<Vec<i64>>` result | iterators |
| C | a flat array in row-major order, with three loops | two loops |
| C++ | `std::vector<std::vector<long long>>`, with three loops | two loops |
| Bash | a string of rows, separated by `;`, and a flat array | loops |

- **Python.** `zip(*b)` gives the columns of `b`, because "zip() in conjunction with the * operator can be used to unzip a list"[^4]. Then each item of the product is `sum(x * y for x, y in zip(row, col))`. Python has the operator `@` for matrix multiplication: "This PEP proposes a new binary operator to be used for matrix multiplication, called @"[^5]. But "No implementations of these methods are added to the builtin or standard library types"[^5], so `@` works on NumPy arrays and not on lists.
- **JavaScript.** There is no matrix type. `a.map` makes the rows of the product, and `row.reduce` makes each sum. The function checks the sizes first, as the pitfalls show.
- **TypeScript.** The same functions as JavaScript, with the type `readonly (readonly number[])[]`.
- **Java.** `long[][]` is an array of rows. The loops go in the order `i`, `k`, `j`, so the inner loop reads a row of `b`, not a column.
- **C#.** A jagged array `long[][]`, with the same loops as Java. `System.Numerics` has `Matrix3x2` and `Matrix4x4` for graphics, with `float` items and fixed sizes, so they do not fit this task.
- **Go.** `[][]int64` is a slice of rows. `MatrixMultiply` returns an error if the sizes do not match.
- **Rust.** The standard library has no matrix type. The function accepts a slice of rows of any type that gives a `&[i64]`, and returns `None` if the sizes do not match. The crates `nalgebra` and `ndarray` have matrix types.
- **C.** The matrices are flat arrays in row-major order: the item in row `i` and column `j` of a matrix with `m` columns is at index `i * m + j`. The caller gives the sizes.
- **C++.** `std::vector<std::vector<long long>>` holds the rows. The function throws `std::invalid_argument` if the sizes do not match.
- **Bash.** Bash has no two-dimensional arrays. A matrix is a string such as `"1 2;3 4"`, and the function puts the numbers in a flat array and uses the index `i * m + k`.

The Python functions:

```python run id=python-matrix-multiply
def transpose(m):
    return [list(col) for col in zip(*m)]


def matrix_multiply(a, b):
    cols = list(zip(*b))
    return [[sum(x * y for x, y in zip(row, col)) for col in cols] for row in a]


a = [[1, 2], [3, 4]]
b = [[5, 6], [7, 8]]
print(matrix_multiply(a, b))
print(matrix_multiply(b, a))
print(transpose([[1, 2, 3], [4, 5, 6]]))
```

```output for=python-matrix-multiply
[[19, 22], [43, 50]]
[[23, 34], [31, 46]]
[[1, 4], [2, 5], [3, 6]]
```

`a` times `b` and `b` times `a` are different. The Go function, with a 2 x 3 matrix and a 3 x 2 matrix. In the other order, the product is 3 x 3:

```go run id=go-matrix-multiply
package main

import "fmt"

func multiply(a, b [][]int) [][]int {
	c := make([][]int, len(a))
	for i := range a {
		c[i] = make([]int, len(b[0]))
		for k, x := range a[i] {
			for j, y := range b[k] {
				c[i][j] += x * y
			}
		}
	}
	return c
}

func main() {
	a := [][]int{{1, 2, 3}, {4, 5, 6}}
	b := [][]int{{7, 8}, {9, 10}, {11, 12}}
	fmt.Println(multiply(a, b))
	fmt.Println(multiply(b, a))
}
```

```output for=go-matrix-multiply
[[58 64] [139 154]]
[[39 54 69] [49 68 87] [59 82 105]]
```

**Row-major order.** A two-dimensional array in C is one block of memory. "In row-major order, the consecutive elements of a row reside next to each other"[^6]. "Row-major order is used in C/C++/Objective-C (for C-style arrays)"[^6], and "Row-major order is the default in NumPy"[^6]. Fortran, MATLAB, Julia and R use column-major order[^6]. The C program shows the order:

```c run id=c-row-major
#include <stdio.h>

int main(void) {
    int m[2][3] = {{1, 2, 3}, {4, 5, 6}};
    const int *flat = &m[0][0];
    for (int i = 0; i < 6; i++) {
        printf("%d%c", flat[i], i == 5 ? '\n' : ' ');
    }
    printf("%d %d\n", m[1][2], flat[1 * 3 + 2]);
    return 0;
}
```

```output for=c-row-major
1 2 3 4 5 6
6 6
```

The rows are next to each other, so `m[1][2]` is at index `1 * 3 + 2`. A loop that reads the items in the order of memory is faster, because the processor loads the next items into the cache with each item. That is why the solutions use the loop order `i`, `k`, `j`.

**Cost.** The product of an `n` x `m` matrix and an `m` x `p` matrix takes `n * m * p` multiplications. For two square `n` x `n` matrices, the algorithm of the definition takes `n * n * n` multiplications, so its time is O(n³)[^3]. Faster algorithms exist: Strassen's algorithm, from 1969, has a smaller exponent, about 2.807[^3]. Numerical libraries also divide the matrices into blocks that fit in the cache, so they are much faster than three simple loops. The transpose takes one step for each item, `n * m` steps.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `two-by-two`: the example above.
- `two-by-three-times-three-by-two`: two matrices that are not square. The product is 2 x 2.
- `one-by-one`: a matrix with one item. The product is the product of the two numbers.
- `row-times-column`: a 1 x 3 matrix times a 3 x 1 matrix gives a 1 x 1 matrix, the dot product.
- `column-times-row`: a 3 x 1 matrix times a 1 x 3 matrix gives a 3 x 3 matrix, the outer product.
- `identity`: the identity matrix times `b` is `b`.
- `other-order`: `b` times `a` for the matrices of `two-by-two`. The result is different.
- `zero-matrix`: the zero matrix times any matrix is the zero matrix.
- `negative-numbers`: items below 0, and a sum that is 0.
- `beyond-32-bit`: the item 10000000000 does not fit in 32 bits, so the solutions use 64-bit integers.

## Pitfalls

- **All rows are the same list.** In Python, `[[0] * p] * n` makes one row and `n` references to it. "The reason is that replicating a list with * doesn’t create copies, it only creates references to the existing objects"[^7]. Each write to a row writes to all rows:

```python wrong id=python-shared-rows
def matrix_multiply(a, b):
    c = [[0] * len(b[0])] * len(a)
    for i in range(len(a)):
        for j in range(len(b[0])):
            c[i][j] = sum(a[i][k] * b[k][j] for k in range(len(b)))
    return c


print(matrix_multiply([[1, 2], [3, 4]], [[5, 6], [7, 8]]))
```

```output for=python-shared-rows
[[43, 50], [43, 50]]
```

The result has the last row two times. Make a new list for each row: `[[0] * p for _ in range(n)]`.

- **No check of the sizes.** If the number of columns of `a` is not the number of rows of `b`, the product is not defined. A loop with no check can return a wrong result and no error. In JavaScript, `reduce` stops at the end of the row of `a`, so it does not see the third row of `b`:

```javascript wrong id=javascript-no-size-check
function matrixMultiply(a, b) {
  return a.map((row) => b[0].map((_, j) => row.reduce((sum, x, k) => sum + x * b[k][j], 0)));
}

console.log(matrixMultiply([[1, 2]], [[1], [2], [3]]));
```

```output for=javascript-no-size-check
[ [ 5 ] ]
```

A 1 x 2 matrix times a 3 x 1 matrix has no product, but the function returns `[[5]]`. Check the sizes first, as the solutions do.

- **The order of the matrices.** `a` times `b` is not `b` times `a` in general. When the two matrices are not square, one of the two products can be undefined, or the two products can have different sizes, as in the Go example.
- **Overflow.** Each item of the product is a sum of `m` products, so it can be much larger than the items of `a` and `b`. With 32-bit integers, the `beyond-32-bit` case overflows. In C, a signed overflow is undefined behavior.
- **A jagged array is not a matrix.** In Java, C#, Go and JavaScript, the rows are separate arrays, so they can have different lengths. The solutions read the length of the first row only. Check all rows if the input comes from outside the program.
- **Column-major libraries.** A library in Fortran order, such as a BLAS routine, reads a flat array as columns. A flat C array in row-major order is the transpose for such a library.


## Related tasks

- [[do-complex-number-arithmetic]]: another task with a type that some languages do not have.
- [[sum-and-multiply-a-list]]: each item of the product is the sum of the products of two lists.
- [[use-mathematical-constants-and-functions]]: the other number functions of the standard libraries.

## References

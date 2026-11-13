---
tag: 25N6W4
kind: concept
slug: multidimensional-array
name: Multidimensional array
aliases: [two-dimensional array, 2D array, matrix, nested array, array of arrays, jagged array, ragged array, rectangular array, row-major order, column-major order, Iliffe vector, ndarray, mdspan]
area: data-and-values/arrays-and-lists
level: core
summary: >-
  A multidimensional array selects an item by two or more indices, such as a row and a column. It is either one block in row-major or column-major order, or an array of separate rows.
applies_to: core-languages
viz: optional
formal: none
see_also: [array, array-indexing, dynamic-array, list, slice-concept, pointers-and-references, in-place-and-copying-operations, array-oriented-programming]
sources:
- url: https://en.wikipedia.org/wiki/Row-_and_column-major_order
  title: Row- and column-major order, Wikipedia
  locator: Introduction; Programming languages and libraries; Address calculation in general
- url: https://numpy.org/doc/stable/reference/arrays.ndarray.html
  title: The N-dimensional array (ndarray), NumPy reference
  locator: Internal memory layout of an ndarray
- url: https://en.cppreference.com/c/language/array
  title: Array declaration, C, cppreference.com
  locator: Multidimensional arrays
- url: https://doc.rust-lang.org/reference/types/array.html
  title: Array types, The Rust Reference
  locator: type.array.intro
- url: https://en.wikipedia.org/wiki/Jagged_array
  title: Jagged array, Wikipedia
  locator: Introduction
- url: https://en.wikipedia.org/wiki/Iliffe_vector
  title: Iliffe vector, Wikipedia
  locator: Introduction; Data structure
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: Chapter 10, Arrays, The Java Language Specification, Java SE 25
  locator: 10.2 Array Variables; 10.7 Array Members
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/arrays
  title: Arrays, C# language reference
  locator: Introduction; Multidimensional arrays; Jagged arrays
- url: https://go.dev/doc/effective_go
  title: Effective Go
  locator: Two-dimensional slices
- url: https://en.cppreference.com/cpp/container/mdspan
  title: std::mdspan, cppreference.com
  locator: Description
- url: https://docs.python.org/3/faq/programming.html
  title: Programming FAQ, Python documentation
  locator: How do I create a multidimensional list?
---

## Idea

A multidimensional array names each item by two or more indices. A two-dimensional array is a table or a matrix: `m[r][c]` is the item in row `r` and column `c`. A three-dimensional array adds a third index, such as a plane, and so on.

Memory has one dimension, so the language must put the rows somewhere. There are two common ways:

- a **dense array**: all items are in one block, and the language computes one offset from all the indices. The items of each row are next to each other (row-major order) or the items of each column are next to each other (column-major order).
- an **array of arrays**: an outer array holds references to separate row arrays. The rows can have different lengths, and then the array is called jagged.

Wikipedia states the difference: "In row-major order, the consecutive elements of a row reside next to each other, whereas the same holds true for consecutive elements of a column in column-major order"[^1].

```python run id=idea
# A 2 x 3 matrix as rows, and the same items in one flat list in two orders.
m = [[1, 2, 3],
     [4, 5, 6]]
rows, cols = 2, 3
row_major = [m[r][c] for r in range(rows) for c in range(cols)]
col_major = [m[r][c] for c in range(cols) for r in range(rows)]
print(row_major, col_major)

# The offset of item (r, c) in each order.
r, c = 1, 2
print(m[r][c], row_major[r * cols + c], col_major[c * rows + r])
```

```output for=idea
[1, 2, 3, 4, 5, 6] [1, 4, 2, 5, 3, 6]
6 6 6
```

Both flat lists hold the same six items. In row-major order, item `(r, c)` is at `r * cols + c`. In column-major order, it is at `c * rows + r`. All three expressions find the item 6.

## How it works

**Dense arrays.** A dense array stores `rows * cols` items in one block. For row-major order, the offset of `(r, c)` is `r * cols + c`, and in general "the last dimension is contiguous"[^1]. For column-major order, the offset is `c * rows + r`, and "the first dimension is contiguous"[^1]. The number that one index is multiplied by is the stride of that dimension. NumPy describes its arrays in these terms: "An instance of class ndarray consists of a contiguous one-dimensional segment of computer memory (owned by the array, or by some other object), combined with an indexing scheme that maps N integers into the location of an item in the block"[^2]. It adds that "The column-major order (used, for example, in the Fortran language and in Matlab) and row-major order (used in C) schemes are just specific kinds of strided scheme"[^2].

The order depends on the language. Wikipedia lists C and C++ (for C-style arrays), PL/I, Pascal and SAS as row-major, and Fortran, MATLAB, GNU Octave, Julia and R as column-major[^1].

**C arrays.** In C, "When the element type of an array is another array, it is said that the array is multidimensional"[^3]. `int a[2][3]` is an array of 2 arrays of 3 ints, and cppreference notes that it "can be viewed as a 2x3 matrix" "with row-major layout"[^3]. The type is an array of arrays, but the memory is one dense block: in the C example, `sizeof a` is 24 bytes, and the item after `a[0][2]` is `a[1][0]`. Rust `[[i32; 3]; 2]` and C++ `std::array<std::array<int, 3>, 2>` have the same layout, because a Rust array "is a fixed-size sequence of N elements of type T"[^4] with no gaps.

**Arrays of arrays.** Java, Python, JavaScript and Go slices build a table from separate row objects. Wikipedia defines a jagged array as "an array of arrays of which the member arrays can be of different lengths"[^5], and it notes that two-dimensional arrays are always rectangular, "so jagged arrays should not be confused with multidimensional arrays, but the former is often used to emulate the latter"[^5]. The structure behind it has a name: "an Iliffe vector, also known as a display, is a data structure used to implement jagged arrays"[^6]. In Java, `int[][]` is an array whose items are `int[]` objects. Each row can be replaced or be null, and a copy of the outer array shares the rows: "A clone of a multidimensional array is shallow, which is to say that it creates only a single new array. Subarrays are shared"[^7].

**Both forms in one language.** C# has a type for each form. "An array can be single-dimensional, multidimensional, or jagged"[^8]. `int[,]` is one rectangular object, and "A jagged array is an array of arrays, and each member array has the default value of null"[^8]. For `int[,]`, "traversal increments the indices of the rightmost dimension first, then the next left dimension, and so on, to the leftmost index"[^8], which is row-major order. Go also gives two choices for a slice of slices: "One is to allocate each slice independently; the other is to allocate a single array and point the individual slices into it"[^9]. Go explains that "If the slices might grow or shrink, they should be allocated independently to avoid overwriting the next line; if not, it can be more efficient to construct the object with a single allocation"[^9].

**Views.** A view gives multidimensional indices to a block that the program owns elsewhere. C++23 added one: "std::mdspan is a multidimensional array view that maps a multidimensional index to an element of the array"[^10]. Its default layout policy is `std::layout_right`[^10], where the rightmost index changes fastest, as in row-major order. The policy `layout_left` gives column-major order. A NumPy array with other strides is also a view, for example the transpose of a matrix.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | lists of lists (jagged); NumPy `ndarray` for dense arrays in C or Fortran order |
| JavaScript | idiom | arrays of arrays; a typed array with an index formula for a dense matrix |
| TypeScript | idiom | the JavaScript forms with types such as `number[][]` |
| Java | native | `int[][]` is an array of row arrays, which can be jagged; a flat array for a dense matrix |
| Kotlin | native | `Array<IntArray>` and `List<List<T>>`, both arrays of rows; `Array(rows) { IntArray(cols) }` |
| C# | native | `int[,]` for a rectangular array in one object; `int[][]` for a jagged array |
| Go | native | `[r][c]T` is a dense array value; `[][]T` is a slice of slices, with one or many allocations |
| Rust | native | `[[T; C]; R]` is dense; `Vec<Vec<T>>` is jagged; a flat `Vec<T>` with an index formula |
| C | native | `T a[R][C]` is dense in row-major order; an array of pointers for a jagged array |
| C++ | stdlib | C arrays and `std::array` of `std::array` are dense; `vector<vector<T>>`; `std::mdspan` (C++23) views |
| Bash | n/a | arrays have one dimension; an associative array with keys such as `"$r,$c"` imitates a table |
| PowerShell | native | jagged arrays of arrays; `New-Object 'int[,]' 2,3` makes a .NET rectangular array |
| SQL | native | a table is the usual form; PostgreSQL arrays can have many dimensions, such as `int[][]` |
| Regex | n/a | a pattern has no arrays |
| HTML | n/a | markup; a `<table>` shows rows and columns but holds no data values |
| CSS | n/a | CSS Grid lays out elements in rows and columns, but it is layout, not data |

The example files show both forms. Python builds a grid with a comprehension, shows the shared-row trap with `[[0] * 3] * 2`, a shallow copy, a transpose with `zip` and a flat list in row-major order. JavaScript uses `Array.from`, shows that `fill` with an array shares one row, and uses a `Float64Array` as a dense matrix. Java shows `new int[2][3]`, a jagged `int[3][]`, a row that the program replaces, and a shallow `clone`. C# compares `int[,]`, with `Rank`, `GetLength` and row-major `foreach`, against a jagged `int[][]` with null rows. Go shows that an array of arrays is copied by assignment, a slice of slices with one allocation per row, and the single-allocation method from Effective Go. Rust shows the size of a dense array, `flatten`, a jagged `Vec<Vec<i32>>` and row-major and column-major order of a flat `Vec`. C prints the size and the offsets of `int a[2][3]`, walks it with one pointer, and makes a jagged array of pointers. C++ compares `std::array` of `std::array` with `vector<vector<int>>` and writes a small `Matrix` class with one `vector`.

## Variations and related ideas

- **Jagged arrays.** Rows of different lengths fit data such as the lines of a text or the neighbors of each node in a graph. A jagged array saves memory when the rows differ a lot.
- **Higher dimensions.** The same rules work for three or more indices. In row-major order, the offset of `(z, y, x)` is `(z * Y + y) * X + x`, where `Y` and `X` are the sizes of the last two dimensions[^1].
- **Strided views.** A view with strides can show a transpose, a column or every second row without a copy. NumPy and `std::mdspan` with `layout_stride` work this way[^2][^10].
- **Sparse matrices.** When most items are 0, a program stores only the others, for example in a map from `(r, c)` to the value, or in compressed row formats.
- **Tables in SQL.** A table with columns `row`, `col` and `value` is a sparse matrix. A table with one column per field is a list of records, not an array.
- **Array languages.** APL, NumPy and MATLAB apply operations to whole arrays of any rank. See [array-oriented programming](/array-oriented-programming/).

## Pitfalls

- **Shared rows.** `[[0] * 3] * 2` in Python and `Array(2).fill(Array(3).fill(0))` in JavaScript put the same row object in each slot. The Python FAQ explains: "replicating a list with * doesn’t create copies, it only creates references to the existing objects"[^11], so a write to one row "shows up in multiple places"[^11]. Make a new row for each index with a comprehension or `Array.from`.
- **Shallow copies.** A copy of the outer array copies only the references to the rows. In the Java and Python examples, a change through the copy changes the original. Use a deep copy, or copy each row.
- **Loop order and the cache.** A loop that visits a row-major array column by column jumps `cols` items at each step. The order of access "is also important for performance when traversing an array because modern CPUs process sequential data more efficiently than nonsequential data"[^1]. Put the last index in the inner loop for row-major arrays, and the first index for column-major arrays.
- **Order across languages.** A matrix from Fortran, R, MATLAB or Julia is in column-major order. When C or Python code reads the same memory, it sees the transpose unless it uses the column-major formula.
- **Null or missing rows.** A new jagged array in Java or C# has null rows until the program creates them. The JLS shows a loop over `{ { 1, 2 }, null }` that ends with a `NullPointerException`[^7].
- **Swapped indices.** `m[r][c]` and `m[x][y]` have opposite meanings when `x` is the column. Choose one convention, such as row first, and keep it.

## Cost

| Operation | Dense block | Array of arrays |
|---|---|---|
| create `R` by `C` | one allocation | `R + 1` allocations |
| read `m[r][c]` | one offset computation, one memory read | two memory reads: the row reference, then the item |
| memory | `R * C` items | `R * C` items, `R` references and `R` object headers |
| grow one row | not possible without a copy of the whole block | append to that row only |
| visit all items in storage order | sequential, cache friendly | sequential in each row; rows can be anywhere |

A dense array is the better choice for numbers in a fixed shape, such as images and matrices. An array of arrays is the better choice when rows have different lengths or grow. For large dense arrays in a language without a dense type, a flat array and an index formula give the dense layout.

## History

Fortran stores arrays in column-major order[^1], and numerical code written for Fortran, MATLAB and R uses that order. C made a multidimensional array an array of arrays in row-major order[^3], and its descendants kept this form. The array of pointers to rows is called an Iliffe vector, and "The data structure is named after John K. Iliffe"[^6]. Java chose arrays of arrays only, so every Java two-dimensional array can be jagged[^7]. Later languages added dense types for numerical work: C# has `int[,]`[^8], NumPy gives Python the `ndarray`[^2], and C++23 added `std::mdspan`[^10].

## References

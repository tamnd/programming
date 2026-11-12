---
tag: QA48DE
kind: concept
slug: array-oriented-programming
name: Array programming
aliases: [array programming, vector programming, vectorization, broadcasting, APL style, whole-array operations]
area: paradigms/array-oriented
level: core
summary: >-
  Array programming treats a whole array as one value, so operations such as + and max apply to every element at once, and reductions, scans and outer products replace explicit loops.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1962
  people: [Kenneth E. Iverson, Adin Falkoff]
  note: Iverson published his notation in A Programming Language in 1962, and IBM implemented it as APL\360 in 1966.
see_also: [functional-programming, higher-order-functions, declarative-programming, imperative-programming, iterators, laziness, immutability, reactive-programming]
same_as:
  wikidata: Q4307787
  wikipedia: Array_programming
sources:
- url: https://numpy.org/doc/stable/user/basics.broadcasting.html
  title: 'NumPy user guide: Broadcasting'
  locator: Broadcasting
- url: https://www.jsoftware.com/papers/tot1.htm
  title: Kenneth E. Iverson, Notation as a Tool of Thought, 1979 ACM Turing Award Lecture
  locator: Introduction; 1.3 Subordination of Detail
- url: https://numpy.org/doc/stable/user/whatisnumpy.html
  title: 'NumPy user guide: What is NumPy?'
  locator: Why is NumPy fast?
---

## Idea

Array programming treats a whole array as one value. An operation such as `+` or `max` applies to every element of its arguments at once, and the program has no explicit loop over the indices. Where an imperative program writes `for i in range(n): c[i] = a[i] * b[i]`, an array program writes `c = a * b`. Higher-order operations such as reduce, scan and outer product turn functions on numbers into functions on arrays, so most computations become short expressions without loops. APL, J, K, BQN, Fortran, MATLAB, Julia and R are array languages, and NumPy makes Python one.

```python run id=idea
import operator


def pervade(f):
    """Lift a function of two numbers to numbers, lists and nested lists."""

    def g(a, b):
        if isinstance(a, list) and isinstance(b, list):
            if len(a) != len(b):
                raise ValueError(f"length error: {len(a)} and {len(b)}")
            return [g(x, y) for x, y in zip(a, b)]
        if isinstance(a, list):
            return [g(x, b) for x in a]
        if isinstance(b, list):
            return [g(a, y) for y in b]
        return f(a, b)

    return g


add, mul = pervade(operator.add), pervade(operator.mul)

price = [12, 5, 8]
qty = [3, 10, 2]
print(mul(price, qty))       # element by element
print(sum(mul(price, qty)))  # a reduction gives the total
print(add(price, 1))         # the scalar meets every element

table = [[1, 2, 3], [4, 5, 6]]
print(add(table, 10))
print(add(table, [100, 200]))  # one number for each row

try:
    add(price, [1, 2])
except ValueError as e:
    print(e)
```

```output for=idea
[36, 50, 16]
102
[13, 6, 9]
[[11, 12, 13], [14, 15, 16]]
[[101, 102, 103], [204, 205, 206]]
length error: 3 and 2
```

`pervade` takes a function of two numbers and gives a function that works on numbers, lists and lists of lists. Two lists of the same length combine element by element, a number combines with each element of a list, and two lists of different lengths are an error. After this one definition, `mul(price, qty)` and `add(table, 10)` need no loop. Array languages give every arithmetic function this behavior, and they run the loops in compiled code.

## How it works

**Arrays, rank and shape.** An array is a rectangular collection of values. Its rank is the number of axes, and its shape is the list of the lengths of the axes. A single number is an array of rank 0, a vector has rank 1, a matrix has rank 2, and a batch of color images has rank 4. Because the data is rectangular, an implementation can store it in one block of memory and find any element from its indices with arithmetic.

**Scalar functions pervade.** Arithmetic, comparison and logic functions work on numbers. An array language extends each of them to arrays: on two arrays of the same shape the function applies to each pair of elements, and a scalar combines with every element of the other argument. A comparison such as `a > 2` gives an array of booleans, which is the input to selection and counting.

**Agreement and broadcasting.** When two arguments have different shapes, the language must state which shapes agree. NumPy uses broadcasting: "The term broadcasting describes how NumPy treats arrays with different shapes during arithmetic operations. Subject to certain constraints, the smaller array is “broadcast” across the larger array so that they have compatible shapes."[^1] NumPy compares the shapes from the last axis, and two lengths agree when they are equal or when one of them is 1. J and BQN match from the first axis, as the `pervade` function does: a vector of two numbers adds one number to each row of a matrix with two rows. APL accepts only equal shapes or a single element. Shapes that do not agree are an error, not a silent truncation.

**Operators make new functions.** APL calls its higher-order functions operators. Reduce puts a function between the elements of an array: `+/` gives the sum, `×/` the product and `⌈/` the maximum. Scan gives the running results of a reduction. Outer product applies a function to every pair of elements of two arrays and gives a table, and inner product combines two functions to give, for example, the product of two matrices. Iverson named three ways in which a notation subordinates detail: "the use of arrays, the assignment of names to functions and variables, and the use of operators."[^2] He added that "functions defined on vectors are extended systematically to arrays of higher rank."[^2] See [higher-order functions](/higher-order-functions/).

**Selection without loops.** A boolean array selects elements: `a[a > 2]` in NumPy and `(a>2)/a` in APL keep the elements that pass the test. An array of indices reorders or gathers elements, and a grade function returns the indices that would sort an array. Counting, searching, grouping and sorting become operations on whole arrays.

**Speed from vectorization.** An interpreter pays its overhead once for each operation, not once for each element, so `a * b` on a million numbers costs one dispatch and one compiled loop. The NumPy documentation says that "Vectorization describes the absence of any explicit looping, indexing, etc., in the code - these things are taking place, of course, just “behind the scenes” in optimized, pre-compiled C code."[^3] The compiled loop can use the SIMD instructions of the processor, many cores, or a GPU, because the operation states that the elements are independent.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | library | `map`, `sum`, comprehensions; NumPy `ndarray` |
| JavaScript | idiom | `map`, `filter`, `reduce` on arrays and typed arrays |
| TypeScript | idiom | as JavaScript |
| Java | idiom | `Arrays.setAll`, `IntStream`, `Arrays.parallelPrefix` |
| Kotlin | idiom | `map`, `zip`, `fold`, `scan`; operator overloading |
| C# | idiom | LINQ `Zip`, `Select`, `Sum`; `Vector<T>` |
| Go | absent | loops and generic helper functions |
| Rust | idiom | iterator adaptors; the ndarray crate |
| C | absent | loops; vector extensions of the compilers; BLAS |
| C++ | stdlib | `std::valarray`; Eigen and xtensor |
| Bash | absent | loops over arrays of strings; awk |
| PowerShell | idiom | comparison operators that filter arrays |
| SQL | idiom | expressions over all rows, aggregate functions |
| Regex | n/a | no arrays of values |
| HTML | n/a | no values to compute with |
| CSS | n/a | no arrays of values |

None of the core languages of this site is an array language. Their operators work on single numbers, and array operations come from higher-order functions such as `map`, `zip` and `reduce`, or from libraries. The C++ standard library is the exception: `std::valarray` has element-wise operators, masks and slices on one axis.

Libraries carry array programming into general-purpose languages. NumPy gives Python n-dimensional arrays, broadcasting and fast reductions, and the data science stack of Python, from pandas to PyTorch and JAX, builds on the same model. Eigen and xtensor do this for C++, ndarray for Rust, and Gonum for Go. In these libraries, operator overloading lets `a * b + c` read as it does in an array language.

Fortran, MATLAB, Julia and R have array operations in the core language. In Fortran, `c = a * b` on two arrays multiplies the elements, and array sections such as `a(1:n:2)` select every second element. MATLAB and R treat every number as an array. Julia uses a dot to apply any function element-wise, as in `sin.(x) .+ 1`, and fuses the dotted calls of one expression into a single loop.

## Variations and related ideas

- **The APL family.** APL, J, K, q, BQN and Uiua write programs as short expressions of primitive functions and operators, each usually one symbol or one letter. They differ in their symbols, in how arrays nest, and in how functions apply to the axes of an array.
- **Numeric array languages.** Fortran, MATLAB, Julia and R are built for scientific and statistical computing, with matrices and vectors as their main values.
- **Tensor libraries.** PyTorch, JAX and TensorFlow add automatic differentiation and GPU execution to the NumPy model. A tensor is an array of any rank.
- **Data frames and columnar databases.** A data frame is a table of named columns, each an array. Columnar databases such as kdb+, which uses the q language, run queries as array operations over whole columns.
- **Data parallelism.** Each element-wise operation is a parallel loop with no dependencies between iterations, so array programs map well to SIMD units and GPUs. NESL and the data-parallel Haskell projects studied nested data parallelism.
- **Tacit programming.** J and APL let a programmer write functions without naming their arguments, by combining functions with trains and operators. This is the point-free style of [functional programming](/functional-programming/).
- **Rank polymorphism.** One definition works on arrays of any rank, because the language applies it to the cells of the right rank. J's rank operator makes this explicit.
- **Declarative style.** An array expression states what to compute over a whole collection and leaves the order of the work to the implementation. See [declarative programming](/declarative-programming/).

## Pitfalls

- **Broadcasting by accident.** Adding an array of shape (3,) to an array of shape (3, 1) in NumPy gives a 3 by 3 table, not an error. Check the shapes, or reshape the arguments with intent.
- **Lists are not arrays.** In Python `[1, 2, 3] * 2` repeats the list, and in JavaScript `[1, 2] + [3, 4]` joins two strings. The wrong blocks of the Python and JavaScript files show both.
- **Loops over array elements.** A Python loop that reads one NumPy element at a time pays the interpreter cost for each element, and is often slower than a plain list. Express the work as whole-array operations.
- **Large temporaries.** `a * b + c` makes a temporary array for `a * b` before it adds `c`. On large arrays the temporaries use memory and memory bandwidth. Use operations in place, or a library that fuses expressions.
- **Views and copies.** A NumPy slice is a view of the original data, so writing to the slice changes the original, while indexing with a mask gives a copy. Know which operations share memory.
- **Fixed-width overflow.** Arrays of 32-bit or 64-bit integers wrap around on overflow with no error, unlike Python integers. Choose the element type for the range of the data.
- **Dense code.** A line of APL or NumPy can do the work of a page of loops. Name the intermediate results and comment the shapes, so that a reader can follow the data.

## Cost

An element-wise operation on arrays of n elements takes O(n) time and makes a result of n elements. A reduction takes O(n) time and a scan O(n) time and memory. An outer product of arrays of m and n elements takes O(mn) time and memory, which is the main way an array program uses more memory than the loop it replaces. The constant factor is small: the interpreter pays once for each operation, and the inner loop runs as compiled code over contiguous memory, often with SIMD instructions. A chain of operations reads and writes memory once for each step, unless the implementation fuses the steps into one loop, as Julia's dot syntax, expression templates in C++ and tracing compilers such as JAX do.

## History

Kenneth E. Iverson developed a mathematical notation for algorithms at Harvard in the late 1950s and published it in his 1962 book *A Programming Language*. At IBM, Iverson, Adin Falkoff and others implemented it as APL\360, which ran on time-sharing systems from 1966. Iverson wrote that APL "originated in an attempt to provide clear and precise expression in writing and teaching, and which was implemented as a programming language only after several years of use and development".[^2] He received the Turing Award in 1979, and his lecture *Notation as a Tool of Thought* argued that programming languages are "executable and unambiguous" and so can be tools of thought, as mathematical notation is.[^2]

Cleve Moler wrote the first MATLAB in the late 1970s as an interactive interface to the LINPACK and EISPACK libraries. Iverson and Roger Hui released J, a successor of APL that uses ASCII characters, in 1990, and Arthur Whitney released K in 1993. Fortran 90 added array operations and array sections to Fortran in 1991. In Python, Jim Hugunin wrote the Numeric package in 1995, and Travis Oliphant merged Numeric and Numarray into NumPy in 2006. Deep learning made array programming common: PyTorch, TensorFlow and JAX are array languages embedded in Python, and their arrays run on GPUs.

## References

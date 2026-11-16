---
tag: 5W0QZT
kind: feature
owner: python
slug: data-and-science-libraries
name: Data and science libraries
aliases: [NumPy, pandas, SciPy, Matplotlib, scikit-learn, Jupyter, ndarray, DataFrame, matrix multiplication operator, PEP 465]
area: languages/python/ecosystem
level: core
summary: >-
  NumPy gives Python the ndarray. pandas, SciPy, Matplotlib and scikit-learn build on it, and Jupyter runs code in notebooks. The @ operator of PEP 465 is for these libraries.
grammar: null
spec: {standard: null, url: "https://numpy.org/doc/stable/user/whatisnumpy.html", locator: "What is NumPy?"}
see_also: [python/the-python-package-index, python/web-libraries]
sources:
  - url: https://numpy.org/doc/stable/user/whatisnumpy.html
    title: What is NumPy?, NumPy Manual
    locator: What is NumPy?; Why is NumPy fast?
    accessed: 2026-11-16
  - url: https://pandas.pydata.org/docs/getting_started/overview.html
    title: Package overview, pandas documentation
    locator: Package overview; Data structures
    accessed: 2026-11-16
  - url: https://scipy.org/
    title: SciPy
    locator: home page
    accessed: 2026-11-16
  - url: https://matplotlib.org/stable/
    title: Matplotlib documentation
    locator: home page
    accessed: 2026-11-16
  - url: https://scikit-learn.org/stable/
    title: "scikit-learn: machine learning in Python"
    locator: home page
    accessed: 2026-11-16
  - url: https://docs.jupyter.org/en/latest/
    title: Project Jupyter Documentation
    locator: What is a Notebook?
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0465/
    title: "PEP 465: A dedicated infix operator for matrix multiplication"
    locator: Abstract; Specification; (Non)-Definitions for built-in types
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/statistics.html
    title: statistics, Mathematical statistics functions
    locator: introduction
    accessed: 2026-11-16
  - url: https://lp.jetbrains.com/python-developers-survey-2024/
    title: Python Developers Survey 2024 Results
    locator: Data Science
    accessed: 2026-11-16
  - url: https://www.gonum.org/
    title: Gonum
    locator: home page
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#overloading
    title: Go FAQ
    locator: Why does Go not support overloading of methods and operators?
    accessed: 2026-11-16
---

## Synopsis

```python run id=matmul
class Matrix:
    def __init__(self, rows):
        self.rows = rows

    def __matmul__(self, other):
        cols = list(zip(*other.rows))
        return Matrix([[sum(a * b for a, b in zip(row, col)) for col in cols]
                       for row in self.rows])

a = Matrix([[1, 2], [3, 4]])
b = Matrix([[5, 6], [7, 8]])
print((a @ b).rows)
```

```output for=matmul
[[19, 22], [43, 50]]
```

Python has the `@` operator, but no built-in type uses it. A library defines `__matmul__`, as the small class above does. NumPy is the library that most of the data and science libraries build on. Its `ndarray` does the work in compiled code, and pandas, SciPy, Matplotlib and scikit-learn use it.

## Semantics

**r1.** "NumPy is the fundamental package for scientific computing in Python."[^1] "At the core of the NumPy package, is the ndarray object. This encapsulates n-dimensional arrays of homogeneous data types, with many operations being performed in compiled code for performance."[^1]

**r2.** "NumPy arrays have a fixed size at creation, unlike Python lists (which can grow dynamically)."[^1] "The elements in a NumPy array are all required to be of the same data type, and thus will be the same size in memory."[^1]

**r3.** "Vectorization describes the absence of any explicit looping, indexing, etc., in the code".[^1] "Broadcasting is the term used to describe the implicit element-by-element behavior of operations".[^1] Example: [`elementwise`](#elementwise).

**r4.** "pandas is a Python package that provides fast, flexible, and expressive data structures designed to make working with “relational” or “labeled” data both easy and intuitive."[^2] "The two primary data structures of pandas, Series (1-dimensional) and DataFrame (2-dimensional), handle the vast majority of typical use cases in finance, statistics, social science, and many areas of engineering."[^2] "pandas is built on top of NumPy".[^2]

**r5.** "SciPy provides algorithms for optimization, integration, interpolation, eigenvalue problems, algebraic equations, differential equations, statistics and many other classes of problems."[^3] It "Extends NumPy providing additional tools for array computing and provides specialized data structures, such as sparse matrices and k-dimensional trees."[^3]

**r6.** "Matplotlib is a comprehensive library for creating static, animated, and interactive visualizations."[^4] scikit-learn gives "Simple and efficient tools for predictive data analysis" and is "Built on NumPy, SciPy, and matplotlib".[^5]

**r7.** "A notebook is a shareable document that combines computer code, plain language descriptions, data, rich visualizations like 3D models, charts, graphs and figures, and interactive controls."[^6] JupyterLab is an editor for notebooks.[^6]

**r8.** PEP 465 "proposes a new binary operator to be used for matrix multiplication, called @."[^7] A class supports `@` with the methods `__matmul__` and `__rmatmul__`, and `@=` with `__imatmul__`.[^7] Example: [`matmul`](#matmul).

**r9.** "No __matmul__ or __matpow__ are defined for builtin numeric types (float, int, etc.) or for the numbers.Number hierarchy, because these types represent scalars".[^7] Example: [`no-builtin-matmul`](#no-builtin-matmul).

**r10.** The standard library has the module `statistics`. "This module provides functions for calculating mathematical statistics of numeric (Real-valued) data."[^8] "The module is not intended to be a competitor to third-party libraries such as NumPy, SciPy, or proprietary full-featured statistics packages aimed at professional statisticians such as Minitab, SAS and Matlab."[^8] Example: [`stats`](#stats).

**r11.** In the Python Developers Survey 2024, "51% of all surveyed Python developers are involved in data exploration and processing, with pandas and NumPy being the tools mostly used for it."[^9]

## Examples

The module `statistics` is enough for small data (rule r10):

```python run id=stats
import statistics

temps = [21.5, 23.0, 22.4, 25.1, 24.3, 22.9, 23.8]
print(statistics.mean(temps))
print(statistics.median(temps))
print(round(statistics.stdev(temps), 3))
```

```output for=stats
23.285714285714285
23.0
1.21
```

With plain lists, an element-by-element operation needs a loop or a comprehension. NumPy writes the same operation as `a * b` on two arrays, and the loop runs in compiled code (rule r3):

```python run id=elementwise
a = [1.0, 2.0, 3.0, 4.0]
b = [10.0, 20.0, 30.0, 40.0]
print([x * y for x, y in zip(a, b)])
print(a * 2)
```

```output for=elementwise
[10.0, 40.0, 90.0, 160.0]
[1.0, 2.0, 3.0, 4.0, 1.0, 2.0, 3.0, 4.0]
```

The `@` operator on built-in types raises `TypeError` (rule r9):

```python run id=no-builtin-matmul
for left, right in [([[1, 2]], [[3], [4]]), (2, 3)]:
    try:
        left @ right
    except TypeError:
        print(type(left).__name__, '@', type(right).__name__, '-> TypeError')
```

```output for=no-builtin-matmul
list @ list -> TypeError
int @ int -> TypeError
```

## Pitfalls

`*` on a list is not element-by-element arithmetic. `a * 2` repeats the list (example [`elementwise`](#elementwise)). On a NumPy array, the same operator multiplies each element (rule r3).

A NumPy array has a fixed size and one data type (rule r2). A program that appends to an array in a loop makes a new array each time, because "Changing the size of an ndarray will create a new array and delete the original."[^1]

The module `statistics` is for small data (rule r10). "Unless explicitly noted, these functions support int, float, Decimal and Fraction."[^8] For large arrays, use NumPy or SciPy.

`@` is not `*`. On NumPy arrays, `*` multiplies element by element, and `@` is matrix multiplication (rule r8).

## Comparison

"Gonum is a set of packages designed to make writing numerical and scientific algorithms productive, performant, and scalable."[^10] Gonum "contains libraries for matrices and linear algebra; statistics, probability distributions, and sampling".[^10] The Go FAQ says: "Regarding operator overloading, it seems more a convenience than an absolute requirement. Again, things are simpler without it."[^11] So a Go matrix library uses methods for matrix multiplication. In Python, a library can define `@` (rule r8).

[^1]: [What is NumPy?](https://numpy.org/doc/stable/user/whatisnumpy.html)
[^2]: [Package overview, pandas documentation](https://pandas.pydata.org/docs/getting_started/overview.html)
[^3]: [SciPy](https://scipy.org/)
[^4]: [Matplotlib documentation](https://matplotlib.org/stable/)
[^5]: [scikit-learn: machine learning in Python](https://scikit-learn.org/stable/)
[^6]: [Project Jupyter Documentation](https://docs.jupyter.org/en/latest/)
[^7]: [PEP 465: A dedicated infix operator for matrix multiplication](https://peps.python.org/pep-0465/)
[^8]: [statistics, Mathematical statistics functions](https://docs.python.org/3/library/statistics.html)
[^9]: [Python Developers Survey 2024 Results](https://lp.jetbrains.com/python-developers-survey-2024/)
[^10]: [Gonum](https://www.gonum.org/)
[^11]: [Go FAQ, Why does Go not support overloading of methods and operators?](https://go.dev/doc/faq#overloading)

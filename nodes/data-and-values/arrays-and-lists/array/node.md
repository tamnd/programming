---
tag: 9X57XH
kind: concept
slug: array
name: Array
aliases: [static array, fixed-size array, fixed-length array, C array, raw array, typed array, vector]
area: data-and-values/arrays-and-lists
level: core
summary: >-
  An array is a collection of items of one type that sit side by side in memory, so the program finds item i by a computation on i. A plain array has a fixed length.
applies_to: core-languages
viz: optional
formal: none
see_also: [memory-address, pointers-and-references, out-of-bounds-read, out-of-bounds-write, undefined-behavior, value-equality-and-identity, string-concept, array-oriented-programming]
sources:
- url: https://en.wikipedia.org/wiki/Array_(data_structure)
  title: Array (data structure), Wikipedia
  locator: Introduction; History; Resizing; Efficiency
- url: https://en.cppreference.com/w/c/language/array.html
  title: Array declaration, cppreference.com (C)
  locator: Introduction; Assignment; Array to pointer conversion
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: Chapter 10, Arrays, The Java Language Specification, Java SE 25 Edition
  locator: 10 Arrays; 10.3 Array Creation; 10.4 Array Access; 10.5 Array Store Exception
- url: https://go.dev/blog/slices-intro
  title: Go Slices, usage and internals, The Go Blog
  locator: Arrays; Slices
- url: https://doc.rust-lang.org/std/primitive.array.html
  title: Primitive Type array, The Rust Standard Library
  locator: Description
- url: https://doc.rust-lang.org/book/ch03-02-data-types.html
  title: Data Types, The Rust Programming Language
  locator: The Array Type; Invalid Array Element Access
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/arrays
  title: The array reference type, C# reference, Microsoft Learn
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/container/array.html
  title: std::array, cppreference.com
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: Array, MDN Web Docs
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray
  title: TypedArray, MDN Web Docs
  locator: Description
- url: https://docs.python.org/3/library/array.html
  title: array, Efficient arrays of numeric values, The Python Standard Library
  locator: Introduction
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Lists
- url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html
  title: Everyday Types, The TypeScript Handbook
  locator: Arrays
- url: https://kotlinlang.org/docs/arrays.html
  title: Arrays, Kotlin documentation
  locator: Introduction; When to use arrays; Primitive-type arrays
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays
- url: https://www.postgresql.org/docs/current/arrays.html
  title: Arrays, PostgreSQL documentation
  locator: 8.15.1 Declaration of Array Types; 8.15.2 Array Value Input
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_arrays
  title: about_Arrays, PowerShell documentation, Microsoft Learn
  locator: Long description; Manipulating an array
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Array types
---

## Idea

An array keeps many values of one type under one name, and the program selects a value by its position, the index. Wikipedia describes the data structure as "a collection of elements (values or variables), of the same memory size, each identified by at least one array index or key"[^1]. The important property is the layout: "An array is stored such that the position (memory address) of each element can be computed from its index tuple by a mathematical formula"[^1].

For a one-dimensional array, the formula is simple. If the first item is at the base address `B` and each item takes `k` bytes, item `i` is at `B + i * k`. Wikipedia gives an example: ten 4-byte integers at address 2000, "so that the element with index i has the address 2000 + (i × 4)"[^1]. One multiplication and one addition find any item, so the program reads or writes item 5 or item 5000 in the same time.

A plain array has a fixed length. The C rules say that the number of items "never changes during the array lifetime"[^2], and Java says: "Once an array object is created, its length never changes"[^3]. A collection that can grow is a [dynamic array](/dynamic-array/). It keeps its items in an array and moves them to a bigger array when it is full.

```python run id=idea
import ctypes
from array import array

# A fixed-size array of four C ints. The size is part of the type.
Four = ctypes.c_int * 4
a = Four(10, 20, 30, 40)
print(len(a), ctypes.sizeof(a), ctypes.sizeof(ctypes.c_int))

# Item i is at the base address plus i times the item size.
base = ctypes.addressof(a)
print([ctypes.addressof(ctypes.c_int.from_buffer(a, i * 4)) - base for i in range(4)])

# Reading or writing an item takes the same time for any index.
a[2] = 99
print(list(a))

# The array has no room for a fifth item.
try:
    a[4] = 50
except IndexError as e:
    print("IndexError:", e)

# The array module stores numbers of one type, packed side by side.
b = array("i", [1, 2, 3])
print(b.itemsize * len(b), len(b.tobytes()))
try:
    b.append(2.5)
except TypeError as e:
    print("TypeError:", e)

# A Python list is a dynamic array of references, so it can grow and mix types.
items = [1, "two", 3.0]
items.append([4])
print(len(items), items)
```

```output for=idea
4 16 4
[0, 4, 8, 12]
[10, 20, 99, 40]
IndexError: invalid index
12 12
TypeError: 'float' object cannot be interpreted as an integer
4 [1, 'two', 3.0, [4]]
```

The `ctypes` array has four 4-byte integers, so it takes 16 bytes, and the items start at offsets 0, 4, 8 and 12 from the base address. The array has no fifth slot, so the write to index 4 fails. The `array` module packs numbers of one type and refuses a float. The Python list is different: it holds references to objects of any type, and it grows.

## How it works

An array has four properties.

- **One item type.** All items have the same type and the same size. Wikipedia gives the reason: the indices "can be computed at run time", and "For that reason, the elements of an array data structure are required to have the same size and should use the same data representation"[^1]. In Java, an array of `int` holds the values. An array of objects holds references, which all have the same size, so the objects can have different sizes.
- **Contiguous storage.** The items sit next to each other with no gaps between them. C defines an array as "a type consisting of a contiguously allocated nonempty sequence of objects with a particular element type"[^2]. The Go blog says that "The in-memory representation of [4]int is just four integer values laid out sequentially"[^4].
- **Integer indices.** The program names an item by its index. In most languages, the first index is 0, so an array of `n` items has the indices 0 to `n - 1`. Java says: "All arrays are 0-origin. An array with length n can be indexed by the integers 0 to n-1"[^3].
- **A fixed length.** The program gives the length when it makes the array. In Go and Rust, the length is part of the type. The Go blog says: "An array’s size is fixed; its length is part of its type ([4]int and [5]int are distinct, incompatible types)"[^4]. Rust writes the type as `[T; N]`, for "the element type, T, and the non-negative compile-time constant size, N"[^5]. In Java and C#, the length is part of the array object, not of its type, so one variable of type `int[]` can refer to arrays of different lengths[^3].

**Bounds.** An index outside 0 to `n - 1` names memory that is not part of the array. Most languages check each index when the program runs. Java says that "an attempt to use an index that is less than zero or greater than or equal to the length of the array causes an ArrayIndexOutOfBoundsException to be thrown"[^3]. Rust stops the program with a panic, and the Rust book explains: "In many low-level languages, this kind of check is not done, and when you provide an incorrect index, invalid memory can be accessed"[^6]. C and C++ do not check `a[i]`. An access outside the array is [undefined behaviour](/undefined-behavior/), and it is the cause of [out-of-bounds reads](/out-of-bounds-read/) and [out-of-bounds writes](/out-of-bounds-write/).

**Default values.** A new array must have some value in each slot. Java sets each component "to its default value"[^3], which is 0, `false` or `null`. Go makes "a ready-to-use array whose elements are themselves zeroed"[^4]. C# says that the "elements in an array are set to the default value for that type"[^7]. C leaves a local array without an initializer with unknown values, but an initializer that gives some items sets the other items to zero: `int a[5] = {1,2,3}` "declares int[5] initialized to 1,2,3,0,0"[^2].

**Values and references.** Languages differ in what assignment does with an array.

- In C, an array is not a value that the program can assign. "Objects of array type are not modifiable lvalues, and although their address may be taken, they cannot appear on the left hand side of an assignment operator"[^2]. In most expressions, an array converts to a pointer to its first item. The C rules say that an array expression "undergoes an implicit conversion to the pointer to its first element"[^2], so a function that takes an array gets a pointer and does not know the length. See [pointers and references](/pointers-and-references/).
- In Go and Rust, an array is a value. The Go blog says: "Go’s arrays are values. An array variable denotes the entire array; it is not a pointer to the first array element (as would be the case in C)"[^4]. Assignment and function calls copy all the items. C++ `std::array` also copies, because it has "the same semantics as a struct holding a C-style array T[N] as its only non-static data member"[^8].
- In Java, C#, JavaScript and Python, an array is an object on the heap, and a variable holds a reference to it. Java says: "In the Java programming language, arrays are objects"[^3], and C# says: "An array is a reference type"[^7]. Assignment copies the reference, so two variables can name one array. See [value equality and identity](/value-equality-and-identity/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | no fixed-size array in the core language; `array.array` for numbers of one C type and `ctypes` arrays; the built-in `list` is a dynamic array of references |
| JavaScript | native | `Array`, which can grow and can have holes; typed arrays such as `Int32Array` with a fixed length and one numeric type |
| TypeScript | native | the JavaScript arrays, with the types `number[]` or `Array<number>` and tuple types for a fixed length |
| Java | native | `int[]` and `String[]`; objects with a fixed `length`, checked indices and default values |
| Kotlin | native | the class `Array<T>` and the primitive arrays `IntArray`, `DoubleArray` and others; fixed length |
| C# | native | `int[]`, reference types derived from `System.Array`; single-dimensional, multidimensional and jagged arrays |
| Go | native | `[N]T`, a value whose length is part of its type; most code uses slices of arrays |
| Rust | native | `[T; N]`, a value whose length is part of its type; it coerces to the slice `&[T]` |
| C | native | `T a[N]`; no index checks; the array converts to a pointer in most expressions |
| C++ | native | C arrays and `std::array<T, N>`, which copies, compares with `==` and has a checked `at` |
| Bash | native | indexed arrays `a=(x y z)`, which can have gaps and grow; they are not contiguous storage |
| PowerShell | native | `@(1, 2, 3)`, a .NET `System.Object[]` with a fixed size; `+=` makes a new array |
| SQL | native | array column types in some databases, such as PostgreSQL `integer[]`, with one-based indices |
| Regex | n/a | a pattern matches text and has no arrays |
| HTML | n/a | markup has no arrays |
| CSS | n/a | style rules have no arrays |

Most languages have an array type in the core language, but the word array does not mean the same thing in each one. In C, Go, Rust and C++ `std::array`, an array is a fixed number of items stored inline. In Java, C# and Kotlin, an array is a fixed-size object on the heap. The C# reference says that the number of dimensions and the length of each dimension are set when the program makes the array, and "You can't change these values during the lifetime of the instance"[^7]. In JavaScript, Python, Bash and PowerShell, the usual array type is a dynamic collection. MDN says: "JavaScript arrays are resizable and can contain a mix of different data types. (When those characteristics are undesirable, use typed arrays instead.)"[^9]. A typed array is the fixed-size form: it "describes an array-like view of an underlying binary data buffer"[^10]. Python has the `array` module, which "can compactly represent an array of basic values"[^11], and the built-in list, which is a mutable sequence[^12]. TypeScript adds a type for the items. For an array such as `[1, 2, 3]`, "you can use the syntax number[]; this syntax works for any type"[^13].

Kotlin gives advice that applies in many languages: "Use arrays for interoperability with Java APIs or low-level requirements", and "For most use cases, prefer collections instead"[^14]. The Rust book says: "If you’re unsure whether to use an array or a vector, chances are you should use a vector"[^6]. The Go blog says: "Arrays have their place, but they’re a bit inflexible, so you don’t see them too often in Go code. Slices, though, are everywhere"[^4].

Shell and SQL arrays are not arrays in the data-structure sense. Bash says: "There is no maximum limit on the size of an array, nor any requirement that members be indexed or assigned contiguously"[^15]. PostgreSQL "allows columns of a table to be defined as variable-length multidimensional arrays"[^16], and it uses one-based indices by default[^16]. PowerShell arrays are .NET arrays with a fixed size. When a script adds an item with `+=`, "PowerShell actually creates a new array with the values of the original array and the added value"[^17].

The example files show these forms. Python shows a list as the usual array, negative indices, aliasing and the `array` module with a type code. JavaScript shows a growing `Array`, a hole, `Array.from` with a length and typed arrays that ignore an index out of range and keep only the low bits of a value. Java shows default values, the bounds exception, `clone`, `Arrays.equals` and the store check of covariant arrays. C# shows the same rules, `Array.Resize` and a collection expression. Go shows an array as a value that assignment and calls copy. Rust shows `[0; 5]`, copying, the coercion to a slice and `get`. C shows `sizeof`, address arithmetic, the conversion to a pointer and a struct that copies an array. C++ shows `std::array` with copies, `==`, `at` and `std::to_array`.

## Variations and related ideas

- **Dynamic arrays.** A dynamic array keeps a fixed array and a count of the items in use. When the array is full, it makes a bigger array and copies the items. Wikipedia says: "If this operation is done infrequently, insertions at the end of the array require only amortized constant time"[^1]. Python `list`, Java `ArrayList`, C# `List<T>`, C++ `std::vector`, Rust `Vec` and Go slices are dynamic arrays. See [dynamic array](/dynamic-array/) and [list](/list/).
- **Slices and views.** A [slice](/slice-concept/) is a view of a part of an array: a pointer, a length and in Go a capacity. Rust says: "Arrays coerce to slices ([T]), so a slice method may be called on an array. Indeed, this provides most of the API for working with arrays"[^5].
- **Multidimensional arrays.** A grid can be one block of memory with a formula for two indices, or an array of arrays, which C# calls a jagged array[^7].
- **Arrays of bytes.** A byte array holds raw data, such as the content of a file. JavaScript typed arrays share one buffer of bytes in different numeric types[^10]. A string is a sequence of characters, and in many languages it is stored in an array of bytes or code units. See [string](/string-concept/).
- **Associative arrays.** Some languages use the word array for a map from keys to values, such as PHP arrays and Bash associative arrays[^15]. Wikipedia notes that the word can mean an "associative array" or "abstract array"[^1]. MDN warns that "JavaScript arrays are not associative arrays"[^9].
- **Array programming.** Languages such as APL and libraries such as NumPy apply an operation to a whole array at once. See [array-oriented programming](/array-oriented-programming/).

## Pitfalls

- **An index off by one.** The last item of an array of length `n` has the index `n - 1`. A loop with `i <= n` reads one item too many. In C, this read is undefined behaviour and the program can read or change other data.
- **The length of an array in a C function.** `sizeof a / sizeof a[0]` gives the number of items only where `a` is declared. In a function parameter, `int a[]` means `int *a`, so `sizeof a` is the size of a pointer. Pass the length as a separate parameter.
- **Aliasing.** In Java, C#, JavaScript and Python, `b = a` makes a second name for the same array. A change through `b` is visible through `a`. MDN says that "JavaScript array-copy operations create shallow copies"[^9]. Use `clone`, `copy`, spread syntax or `Arrays.copyOf` to make a new array.
- **Copies that the program does not expect.** In Go and Rust, an array is a value. A function that changes its array parameter changes a copy, and a large array is copied at each call. Pass a slice or a pointer.
- **Comparing arrays.** In Java and C#, `==` compares references, so two arrays with the same items are not equal under `==`. Use `Arrays.equals` in Java or `SequenceEqual` in C#. In C, `a == b` compares two pointers. Go, Rust and C++ `std::array` compare the items.
- **Covariant arrays.** In Java and C#, a `String[]` is also an `Object[]`, so the compiler accepts a store of an `Integer` into it. The check runs when the program runs. Java says that "an assignment to a component of the array is checked at run time"[^3], and a wrong store throws `ArrayStoreException`. C# throws `ArrayTypeMismatchException`.
- **Growing an array one item at a time.** A plain array cannot grow. `Array.Resize` in C# and `+=` in PowerShell make a new array and copy all the items, so a loop that adds `n` items this way does about `n * n / 2` copies. Use a dynamic array.
- **Silent conversion in typed arrays.** A JavaScript typed array converts each value to its item type. An `Int8Array` stores 200 as -56, and a write to an index out of range does nothing.

## Cost

Reading or writing item `i` takes constant time. Wikipedia says: "Both store and select take (deterministic worst case) constant time. Arrays take linear (O(n)) space in the number of elements n that they hold"[^1]. An array has no space for links or keys, so it uses less memory than a linked list or a tree with the same items. It cannot insert or remove an item, because its length is fixed. To insert in the middle, a program must make a new array or move the items after the position, which takes time in proportion to the number of items that move.

Contiguous storage also makes a scan fast. Wikipedia explains: "iterating through an array of n elements requires the minimum of ceiling(nk/B) cache misses, because its elements occupy contiguous memory locations", where `k` is the item size and `B` is the size of a cache line. It adds: "As a consequence, sequential iteration over an array is noticeably faster in practice than iteration over many other data structures, a property called locality of reference"[^1]. An array of references, such as a Python list or a Java `Object[]`, has this property for the references but not for the objects that they point to.

## History

Arrays are older than programming languages. Wikipedia says: "The first digital computers used machine-language programming to set up and access array structures for data tables, vector and matrix computations, and for many other purposes. John von Neumann wrote the first array-sorting program (merge sort) in 1945, during the building of the first stored-program computer"[^1]. It adds: "Array indexing was originally done by self-modifying code, and later using index registers and indirect addressing"[^1].

Wikipedia also gives the first languages with arrays: "FORTRAN (1957), Lisp (1958), COBOL (1960), and ALGOL 60 (1960), had support for multi-dimensional arrays, and so has C (1972)"[^1]. FORTRAN 77 started indices at 1, and Pascal and ALGOL let the program choose the lower bound[^1]. C made an array a block of memory that converts to a pointer[^2]. Later languages kept the contiguous layout but added checks: Java checks every index and every store into an array of references[^3], and Go and Rust put the length in the type[^18][^5]. JavaScript added typed arrays for binary data, and MDN says that they have been "available across browsers since July 2015"[^10], and Python has the `array` module for packed numbers[^11].

## References

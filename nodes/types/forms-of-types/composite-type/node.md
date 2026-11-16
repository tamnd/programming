---
tag: E4HT1G
kind: concept
slug: composite-type
name: Composite type
aliases: [compound type, compound data type, composite data type, aggregate type, derived type, structured type, structured data type, data structure type]
area: types/forms-of-types
level: intro
summary: >-
  A composite type builds a value from values of other types: arrays, records, tuples, maps, unions, functions and pointers. A type constructor such as list[int] makes it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1959
  people: []
  note: COBOL, whose design started in 1959, was the first widespread language with record types. FORTRAN and ALGOL 60 had no records. Pascal joined records with the other types into one type system, and C has records as structs.
see_also: [type, array, record-type, tuple, union-type, sum-types, map-and-dictionary, recursive-type]
same_as:
  wikidata: Q1413465
  wikipedia: Composite_data_type
sources:
- url: https://en.wikipedia.org/wiki/Composite_data_type
  title: Composite data type, Wikipedia
  locator: Introduction
- url: https://en.cppreference.com/w/c/language/type.html
  title: Type, C reference, cppreference.com
  locator: Type classification; Type groups
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Types; Array types; Struct types; Map types; Function types; Comparison operators
- url: https://www.typescriptlang.org/docs/handbook/2/objects.html
  title: Object Types, The TypeScript Handbook
  locator: Tuple Types
- url: https://en.cppreference.com/w/c/language/union.html
  title: Union declaration, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/type.html
  title: Fundamental and compound types, C++ reference, cppreference.com
  locator: Type classification
- url: https://doc.rust-lang.org/reference/types.html
  title: Types, The Rust Reference
  locator: Introduction; Type kinds
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: 4.3 Reference Types and Values; 4.3.1 Objects
- url: https://en.wikipedia.org/wiki/Type_constructor
  title: Type constructor, Wikipedia
  locator: Introduction
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: Built-in Types, Python 3.14 documentation
  locator: Common Sequence Operations, note 2; Generic Alias Type
- url: https://en.cppreference.com/w/c/language/struct.html
  title: Struct declaration, C reference, cppreference.com
  locator: Explanation
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Equality
  title: Equality (==), MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
  title: Java SE 25 API, Class Arrays
  locator: equals(int[], int[])
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-tuples
  title: Tuple types, C# language reference
  locator: Introduction
- url: https://docs.python.org/3.14/library/dataclasses.html
  title: dataclasses, Python 3.14 documentation
  locator: "@dataclass, the eq parameter"
- url: https://en.wikipedia.org/wiki/Record_(computer_science)
  title: Record (computer science), Wikipedia
  locator: History
- url: https://en.wikipedia.org/wiki/COBOL
  title: COBOL, Wikipedia
  locator: Introduction
---

## Idea

A program seldom works with one number at a time. A point has two coordinates, a person has a name and an age, and a class has a list of students. A composite type puts values of other types together into one value. The parts can have the same type, as in an array of numbers, or different types, as in a record with a name and an age. A part can be composite too, so a list of records that hold lists is still one type.

```python run id=idea
from dataclasses import dataclass

# Each value below is built from values of other types.
point = (3, 4)               # a tuple: two ints, found by position
scores = [90, 85, 77]        # a list: many ints, found by index
ages = {"ann": 31, "bob": 27}  # a dict: str keys that map to int values


@dataclass
class Person:                # a record: named fields of different types
    name: str
    age: int
    tags: list[str]


p = Person("Ann", 31, ["admin"])
print(type(point).__name__, type(scores).__name__, type(ages).__name__, type(p).__name__)
print(point[1], scores[0], ages["bob"], p.name)

# A part can be composite too.
print(p.tags[0], len(p.tags))

# list[str] is a type that a type constructor builds from str.
t = list[str]
print(t, t.__origin__.__name__, t.__args__)

# The record compares part by part.
print(p == Person("Ann", 31, ["admin"]))
```

```output for=idea
tuple list dict Person
4 90 27 Ann
admin 1
list[str] list (<class 'str'>,)
True
```

The tuple, the list, the dict and the record each hold several values, and the program gets one part by a position, an index, a key or a field name. The field `tags` of the record is a list, so a composite value can contain other composite values. The expression `list[str]` is a type, not a value: it is the type constructor `list` applied to the type `str`. The last line shows that the generated `==` of a dataclass compares the fields one by one.

## How it works

**Built from other types.** "In computer science, a composite data type or compound data type is a data type that consists of programming language scalar data types and other composite types that may be heterogeneous and hierarchical in nature"[^1]. A scalar type, such as an integer, a character or a Boolean, has values that the language does not split into parts. C names the two groups: "scalar types: arithmetic types, pointer types, and nullptr_t" and "aggregate types: array types and structure types"[^2].

**The main forms.** Most languages have a few forms, and they differ in how a program finds a part:

- An array: "An array is a numbered sequence of elements of a single type, called the element type"[^3]. The program finds an element by an index. See [array](/array/).
- A record or struct: "A struct is a sequence of named elements, called fields, each of which has a name and a type"[^3]. The program finds a field by its name. See [record type](/record-type/).
- A tuple: a record whose parts have positions and no names. In TypeScript, "A tuple type is another sort of Array type that knows exactly how many elements it contains, and exactly which types it contains at specific positions"[^4]. See [tuple](/tuple/).
- A map: "A map is an unordered group of elements of one type, called the element type, indexed by a set of unique keys of another type, called the key type"[^3]. See [map and dictionary](/map-and-dictionary/).
- A union: a value that holds one of several types. In C, "The value of at most one of the members can be stored in a union at any one time"[^5]. A tagged union or a [sum type](/sum-types/) also records which member it holds. See [union type](/union-type/).
- A function type and a pointer type: they are built from other types too. In Go, "A function type denotes the set of all functions with the same parameter and result types"[^3]. See [pointers and references](/pointers-and-references/).

**Each language draws its own list.** Go says that its composite types, which are array, struct, pointer, function, interface, slice, map and channel types, "may be constructed using type literals"[^3]. C has the derived types: array, structure, union, function, pointer and atomic types[^2]. C++ calls them compound types and lists reference, pointer, array, function, enumeration, class and union types among them[^6]. The Rust Reference lists sequence types (tuple, array and slice), user-defined types (struct, enum and union), function types and pointer types[^7]. Java divides its types into "two kinds: primitive types and reference types", and "There are four kinds of reference types": class types, interface types, type variables and array types[^8]. In Java, every composite value is an object: "An object is a class instance or an array", and "The reference values (often just references) are pointers to these objects"[^8].

**Type constructors.** "In the area of mathematical logic and computer science known as type theory, a type constructor is a feature of a typed formal language that builds new types from old ones"[^9]. "Some type constructors take another type as an argument, e.g., the constructors for product types, function types, power types and list types"[^9]. In code, `[]int` and `map[string]int` in Go, `Vec<i32>` in Rust, `int[]` in Java and `list[int]` in Python apply a type constructor to a type. In Python, such type objects "are generally created by subscripting a class"[^10]. Because the result is a type, it can be an argument again, as in `list[tuple[str, int]]`.

**Layout in memory.** In C, a struct "is a type consisting of a sequence of members whose storage is allocated in an ordered sequence", and a union is "a type consisting of a sequence of members whose storage overlaps"[^11]. "The size of a struct is at least as large as the sum of the sizes of its members"[^11], because the compiler can add padding, and "The union is only as big as necessary to hold its largest member"[^5]. A type cannot contain itself directly: in C, "a struct cannot have a member of its own type. A pointer to its own type is allowed, and is commonly used to implement nodes in linked lists or trees"[^11]. See [recursive type](/recursive-type/).

**Copy and equality.** A composite value that the language stores inline, such as a C struct, a Go array or a C# struct, is copied as a whole on assignment. A composite value behind a reference, such as a Java array or a JavaScript object, is shared, and a change through one name shows through the other. Equality follows the same split. In Go, "Struct types are comparable if all their field types are comparable", but "Slice, map, and function types are not comparable"[^3]. In JavaScript, `==` on two objects is true "only if both operands reference the same object"[^12]. See [value equality and identity](/value-equality-and-identity/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `tuple`, `list`, `dict` and `set`; classes and `@dataclass` for records; `list[int]` as a type |
| JavaScript | native | objects and arrays; `Map` and `Set`; no separate tuple or record type |
| TypeScript | native | object types, array types such as `number[]`, tuple types such as `[string, number]`, union types |
| Java | native | arrays, classes and records; each composite value is an object behind a reference |
| Kotlin | native | classes, `data class`, `Array<T>`, `Pair` and `Triple`, and the collection interfaces |
| C# | native | arrays, `class`, `struct`, `record` and tuples such as `(int, string)` |
| Go | native | arrays, slices, structs, maps, pointers, functions, interfaces and channels |
| Rust | native | tuples, arrays, slices, `struct`, `enum` and `union`; references and `Box<T>` |
| C | native | arrays, `struct`, `union`, pointers and function types |
| C++ | native | arrays, classes, `union`, references and pointers; `std::pair`, `std::tuple` and `std::array` |
| Bash | idiom | indexed and associative arrays of strings; an array cannot hold another array |
| PowerShell | native | arrays, hashtables and `[pscustomobject]`; all .NET types |
| SQL | idiom | a table row is a record of column values; some databases add composite column types |
| Regex | n/a | a pattern has groups, but no values and no types |
| HTML | n/a | elements nest inside elements, but HTML has no types of values |
| CSS | idiom | a shorthand property such as `border` takes a value built from values of other types |

Every general-purpose language has arrays and some form of record, so the verdict is native for all of them except Bash, where an array holds only strings. The languages differ in which forms they build in, in whether a composite value is stored inline or behind a reference, and in what `==` means for it.

The example files show these differences. Python shows that `*` on a list of lists repeats one inner list, that tuples compare part by part, and that a frozen dataclass rejects a change. JavaScript shows that `===` compares objects by reference, that a spread copy shares the inner array, and that `structuredClone` makes a deep copy. Java compares arrays with `==` and `Arrays.equals` and shows that a record compares by value. C# shows that tuples and structs are copied and classes are shared. Go copies arrays and structs, compares them with `==`, and shares the storage of a slice. Rust builds a tuple, an array, a struct, an enum with data and a recursive list with `Box`. C shows padding, the size of a union, the copy of a struct and a linked list through a pointer. C++ uses `std::pair`, `std::tuple`, `std::array` and a struct with a defaulted `==`.

## Variations and related ideas

- **Product and sum types.** A record or a tuple holds all its parts at once, and a sum type holds one of its variants. Together they are the [algebraic data types](/algebraic-data-type/). See [product type](/product-type/) and [sum types](/sum-types/).
- **Collections.** A list, a set and a map hold any number of items of one type. A record or a tuple holds a fixed number of parts, often of different types.
- **Recursive types.** A list node that points to the next node, or a tree whose children are trees, is a composite type that refers to itself through a pointer or a reference. See [recursive type](/recursive-type/).
- **Generic types.** `List<T>` in Java and `Vec<T>` in Rust are type constructors that the program applies to many element types. See [parametric polymorphism](/parametric-polymorphism/).
- **Named and anonymous types.** A Go type literal such as `struct { X, Y int }` or a TypeScript object type has no name of its own. A `class` or a `record` declaration gives the type a name.

## Pitfalls

- **A copy that shares its parts.** In Python, `[[0] * 3] * 3` makes one inner list and three references to it, because "items in the sequence s are not copied; they are referenced multiple times"[^10]. A change to `grid[0][0]` changes every row. A spread copy in JavaScript and `copy.copy` in Python are shallow in the same way.
- **`==` compares references.** In Java, `a == b` on two arrays is true only for the same array. `Arrays.equals` compares the contents: "Two arrays are considered equal if both arrays contain the same number of elements, and all corresponding pairs of elements in the two arrays are equal"[^13]. In JavaScript, there is no built-in deep equality for objects. In Go, `==` on two slices does not compile.
- **A value type that is copied.** C# says that "Tuple types are value types; tuple elements are public fields. That design makes tuples mutable value types"[^14]. A change to a copy of a tuple or a struct does not change the original. That is correct, but it surprises a programmer who expects a shared object.
- **Equality of records that are not value records.** A Python class compares by identity unless it defines `__eq__`. A dataclass adds one that "compares the class by comparing each field in order"[^15]. A Java record and a C# record do the same, but a plain class does not.
- **Padding and layout.** `sizeof` of a C struct can be more than the sum of its members[^11], so code that writes a struct to a file or a network byte by byte depends on the compiler. Write each field instead.
- **The wrong member of a union.** A C union holds one member at a time[^5]. Reading another member gives the bytes of the last one written. Keep a tag next to the union, or use a sum type.

## Cost

An array or a struct of value types keeps its parts next to each other in one block of memory, and the program finds a part at an offset that the compiler knows. In Java, JavaScript and Python, a composite value is an object behind a reference, and each part that is an object is another reference, so a list of points is a list of pointers to separate objects. That costs an allocation for each object and one more memory read for each part. A copy of a large value type copies all its bytes, so code passes a large struct by pointer or by reference. A union costs only the size of its largest member[^5], and padding can make a struct larger than its members[^11].

## History

Records came from data processing. According to Wikipedia, "COBOL was the first widespread programming language to support record types"[^16], and "COBOL's design was started in 1959 by CODASYL"[^17]. "The early languages developed for numeric computing, such as FORTRAN (up to FORTRAN IV) and ALGOL 60, did not support record types"[^16]. Later, "The Pascal programming language was one of the first languages to fully integrate record types with other basic types into a logically consistent type system"[^16], and "The C language provides the record concept using structs"[^16]. Records as a short form for data classes came much later: "Java introduced records in Java 17"[^16].

## References

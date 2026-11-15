---
tag: S5GV9J
kind: concept
slug: recursive-type
name: Recursive type
aliases: [recursive data type, inductive data type, self-referential type, recursively defined type, recursive type alias]
area: types/algebraic-data-types
level: core
summary: >-
  A recursive type refers to itself in its own definition, as a list node refers to the next node. Lists, trees and expression trees are recursive types.
applies_to: core-languages
viz: recommended
formal: optional
see_also: [algebraic-data-type, sum-types, product-type, recursion, type, pattern-matching]
same_as:
  wikidata: Q3098441
  wikipedia: Recursive_data_type
sources:
  - url: https://en.wikipedia.org/wiki/Recursive_data_type
    title: Recursive data type, Wikipedia
    locator: the introduction; Theory; Isorecursive types; Equirecursive types
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch15-01-box.html
    title: The Rust Programming Language, 15.1 Using Box<T> to Point to Data on the Heap
    locator: Enabling Recursive Types with Boxes
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.2.3
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.7.2.3 Tags, EXAMPLE 1
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification
    locator: Struct types
    accessed: 2026-11-16
  - url: https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/Enumerations.md
    title: The Swift Programming Language, Enumerations
    locator: Recursive Enumerations
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-3-7.html#more-recursive-type-aliases
    title: TypeScript 3.7 release notes
    locator: More Recursive Type Aliases
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/typing.html#type-aliases
    title: The Python Standard Library, typing
    locator: Type aliases
    accessed: 2026-11-16
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html
    title: Haskell 2010 Language Report, Chapter 4 Declarations and Bindings
    locator: 4.2.1 Algebraic Datatype Declarations
    accessed: 2026-11-16
---

## Idea

Some data contains smaller data of the same kind. A list is empty, or it is a first item followed by a shorter list. A tree is a value with a list of smaller trees. A type that describes such data must mention itself. Wikipedia says that "a recursive data type is a data type whose definition contains values of the same type".[^1]

```python run id=idea
from dataclasses import dataclass


@dataclass
class Tree:
    value: int
    children: list["Tree"]


def total(t: Tree) -> int:
    return t.value + sum(total(c) for c in t.children)


def depth(t: Tree) -> int:
    return 1 + max((depth(c) for c in t.children), default=0)


tree = Tree(1, [Tree(2, [Tree(4, [])]), Tree(3, [])])
print("total:", total(tree))
print("depth:", depth(tree))
```

```output for=idea
total: 10
depth: 3
```

The type `Tree` has a field whose type is a list of `Tree`. The functions that walk the data follow the shape of the type: a function on a tree calls itself on each child (see [[recursion]]). The recursion stops at a tree with no children.

## How it works

**A base case and a recursive case.** A useful recursive type has at least one case that does not contain the type, or the values could never end. A list has the empty list. The natural numbers of Peano arithmetic have zero: in Haskell, `data Nat = Zero | Succ Nat`.[^1] So most recursive types are sum types with a base case and one or more recursive cases (see [[sum-types]]).

**Indirection.** A value cannot contain a full copy of a value of its own type, because its size would be infinite. The Rust book explains: "Rust needs to know at compile time how much space a type takes up. However, the nesting of values of recursive types could theoretically continue infinitely".[^2] So the recursive field is a pointer or a reference:

- In C, a struct holds a pointer to its own type. The C standard shows `struct tnode { int count; struct tnode *left, *right; };`, "a structure that contains an integer and two pointers to objects of the same type".[^3]
- Go forbids a struct that contains itself directly: "A struct type T may not contain a field of type T", and the same holds through arrays and other structs.[^4] A pointer field, such as `next *Node`, is allowed.
- Rust uses `Box<T>`: "Because boxes have a known size, we can enable recursive types by inserting a box in the recursive type definition."[^2]
- Swift marks a recursive enum case with `indirect`, "which tells the compiler to insert the necessary layer of indirection".[^5]
- In Java, C#, Python, JavaScript and most functional languages, a field of a class type already holds a reference, so a type can refer to itself with no extra syntax.

**Recursive type aliases.** A type alias can also refer to itself, when the reference is inside another type. TypeScript 3.7 relaxed its rules so that aliases such as `type Json = string | number | boolean | null | { [property: string]: Json } | Json[]` are allowed.[^6] Python added the `type` statement for type aliases: "The type statement is new in Python 3.12."[^7] The docs note that it "natively supports forward references", so an alias can use its own name, as in `type Tree = int | list[Tree]`.[^7]

**Theory.** "In type theory, a recursive type has the general form μα.T where the type variable α may appear in the type T and stands for the entire type itself."[^1] For example, the natural numbers are `μα. 1 + α`: a value is zero (the unit) or the successor of another natural number. There are "two forms of recursive types: the so-called isorecursive types, and equirecursive types".[^1] With isorecursive types, the program moves between the type and its one-step unrolling with explicit operations. With equirecursive types, the two are the same type. Wikipedia notes that "equirecursive types contribute significantly more complexity to a type system than isorecursive types do".[^1] Wikipedia notes that "In functional programming languages, isorecursive types (in the guise of datatypes) are common too."[^1] There, the constructors do the rolling and the pattern matches do the unrolling.

## In languages

The example files define a linked list of integers, build the list 1, 2, 3, and walk it with a recursive function.

| Language | Verdict | Form |
|---|---|---|
| Python | native | a class whose field refers to the class, written as a string or with deferred annotations; a `type` alias[^7] |
| JavaScript | idiom | objects whose properties refer to other objects; no types to declare |
| TypeScript | native | a recursive interface or type alias[^6] |
| Java | native | a class or record whose field has its own type |
| C | native | a struct with a pointer to its own type[^3] |
| C++ | native | a class with a pointer or a `std::unique_ptr` to its own type |
| C# | native | a class or record whose field has its own type |
| Go | native | a struct with a pointer to its own type[^4] |
| Rust | native | an `enum` or a `struct` with a `Box`, `Rc` or reference to its own type[^2] |
| Kotlin | native | a class or a sealed interface whose cases refer to it |
| PowerShell | idiom | objects whose properties refer to other objects |
| Bash | n/a | Bash has no types that can refer to themselves |
| SQL | idiom | a table with a foreign key to itself, and a recursive common table expression to walk it |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup has no types; an HTML document is a tree of elements |

**Haskell and ML.** A data declaration can refer to the type that it declares, as in `data List a = Nil | Cons a (List a)`. The Haskell report defines data declarations in section 4.2.1.[^8] Two types can also refer to each other: Wikipedia shows `data Tree a = Empty | Node (a, Forest a)` with `data Forest a = Nil | Cons (Tree a) (Forest a)`.[^1] In Standard ML, the keyword `and` joins the two definitions.[^1]

## Variations and related ideas

- **Mutually recursive types.** Two or more types that refer to each other, such as a tree and a forest.[^1]
- **Generic recursive types.** A list of any element type, such as `List<T>`. See [[parametric-polymorphism]].
- **Recursive types with cycles.** References can make a structure point back to itself, as in a doubly linked list or a graph. The type is the same, but a walk must remember the nodes that it has seen.
- **Infinite structures.** With lazy evaluation, a recursive value can be infinite, such as the list of all natural numbers. See [[lazy-evaluation]].
- **Folds.** A function that replaces each constructor of a recursive type with a function, such as `sum` on a list. See [[fold]].

## Pitfalls

### A recursive type without indirection

In Rust, `enum List { Cons(i32, List), Nil }` does not compile. The compiler reports "recursive type `List` has infinite size" and suggests "insert some indirection (e.g., a `Box`, `Rc`, or `&`) to break the cycle".[^2] C and Go reject the same definition. Put the recursive field behind a pointer, a box or a reference.

### Deep data overflows the stack

A recursive function on a recursive type uses one stack frame for each level. A list of a hundred thousand nodes is a hundred thousand levels deep. Python stops such a call at its recursion limit.

```python wrong id=deep
from dataclasses import dataclass


@dataclass
class Node:
    value: int
    next: "Node | None"


def length(node: Node | None) -> int:
    return 0 if node is None else 1 + length(node.next)


head = None
for i in range(100_000):
    head = Node(i, head)

try:
    print(length(head))
except RecursionError:
    print("RecursionError")
```

```output for=deep
RecursionError
```

The fix is a loop for long, thin structures such as lists: `while node is not None: count += 1; node = node.next`. In languages without such a limit, the same code can crash the process. Trees that stay balanced are less of a risk, because their depth grows with the logarithm of their size.

### A cycle makes a walk run forever

When nodes refer to each other through mutable references, the data can contain a cycle. A function that follows the references until it reaches a base case then never stops. Code that can meet shared or cyclic data must keep a set of the nodes that it has visited.

## Cost

Each recursive field is a pointer, so each node costs at least one pointer per recursive field plus the allocation overhead of a separate object. The nodes are spread over the heap, so a walk has worse cache behavior than a loop over an array. A recursive walk uses stack space in proportion to the depth of the data.

## History

The C standard shows a self-referential structure, `struct tnode`, as an example.[^3] Standard ML and Haskell define recursive data types with constructors, as in the mutually recursive tree and forest types that Wikipedia shows for both languages.[^1][^8] Later languages added more direct forms: `indirect` enum cases in Swift, recursive type aliases in TypeScript 3.7, and the `type` statement in Python 3.12.[^5][^6][^7]

## References

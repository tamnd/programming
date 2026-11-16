---
tag: F4T2ZQ
kind: concept
slug: object-cloning
name: Object cloning
aliases: [shallow copy, deep copy, copy constructor, Clone trait, Cloneable, deepcopy, structuredClone, MemberwiseClone, object copying]
area: objects-and-classes/standard-object-methods
level: core
summary: Object cloning makes a new object with the same state as an existing one. A shallow copy shares the objects that the fields refer to, and a deep copy copies them too.
applies_to: core-languages
viz: optional
history:
  year: 1980
  note: The terms shallow copy and deep copy come from Smalltalk-80, which had the methods shallowCopy and deepCopy; Java, C#, Python and Rust later gave the two copies their own methods and traits.
see_also: [copy-an-object, universal-object-methods, immutability, value-equality-and-identity, record-type, data-classes]
sources:
  - url: https://docs.python.org/3/library/copy.html
    title: copy, Shallow and deep copy operations, Python standard library
    locator: Introduction
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Shallow_copy
    title: Shallow copy, MDN Glossary
    locator: Definition
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Deep_copy
    title: Deep copy, MDN Glossary
    locator: Definition
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Object (Java SE 21 & JDK 21), API documentation
    locator: clone
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Cloneable.html
    title: Cloneable (Java SE 21 & JDK 21), API documentation
    locator: Description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.memberwiseclone
    title: Object.MemberwiseClone Method, .NET API reference
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.icloneable
    title: ICloneable Interface, .NET API reference
    locator: Remarks
  - url: https://doc.rust-lang.org/std/clone/trait.Clone.html
    title: Clone trait, Rust standard library
    locator: Description
  - url: https://doc.rust-lang.org/std/marker/trait.Copy.html
    title: Copy trait, Rust standard library
    locator: Description; What's the difference between Copy and Clone?
  - url: https://en.cppreference.com/w/cpp/language/copy_constructor
    title: Copy constructors, cppreference.com
    locator: Introduction; Implicitly-defined copy constructor
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Slice types
  - url: https://pkg.go.dev/slices
    title: slices package, Go standard library
    locator: func Clone
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Structured_clone_algorithm
    title: The structured clone algorithm, MDN
    locator: Things that don't work with structured clone
  - url: https://kotlinlang.org/docs/data-classes.html
    title: Data classes, Kotlin documentation
    locator: Copying
  - url: https://en.wikipedia.org/wiki/Object_copying
    title: Object copying, Wikipedia
    locator: Methods of copying; Lazy copy
---

## Idea

An object often holds references to other objects: a team holds a list of members, and a document holds a list of pages. To copy such an object, a program must choose what to do with the objects that the fields refer to. There are three levels:

- **A new reference.** Assignment in Python, Java, JavaScript and C# copies only the reference, so both names refer to one object. In Python, "Assignment statements in Python do not copy objects, they create bindings between a target and an object."[^1]
- **A shallow copy.** A new object gets the same field values. Its fields refer to the same inner objects as the original. MDN: "A shallow copy of an object is a copy whose properties share the same references (point to the same underlying values) as those of the source object from which the copy was made."[^2]
- **A deep copy.** A new object gets copies of the inner objects too, at each level. MDN: "A deep copy of an object is a copy whose properties do not share the same references (point to the same underlying values) as those of the source object from which the copy was made."[^3]

The difference matters only for an object that refers to mutable objects. For a value that has only numbers and strings, or only immutable parts, a shallow copy and a deep copy behave the same.

## How it works

Python's `copy` module has both copies. A shallow copy "constructs a new compound object and then (to the extent possible) inserts references into it to the objects found in the original", and a deep copy "constructs a new compound object and then, recursively, inserts copies into it of the objects found in the original".[^1] A list of lists shows the three levels:

```python run id=three-levels
import copy

grid = [[1, 2], [3, 4]]
alias = grid
shallow = copy.copy(grid)
deep = copy.deepcopy(grid)

grid[0][0] = 99
grid.append([5, 6])
print(alias)
print(shallow)
print(deep)
```

```output for=three-levels
[[99, 2], [3, 4], [5, 6]]
[[99, 2], [3, 4]]
[[1, 2], [3, 4]]
```

The alias sees both changes, because it is the same list. The shallow copy is a new outer list, so it does not get the new row, but its rows are the rows of `grid`, so it sees the change to `grid[0]`. The deep copy shares nothing.

A deep copy must walk a graph, not a tree. Python names the two problems: "Recursive objects (compound objects that, directly or indirectly, contain a reference to themselves) may cause a recursive loop", and "Because deep copy copies everything it may copy too much, such as data which is intended to be shared between copies."[^1] `deepcopy` keeps a memo of the objects that it copied, so an object that appears twice is copied once and a cycle ends. A class can control its copies with `__copy__` and `__deepcopy__`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `copy.copy` and `copy.deepcopy`; `__copy__` and `__deepcopy__` |
| JavaScript | native | spread syntax and `Object.assign` copy shallowly; `structuredClone` copies deeply |
| TypeScript | native | as JavaScript |
| Java | native | `Object.clone` with `Cloneable`, shallow by default; copy constructors |
| Kotlin | native | `copy()` of a data class, shallow |
| C# | native | `MemberwiseClone`, shallow; `ICloneable`; `with` on records |
| Go | idiom | assignment copies a struct; `slices.Clone` and `maps.Clone` copy one level |
| Rust | stdlib | trait `Clone`, explicit and usually deep; trait `Copy` for bitwise copies |
| C | idiom | assignment copies a struct; a function copies what the pointers point to |
| C++ | native | copy constructor and copy assignment, member-wise by default |
| Bash | n/a | variables hold strings; `b=("${a[@]}")` copies an array |
| PowerShell | native | .NET objects; `Clone()` on arrays and hash tables copies one level |
| SQL | n/a | no objects; `INSERT ... SELECT` copies rows |
| Regex | n/a | no objects |
| HTML | n/a | no objects; the DOM method `cloneNode` copies an element |
| CSS | n/a | no objects |

- **Java** puts `clone` on `Object` as a protected method. It makes a field-for-field copy, which "performs a "shallow copy" of this object, not a "deep copy" operation".[^4] A class must implement `Cloneable`, or `clone` throws `CloneNotSupportedException`, but "this interface does not contain the clone method".[^5]
- **C#** has the protected `MemberwiseClone`: "If a field is a reference type, the reference is copied but the referred object is not; therefore, the original object and its clone refer to the same object."[^6] The interface `ICloneable` "does not specify whether the cloning operation performs a deep copy, a shallow copy, or something in between".[^7]
- **Rust** makes copies explicit. `Clone` is "A common trait that allows explicit creation of a duplicate value", and `#[derive(Clone)]` clones each field, so a `Vec` field becomes a new vector.[^8] `Copy` is for "Types whose values can be duplicated simply by copying bits"; other types move on assignment.[^9]
- **C++** calls the copy constructor for `T b = a;`. The implicit one "performs full member-wise copy of the object's direct base class subobjects and member subobjects".[^10] A `std::vector` member copies its elements, so the copy is deep for values; a raw pointer or a `std::shared_ptr` member copies only the pointer.
- **Go** copies a struct on assignment, field by field. A slice field shares its array: "A slice therefore shares storage with its array and with other slices of the same array".[^11] `slices.Clone` makes a new array, but "The elements are copied using assignment, so this is a shallow clone."[^12]
- **JavaScript** spread syntax `{ ...obj }` is a shallow copy; "all standard built-in object-copy operations (spread syntax, Array.prototype.concat(), Array.prototype.slice(), Array.from(), and Object.assign()) create shallow copies".[^2] `structuredClone` makes a deep copy of plain data, but "The prototype chain is not walked or duplicated."[^13]
- **Kotlin** data classes have `copy()`, which can change some properties; "The copy() function creates a shallow copy of the instance."[^14]

## Variations and related ideas

- **Copy constructors and factories.** `new Team(other)` or `Team.copyOf(other)` is a normal method, so it can choose the depth for each field.
- **Copy with changes.** Kotlin's `copy(age = 20)`, C#'s `with` expression and Python's `dataclasses.replace` make a shallow copy and set some fields. They suit immutable types: see [data classes](/data-classes/).
- **Immutable objects.** An object that never changes can be shared without a copy. A deep copy of an immutable value is a waste. See [immutability](/immutability/).
- **Lazy copy.** A copy can share the data at first and copy it only when one side writes. Wikipedia calls this a lazy copy: "When initially copying an object, a (fast) shallow copy is used. A counter is also used to track how many objects share the data."[^15] This is [copy-on-write](/copy-on-write/).
- **Move.** Rust and C++ can move a value: the new owner takes the data, and the old name can no longer use it. A move costs as little as a shallow copy and avoids sharing.

## Pitfalls

- **A shallow copy where a deep copy was meant.** The copy and the original share the inner objects, so a change through one shows in the other:

```javascript run id=spread-is-shallow
const team = { name: "red", members: ["Ann", "Bo"] };
const copy = { ...team };
copy.name = "blue";
copy.members.push("Cy");
console.log(team.name, team.members);
```

```output for=spread-is-shallow
red [ 'Ann', 'Bo', 'Cy' ]
```

  The new name stays in the copy, but the new member is in both, because both refer to one array.
- **A deep copy of a shared resource.** A deep copy of an object that holds a file, a connection, a cache or a parent link copies too much. Python does not copy "module, method, stack trace, stack frame, file, socket, window, or any similar types".[^1] Copy the data and share the resource, or write the copy method by hand.
- **A derived clone of a shared pointer.** In Rust, `#[derive(Clone)]` on a struct that holds an `Rc` or an `Arc` clones the pointer, not the data: "the cloned struct will share mutable state with the original".[^8] The same is true for a C++ `shared_ptr` member.
- **A copy that loses the type.** `structuredClone` copies the fields of an object but not its class, so the copy of a class instance is a plain object without the methods. Functions cannot be copied at all: "attempting to throws a DataCloneError exception".[^13]
- **A `clone` contract that is not checked.** Java's `clone` should return an object that is "independent of this object (which is being cloned)", but `super.clone()` alone does not give that for a mutable field.[^4] The class must replace each such field with a copy.

## Cost

A shallow copy costs time and memory in proportion to the number of fields of the top object. A deep copy costs time and memory in proportion to the number of objects that are reachable from it, which can be the whole data structure. A deep copy that handles shared objects and cycles, such as `deepcopy` with its memo, also uses a table of the copied objects. Cloning a large structure in a loop is a common hidden cost; share immutable parts, or use copy-on-write.

## History

The terms come from Smalltalk-80: "The terminology of shallow copy and deep copy dates to Smalltalk-80."[^15] Its classes had the methods `shallowCopy` and `deepCopy`. Java put `clone` on `Object` with the marker interface `Cloneable`, and .NET added `MemberwiseClone` and `ICloneable`. Later designs made copies explicit and typed: Rust separates the cheap `Copy` from the explicit `Clone`, and records and data classes give a shallow copy with changes.

## References

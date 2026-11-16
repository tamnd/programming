---
tag: YFXM5G
kind: task
slug: copy-an-object
name: Copy an object
aliases: [shallow copy an object, deep copy an object, clone an object, copy a class instance, duplicate an object]
area: objects-and-classes/standard-object-methods
level: core
summary: Make a shallow copy and a deep copy of a Playlist object with a list field, then change the list through the original and show that the shallow copy sees the change and the deep copy does not.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [object-cloning, universal-object-methods, implement-equality-for-a-type, immutability, value-equality-and-identity]
sources:
  - url: https://docs.python.org/3/library/copy.html
    title: copy, Shallow and deep copy operations, Python standard library
    locator: Introduction; Interface summary
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Object (Java SE 21 & JDK 21), API documentation
    locator: clone
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.memberwiseclone
    title: Object.MemberwiseClone Method, .NET API reference
    locator: Remarks; Examples
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Slice types
  - url: https://pkg.go.dev/slices
    title: slices package, Go standard library
    locator: func Clone
  - url: https://doc.rust-lang.org/std/rc/struct.Rc.html
    title: Rc, Rust standard library
    locator: impl Clone for Rc
  - url: https://doc.rust-lang.org/std/clone/trait.Clone.html
    title: Clone trait, Rust standard library
    locator: Description
  - url: https://en.cppreference.com/w/cpp/language/copy_constructor
    title: Copy constructors, cppreference.com
    locator: Implicitly-defined copy constructor
  - url: https://kotlinlang.org/docs/data-classes.html
    title: Data classes, Kotlin documentation
    locator: Copying
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record
    title: Records, C# reference
    locator: Nondestructive mutation
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Structured_clone_algorithm
    title: The structured clone algorithm, MDN
    locator: Things that don't work with structured clone
---

## Task

Write a type `Playlist` with two fields: a name, and a list of song titles. Make a copy of a playlist in two ways. A shallow copy is a new `Playlist` whose list field refers to the same list as the original. A deep copy is a new `Playlist` with a new list that holds the same titles. Then change one title through the original and show the difference: the shallow copy sees the change, and the deep copy does not.

The function `copy_and_change(songs, index, title)` makes the original from `songs`, makes the two copies, sets item `index` of the original's list to `title`, and returns the titles of the original, of the shallow copy and of the deep copy, in that order. For example, `copy_and_change(["Intro", "Song", "Outro"], 1, "Hit")` returns `[["Intro", "Hit", "Outro"], ["Intro", "Hit", "Outro"], ["Intro", "Song", "Outro"]]`. The list has 1 to 1,000 items, and `index` is a valid index. The tests also check that each copy is a new object, so that a change of the copy's name does not change the original.

Python's `copy` module gives the two definitions. "A shallow copy constructs a new compound object and then (to the extent possible) inserts references into it to the objects found in the original." "A deep copy constructs a new compound object and then, recursively, inserts copies into it of the objects found in the original."[^1]

## Solutions

| Language | Shallow copy | Deep copy |
|---|---|---|
| Python | `copy.copy(p)` | `copy.deepcopy(p)` |
| JavaScript | a method that calls the constructor with the same array | a method that copies the array with `structuredClone` |
| TypeScript | as JavaScript | as JavaScript |
| Java | `clone()` that calls `super.clone()` | a copy constructor with `new ArrayList<>(other.songs)` |
| C# | `MemberwiseClone()` | a method that makes a new `List<string>` |
| Go | assignment, `shallow := original` | assignment, then `slices.Clone` of the slice field |
| Rust | the derived `clone()` of a struct with an `Rc<RefCell<Vec<String>>>` field | a method that makes a new `Rc` with a copy of the vector |
| C | struct assignment | a function that allocates a new array and copies each string |
| C++ | the implicit copy constructor of a struct with a `std::shared_ptr<std::vector<std::string>>` field | a method that makes a new vector |

- **Python** needs no code in the class. `copy.copy` makes a new `Playlist` and copies the attribute references, and `copy.deepcopy` copies the list too. A class can change the behavior with `__copy__` and `__deepcopy__`.
- **Java** overrides `clone` with a public method that returns `Playlist` and implements `Cloneable`. `Object.clone` copies each field, so "this method performs a "shallow copy" of this object, not a "deep copy" operation".[^2] The copy constructor `Playlist(Playlist other)` makes the deep copy with a new `ArrayList`.
- **C#** calls the protected `MemberwiseClone`. "If a field is a reference type, the reference is copied but the referred object is not; therefore, the original object and its clone refer to the same object."[^3] The documentation's own example has a `ShallowCopy` and a `DeepCopy` method, as this solution does.
- **Go** copies a struct with assignment. The slice field is a small header: "A slice value contains the slice length, capacity, and a reference to its underlying array."[^4] So assignment gives a shallow copy, and `DeepCopy` replaces the field with `slices.Clone`. That function copies only one level: "The elements are copied using assignment, so this is a shallow clone."[^5] The strings do not need more, because Go strings cannot change.
- **Rust** shows the difference only with a shared pointer. A derived `Clone` on a struct with a `Vec<String>` field copies the vector, which is a deep copy. With an `Rc` field, `clone` "creates another pointer to the same allocation, increasing the strong reference count", so the derived `clone` is the shallow copy.[^6] The `Clone` documentation warns about this case: "for types that are references to other data (such as smart pointers or references), the new value may still point to the same underlying data, rather than duplicating it".[^7] `RefCell` lets the original change the shared list.
- **C** copies a struct with `=`, which copies the pointer to the array of strings. The deep copy allocates a new array and a new copy of each string. The shallow copy owns nothing, so the program frees the original and the deep copy, and not the shallow copy.
- **C++** has the same choice as Rust. The implicit copy constructor "performs full member-wise copy of the object's direct base class subobjects and member subobjects", so a `std::vector` member is copied element by element.[^8] With a `std::shared_ptr` member, the copy shares the vector, and `deep_copy` makes a new one with `std::make_shared`.
- **JavaScript and TypeScript** have no copy operation for class instances, so the class has two methods. `shallowCopy` passes the same array to the constructor. `deepCopy` passes `structuredClone(this.songs)`; a copy of the whole object with `structuredClone` loses the class, as the Pitfalls show.

### Alternatives

- **Immutable fields.** If the list cannot change, for example a Python tuple, a Java `List.copyOf` result or a Rust `Rc<[String]>`, a shallow copy is safe, because no one can change the shared part.
- **A copy method on the type.** A Kotlin data class has `copy()`, and a C# record has the `with` expression. Both are shallow. Kotlin says that "The copy() function creates a shallow copy of the instance", and C# says that "The result of a with expression is a shallow copy".[^9][^10]
- **Copy through serialization.** A program can write the object to bytes and read it back, which gives a deep copy of everything that the format can hold. The .NET documentation lists this way among the ways to make a deep copy.[^3] It is slow, and it loses the parts that the format cannot hold.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `single`: a list of one song.
- `first` and `last`: the change at index 0 and at the last index.
- `same-title`: the new title is the old one, so all three lists are equal. A test that compares only the lists cannot see the sharing here; the tests of each language also check the identity of the lists.
- `duplicates`: three equal titles. Only the item at the index changes, so a solution that replaces the first match of the old title in place of the item at the index fails.

## Pitfalls

- **Assignment is not a copy.** In Python, Java, C#, JavaScript and TypeScript, `b = a` copies a reference, so `b` is the same object. "Assignment statements in Python do not copy objects, they create bindings between a target and an object."[^1]
- **A deep copy that loses the type.** `structuredClone` copies plain data. "The prototype chain is not walked or duplicated. Class private elements are not duplicated."[^11] The copy of a class instance is a plain object without the methods:

```javascript run id=structured-clone-drops-the-class
class Playlist {
  constructor(name, songs) {
    this.name = name;
    this.songs = songs;
  }

  count() {
    return this.songs.length;
  }
}

const copy = structuredClone(new Playlist("mix", ["a", "b"]));
console.log(copy instanceof Playlist, copy.songs.length, typeof copy.count);
```

```output for=structured-clone-drops-the-class
false 2 undefined
```

- **A `clone` that is too shallow.** A Java class with a mutable list field that only calls `super.clone()` gives a copy that shares the list, which is often not what the caller expects. Copy each mutable field after `super.clone()`, or use a copy constructor.
- **Freeing a shared part twice.** In C, a shallow copy and the original point to the same array. If the program frees both, it frees the array twice. Give each piece of memory one owner.
- **Too much copying.** A deep copy copies everything that the object reaches, including data that the copies should share. The Python documentation names this problem: "Because deep copy copies everything it may copy too much, such as data which is intended to be shared between copies."[^1] It also warns that recursive objects "may cause a recursive loop"; `deepcopy` keeps a memo dictionary to stop it.

## Related tasks

- [[implement-equality-for-a-type]] compares two objects by value, which is how a test can check a copy.
- [[implement-a-string-form-for-a-type]] gives the type a string form for test messages.
- [[define-a-record-type]] defines a type whose generated copy operation copies the fields.

## References

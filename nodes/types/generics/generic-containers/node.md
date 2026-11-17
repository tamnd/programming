---
tag: EX6J3D
kind: concept
slug: generic-containers
name: Generic containers
aliases: [generic collections, typed collections, container of T, List of T, Vec, HashMap, parameterized collection, raw type]
area: types/generics
level: intro
summary: >-
  A generic container, such as List<T> or HashMap<K, V>, takes the types of its items as type parameters, so one container type works for every item type and the compiler checks each use.
applies_to: core-languages
viz: optional
formal: none
see_also: [type-constructor, parametric-polymorphism, list, dynamic-array, map-and-dictionary, hash-table, linked-list, type-erasure]
same_as:
  wikidata: Q1643805
  wikipedia: Container_(abstract_data_type)
sources:
- url: https://en.wikipedia.org/wiki/Container_(abstract_data_type)
  title: Container (abstract data type), Wikipedia
  locator: Introduction; Types
- url: https://docs.oracle.com/javase/tutorial/java/generics/why.html
  title: Why Use Generics?, The Java Tutorials
  locator: Whole page
- url: https://docs.python.org/3.14/glossary.html
  title: Glossary, Python 3.14 documentation
  locator: generic type
- url: https://learn.microsoft.com/en-us/dotnet/standard/generics/collections
  title: Generic collections in .NET, .NET documentation
  locator: System.Collections.Generic
- url: https://doc.rust-lang.org/std/vec/struct.Vec.html
  title: Struct Vec, Rust standard library
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/container/vector.html
  title: std::vector, C++ reference, cppreference.com
  locator: Introduction
- url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html
  title: Everyday Types, The TypeScript Handbook
  locator: Arrays
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Struct HashMap, Rust standard library
  locator: Introduction
- url: https://docs.oracle.com/javase/tutorial/java/generics/restrictions.html
  title: Restrictions on Generics, The Java Tutorials
  locator: Cannot Instantiate Generic Types with Primitive Types
- url: https://pkg.go.dev/container/list
  title: Package list, Go standard library
  locator: Overview; type Element; func (*List) PushBack
- url: https://docs.oracle.com/javase/tutorial/java/generics/rawTypes.html
  title: Raw Types, The Java Tutorials
  locator: Whole page
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: Built-in Types, Python 3.14 documentation
  locator: Generic Alias Type
- url: https://docs.oracle.com/javase/tutorial/java/generics/inheritance.html
  title: Generics, Inheritance, and Subtypes, The Java Tutorials
  locator: Whole page
- url: https://learn.microsoft.com/en-us/dotnet/standard/generics/
  title: Generics in .NET, .NET documentation
  locator: Advantages and disadvantages of generics
- url: https://en.wikipedia.org/wiki/Generic_programming
  title: Generic programming, Wikipedia
  locator: Introduction
---

## Idea

A program keeps many kinds of things in lists, maps and sets: numbers, names, orders. The code of a list is the same for each kind of item, so a language should not need one list type for numbers and another for names. A generic container takes the type of its items as a parameter. `list[int]` is a list of integers and `dict[str, int]` is a map from strings to integers, but both use the code of `list` and `dict`.

```python run id=idea
from collections import deque

# The type arguments say what each container holds.
scores: list[int] = [90, 85]
ages: dict[str, int] = {"ann": 31}
waiting: deque[str] = deque(["a", "b"])

scores.append(77)
ages["bob"] = 25
waiting.append("c")
print(sum(scores), sorted(ages), waiting.popleft())

# Python does not check the items when the program runs.
scores.append("ninety")  # a type checker reports this line
print(scores)
```

```output for=idea
252 ['ann', 'bob'] a
[90, 85, 77, 'ninety']
```

The annotations make `scores` a list of `int` and `ages` a map from `str` to `int`. A type checker such as mypy uses them to reject the call that adds a string to `scores`. Python itself runs that line, because the list stores any object.

## How it works

**Containers.** A container is a class or a data structure "whose instances are collections of other objects"[^1]. "Containers may be classified as either single-value containers or associative containers"[^1]: a list or a set stores single values, and a map stores pairs of a key and a value.

**The item type is a parameter.** Generics "enable types (classes and interfaces) to be parameters when defining classes, interfaces and methods"[^2]. A container declares a type parameter for its items, such as `T` in `List<T>`, and the code that uses it supplies a type argument, such as `List<String>`. The Python glossary defines a generic type as "A type that can be parameterized; typically a container class such as list or dict"[^3]. A map has two parameters, one for the keys and one for the values. See [type constructor](/type-constructor/).

**The compiler checks the items.** Without generics, a container holds the most general type, such as `Object`, and the code must cast each item that it reads. The Java tutorial lists the benefits of generics: "Stronger type checks at compile time" and "Elimination of casts"[^2]. A `List<String>` accepts only strings, and `get` returns a `String`.

**The usual containers.** Most languages have a growable array, a hash map and a set in the standard library. In .NET, "List<T> is a generic version of ArrayList"[^4] and `Dictionary<TKey,TValue>` "is a generic version of Hashtable"[^4]. Rust has `Vec<T>`, "A contiguous growable array type"[^5], and `HashMap<K, V>`. In C++, "std::vector is a sequence container that encapsulates dynamic size arrays"[^6]. In TypeScript, an array type is written `number[]`, and "You may also see this written as Array<number>, which means the same thing"[^7].

**Constraints on the items.** Some containers need more from the item type. A hash map must hash and compare its keys. In Rust, "It is required that the keys implement the Eq and Hash traits"[^8]. A sorted container needs an order on its keys.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list`, `dict`, `set`, `tuple` and `collections` types, with type arguments for static checkers |
| JavaScript | absent | `Array`, `Map` and `Set` hold values of any type; the language has no item types |
| TypeScript | native | `T[]` and `Array<T>`, `Map<K, V>`, `Set<T>` |
| Java | stdlib | `List<E>`, `Map<K, V>`, `Set<E>` in `java.util`; items must be objects |
| Kotlin | stdlib | `List<T>` and `MutableList<T>`, `Map<K, V>`, `Set<T>` |
| C# | stdlib | `List<T>`, `Dictionary<TKey,TValue>`, `HashSet<T>` in `System.Collections.Generic` |
| Go | native | built-in slice and map types such as `[]T` and `map[K]V`; generic types for others |
| Rust | stdlib | `Vec<T>`, `HashMap<K, V>`, `HashSet<T>`, `VecDeque<T>` |
| C | idiom | `void *` items with the size of an item, or a macro that makes a struct for each type |
| C++ | stdlib | `std::vector<T>`, `std::unordered_map<K, V>`, `std::set<T>` |
| Bash | n/a | arrays hold strings only |
| PowerShell | stdlib | .NET generic collections, such as `[System.Collections.Generic.List[int]]` |
| SQL | n/a | a table is not a value with an item type |
| Regex | n/a | a pattern has no containers |
| HTML | n/a | HTML has no containers of typed values |
| CSS | n/a | CSS has no containers of typed values |

Java, C#, Rust and C++ have their generic containers in the standard library. Go has slices and maps in the language, and its older packages, such as `container/list`, store items of type `any`. Python and TypeScript have item types only for the checker. JavaScript containers hold any value, so the program must check the items itself.

The example files show these differences. Python shows `list`, `dict` and `defaultdict` with type arguments. TypeScript shows `Array<T>` and `Map<K, V>`. Java shows a `List<String>`, a raw `List` and the boxing of `int`. C# shows `List<int>` and `Dictionary<string, List<int>>`. Go shows a slice, a map and `container/list` with a type assertion. Rust shows `Vec<T>`, `HashMap<K, V>` and a key type that derives `Hash`. C shows a list of `void *` items. C++ shows `std::vector` and `std::unordered_map`.

## Variations and related ideas

- **Erased or specialized.** Java keeps one copy of the container code, and the items are objects. "You can substitute only non-primitive types for the type parameters"[^9], so a `List<Integer>` holds boxed integers. .NET, Rust and C++ make a version of the container for each item type, so a `List<int>` holds the integers themselves. See [type erasure](/type-erasure/).
- **Containers of any type.** Before generics, a container held the most general type. Go has `container/list`, whose `PushBack` takes `v any`, and each `Element` has a field `Value any`[^10]. The code must check the type of each item that it reads.
- **Read-only views.** Many libraries have a read-only interface for a container, such as `IReadOnlyList<T>` in .NET or `List<T>` in Kotlin, so that a function can take a list without the right to change it.

## Pitfalls

- **Raw types in Java.** "A raw type is the name of a generic class or interface without any type arguments"[^11]. "Raw types show up in legacy code because lots of API classes (such as the Collections classes) were not generic prior to JDK 5.0"[^11]. A raw `List` accepts any object, and the error comes when the program reads an item. Write the type argument.
- **No primitive types in Java.** `List<int>` is a compile error. Use `List<Integer>`. The compiler boxes each `int`, as it "autoboxes 8 to Integer.valueOf(8)"[^9].
- **Item types that are not checked.** In Python and TypeScript, the type arguments are checked only by the type checker. "The Python runtime does not enforce type annotations"[^12], so a `list[int]` can hold a string. Data that comes from a file or the network must be checked when it is read.
- **A list of a subtype is not a list of the supertype.** In Java, "MyClass<A> has no relationship to MyClass<B>, regardless of whether or not A and B are related"[^13]. A method that takes a `List<Number>` does not take a `List<Integer>`, because it could add a `Double` to it. See [subtyping](/subtyping/).

## Cost

A generic container costs nothing extra in the code that the compiler checks. The cost comes from how the language stores the items. In Java, the items are objects, so a list of numbers holds a reference to each boxed number. .NET avoids this: "Generic collection types generally perform better for storing and manipulating value types because there is no need to box the value types"[^14]. In C++, the elements of a `std::vector` "are stored contiguously"[^6], and a `Vec<T>` in Rust is contiguous too[^5].

## History

"Generic programming was introduced to the mainstream with Ada in 1977. With templates in C++, generic programming became part of the repertoire of professional library design."[^15] Java collections took type parameters in JDK 5.0, and older code still has raw types from before that change[^11]. .NET has generic versions of its older collections, so `List<T>` replaced `ArrayList`[^4].

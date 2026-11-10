---
tag: NEW-flyweight-pattern
kind: pattern
slug: flyweight-pattern
name: Flyweight pattern
aliases: [Flyweight, interning, hash consing, instance cache]
area: software-design/structural-patterns
level: core
summary: >-
  Share one immutable object between many uses, and pass the state that differs between the uses in as arguments. A factory hands out the shared objects, so that each value has one object.
family: structural-design
polarity: positive
intent: "Use sharing to support large numbers of fine-grained objects at a low cost in memory."
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with the characters of a document editor as the motivating example. Calder and Linton had described glyphs as flyweight objects for user interfaces in 1990.
see_also: [composite-pattern, state-pattern, strategy-pattern, singleton-pattern, prototype-pattern, proxy-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Flyweight (Intent, Motivation, Applicability, Participants, Consequences, Implementation, Known Uses, Related Patterns)
  - paper: doi:10.1145/97924.97935
    title: Paul R. Calder and Mark A. Linton, Glyphs, flyweight objects for user interfaces, UIST 1990
    locator: title
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Integer.html#valueOf(int)
    title: Java SE 25 API, java.lang.Integer
    locator: method valueOf(int)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.7
    title: The Java Language Specification, Java SE 25 Edition
    locator: 5.1.7 Boxing Conversion
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-3.html#jls-3.10.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 3.10.5 String Literals
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html#intern()
    title: Java SE 25 API, java.lang.String
    locator: method intern
  - url: https://docs.python.org/3.14/c-api/long.html#c.PyLong_FromLong
    title: Python/C API Reference Manual, Integer Objects, Python 3.14
    locator: PyLong_FromLong
  - url: https://docs.python.org/3.14/library/sys.html#sys.intern
    title: The Python Standard Library, sys, Python 3.14
    locator: sys.intern
  - url: https://docs.python.org/3.14/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.1 Objects, values and types
  - url: https://docs.python.org/3.14/reference/expressions.html#is-not
    title: The Python Language Reference, Expressions, Python 3.14
    locator: Identity comparisons
  - url: https://pkg.go.dev/unique
    title: Go standard library, package unique
    locator: package overview; type Handle; func Make
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.string.intern
    title: .NET API reference, String.Intern
    locator: Remarks; Performance considerations
  - url: https://doc.rust-lang.org/std/rc/struct.Rc.html#method.ptr_eq
    title: Rust standard library, std::rc::Rc
    locator: method ptr_eq
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, 1996
    locator: slide 10, Design Patterns in Dylan or Lisp
---

## Intent

Use sharing to support large numbers of fine-grained objects at a low cost in memory.[^1] A flyweight is a shared object that can be used in many contexts at the same time. It holds only the state that is the same in all of them. Each use passes the rest of the state in as an argument.

## Problem

Design Patterns starts with a document editor. An editor that makes each character an object gets a uniform design: characters, lines and pages are all objects in one tree, and the code that formats and draws them treats them the same way. But a document has hundreds of thousands of characters, and an object for each one costs too much memory.[^1]

The same need appears wherever a program has many small objects with few distinct values: the tiles of a game map, the cells of a spreadsheet with the same format, the tokens of a compiler, the trees of a forest in a 3D scene, and the boxed integers and the strings of a language runtime.

The forces are these. The design wants an object for each item, because the uniform treatment is clear and correct. The memory budget allows few objects. And most of the state of each object is the same as in many others.

## Solution

Split the state of the object in two.[^1]

- **Intrinsic state** is the same in each context. It is stored in the flyweight, and it does not change. For a character, it is the character code and the shape of the glyph.
- **Extrinsic state** depends on the context. The flyweight does not store it. The client computes it or stores it, and passes it to each operation. For a character, it is the position on the page and often the font.

Design Patterns names these participants.[^1]

- **Flyweight** declares the operations that take the extrinsic state as arguments.
- **ConcreteFlyweight** stores the intrinsic state. It must be shareable.
- **UnsharedConcreteFlyweight** implements the same interface but is not shared. A row or a column in the document is an example: it has children, so it is not the same in two places.
- **FlyweightFactory** makes and manages the flyweights. A client asks the factory for a flyweight with a key. The factory returns the existing object for that key, or makes one and keeps it.
- **Client** keeps references to flyweights, and it computes or stores their extrinsic state.

```text
 text: a b r a c a d a b r a     (11 references)
       | | | | | | | | | | |
       v v v v v v v v v v v
 factory pool:  a   b   r   c   d    (5 shared Glyph objects)

 glyph.draw(column)    the column is extrinsic; the client passes it
```

The examples draw the word "abracadabra". It has 11 characters and 5 distinct ones, so 11 references point to 5 glyph objects. Each glyph stores its character and its width. The loop computes the column of each glyph from the widths of the glyphs before it, and passes the column to `draw`.

### A flyweight must not change

A flyweight is shared, so a change to it is a change in each place that uses it. A program that stores extrinsic state in a flyweight changes every use at once. Here the program wants to mark the first letter in red:

```python wrong id=shared-change
class Glyph:
    def __init__(self, char):
        self.char = char
        self.colour = "black"  # extrinsic state, stored in the shared object


pool = {}


def glyph(char):
    return pool.setdefault(char, Glyph(char))


line = [glyph(c) for c in "abracadabra"]
line[0].colour = "red"  # mark the first letter only
print(" ".join(f"{g.char}:{g.colour}" for g in line))
```

```output for=shared-change
a:red b:black r:black a:red c:black a:red d:black a:red b:black r:black a:red
```

Each `a` is red, because there is one `a`. The fix is to keep the colour out of the glyph. The client stores it for each position, as it does for the column, or a style object for a run of text holds it. A flyweight type that is immutable, such as a frozen dataclass, a Java record, a `const` C++ object or a Rust value behind `Rc`, makes the bug impossible to write.

### The client must not depend on identity

Design Patterns lists, as a condition for the pattern, that the application does not depend on object identity.[^1] Two equal values that come from the factory are one object. Two equal values that come from somewhere else may not be. Code that compares with an identity test then gives a result that depends on how the value was made.

Language runtimes are the most common place where this bites, because they use the pattern for boxed numbers and strings. In Java, `Integer.valueOf` always caches the values from -128 to 127, and it may cache others.[^3] The Java Language Specification requires that the boxing of an `int` in that range gives the same object each time.[^4] Each string literal refers to an interned string, and `String.intern` returns the string from a pool that the class `String` keeps.[^5][^6] The `==` operator on two references tests identity, not value:

```java wrong id=identity
public class Main {
    public static void main(String[] args) {
        Integer a = 127, b = 127;
        Integer c = 128, d = 128;
        System.out.println("127 == 127: " + (a == b));
        System.out.println("128 == 128: " + (c == d));
        System.out.println("128 equals 128: " + c.equals(d));

        String literal = "glyph";
        String built = new StringBuilder("gly").append("ph").toString();
        System.out.println("built == literal: " + (built == literal));
        System.out.println("built.intern() == literal: " + (built.intern() == literal));
    }
}
```

```output for=identity
127 == 127: true
128 == 128: false
128 equals 128: true
built == literal: false
built.intern() == literal: true
```

The specification allows a runtime to cache more values, so the result for 128 can differ between runtimes.[^3][^4] The fix is `equals` for objects, or the primitive `int`.

CPython does the same for small integers. It keeps an array of integer objects for each value from -5 to 256, and the documentation calls this an implementation detail.[^7] The language reference allows it: for an immutable type, an operation that computes a value may return any existing object with the same type and value.[^9] So `is`, which tests identity, gives different answers for equal integers:[^10]

```python wrong id=small-ints
a, b = int("256"), int("256")
c, d = int("257"), int("257")
print("256 is 256:", a is b)
print("257 is 257:", c is d)
print("257 == 257:", c == d)
```

```output for=small-ints
256 is 256: True
257 is 257: False
257 == 257: True
```

The fix is `==`. Python's `sys.intern` interns a string on request.[^8] .NET's `String.Intern` returns the one string with a given value from the intern pool. Its documentation warns that this memory is not likely to be released until the runtime terminates.[^12]

### Interning as a library

Go's package `unique` is a flyweight factory in the standard library. It canonicalizes, or interns, comparable values. `unique.Make` returns a `Handle`, and two handles are equal if and only if the values used to make them are equal.[^11] A comparison of two handles is a comparison of two pointers, which costs less than a comparison of two long strings:

```go run id=unique
package main

import (
	"fmt"
	"strings"
	"unique"
)

func main() {
	built := unique.Make(strings.Repeat("ab", 3)) // made at run time
	literal := unique.Make("ababab")
	other := unique.Make("abab")
	fmt.Println("built == literal:", built == literal)
	fmt.Println("built == other:", built == other)
	fmt.Println("value:", built.Value())
}
```

```output for=unique
built == literal: true
built == other: false
value: ababab
```

Here the handle is the identity that the program compares, and the factory makes it safe to depend on. In Rust, `Rc::ptr_eq` tests whether two `Rc` pointers point to the same value, so a client can test for a shared flyweight on purpose and not by accident.[^13]

## Example

Each example file builds a flyweight factory, draws "abracadabra" with it, and prints the count of characters, the count of glyph objects and an identity test on the first and last `a`.

| Language | Factory | Immutability |
|---|---|---|
| Python | a function with `functools.cache` | a frozen dataclass with slots |
| JavaScript | a `Map` in a class | `Object.freeze` |
| TypeScript | a static method, a private constructor | `readonly` fields |
| Java | `HashMap.computeIfAbsent` | a `record` |
| C# | `Dictionary.TryGetValue` | a positional `record` |
| Go | a map of pointers | unexported fields |
| Rust | `HashMap::entry` with `Rc<Glyph>` | `Rc` gives out only shared references |
| C++ | a `std::map` of `std::unique_ptr<const Glyph>` | `const` members |

## Consequences

The benefit is memory. Design Patterns says that the savings depend on three things: how far the count of instances falls through sharing, how much intrinsic state each object has, and whether the extrinsic state is computed or stored.[^1] The savings are largest when the extrinsic state is computed, as the column is in the examples, so that the program stores almost nothing for each use. Fewer objects also mean less work for a garbage collector and better use of the cache. Shared objects can be compared by identity, which is cheap, once all of them come from the factory.

The costs are these. Design Patterns notes the run-time cost to transfer, find or compute the extrinsic state.[^1] Each operation takes more arguments, and the code that computes them is in the client. The factory is a global pool: it must be safe for concurrent use when threads share it, and it keeps each flyweight alive unless it uses weak references. A pool that interns values from input, such as user names or log lines, grows without limit. And the design is harder to read, because the state of one logical object is in two places.

A flyweight also cannot know its context. A shared leaf in a tree cannot store a pointer to its parent, because it has many parents. Design Patterns notes that a flyweight leaf in a composite has this limit.[^1]

## When not to use it

Do not use it until a measurement shows that the objects cost too much. Design Patterns says that all of its applicability conditions must hold: the program uses many objects, the storage cost is high, most of the state can be made extrinsic, many groups of objects can then be replaced by few shared ones, and the program does not depend on object identity.[^1]

Do not use it for values that are cheap to copy. In Go, Rust, C# and C++, a small struct can be a value type stored inline in an array. An array of 11 small values can cost less than 11 pointers to 5 shared heap objects, and the values need no factory and no pool.

Do not use it for objects that change. A shared mutable object is a shared global variable, and the bug above follows.

Do not intern untrusted input in a pool that never shrinks. The pool becomes a memory leak that an attacker can fill.

Peter Norvig lists Flyweight among the patterns that first-class types make invisible or simpler in a dynamic language.[^14] In many runtimes, interning and small-integer caches already apply the pattern to the most common values.

## Variants and related patterns

**Composite.** Design Patterns says that the pattern is often combined with Composite to build a hierarchy with shared leaf nodes.[^1] The editor's characters are shared leaves in a tree of rows and pages. See [[composite-pattern]].

**State and Strategy.** Design Patterns says that state and strategy objects are often built as flyweights.[^1] A state object with no fields, or a strategy that is a pure function, can be one shared object for all contexts. See [[state-pattern]] and [[strategy-pattern]].

**Singleton.** A singleton is one object for a class. A flyweight factory keeps one object for each key. The factory itself is often a singleton or a module-level value. See [[singleton-pattern]].

**Prototype.** A prototype makes a copy for each use. A flyweight shares one object for each value. A prototype whose fields are all immutable needs no copy, and it can be shared as a flyweight. See [[prototype-pattern]].

**Interning and hash consing.** Interning is the flyweight factory for values such as strings and symbols. Hash consing applies it to the nodes of a data structure, so that equal subtrees are one object.

**Memoization.** `functools.cache` in the Python example is a memoizing decorator. A flyweight factory is memoization of a constructor.

## Known uses

- Java: `Integer.valueOf` caches the values from -128 to 127, and string literals are interned.[^3][^5]
- CPython: one integer object for each value from -5 to 256, and `sys.intern` for strings.[^7][^8]
- .NET: the intern pool of `String.Intern`.[^12]
- Go: package `unique` interns comparable values.[^11]
- InterViews: Calder and Linton describe glyphs as flyweight objects for user interfaces.[^2]

## References

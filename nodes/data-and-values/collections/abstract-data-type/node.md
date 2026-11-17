---
tag: M1NV6B
kind: concept
slug: abstract-data-type
name: Abstract data type
aliases: [ADT, data abstraction, abstract type, abstract data types, cluster]
area: data-and-values/collections
level: core
summary: >-
  An abstract data type is a type defined by its operations and their rules, not by how its values are stored. A stack is push, pop and top, whether an array or a list holds it.
applies_to: core-languages
viz: optional
formal: none
see_also: [data-structure, collection, collection-interfaces, interfaces, opaque-type, encapsulation-and-visibility, stack-data-structure, algebraic-data-type]
same_as:
  wikidata: Q827335
  wikipedia: Abstract_data_type
sources:
- url: https://xlinux.nist.gov/dads/HTML/abstractDataType.html
  title: abstract data type, Dictionary of Algorithms and Data Structures, NIST
  locator: Definition; Note
- url: https://en.wikipedia.org/wiki/Abstract_data_type
  title: Abstract data type, Wikipedia
  locator: Introduction; History; Implementation
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-14.html
  title: 2.1 Introduction to Data Abstraction, Structure and Interpretation of Computer Programs
  locator: 2.1; 2.1.2 Abstraction Barriers
- url: https://en.wikipedia.org/wiki/Opaque_data_type
  title: Opaque data type, Wikipedia
  locator: Introduction
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/doc-files/coll-overview.html
  title: Collections Framework Overview, Java SE 21
  locator: Introduction
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Exported identifiers
- url: https://doc.rust-lang.org/book/ch07-03-paths-for-referring-to-an-item-in-the-module-tree.html
  title: Paths for Referring to an Item in the Module Tree, The Rust Programming Language
  locator: Making Structs and Enums Public
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Private_elements
  title: Private elements, MDN Web Docs
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/access-modifiers
  title: Access Modifiers, C# programming guide
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/language/access.html
  title: Access specifiers, C++ reference, cppreference.com
  locator: Private member access
- url: https://docs.python.org/3.14/library/collections.abc.html
  title: collections.abc, Python 3.14 documentation
  locator: Introduction
- url: https://docs.python.org/3.14/tutorial/classes.html
  title: Classes, The Python Tutorial
  locator: 9.6 Private Variables
- url: https://en.wikipedia.org/wiki/CLU_(programming_language)
  title: CLU (programming language), Wikipedia
  locator: Introduction; Clusters
---

## Idea

A program that uses a stack needs three things: put a value on top, look at the top value, and take it off. It does not need to know whether the stack keeps its values in an array or in linked nodes. An abstract data type (ADT) is a type that is defined in this way, by its operations and the rules that they follow. The code that uses the type sees only the operations. The code that implements it chooses a representation and can change it later.

```python run id=idea
class ArrayStack:
    def __init__(self):
        self._items = []          # the representation: a Python list

    def push(self, v):
        self._items.append(v)

    def pop(self):
        return self._items.pop()

    def is_empty(self):
        return not self._items


class LinkedStack:
    def __init__(self):
        self._top = None          # the representation: (value, rest) pairs

    def push(self, v):
        self._top = (v, self._top)

    def pop(self):
        v, self._top = self._top
        return v

    def is_empty(self):
        return self._top is None


def drain(stack):                 # client code: uses only the operations
    for v in (1, 2, 3):
        stack.push(v)
    out = []
    while not stack.is_empty():
        out.append(stack.pop())
    return out


for s in (ArrayStack(), LinkedStack()):
    print(type(s).__name__, drain(s))
```

```output for=idea
ArrayStack [3, 2, 1]
LinkedStack [3, 2, 1]
```

The function `drain` works with both classes, because both give the same operations with the same rules: the last value pushed is the first value popped. The underscore in `_items` and `_top` tells other code not to touch the representation.

## How it works

**Values and operations.** NIST defines an ADT as "A set of data values and associated operations that are precisely specified independent of any particular implementation"[^1]. Wikipedia describes it "from the point of view of a user of the data, specifically in terms of possible values, possible operations on data of this type, and the behavior of these operations"[^2]. A [data structure](/data-structure/) is the other side: it is a concrete representation, from the point of view of an implementer[^2].

**Rules for the operations.** The operations must follow rules, and the rules can be written as equations. For a stack, NIST gives two axioms: `popOff(push(v, S)) = S` and `top(push(v, S)) = v`[^1]. "Since the data values and operations are defined with mathematical precision, rather than as an implementation in a computer language, we may reason about effects of the operations"[^1]. Any implementation that keeps the rules is a correct stack.

**Constructors and selectors.** SICP builds a rational number type from three procedures. The constructor `make-rat` makes a value, and the selectors `numer` and `denom` read its parts. "The interface between these two parts of our system will be a set of procedures, called selectors and constructors, that implement the abstract data in terms of the concrete representation"[^3]. The arithmetic procedures use only these three, so a change to the representation, such as reducing the fraction in `make-rat`, needs no change elsewhere: SICP calls the separating layers "abstraction barriers"[^3].

**Hiding the representation.** "In order to prevent clients from depending on the implementation, an ADT is often packaged as an opaque data type or handle of some sort"[^2]. An opaque type is "a data type whose concrete data structure is not defined in an interface"[^4], so other code can use its values only through the operations. Each language has its own way to hide the representation, as the next section shows. See [opaque type](/opaque-type/) and [encapsulation and visibility](/encapsulation-and-visibility/).

**One type, many implementations.** "Usually, there are many ways to implement the same ADT, using several different concrete data structures"[^2]. The Java collections framework is built on this. Its interfaces, such as `List`, are the abstract types, and "Because the various implementations of each interface are interchangeable, programs can be tuned by switching implementations"[^5]. Code that is written to the interface keeps working when the implementation changes. See [collection interfaces](/collection-interfaces/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | a class with the operations as methods; a leading underscore marks the representation; `abc` and `Protocol` for the interface |
| JavaScript | native | a class with `#private` fields, or a closure that keeps the representation |
| TypeScript | native | an interface for the operations, and a class with `private` or `#private` fields |
| Java | native | an interface for the operations, and classes with `private` fields |
| Kotlin | native | an interface, and classes with `private` properties |
| C# | native | an interface, and classes with `private` fields |
| Go | native | an interface, and a struct whose fields are not exported from its package |
| Rust | native | a trait for the operations, and a struct whose fields are private to its module |
| C | idiom | an incomplete struct type in the header, functions that take a pointer to it, and the struct body in the `.c` file |
| C++ | native | a class with `private` members; an abstract base class with pure virtual functions for the interface |
| Bash | idiom | a set of functions that work on one array; nothing stops other code from changing the array |
| PowerShell | native | a class with `hidden` members, or a module that exports only its functions |
| SQL | n/a | a query works on tables and views, not on abstract types in program code |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types |

Most languages can hide a representation, but they do it at different levels. Java, C#, C++ and JavaScript hide a field in a class. Go hides a name in a package: "An identifier is exported if both: the first character of the identifier's name is a Unicode uppercase letter"[^6] and it is declared at package level or is a field or a method. Rust hides a field in a module, because "struct fields follow the general rule of everything being private by default unless annotated with pub"[^7]. In C, the header declares the type without its fields. Python and Bash do not enforce the barrier.

The example files define a stack ADT in each language. Each file has the interface, two implementations, one on an array and one on linked nodes, and one client function that works with both. Python uses two classes and duck typing. JavaScript uses `#private` fields. TypeScript, Java and C# use an interface. Go uses an interface and two structs. Rust uses a trait and keeps the fields private in a module. C keeps the struct body behind a pointer. C++ uses an abstract base class. Bash has one implementation, a set of functions over a named array.

## Variations and related ideas

- **Classes as ADTs.** "Modern object-oriented languages, such as C++ and Java, support a form of abstract data types"[^2]. A class holds the representation in private fields and gives the operations as methods. JavaScript private elements "get created by using a hash # prefix and cannot be legally referenced outside of the class"[^8]. In C#, a `private` member is one that "Only code declared in the same class or struct can access"[^9]. In C++, "A private member of a class is only accessible to the members and friends of that class"[^10].
- **Modules as ADTs.** A module can export a type name and its functions but not the fields of the type. Go, Rust and the C header work this way. One module then holds the whole implementation, and the type can have operations that look at two values at once, such as a merge of two sets.
- **Interfaces.** An interface, a trait or an abstract base class names the operations without a representation. Python's abstract base classes "can be used to test whether a class provides a particular interface"[^11]. See [interfaces](/interfaces/).
- **Built-in ADTs.** Some built-in types are ADTs: the language says what their operations do, but not how they are stored. Wikipedia names the arrays of Awk, Lua and Perl, "which can be regarded as an implementation of the abstract list"[^2].

## Pitfalls

- **Abstract data type and algebraic data type.** Both are written ADT, but they are opposite ideas. An abstract data type hides its representation behind operations. An [algebraic data type](/algebraic-data-type/) shows its representation: its values are built from named cases, and code takes them apart with pattern matching. Write the full name when the meaning is not clear.
- **A leaky representation.** A method that returns the internal list lets the caller change the stack without `push` or `pop`. Return a copy, a read-only view or an iterator.
- **A barrier only by convention.** Python does not hide `_items`. The tutorial says that a name with a leading underscore "should be treated as a non-public part of the API"[^12], but code outside the class can still read and change it. Bash has no barrier at all.
- **Rules that are not written down.** Most languages check only the names and the types of the operations. "Most mainstream computer languages do not directly support formally specifying ADTs"[^2]. A class can have `push` and `pop` and still break the stack rules. Write the rules in the documentation and test them.
- **The limits of the representation.** An implementation can fail where the abstract type does not. Integers as an ADT have no largest value, but "in a computer, integers are most commonly represented as fixed-width 32-bit or 64-bit binary numbers"[^2], so they overflow.

## Cost

A call through an interface goes to code that the caller does not know, so the compiler cannot inline it. In C, an opaque struct must live behind a pointer, because the code that uses it does not know its size. A generic function in Rust or C++ avoids the indirect call, because the compiler makes a copy of it for each concrete type. See [templates and generics](/templates-and-generics/). The larger gain is that the representation can change for speed without any change to the code that uses it[^2].

## History

"ADTs were first proposed by Barbara Liskov and Stephen N. Zilles in 1974, as part of the development of the CLU language"[^2]. Their paper was "Programming with abstract data types", in the proceedings of the ACM SIGPLAN Symposium on Very High Level Languages[^2]. CLU first appeared in 1975[^13]. Its "key addition was the concept of a cluster"[^13]: "A cluster implements a data abstraction, which is a set of objects and a set of primitive operations to create and manipulate those objects"[^13], and its representation type "is hidden from external clients"[^13]. "Algebraic specification was an important subject of research in CS around 1980 and almost a synonym for abstract data types at that time"[^2].

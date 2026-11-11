---
tag: 6Q423K
kind: concept
slug: encapsulation-and-visibility
name: Encapsulation
aliases: [encapsulation, access control, access modifiers, visibility, private members, data hiding, opaque types]
area: objects-and-classes/encapsulation
level: core
summary: >-
  Encapsulation keeps the data of an object behind the operations that may change it, so the object can keep its invariants and change its implementation without breaking callers.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1972
  people: [David Parnas, Barbara Liskov, Bjarne Stroustrup]
  note: Parnas argued for modules that hide design decisions in 1972, and CLU, Smalltalk and C++ made the idea part of their types.
see_also: [interfaces, closures, immutability, class-inheritance, object-oriented-programming, prototype-based-programming, dynamic-dispatch]
same_as:
  wikidata: Q1305241
  wikipedia: Encapsulation_(computer_programming)
sources:
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/accesscontrol.html
  title: Controlling Access to Members of a Class, The Java Tutorials
  locator: The opening paragraph
- url: https://doc.rust-lang.org/reference/visibility-and-privacy.html
  title: Visibility and privacy, The Rust Reference
  locator: The opening rules
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Exported identifiers
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Private_elements
  title: Private elements, MDN Web Docs
  locator: Description
- url: https://docs.python.org/3/tutorial/classes.html
  title: The Python Tutorial, Classes
  locator: 9.6 Private Variables
---

## Idea

Encapsulation keeps the data of an object together with the code that may change it, and stops other code from changing the data directly. Outside code works through a small set of operations, the interface, and the object keeps its fields and helpers to itself. Two benefits follow. The object can guarantee rules about its own state, its invariants, because every change goes through code that checks them. And the implementation can change without breaking callers, because no caller depends on it. Languages enforce encapsulation with access modifiers, module boundaries or closures, or leave it to convention.

```python run id=idea
class Account:
    """Invariant: balance == sum(history) and balance >= 0."""

    def __init__(self):
        self._balance = 0  # the underscore says: not part of the interface
        self._history = []

    def deposit(self, amount):
        if amount <= 0:
            raise ValueError("deposit must be positive")
        self._history.append(amount)
        self._balance += amount

    def withdraw(self, amount):
        if amount > self._balance:
            raise ValueError("insufficient funds")
        self._history.append(-amount)
        self._balance -= amount

    @property
    def balance(self):  # read access without write access
        return self._balance

    def consistent(self):
        return self._balance == sum(self._history) and self._balance >= 0


a = Account()
a.deposit(100)
a.withdraw(30)
print(a.balance, a.consistent())

try:
    a.withdraw(500)  # the method guards the invariant
except ValueError as e:
    print("refused:", e)

try:
    a.balance = 10**6  # the property has no setter
except AttributeError as e:
    print("refused:", e)

a._balance = 10**6  # Python does not stop code that ignores the convention
print(a.balance, a.consistent())


def make_counter():  # a closure gives state that no outside code can reach
    count = 0

    def increment():
        nonlocal count
        count += 1
        return count

    return increment


tick = make_counter()
tick()
print(tick(), hasattr(tick, "count"))
```

```output for=idea
70 True
refused: insufficient funds
refused: property 'balance' of 'Account' object has no setter
1000000 False
2 False
```

`Account` keeps two fields that must agree: the balance and the list of changes. The methods `deposit` and `withdraw` change both together and refuse a change that would make the balance negative, and the property `balance` gives read access only. While all code goes through these operations, `consistent()` stays true. The line that writes `_balance` directly breaks the invariant, and Python allows it, because its privacy is a convention. The counter at the end hides its state in a closure, where no outside code can name it.

## How it works

**Interface and implementation.** An encapsulated type has two parts. The interface is the set of operations that other code may use, with their meaning. The implementation is the fields, the helper functions and the algorithms behind them. Callers depend only on the interface, so the implementation can change, for example from a list to a tree or from a stored value to a computed one, without a change to any caller. This is the idea of information hiding applied to a single type.

**Invariants.** An invariant is a rule about the state of an object that holds whenever no operation is running: a balance is not negative, a sorted list is sorted, a count matches the number of items. If only the object's own methods can change its fields, then to prove that the invariant always holds, a programmer must check only those methods: each constructor must establish it, and each method must keep it. Without encapsulation, any line of the program might break it.

**Access control.** Most languages with classes mark each member with an access level. Java's tutorial states the purpose: "Access level modifiers determine whether other classes can use a particular field or invoke a particular method."[^1] The common levels are private to the class, visible to subclasses, visible to the package or module, and public. Rust makes privacy the default: "By default, everything is private", and "If an item is private, it may be accessed by the current module and its descendants."[^2] Go uses the case of the first letter: "An identifier may be exported to permit access to it from another package", and a name is exported when it starts with an upper case letter.[^3] In C++, Java, C# and Rust the check happens at compile time, and in JavaScript, whose `#` fields are "enforced by JavaScript itself", it happens when the engine parses the code.[^4]

**Convention instead of enforcement.** Python has no access modifiers. Its tutorial says: "“Private” instance variables that cannot be accessed except from inside an object don’t exist in Python." Instead, "a name prefixed with an underscore (e.g. _spam) should be treated as a non-public part of the API".[^5] Linters, editors and readers follow the convention. Smalltalk takes the opposite position for fields: instance variables are visible only to the object's own methods, and all methods are public.

**Encapsulation leaks through references.** A private field that holds a reference to a mutable object protects only the reference. If a constructor stores a list that the caller still holds, or a getter returns the internal list, outside code can change the object's state without calling any method. Defensive copies, read-only views and immutable types close these leaks. The wrong blocks in the Python, Java and Go files show them.

**Properties and accessors.** A getter and setter pair lets a class check every write and compute values on read. Properties in Python, C# and Kotlin, and `get` and `set` in JavaScript classes, make these methods look like fields, so a class can start with a plain field and add checks later without a change to callers. A getter for every field, with a setter that checks nothing, hides the name of the field but not the design, and gives little of the benefit.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | `_name` by convention; properties; name mangling for `__name` |
| JavaScript | native | `#private` fields and methods; closures |
| TypeScript | native | `private` and `protected` checked at compile time; `#private` |
| Java | native | `private`, package, `protected`, `public`; module exports |
| Kotlin | native | `private`, `protected`, `internal`, `public` |
| C# | native | six access levels; accessors with their own level |
| Go | native | unexported lower case names, per package |
| Rust | native | private by default per module; `pub`, `pub(crate)` |
| C | idiom | opaque pointers; `static` functions |
| C++ | native | `public`, `protected`, `private`; `friend` |
| Bash | idiom | `local` and `readonly` variables |
| PowerShell | idiom | `hidden` class members |
| SQL | native | `GRANT` and `REVOKE`; views and procedures |
| Regex | n/a | no state |
| HTML | idiom | closed shadow roots |
| CSS | idiom | styles scoped to a shadow root |

Every example file builds the same bank account, with a balance that must not go negative and a history that must agree with it, and then shows what the language does when outside code tries to change the balance directly. Java, C#, C++, Rust and JavaScript reject the access. Go rejects it only from another package, and C rejects it only because the caller does not know the layout of the struct. Python lets it happen.

The unit of privacy differs. In Java, C# and C++ it is the class: a method can read the private fields of any object of its class, not only its own. In Go and Rust it is the package or module, so all code in the module can use the fields of every type defined there. In JavaScript it is the class body. Ruby and Smalltalk restrict access per object, so a method can read the fields of its own receiver only.

## Variations and related ideas

- **Information hiding.** The wider principle that each module hides a design decision behind its interface, of which encapsulation in classes is one form.
- **Modules and packages.** Go, Rust, Java modules and ES modules hide whole groups of types and functions, not only fields.
- **Abstract data types.** A type known only through its operations, as in the opaque types of ML modules, Ada private types and C's opaque pointers.
- **Closures.** Variables captured by a function are unreachable from outside, which makes closures a complete form of encapsulation. See [closures](/closures/).
- **Immutability.** An immutable object needs no guard on its fields after construction, because nothing can change them. See [immutability](/immutability/).
- **Interfaces.** A type that has only abstract operations is the most complete separation of interface from implementation. See [interfaces](/interfaces/).
- **Capabilities.** In object-capability systems, a reference to an object is the permission to use it, so encapsulation becomes a security boundary.

## Pitfalls

- **Leaking internal references.** Returning a mutable field, or storing a mutable argument without copying it, gives outside code a way around the methods. Copy, wrap in a read-only view, or use immutable types.
- **Getters and setters for every field.** Public accessors that do nothing but read and write the field expose the representation as much as a public field does. Design the operations from what callers need.
- **Protected fields.** A field visible to subclasses is part of the interface for every subclass, written by people who may not know the invariant. Prefer private fields with protected methods. See [class inheritance](/class-inheritance/).
- **Privacy is not security.** Reflection in Java and C#, pointer arithmetic in C and C++, and the mangled names of Python can all reach private data. Access control organizes code; it does not protect secrets from code in the same process.
- **Invariants broken during a method.** An exception thrown half way through an update can leave the object in a state that breaks its invariant. Check before changing anything, or change a copy and swap it in.
- **Encapsulating too much.** A wrapper that hides a plain record behind methods adds code without protecting any rule. Data with no invariant can be a plain public record.

## Cost

Access control costs nothing at run time in compiled languages: the compiler checks it and generates the same code as for a public field. A small getter is inlined by C++, Rust, Java and C# compilers, so it costs no more than reading the field. Private `#` fields in JavaScript engines are as fast as ordinary properties. Opaque pointers in C force allocation on the heap and an indirect call through the library for every operation, which inlining across files can remove. Defensive copies cost time and memory in proportion to the size of what they copy, and read-only views avoid that cost.

## History

David Parnas argued in 1972 that a program should be split into modules that each hide a design decision, which became the principle of information hiding. Simula 67 had classes but no access control; later versions of Simula added `hidden` and `protected`. Smalltalk, from the 1970s, made instance variables visible only to the object's own methods. Barbara Liskov's CLU, from 1974, and Ada, from 1980, gave abstract data types whose representation only the type's own operations could see.

C with Classes had `public` and `private` from 1980, C++ added `protected` in 1987, and Java and C# kept all three in the 1990s. Go, released in 2009, chose package-level visibility marked by the case of a name, and Rust made privacy the default for every item and field.[^2] JavaScript had only closures for private state until ECMAScript 2022 added `#` private fields and methods.[^4]

## References

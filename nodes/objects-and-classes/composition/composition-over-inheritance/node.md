---
tag: NEW-composition-over-inheritance
kind: concept
slug: composition-over-inheritance
name: Composition over inheritance
aliases: [composition, object composition, has-a relationship, favor composition over inheritance, forwarding, fragile base class]
area: objects-and-classes/composition
level: core
summary: >-
  Composition over inheritance is the rule that a class should get behavior by holding other objects and calling them, not by extending a base class whose implementation it then depends on.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides, Alan Snyder, Joshua Bloch]
  note: Snyder showed in 1986 that inheritance breaks encapsulation, and Design Patterns stated the rule in 1994.
see_also: [class-inheritance, interfaces, dynamic-dispatch, encapsulation-and-visibility, strategy-pattern, decorator-pattern, dependency-injection, prototype-based-programming]
same_as:
  wikidata: Q1780968
  wikipedia: Composition_over_inheritance
sources:
- url: https://doc.rust-lang.org/book/ch18-01-what-is-oo.html
  title: Characteristics of Object-Oriented Languages, The Rust Programming Language
  locator: Inheritance as a Type System and as Code Sharing
- url: https://kotlinlang.org/docs/delegation.html
  title: Delegation, Kotlin documentation
  locator: Delegation
- url: https://go.dev/doc/effective_go#embedding
  title: Effective Go
  locator: Embedding
- url: https://go.dev/doc/faq#inheritance
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: Why is there no type inheritance?
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/AbstractCollection.html#addAll(java.util.Collection)
  title: AbstractCollection, Java SE 25 API documentation
  locator: addAll, Implementation Requirements
- url: https://docs.python.org/3/library/collections.html#userdict-objects
  title: collections, Container datatypes, The Python Standard Library
  locator: UserDict objects
---

## Idea

Composition over inheritance is the design rule that a class should get behavior by holding other objects and calling them, not by extending a base class. Inheritance says that a type is a kind of another type, and it gives the subclass every method and field of the base class. Composition says that a type has parts, and the type decides which operations of its parts it offers. With composition, a class depends only on the interfaces of its parts, the parts can change at run time, and a change inside a part cannot break the class that holds it. Go and Rust have no inheritance between classes, so composition is their only way to build types from other types. In Java, C#, C++, Python and JavaScript, both are available, and the rule says which to choose by default.

```python run id=idea
class ListStack(list):  # inheritance: a stack is-a list, so it gets every list method
    def push(self, x):
        self.append(x)


class Stack:  # composition: a stack has-a list and shows only what it chooses
    def __init__(self):
        self._items = []

    def push(self, x):
        self._items.append(x)

    def pop(self):
        return self._items.pop()


a = ListStack()
a.push(1)
a.push(2)
a.insert(0, 99)  # allowed, because ListStack is a list
a.sort()  # allowed too, and the stack order is gone
print(a.pop(), hasattr(a, "insert"))

b = Stack()
b.push(1)
b.push(2)
print(b.pop(), hasattr(b, "insert"))


class Plain:
    def format(self, msg):
        return msg


class Loud:
    def format(self, msg):
        return msg.upper() + "!"


class Logger:
    def __init__(self, style):
        self.style = style  # a part, chosen by the caller

    def log(self, msg):
        print(self.style.format(msg))


log = Logger(Plain())
log.log("disk full")
log.style = Loud()  # swap the part at run time
log.log("disk full")
```

```output for=idea
99 True
2 False
disk full
DISK FULL!
```

`ListStack` inherits from `list`, so it is a list: a caller can insert at the bottom and sort the items, and `pop` no longer returns the last item that `push` added. `Stack` holds a list in a field and offers only `push` and `pop`, so no caller can break its order. `Logger` holds a style object and calls its `format` method. Changing the style at run time changes the output with no new class, while a design with inheritance would need a subclass for each style and could not change the style of an existing logger.

## How it works

**Is-a and has-a.** Inheritance states that each object of the subclass is also an object of the base class, and code that expects the base class must work with every subclass. Composition states only that an object uses another object to do part of its work. Many uses of inheritance in practice are a way to reuse code, not a statement that one type is a kind of another, and these uses suit composition better. The Rust book gives the cost: "Inheritance is often at risk of sharing more code than necessary. Subclasses shouldn’t always share all characteristics of their parent class but will do so with inheritance."[^1] A stack that inherits from a list is the classic example: a stack is not a list, because a list allows operations that a stack must forbid. See [class inheritance](/class-inheritance/).

**Forwarding and delegation.** A class that holds a part calls the methods of the part from its own methods. This is called forwarding, and each forwarding method is short. When the part does all the work for a set of methods, a language can write the forwarding for the programmer. Kotlin does it with the keyword `by`: "The Delegation pattern has proven to be a good alternative to implementation inheritance, and Kotlin supports it natively requiring zero boilerplate code."[^2] Go promotes the methods of an embedded field to the outer type, and the Go documentation states the difference from inheritance: "When we embed a type, the methods of that type become methods of the outer type, but when they are invoked the receiver of the method is the inner type, not the outer one."[^3] In both cases, the part never calls back into the outer object, so the part's own calls stay inside the part.

**Parts through interfaces.** A class gets the most from composition when it depends on an interface, not on a concrete class. Then any object that implements the interface can be the part: a hash set or a tree set, a writer to a file or to a test buffer. The caller chooses the part and passes it in, which is [dependency injection](/dependency-injection/). Go goes further and needs no declaration at all. The Go FAQ explains the choice: "Object-oriented programming, at least in the best-known languages, involves too much discussion of the relationships between types, relationships that often could be derived automatically."[^4] A Go type satisfies every interface whose methods it has, so a part needs no common base class. See [interfaces](/interfaces/).

**The fragile base class.** A subclass depends on the implementation of its base class, not only on its interface. When a method of the base class calls another method that the subclass overrides, the subclass changes the behavior of the base method, often without the author of either class knowing. The Java documentation of `AbstractCollection.addAll` states such a dependency: "This implementation iterates over the specified collection, and adds each object returned by the iterator to this collection, in turn."[^5] A subclass of `HashSet` that counts in both `add` and `addAll` therefore counts every element twice, and a subclass that counts only in `add` breaks when a later version of the base class changes `addAll`. Composition avoids the problem, because the part calls only its own methods, and the outer class does not override anything. See [dynamic dispatch](/dynamic-dispatch/).

**Change at run time.** The base class of an object is fixed when the program defines the class, but a part is a field, and the program can replace it at any time. A logger can change its destination, a game character its weapon, and a parser its error handler. The strategy, decorator and proxy patterns all rest on this. A combination of features with inheritance needs a subclass for each combination: three formats and four destinations need twelve subclasses, but with composition they need seven small classes and one logger. See [the strategy pattern](/strategy-pattern/) and [the decorator pattern](/decorator-pattern/).

**When inheritance is right.** The rule is a default, not a ban. Inheritance fits when the subclass is a true subtype that keeps every promise of the base class, when the base class is designed and documented for extension, and when a framework expects it, as GUI toolkits and test frameworks often do. An abstract base class that implements a large interface from a few abstract methods is a safe kind of inheritance, because its authors document which methods call which. Python's `collections.abc` classes and Java's `AbstractList` work in this way. The Python documentation describes the trade-off for wrapping a dictionary: "The need for this class has been partially supplanted by the ability to subclass directly from dict; however, this class can be easier to work with because the underlying dictionary is accessible as an attribute."[^6]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | fields, duck typing, `collections.abc` mixins |
| JavaScript | idiom | private fields, objects built with `Object.assign` |
| TypeScript | idiom | as JavaScript, with interfaces for parts |
| Java | idiom | interfaces and forwarding classes |
| Kotlin | native | class delegation with `by` |
| C# | idiom | interfaces, forwarding, delegates |
| Go | native | struct fields and embedding; no inheritance |
| Rust | native | struct fields, traits, default methods; no inheritance |
| C | idiom | struct members and function pointers |
| C++ | idiom | members, templates, `std::function` |
| Bash | n/a | no objects |
| PowerShell | idiom | properties that hold objects |
| SQL | n/a | no classes |
| Regex | n/a | no objects |
| HTML | idiom | elements nested in custom elements, slots |
| CSS | n/a | no objects |

The table splits the languages by what they leave out. Go and Rust have no inheritance between classes, so composition is native: Go embeds structs and promotes their methods, and Rust shares behavior through traits and their default methods. Kotlin has inheritance but makes classes final by default and writes the forwarding methods for a class that delegates an interface. The other languages with classes support both, and composition is an idiom that the programmer writes by hand, with one forwarding method for each operation.

Each example file builds a set or a map that counts the elements that callers add, by holding a standard collection in a field, and most show a part that changes at run time. The wrong blocks show what goes wrong with inheritance in each language: methods of `dict` that skip the override in Python, a `Map` constructor that calls the override too early in JavaScript, double counting through `addAll` in Java, a hidden method that interface calls skip in C#, public methods of `std::vector` that break an invariant in C++, and, in Go, embedding that does not act as an override.

## Variations and related ideas

- **Delegation.** In prototype languages, an object passes the lookups it cannot answer to another object, and `this` still refers to the first object. This is inheritance between objects, not classes. See [prototype-based programming](/prototype-based-programming/).
- **Mixins and traits.** A mixin or trait adds a set of methods to a class without a full base class. Python mixins, Ruby modules, Scala traits and Rust traits with default methods share code in this way, between composition and inheritance.
- **Decorators and proxies.** A wrapper holds a part with the same interface and adds behavior before or after each call, such as logging or caching. See [the decorator pattern](/decorator-pattern/) and [the proxy pattern](/proxy-pattern/).
- **Strategy and dependency injection.** A class receives the parts it uses from its caller, so tests and configurations can supply different parts. See [the strategy pattern](/strategy-pattern/) and [dependency injection](/dependency-injection/).
- **Entity component systems.** Game engines describe each game object as a set of components, such as a position, a sprite and a physics body, and systems run over all objects with a given set of components.
- **Generic parts.** A class parameterized by the type of its part, such as `Counting<S: Set>` in Rust or a policy class in C++, gets composition with no run-time cost. See [parametric polymorphism](/parametric-polymorphism/).
- **The expression problem.** Adding new operations or new variants to a type hierarchy is the same tension seen from another side. See [the expression problem](/expression-problem/).

## Pitfalls

- **Inheritance for code reuse.** A class that extends another only to reuse a few of its methods also gets all its other methods and the obligation to keep its promises. The wrong block in the C++ file shows a sorted vector whose base class lets callers break the order.
- **Overrides that the base class skips or calls twice.** Whether a base method calls an overridable method is an implementation detail, and a subclass that depends on it breaks. The wrong blocks in the Python, Java and C# files show both cases.
- **Overrides called from constructors.** A base constructor that calls an overridable method runs the subclass code before the subclass is ready. The wrong block in the JavaScript file shows this. See [constructors](/constructors/).
- **Embedding mistaken for inheritance.** In Go, a method of an embedded type calls the methods of the embedded type, never the outer type's versions. The wrong block in the Go file shows this.
- **Too much forwarding.** Wrapping a large interface by hand gives long, repetitive classes, and a forwarding method that is missing or wrong is easy to overlook. Wrap a narrow interface, generate the forwarding, or use language support such as Kotlin's `by`.
- **Identity and equality.** A wrapper is a different object from its part, so a check of identity or an equality method that compares classes can fail across the wrapper. Decide which object callers see, and define equality on the wrapper if needed. See [value equality and identity](/value-equality-and-identity/).
- **Too many small objects.** Splitting every behavior into a separate part can make a simple class hard to follow. Compose where parts vary or are reused, and keep simple code in one place.

## Cost

A forwarding method costs one extra call, which compilers for C++, Rust, Go, Java and C# usually inline. A part held through an interface costs a virtual call, the same as an overridden method, and a part held as a generic parameter or a direct field costs nothing beyond a normal call. Each part held by pointer costs an allocation and an indirection, while a part embedded by value, as in C, C++, Go and Rust structs, sits inside the outer object with no extra memory. The main cost is code to write: forwarding methods by hand in Java, C#, C++ and Python, none in Kotlin with `by` and Go with embedding.

## History

Alan Snyder argued in 1986 that inheritance breaks encapsulation, because a subclass sees the implementation of its base class, and Henry Lieberman described delegation between objects in the same year as an alternative to classes. The book *Design Patterns* by Erich Gamma, Richard Helm, Ralph Johnson and John Vlissides gave the rule its usual form in 1994: favor object composition over class inheritance. Microsoft's COM, from 1993, had no inheritance of implementation, and components reused each other by containment and aggregation. Research papers in the late 1990s named the fragile base class problem and studied it formally.

Joshua Bloch's *Effective Java*, from 2001, made the rule familiar to Java programmers with the example of a set that counts its additions, which the Java file follows. Go, released in 2009, left out inheritance and offered embedding and implicit interfaces in its place.[^4] Rust, from 2015, shares behavior only through traits.[^1] Kotlin, from 2016, made classes final by default and added class delegation with `by`.[^2] Game engines adopted entity component systems in the 2000s, which apply the same rule to game objects.

## References

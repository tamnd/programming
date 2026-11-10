---
tag: NEW-facade-pattern
kind: pattern
slug: facade-pattern
name: Facade pattern
aliases: [Facade, Façade, front end of a subsystem]
area: software-design/structural-patterns
level: core
summary: >-
  One object or module gives a single, higher-level interface to a subsystem of many classes. Clients call the facade, and the facade calls the subsystem in the right order.
family: structural-design
polarity: positive
intent: "Provide a unified interface to a set of interfaces in a subsystem, through a higher-level interface that clients can use with less knowledge of the subsystem."
also_known_as: []
refactorings: [hide-delegate, remove-middle-man]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with a compiler subsystem as the motivating example.
see_also: [adapter-pattern, proxy-pattern, abstract-factory-pattern, singleton-pattern, decorator-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Facade (Intent, Motivation, Applicability, Participants, Collaborations, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (modules make Facade invisible) and slide 29 (Pattern, Facade)
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/subprocess.py#L512-L579
    title: CPython v3.14.0 source, Lib/subprocess.py
    locator: function run, lines 512 to 579
  - url: https://docs.python.org/3.14/library/subprocess.html#using-the-subprocess-module
    title: The Python Standard Library, subprocess, Python 3.14
    locator: Using the subprocess Module
  - url: https://docs.python.org/3.14/library/shutil.html#module-shutil
    title: The Python Standard Library, shutil, Python 3.14
    locator: module introduction
  - url: https://docs.python.org/3.14/tutorial/modules.html#importing-from-a-package
    title: The Python Tutorial, Modules, Python 3.14
    locator: 6.4.1 Importing * From a Package
  - url: https://go.dev/ref/spec#Exported_identifiers
    title: The Go Programming Language Specification
    locator: Exported identifiers
  - url: https://doc.rust-lang.org/reference/items/use-declarations.html#use-visibility
    title: The Rust Reference, Use declarations
    locator: use Visibility
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html#jls-7.7
    title: The Java Language Specification, Java SE 25 Edition
    locator: 7.7 Module Declarations
  - url: https://refactoring.com/catalog/hideDelegate.html
    title: Martin Fowler, Refactoring catalog, Hide Delegate
    locator: catalog entry
  - url: https://refactoring.com/catalog/removeMiddleMan.html
    title: Martin Fowler, Refactoring catalog, Remove Middle Man
    locator: catalog entry (inverse of Hide Delegate)
---

## Intent

Provide a unified interface to a set of interfaces in a subsystem. The facade defines a higher-level interface, so that clients can use the subsystem with less knowledge of it.[^1] Clients call the facade. The facade knows which classes of the subsystem to call, and in which order.

## Problem

A compiler is a subsystem of many classes: a scanner, a parser, a builder of syntax trees, a code generator. A few clients need these parts one at a time. Most clients only want to compile a program. If each client must make the scanner, give its tokens to the parser and pass the tree to the code generator, then each client depends on all of these classes. Design Patterns starts from this example.[^1]

The forces are these. A subsystem is easier to build and test when it is made of small classes. But small classes make a large surface for clients. Each client that uses the classes directly repeats the same steps, and it breaks when the subsystem changes. And some clients still need the small classes, so the subsystem must not hide them from everyone.

## Solution

Design Patterns names two participants.[^1]

- **Facade** knows which subsystem classes handle a request, and passes each request to the right objects.
- **Subsystem classes** do the work. They do not know about the facade and hold no reference to it.

Clients send requests to the facade. The facade may do some work of its own, such as putting the parts together, choosing the order of the calls, or undoing a step when a later step fails.

The examples model a shop with three parts: an inventory, a payment service and a shipping service. The facade has one operation, `place_order`. It reserves the item, charges the card, and schedules a parcel. When the charge fails, it puts the item back in stock. A client that calls the parts on its own must get these rules right each time. This one charges the card before it checks the stock:

```python wrong id=bypass
stock = {"lamp": 0}
charged = []


def charge(card, amount):
    charged.append(amount)
    return card != "expired"


def reserve(item):
    if stock[item] == 0:
        return False
    stock[item] -= 1
    return True


# A client that uses the subsystem directly, in its own order.
if charge("visa", 30.00):
    if not reserve("lamp"):
        print("failed: lamp is out of stock")
print(f"charged: {charged}")
```

```output for=bypass
failed: lamp is out of stock
charged: [30.0]
```

The customer paid for a lamp that the shop does not have. The facade puts the order of the steps in one place, so that every client gets the same order.

### Facades and modules

A facade in a class-based language is often a class with one instance. Norvig lists Facade as the one pattern that modules make invisible. His slide on Facade says that a program with modules needs no bookkeeping objects or classes: it exports only the names that make up the interface.[^2] Most languages in the example files have this tool:

- A Go package exports a name only when it starts with an upper-case letter.[^7]
- Items in a Rust module are private unless they are marked `pub`. A `pub use` gives an item a public path, so a library can show one front for many inner modules.[^8]
- A Java module declares which packages it exports. Other modules cannot use the classes of a package that is not exported.[^9]
- A Python module has no private names. A leading underscore marks a name as internal, and an `__all__` list gives the names that `from module import *` imports.[^6]
- In C, a header declares the facade, and `static` functions in the `.c` file stay out of it.

With modules, the facade is the public part of the module, and the subsystem is the rest. No class is needed when the facade has no state of its own. When it has state, a class or a struct holds it, as in the Java and C# examples.

### Facades and dependency injection

Design Patterns suggests two ways to reduce coupling between clients and the subsystem. Make the facade an abstract class with a concrete subclass for each implementation of the subsystem. Or configure the facade with the subsystem objects that it uses.[^1] The second way is constructor injection. The Java example passes the inventory, the payments and the shipping service to the constructor of `Shop`. A test can then pass fakes, and the facade does not hide which objects it depends on. The TypeScript example uses the first way: clients hold the interface `Shop`.

## Example

All the example files run one script. A book order succeeds. A second book order fails at the payment, and the book goes back in stock. A lamp order fails, because the lamp is out of stock. At the end, one book is left.

| Language | Facade |
|---|---|
| Python | functions at the top level of a module, with private subsystem objects |
| JavaScript | a function that makes the subsystem and returns an object with two methods |
| TypeScript | an interface and a class that implements it |
| Java | a class that receives the subsystem through its constructor |
| C# | a class with a primary constructor and private fields |
| Go | the exported type of a package, over unexported types |
| Rust | the `pub` items of a module |
| C++ | a class over a `detail` namespace |
| C | the non-`static` functions of a file |
| SQL | a view over three tables |

## Consequences

Design Patterns lists three consequences.[^1] A facade shields clients from the subsystem, so they deal with fewer objects. It gives weak coupling between the subsystem and its clients, so the subsystem can change without a change to the clients. A program can also use facades to layer a system: each layer talks to the next one through that layer's facade. And a facade does not stop clients from using the subsystem classes when they need to. A program can choose between the short path of the facade and the full control of the subsystem.

The last point is the main difference from information hiding. Design Patterns treats the subsystem classes as public or private as a separate question.[^1] A facade gives a short path for the common case. It is not a wall. Python's `subprocess` module shows this. Its documentation recommends `run()` for all the cases it can handle, and says that the underlying `Popen` interface can be used directly for advanced cases.[^4] The first call below is the facade, and the second is the subsystem under it:

```python run id=subprocess-facade
import subprocess
import sys

command = [sys.executable, "-c", "print('hello')"]

# The facade: start, wait, collect the output, and check the timeout in one call.
done = subprocess.run(command, capture_output=True, text=True, timeout=10)
print("run:", done.returncode, done.stdout.strip())

# The subsystem: the same work with Popen and communicate.
with subprocess.Popen(command, stdout=subprocess.PIPE, text=True) as process:
    out, _ = process.communicate(timeout=10)
print("popen:", process.returncode, out.strip())
```

```output for=subprocess-facade
run: 0 hello
popen: 0 hello
```

`run` itself is a facade over `Popen`. It makes a `Popen`, calls `communicate`, kills the process on a timeout or an error, raises `CalledProcessError` when `check` is true and the exit status is not zero, and returns a `CompletedProcess`.[^3]

The costs are these. A facade is one more layer that a reader must go through. It can grow into a class that knows every part of the program, because each added task is a small change to it. And a facade that only passes each call through, with the same arguments, adds a layer and no value.

## When not to use it

Do not write a facade over a subsystem that has one or two classes. The clients can use them directly.

Do not write a facade that forwards every method of one object. Fowler's refactoring Remove Middle Man removes such a class, so that clients call the delegate directly. It is the inverse of Hide Delegate, which adds a method to hide the delegate.[^11][^10] A facade should give a smaller or higher-level interface than the parts it covers.

Do not put business rules in the facade that belong to one part of the subsystem. The facade decides the order of the calls and the work that connects them. A rule about stock belongs in the inventory.

Do not write a facade class in a language that has modules, when the facade has no state. Export the functions of the interface and keep the rest private.[^2]

## Variants and related patterns

**Adapter.** An adapter changes the interface of one object to an interface that a client expects. A facade defines its own interface over many objects. See [[adapter-pattern]].

**Proxy and Decorator.** A proxy and a decorator have the same interface as the one object that they wrap. A facade has its own interface. See [[proxy-pattern]] and [[decorator-pattern]].

**Mediator.** Design Patterns compares the two. Both abstract the work of existing classes. A mediator handles the communication between colleague objects, and the colleagues know the mediator. A facade only abstracts the interface to a subsystem, and the subsystem does not know the facade.[^1]

**Abstract Factory.** Design Patterns says that an abstract factory can work with a facade, to make subsystem objects in a way that does not depend on the subsystem. An abstract factory can also replace a facade, to hide the classes that are specific to a platform.[^1] See [[abstract-factory-pattern]].

**Singleton.** Design Patterns notes that a program usually needs one facade object, so a facade is often a singleton.[^1] A module, or an object that the program makes once and passes to its clients, gives one facade without the costs of a global. See [[singleton-pattern]].

**Refactorings.** Fowler's Hide Delegate adds a method to a class, so that its clients do not reach through it to the delegate. Remove Middle Man is its inverse.[^10][^11]

## Known uses

- Python: `subprocess.run` is a facade over `Popen`, `communicate`, `kill` and `CalledProcessError`. See [its source at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/subprocess.py#L512-L579).[^3][^4]
- Python: the `shutil` module offers high-level operations on files and collections of files, such as copying and removing. Its documentation points to the `os` module for operations on single files.[^5]

## References

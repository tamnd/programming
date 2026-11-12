---
tag: WKQWQK
kind: pattern
slug: null-object-pattern
name: Null Object
aliases: [null object pattern, null object, Stub, Active Nothing]
area: software-design/behavioral-patterns
level: core
summary: >-
  A null object has the same interface as a real collaborator but does nothing. A client can use it in place of null, so the client needs no checks for null.
family: behavioral-design
polarity: positive
intent: "Provide a surrogate for another object that shares the same interface but does nothing, so that clients do not have to check for nil or some other special value."
also_known_as: [Stub, Active Nothing]
history:
  year: 1996
  people: [Thomas Kühne, Bobby Woolf]
  note: Kühne published the pattern as Void Value at WOON'96 in 1996. Woolf published it as Null Object in Pattern Languages of Program Design 3 in 1998.
see_also: [null-references, strategy-pattern, state-pattern, singleton-pattern, proxy-pattern, dynamic-dispatch]
sources:
  - url: https://www.cs.ox.ac.uk/people/jeremy.gibbons/dpa/nullobject.pdf
    title: "Bobby Woolf, The Null Object Pattern, Knowledge Systems Corp., draft of 07/28/96"
    locator: Intent; Also Known As; Motivation; Applicability; Participants; Consequences; Implementation 1, 2, 4, 5, 6, 7; Known Uses; Related Patterns
  - url: https://en.wikipedia.org/wiki/Null_object_pattern
    title: Null object pattern, Wikipedia
    locator: Introduction; Description; Relation to other patterns; Criticism
  - url: https://martinfowler.com/eaaCatalog/specialCase.html
    title: Martin Fowler, Special Case, Catalog of Patterns of Enterprise Application Architecture
    locator: Special Case
  - url: https://refactoring.com/catalog/introduceSpecialCase.html
    title: Martin Fowler, Introduce Special Case, Catalog of Refactorings
    locator: Introduce Special Case, aliases
  - url: https://doc.rust-lang.org/std/option/index.html
    title: The Rust Standard Library, Module std::option
    locator: Module documentation, first paragraph
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Optional.html
    title: Java SE 25 API, Class Optional
    locator: Class description
  - url: https://docs.python.org/3/library/contextlib.html
    title: The Python Standard Library, contextlib, Utilities for with-statement contexts
    locator: contextlib.nullcontext
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/contextlib.py#L775-L799
    title: CPython v3.14.0 source, Lib/contextlib.py
    locator: class nullcontext, lines 775 to 799
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/logging/__init__.py#L2265-L2282
    title: CPython v3.14.0 source, Lib/logging/__init__.py
    locator: class NullHandler, lines 2265 to 2282
  - url: https://github.com/golang/go/blob/go1.25.0/src/io/io.go#L637-L649
    title: Go go1.25.0 source, src/io/io.go
    locator: Discard, lines 637 to 649
---

## Intent

Null Object provides a surrogate for another object. The surrogate shares the same interface but does nothing. It encapsulates the implementation decisions of how to "do nothing" and hides those details from its collaborators.[^1]

## Problem

An object often needs a collaborator, and sometimes no real collaborator is available. A common solution is a null reference. A null reference does not implement the expected interface, so the calling code needs conditional checks.[^2] Fowler says that nulls "defeat polymorphism". If a variable can be null, you must surround it with null test code, and often you write similar code in many places.[^3]

Here a checkout takes an optional logger. `pay` checks for `None`, but `refund` does not:

```python wrong id=forgotten-check
class ConsoleLogger:
    def log(self, message):
        print(f"log: {message}")


class Checkout:
    def __init__(self, logger=None):
        self.logger = logger

    def pay(self, amount):
        if self.logger is not None:
            self.logger.log(f"paid {amount}")
        return f"receipt: {amount}"

    def refund(self, amount):
        self.logger.log(f"refunded {amount}")  # the check is missing
        return f"refund: {amount}"


checkout = Checkout()
print(checkout.pay(10))
try:
    print(checkout.refund(10))
except AttributeError as error:
    print(f"error: {error}")
```

```output for=forgotten-check
receipt: 10
error: 'NoneType' object has no attribute 'log'
```

## Solution

Make a class that implements the interface of the collaborator and does nothing in each method. Give a client an instance of this class instead of null.[^1]

```text
     Client  ---->  AbstractObject
                     request()
                        ^
              __________|__________
             |                     |
        RealObject             NullObject
        request():             request():
        real behaviour         do nothing
```

The participants work as follows:[^1]

- **Client** requires a collaborator.
- **AbstractObject** declares the interface for the collaborator of the client.
- **RealObject** is a concrete subclass whose instances provide the useful behaviour that the client expects.
- **NullObject** provides an interface identical to that of AbstractObject, so that a null object can be substituted for a real object. It implements the interface to do nothing. Some requests can give a null result.

What it means to do nothing is subjective. It depends on the behaviour that the client expects. When there is more than one way to do nothing, more than one NullObject class can be necessary.[^1]

## Example

The example files show one program in each language. A `Checkout` takes a logger with one method, `log`. `ConsoleLogger` prints each message, and `NullLogger` does nothing. The checkout calls its logger with no check. The program pays 10 with a console logger and 20 with a null logger, and prints:

```text
log: paid 10
receipt: 10
receipt: 20
```

## Consequences

The benefits:[^1]

- Client code is simple. Clients treat real and null collaborators uniformly, and they do not need code that handles the null collaborator specially.
- The do-nothing code is in one place, and it is easy to find and to reuse. All clients that need a collaborator to do nothing do nothing the same way.

The costs:

- **A new class for each interface.** The pattern can necessitate a new NullObject class for every new AbstractObject class.[^1]
- **Disagreement.** The pattern can be difficult to implement if different clients do not agree on how the null object should do nothing.[^1]
- **Hidden errors.** The pattern can make errors appear as normal program execution.[^2]

Here a lookup returns a null customer for an unknown id. The caller has a typing error in the id, but the program prints a bill and no error:

```python run id=hidden-error
class Customer:
    def __init__(self, name, balance):
        self.name = name
        self.balance = balance


class NullCustomer:
    name = "occupant"
    balance = 0


customers = {"c-1": Customer("Ada", 30)}


def find_customer(customer_id):
    return customers.get(customer_id, NullCustomer())


customer = find_customer("c-l")  # a typing error: "l" for "1"
print(f"bill for {customer.name}: {customer.balance}")
```

```output for=hidden-error
bill for occupant: 0
```

Where the difference matters to its logic, the caller can still check explicitly, for example whether a returned list is empty.[^2]

## When not to use it

- Some clients must act differently when no collaborator is present. Then the logic that is specific to null moves into the clients again, and the code can become harder to read.[^2]
- The object must change from doing nothing to real behaviour. A null object does not transform into a real object. A single object that must change in this way should be a Proxy.[^1]
- The do-nothing behaviour must be mixed into the real behaviour of several collaborating objects. The same do-nothing behaviour cannot easily be added to several classes, unless they all delegate to a class that can be a null object class.[^1]

## Variants and related patterns

- **Singleton.** A null object usually has no state, so many instances are identical. The NullObject class is often a Singleton.[^1]
- **Special instance.** A special instance of RealObject with null values can be the null object, so no subclass is necessary. For example, a composite object whose list of children is empty acts like a leaf.[^1] In the same way, a function can return an empty list instead of null, and the caller iterates over it with no special handling.[^2]
- **Strategy and State.** A null object can be a special case of Strategy, the strategy that does nothing. It can also be a special case of State, the state in which the client does nothing.[^1][^2]
- **Proxy.** A proxy controls access to a real subject. A null object does not hide a real object; it replaces the real object.[^1]
- **Special Case.** The summary of Fowler's Special Case is "A subclass that provides special behavior for particular cases". Instead of null or an odd value, a function returns a special case that has the same interface as what the caller expects.[^3] The refactoring Introduce Special Case has the alias Introduce Null Object.[^4]
- **Option types.** An option type represents an optional value in the type itself. In Rust, every `Option` is either `Some` and contains a value, or `None`.[^5] Java's `Optional` is a container object that can contain a non-null value or not. It is primarily for method return types where "no result" must be represented and where null is likely to cause errors.[^6] In contrast, clients of a null object normally do not know whether they have a real or a null collaborator.[^1]

## Known uses

- In VisualWorks Smalltalk, `NoController` is a controller for a view that ignores all input. `NullDragMode` does nothing when the user drags a visual that cannot be resized, and `NullScope` is the outer scope of the global scope.[^1]
- Python's `contextlib.nullcontext` is a context manager that does no additional processing. It is a stand-in for an optional context manager.[^7][^8]
- Python's `logging.NullHandler` is a handler that does nothing. A library adds it to its top-level logger, so a user who does not configure logging gets no warning.[^9]
- Go's `io.Discard` is a `Writer` on which all `Write` calls succeed without doing anything.[^10]

## References

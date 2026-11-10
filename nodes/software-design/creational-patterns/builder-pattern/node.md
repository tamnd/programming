---
tag: NEW-builder-pattern
kind: pattern
slug: builder-pattern
name: Builder pattern
aliases: [Builder, builder object, fluent builder]
area: software-design/creational-patterns
level: core
summary: >-
  A separate object collects the parts of a complex object one step at a time and then makes the finished object. In languages with named and default arguments, the call itself often does this work.
family: creational-design
polarity: positive
intent: "Separate the construction of a complex object from its representation, so that the same construction steps can make different results, and so that a caller can give the parts by name, in steps, and check them before the object exists."
also_known_as: []
refactorings: [encapsulate-composite-with-builder]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with a director that drives the steps and builders that make different products. Bloch's Effective Java later made the form without a director, a builder for one class with many optional parameters, common in Java.
see_also: [factory-method-pattern, abstract-factory-pattern, composite-pattern, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Builder (Intent, Motivation, Participants, Collaborations, Consequences, Implementation, Related Patterns)
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Chapter 2, Item 2, Consider a builder when faced with many constructor parameters
  - url: https://docs.python.org/3.14/tutorial/controlflow.html#keyword-arguments
    title: The Python Tutorial, Python 3.14
    locator: 4.9.2 Keyword Arguments
  - url: https://docs.python.org/3.14/tutorial/controlflow.html#default-argument-values
    title: The Python Tutorial, Python 3.14
    locator: 4.9.1 Default Argument Values (the warning that the default value is evaluated only once)
  - url: https://docs.python.org/3.14/library/dataclasses.html#dataclasses.dataclass
    title: The Python Standard Library, dataclasses, Python 3.14
    locator: dataclasses.dataclass (the frozen and kw_only parameters; mutable default values)
  - url: https://docs.python.org/3.14/library/dataclasses.html#dataclasses.replace
    title: The Python Standard Library, dataclasses, Python 3.14
    locator: dataclasses.replace
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/named-and-optional-arguments
    title: C# Programming Guide, Named and Optional Arguments
    locator: Named arguments; Optional arguments
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/object-and-collection-initializers
    title: C# Programming Guide, Object and Collection Initializers
    locator: Object initializers
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/required
    title: C# language reference, required modifier
    locator: description
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/with-expression
    title: C# language reference, with expression
    locator: description
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.init.list#3.1
    title: Working Draft, Programming Languages, C++ (N4950)
    locator: "[dcl.init.list] paragraph 3.1 (designators must follow the declaration order)"
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.init.aggr
    title: Working Draft, Programming Languages, C++ (N4950)
    locator: "[dcl.init.aggr] paragraphs 1 and 5 (the definition of an aggregate; default member initializers for members that are not explicitly initialized)"
  - url: https://commandcenter.blogspot.com/2014/01/self-referential-functions-and-design.html
    title: Rob Pike, Self-referential functions and the design of options, command center, January 24, 2014
    locator: the option type func(*Foo) and the version that returns the previous value
  - url: https://rust-lang.github.io/api-guidelines/type-safety.html#c-builder
    title: Rust API Guidelines, Type safety
    locator: C-BUILDER, Builders enable construction of complex values (non-consuming and consuming builders)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.net.http/java/net/http/HttpRequest.Builder.html
    title: Java SE 25 API, java.net.http.HttpRequest.Builder
    locator: interface description and build
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (multimethods make Builder simpler) and slides 25 to 27 (Builder with multimethods)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/Default_parameters#evaluated_at_call_time
    title: MDN Web Docs, Default parameters
    locator: Evaluated at call time
  - url: https://kotlinlang.org/docs/functions.html#named-arguments
    title: Kotlin documentation, Functions
    locator: Parameters with default values; Named arguments
  - url: https://github.com/rust-lang/rust/blob/1.98.0/library/std/src/process.rs#L530-L533
    title: Rust 1.98.0 source, library/std/src/process.rs
    locator: the documentation of struct Command, lines 530 to 533
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.net.http/share/classes/java/net/http/HttpRequest.java#L95-L117
    title: OpenJDK jdk-25+36 source, java/net/http/HttpRequest.java
    locator: the documentation of HttpRequest.Builder, lines 95 to 117, and of build, lines 295 to 315
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 6, Creation, Encapsulate Composite with Builder
  - url: https://docs.python.org/3.14/tutorial/controlflow.html#special-parameters
    title: The Python Tutorial, Python 3.14
    locator: 4.9.3 Special parameters (keyword-only arguments)
---

## Intent

Separate the construction of a complex object from its representation. The same construction steps can then make different results. Design Patterns states the intent this way.[^1] The form that most code uses has a narrower goal. It lets a caller give many parts of one object by name and leave out the parts that have defaults. At the end, the caller gets a checked and often immutable object.[^2]

## Problem

A constructor takes its arguments by position. This works for two or three parts. It fails when an object has many parts and most of them are optional.

Take an HTTP request. It needs a URL. It also has a method, a list of headers, a timeout and a number of retries, and each of them has a sensible default. A Java class with no builder offers a chain of constructors, each with one more parameter. Bloch calls this the telescoping constructor pattern.[^2] The caller must count the positions, and two parameters of the same type can change places with no error:

```java wrong id=telescoping
public class Main {
    record Request(String url, int timeout, int retries) {
        Request(String url) {
            this(url, 30);
        }

        Request(String url, int timeout) {
            this(url, timeout, 0);
        }
    }

    public static void main(String[] args) {
        // The caller means 3 retries and a timeout of 10 seconds.
        System.out.println(new Request("https://example.com/orders", 3, 10));
    }
}
```

```output for=telescoping
Request[url=https://example.com/orders, timeout=3, retries=10]
```

The compiler accepts the call, and the request waits 3 seconds and tries 10 times. A second answer is a constructor with no arguments and a setter for each part. Bloch names its two faults. The object is in an inconsistent state between the calls, and it cannot be immutable.[^2]

The problem in Design Patterns is wider. An RTF reader must convert a document to plain text, to another rich format, or to a widget that shows the text. The reading algorithm is the same for each output. Only the product differs, and the set of products must stay open.[^1]

So the forces are these. The caller wants to name each part and leave out the defaults. The object wants to check all of its parts together and then stay fixed. In the wider case, one construction process wants to make products that have nothing in common.

## Solution

Design Patterns has four participants.[^1]

- **Builder** declares one operation for each part of the product.
- **ConcreteBuilder** makes and assembles the parts, keeps the product that it is making, and gives a way to get the result.
- **Director** calls the builder's operations in the right order. It knows the steps, not the product.
- **Product** is the complex object that the builder makes.

In the classic form, the director reads its input and calls a builder for each part. The client makes the concrete builder, passes it to the director, and gets the product from the builder at the end. The product does not come from the director, because the products of different builders often have no common type.[^1] Here, one director writes an order to two builders. One makes a receipt text, and the other makes a pair of totals:

```python run id=director
from abc import ABC, abstractmethod


class OrderBuilder(ABC):
    @abstractmethod
    def add_line(self, name: str, count: int, cents: int) -> None: ...

    @abstractmethod
    def add_note(self, text: str) -> None: ...


class ReceiptBuilder(OrderBuilder):
    def __init__(self) -> None:
        self.lines: list[str] = []

    def add_line(self, name, count, cents):
        self.lines.append(f"{count} x {name} {count * cents / 100:.2f}")

    def add_note(self, text):
        self.lines.append(f"note: {text}")

    def result(self) -> str:
        return "\n".join(self.lines)


class TotalsBuilder(OrderBuilder):
    def __init__(self) -> None:
        self.items = 0
        self.cents = 0

    def add_line(self, name, count, cents):
        self.items += count
        self.cents += count * cents

    def add_note(self, text):
        pass  # totals have no place for a note

    def result(self) -> tuple[int, int]:
        return self.items, self.cents


def write_order(order: dict, builder: OrderBuilder) -> None:
    """The director: it knows the steps, not the product."""
    for name, count, cents in order["lines"]:
        builder.add_line(name, count, cents)
    if order.get("note"):
        builder.add_note(order["note"])


order = {"lines": [("pen", 3, 150), ("book", 1, 1200)], "note": "gift wrap"}
receipt = ReceiptBuilder()
write_order(order, receipt)
print(receipt.result())
totals = TotalsBuilder()
write_order(order, totals)
print(totals.result())
```

```output for=director
3 x pen 4.50
1 x book 12.00
note: gift wrap
(4, 1650)
```

The common form has no director. The builder belongs to one product class. It has a setter for each optional part, and each setter returns the builder, so the calls chain. The required parts go to the builder's constructor. A final `build` method checks the parts together and makes the product.[^2] The Java example file has this form. The standard library of Java uses it in `HttpRequest.newBuilder`.[^15]

### Named and default arguments

The common form is a workaround for a missing language feature. Bloch says so: a builder simulates the named optional parameters of Python and Scala.[^2] Where a language has named arguments with default values, the call does the builder's work:

| Language | Form | What it gives |
|---|---|---|
| Python | keyword arguments with defaults; `kw_only=True` on a dataclass[^3][^5][^22] | each part named; defaults; no swaps by position |
| C# | named and optional arguments; object initializers with `required` and `init`[^7][^8][^9] | each part named; a missing required part is a compile error |
| Kotlin | named arguments and parameters with default values[^18] | each part named; defaults |
| C++ | designated initializers on an aggregate with default member initializers[^11][^12] | each part named in declaration order; defaults |
| JavaScript, TypeScript | an options object with destructuring defaults[^17] | each part named; defaults |
| Java, Go, Rust | no named or default arguments | a builder or an options value is the usual answer |

Named arguments cover the main problem: the reader sees which value is which, and no two parts can change places. A constructor with named arguments can check the parts together, as `build` does. A copy with changes, such as `dataclasses.replace` in Python or `with` in C#, takes the place of a builder that starts from an existing object.[^6][^10]

A builder still does three things that one call does not. It can collect parts across many statements, under conditions, or in a loop. It can be passed to other code that adds parts. And it can be kept as a template from which the program builds several products. The Rust API Guidelines prefer a builder whose setters take `&mut self`. The reason is that the same builder then works in a chain and also in a variable that gets parts under a condition.[^14]

Go has no named or default arguments. Rob Pike described an option as a function that sets one field, and a variadic method that applies any number of options.[^13] The Go example file passes such options to a constructor. This form is often called functional options. Each option is a closure (see [[closures]]) over its value.

## Example

The example files make the same request three times. The first uses only the defaults. The second sets the method, one header, the timeout and the retries. The third asks for a timeout of zero and gets an error from the check:

| Language | Form |
|---|---|
| Python | a frozen, keyword-only dataclass with `__post_init__` and `dataclasses.replace`[^5][^6] |
| JavaScript | a function with an options object, destructuring defaults and `Object.freeze` |
| TypeScript | a typed options object, so a misspelled option is a compile error |
| Java | a static nested builder that returns an immutable record[^2] |
| C# | a record with `required` and `init` properties, an object initializer and `with`[^8][^9][^10] |
| C++ | an options aggregate with designated initializers[^11][^12] |
| Go | functional options[^13] |
| Rust | a non-consuming builder whose `build` returns a `Result`[^14] |

## Consequences

Design Patterns lists three consequences.[^1] A builder lets a program vary the internal representation of a product, because the director sees only the abstract builder. It isolates the code for construction from the code for representation. And it gives finer control over the construction process. The director controls each step, and the product is taken from the builder only at the end.

The form for one class has its own benefits. Each argument is named at the call site. Optional parts can be left out. The product can be immutable, because the builder holds the mutable state. And `build` is one place to check the parts against each other.[^2]

The costs are code and a second type. Each part appears in the product, in the builder's field, in the setter and in `build`. A part added to the product and not to the builder cannot be set. Bloch advises a builder when a class has many parameters, not two or three.[^2] Errors move from compile time to run time. A Java builder that has no setter call for a required part compiles, and `build` must throw. A typestate builder, with one type for each stage, can make this a compile error at the cost of more types.

**A builder that shares its state with the product.** The builder is mutable, and the product should not be. When `build` passes a list from the builder to the product, the product and the builder share the list. A later setter call then changes a product that the program built before. A frozen dataclass does not help, because it stops assignment to the field, not changes inside the list:

```python wrong id=shared-state
from dataclasses import dataclass


@dataclass(frozen=True)
class Request:
    url: str
    headers: list


class RequestBuilder:
    def __init__(self, url):
        self.url = url
        self.headers = []

    def header(self, header):
        self.headers.append(header)
        return self

    def build(self):
        return Request(self.url, self.headers)  # the product shares the list


builder = RequestBuilder("https://example.com/orders").header("Accept: text/plain")
first = builder.build()
second = builder.header("Accept-Language: en").build()
print(first.headers)
print(second.headers)
```

```output for=shared-state
['Accept: text/plain', 'Accept-Language: en']
['Accept: text/plain', 'Accept-Language: en']
```

The fix is a copy in `build`, such as `tuple(self.headers)`. The Java example uses `List.copyOf`. The Java `HttpRequest.Builder` documents this contract. Each call to `build` returns an immutable request, and later changes to the builder do not change it.[^20]

**A mutable default argument.** Named arguments with defaults have a trap of their own in Python. A default value is evaluated once, when the function is defined, so a mutable default is shared by every call.[^4] The second request here gets the header of the first:

```python wrong id=mutable-default
def make_request(url, headers=[]):
    headers.append(f"Host: {url.split('/')[2]}")
    return {"url": url, "headers": headers}


make_request("https://example.com/orders")
print(make_request("https://example.org/users")["headers"])
```

```output for=mutable-default
['Host: example.com', 'Host: example.org']
```

JavaScript does not have this trap. A default parameter there is evaluated at each call that leaves the argument out.[^17] The `dataclass` decorator rejects a default of type `list`, `dict` or `set` for this reason, and asks for a `default_factory`:[^5]

```python fails expect=exception:ValueError id=dataclass-default
from dataclasses import dataclass


@dataclass
class Request:
    url: str
    headers: list = []
```

`field(default_factory=list)` makes a fresh list for each instance. The Python example file uses an immutable tuple instead.

## When not to use it

Do not write a builder for a class with a few required parts. A constructor is shorter, and the compiler checks the arity.

Do not write a builder in a language with named and default arguments only to name the parts. Python, C#, Kotlin and C++ do this at the call site with no extra type. Write a builder there when the parts arrive across many statements, or when other code must add parts.

Do not use a builder to hide a long list of parts that do not belong together. Ten optional parameters can be a sign that the class does too much. Grouping the parts into smaller value types, such as a retry policy and a set of headers, can remove the need for the builder.

Do not use the director form when there is one product. A director pays off when one construction process feeds several builders.

## Variants and related patterns

**Fluent builder.** Each setter returns the builder, so the calls chain. Java and Rust builders take this form. A Rust builder can take `self` by value and return it. The guidelines call this a consuming builder, and it suits a product that takes ownership of the builder's data.[^14] The non-consuming form takes `&mut self` and is the one the guidelines prefer.[^14]

**Options object and functional options.** JavaScript passes one object with named properties. Go passes a list of functions, each of which sets one part.[^13] In Pike's final version, each option returns a second option that restores the previous value, so a caller can set an option for a while and then undo it. That is a small form of the undo stack of the [[command-pattern]].[^13]

**Multimethods.** Norvig shows that with multimethods, a converter dispatches on the reader and the target together, and the classic form needs no builder or director classes.[^16]

**Abstract Factory.** Both patterns create objects for a client that does not name the concrete classes. Design Patterns contrasts them. Abstract Factory makes a family of products, and each product returns at once. Builder makes one complex product step by step, and the product returns as a final step.[^1] See [[abstract-factory-pattern]].

**Composite.** What a builder makes is often a composite.[^1] Kerievsky's refactoring Encapsulate Composite with Builder hides the tree's structure behind calls such as `addChild`.[^21] See [[composite-pattern]].

**Factory Method.** A director can get its builder from a factory method, and a builder's `build` is a factory method of a kind. The [[factory-method-pattern]] page covers that pattern.

## Known uses

- Rust: the documentation of `std::process::Command` calls it "A process builder, providing fine-grained control over how a new process should be spawned." Its setters, such as `arg`, take and return `&mut Command`. See [the Command struct in process.rs at 1.98.0](https://github.com/rust-lang/rust/blob/1.98.0/library/std/src/process.rs#L530-L533). The API guidelines use `Command` as their example of a builder.[^19][^14]
- OpenJDK: [HttpRequest.Builder at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.net.http/share/classes/java/net/http/HttpRequest.java#L95-L117) has setters that return the same builder. Its `build` returns an immutable `HttpRequest` each time it is called, and it throws `IllegalStateException` when no URI is set.[^15][^20]

## References

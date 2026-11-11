---
tag: 4ZB5QG
kind: pattern
slug: strategy-pattern
name: Strategy pattern
aliases: [Strategy, Policy, policy pattern]
area: software-design/behavioral-patterns
level: core
summary: >-
  A context object holds a strategy and calls it for one step of its work. The client chooses the strategy, so that step can change without a change to the context.
family: behavioral-design
polarity: positive
intent: "Put each variant of an algorithm behind one interface, so that the client can choose a variant and the code that uses the algorithm does not change."
also_known_as: [Policy]
refactorings: [replace-conditional-logic-with-strategy]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Policy as a second name. In 1996 Peter Norvig showed that in a language with first-class functions the pattern becomes a variable that holds a function.
see_also: [closures, observer-pattern, factory-method-pattern, decorator-pattern, extract-function]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Strategy (Intent, Also Known As, Motivation, Participants, Consequences, Implementation, Related Patterns)
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Template Method (Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 9 and slide 10 (16 of 23 patterns simpler; first-class functions make Command, Strategy, Template-Method and Visitor simpler), slide 16 (Dynamic Pattern, Strategy)
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 7, Simplification, Replace Conditional Logic with Strategy (problem and solution)
  - url: https://docs.python.org/3.14/howto/sorting.html#key-functions
    title: Sorting Techniques, Python 3.14 documentation
    locator: Key Functions
  - url: https://docs.python.org/3.14/faq/programming.html#why-do-lambdas-defined-in-a-loop-with-different-values-all-return-the-same-result
    title: Programming FAQ, Python 3.14 documentation
    locator: Why do lambdas defined in a loop with different values all return the same result?
  - url: https://docs.python.org/3.14/library/functools.html#functools.partial
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.partial
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Comparator.html
    title: Java SE 25 API, java.util.Comparator
    locator: interface description (functional interface)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-9.html#jls-9.8
    title: The Java Language Specification, Java SE 25 Edition
    locator: 9.8 Functional Interfaces
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.comparison-1
    title: .NET API reference, System.Comparison<T>
    locator: delegate description
  - url: https://pkg.go.dev/slices#SortFunc
    title: Go standard library, package slices
    locator: func SortFunc
  - url: https://doc.rust-lang.org/std/primitive.slice.html#method.sort_by
    title: Rust standard library, primitive slice
    locator: method sort_by
  - url: https://doc.rust-lang.org/book/ch18-02-trait-objects.html
    title: The Rust Programming Language, chapter 18.2
    locator: Using Trait Objects to Abstract over Shared Behavior (trait objects and dynamic dispatch)
  - url: https://timsong-cpp.github.io/cppwp/n4950/alg.sort
    title: Working Draft, Programming Languages, C++ (N4950)
    locator: "[alg.sort] (the Compare template parameter of sort)"
  - url: https://tc39.es/ecma262/#sec-array.prototype.sort
    title: ECMAScript Language Specification (draft)
    locator: 23.1.3.30 Array.prototype.sort (comparator)
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899:2024 (C23) working draft N3220
    locator: 7.24.5.2 The qsort function, paragraphs 1 to 4
  - url: https://kotlinlang.org/docs/lambdas.html#function-types
    title: Kotlin documentation, Higher-order functions and lambdas
    locator: Function types
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/TreeMap.java#L403-L419
    title: OpenJDK jdk-25+36 source, java/util/TreeMap.java
    locator: getEntryUsingComparator, lines 403 to 419
  - url: https://github.com/golang/go/blob/go1.27.3/src/slices/sort.go#L30-L33
    title: Go go1.27.3 source, src/slices/sort.go
    locator: SortFunc, lines 30 to 33
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Decorator (Implementation, changing the skin of an object versus changing its guts)
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, State (Implementation, who defines the state transitions)
  - url: https://tc39.es/ecma262/#sec-createperiterationenvironment
    title: ECMAScript Language Specification (draft)
    locator: 14.7.4.4 CreatePerIterationEnvironment
  - url: https://martinfowler.com/articles/injection.html
    title: Martin Fowler, Inversion of Control Containers and the Dependency Injection pattern, January 2004
    locator: Forms of Dependency Injection; Constructor Injection with PicoContainer
---

## Intent

Put each variant of an algorithm behind one interface. The client chooses a variant and gives it to the code that uses the algorithm. That code calls the interface and does not change when a variant is added.[^1]

## Problem

A checkout computes the total of a cart. The shop has three prices. The regular price is the sum of the lines. A promotion takes ten percent off, and a bulk price takes 50 cents off each unit of a line with five or more units. Next month marketing will add another plan. The direct design puts all the plans in one function and selects one with a conditional:

```python run id=conditional
def total(items, plan):
    if plan == "regular":
        return sum(price * quantity for _, price, quantity in items)
    if plan == "ten percent off":
        return total(items, "regular") * 90 // 100
    if plan == "bulk":
        return sum((price - 50 if quantity >= 5 else price) * quantity for _, price, quantity in items)
    raise ValueError(f"unknown plan: {plan}")


cart = [("book", 1500, 2), ("pen", 200, 5)]
for plan in ["regular", "ten percent off", "bulk"]:
    print(f"{plan}: {total(cart, plan)} cents")
```

```output for=conditional
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

This works for three plans. Each added plan changes the function, so each change can break the plans that already work. A test of one plan must go through the same function as all the others. The code that owns the cart also owns every pricing rule, and a library cannot let its users add a plan.

The forces are these. The step that varies must change without a change to the code around it. The variants must be testable one at a time. The choice of a variant can come from configuration or from the user at run time. Kerievsky describes the starting point of the refactoring in the same terms. Conditional logic in a method controls which of several variants of a calculation runs.[^4]

## Solution

Move each variant into its own object or function with the same signature. This is the **strategy**. The checkout becomes the **context**. It holds a reference to one strategy and calls it for the step that varies. The **client** creates the strategy that it wants and gives it to the context.[^1]

```text
 Context (Checkout)                 Strategy (interface)
 -------------------------          ---------------------
 pricing: Strategy  ------------->  total(items) -> cents
 total(items):                                ^
   return pricing.total(items)                |
                               +--------------+--------------+
                               |              |              |
                           Regular      TenPercentOff       Bulk
```

The participants in Design Patterns are the Strategy interface, the concrete strategies, and the Context.[^1] The context depends only on the interface. The concrete strategies do not know the context. The client knows both, because it makes the choice.

There are two ways to give the strategy its data. The context can pass the data as arguments, as `total(items)` does here. Or the context can pass itself, and the strategy reads what it needs from the context. The first keeps the strategy independent of the context. The second gives the strategy access to more data, but it couples the strategy to the interface of the context.[^1]

**In a language with first-class functions, a strategy with one operation is a function.** The interface is a function type, and each concrete strategy is a function value. Norvig counts Strategy among four patterns that first-class functions make simpler. He describes the dynamic form of Strategy as a variable whose value is a function.[^3] The standard libraries use this form everywhere:

| Language | Strategy as a value |
|---|---|
| Python | the `key` function of `sorted` and `list.sort`[^5] |
| Java | `java.util.Comparator`, a functional interface, so a lambda or a method reference can be one[^8][^9] |
| C# | the `Comparison<T>` delegate, given to `List<T>.Sort`[^10] |
| Go | the `cmp` function of `slices.SortFunc`[^11] |
| Rust | the closure of `slice::sort_by`[^12] |
| C++ | the `Compare` template parameter of `std::sort`[^14] |
| JavaScript | the comparator of `Array.prototype.sort`[^15] |
| C | the function pointer `compar` of `qsort`[^16] |
| Kotlin | a parameter of a function type such as `(A, B) -> C`[^17] |

The class form is still useful in two cases. The strategy can have more than one operation that must vary together, such as `compress` and `decompress`. Or it can have configuration that a reader should see as named fields.

**Static and dynamic binding.** A context can take the strategy as a value at run time, or as a type parameter at compile time. Design Patterns describes the second form with C++ templates: the strategy is fixed when the context is compiled, and the call needs no indirection.[^1] Rust generics with `F: Fn(...)` and C++ templates give this form. A `Box<dyn Fn(...)>` in Rust, a `std::function` in C++, and a trait object give the dynamic form, which can change while the program runs.[^13]

## Example

The example files show one program in each language. A cart has two books at 1500 cents and five pens at 200 cents. One checkout object computes the total with three strategies in turn: regular (4000), ten percent off (3600) and bulk (3750). Each language uses its own usual form:

| Language | Form |
|---|---|
| Python | plain functions in an attribute of the context |
| JavaScript | functions in an object field |
| TypeScript | a function type alias |
| Java | a functional interface with method references |
| Kotlin | a function type and function references |
| C# | a delegate type |
| Go | a named func type |
| Rust | a generic context with `F: Fn`, and a `Box<dyn Fn>` list |
| C | a function pointer in a struct |
| C++ | a `std::function` member, and a template for the static form |
| Bash | the name of a function in a variable |

## Consequences

Design Patterns lists these benefits.[^1] The strategies form a family of related algorithms, and code they share can move into a common base or a helper. A strategy is an alternative to a subclass of the context. The context does not need one subclass for each algorithm, so the algorithm and the rest of the context vary on their own. The conditional that selected a variant goes away from the context. The client can choose between implementations with the same result and different costs in time or in space.

The same chapter lists these costs.[^1] The client must know the strategies to choose one, so the knowledge of the variants moves to the client and does not disappear. The interface between the context and the strategy must pass every piece of data that any strategy needs. So a small strategy gets arguments that it ignores. The program has more objects. A strategy with no state can be shared by many contexts, which keeps the number of objects small.

**The conditional moves; it does not vanish.** Some code must still turn a configuration value into a strategy. Put this choice in one place, at the edge of the program, often as a map from a name to a function. The rest of the program then gets a strategy and never asks which one it is.

**A stateful strategy must not be shared.** A strategy that keeps state between calls works for one context only. In this program one discount object serves two customers. The first customer uses up the state, and the second customer loses a discount that is his:

```python wrong id=shared-state
def regular(items):
    return sum(price * quantity for _, price, quantity in items)


class FirstOrderDiscount:
    def __init__(self):
        self.used = False

    def __call__(self, items):
        if self.used:
            return regular(items)
        self.used = True
        return regular(items) * 90 // 100


cart = [("book", 1500, 2), ("pen", 200, 5)]
first_order = FirstOrderDiscount()
for customer in ["ana", "ben"]:
    print(f"{customer}: {first_order(cart)} cents")
```

```output for=shared-state
ana: 3600 cents
ben: 4000 cents
```

The fact "this is the first order" belongs to the customer, not to the pricing rule. Move it into the data that the context passes, and the strategy has no state again. The interface grows by one argument, which is the communication cost above:

```python run id=shared-fixed
def regular(items, orders_before):
    return sum(price * quantity for _, price, quantity in items)


def first_order_discount(items, orders_before):
    total = regular(items, orders_before)
    return total * 90 // 100 if orders_before == 0 else total


cart = [("book", 1500, 2), ("pen", 200, 5)]
history = {"ana": 0, "ben": 0, "cy": 3}
for customer, orders_before in history.items():
    print(f"{customer}: {first_order_discount(cart, orders_before)} cents")
```

```output for=shared-fixed
ana: 3600 cents
ben: 3600 cents
cy: 4000 cents
```

**Closures made in a loop.** When strategies are functions, a program often makes them in a loop. A [[closures|closure]] in Python captures the variable, not its value at the time of creation. Every function made in the loop below reads `percent` when it runs, after the loop has ended:[^6]

```python wrong id=late-binding
def regular(items):
    return sum(price * quantity for _, price, quantity in items)


strategies = {}
for percent in [10, 20]:
    strategies[f"{percent} percent off"] = lambda items: regular(items) * (100 - percent) // 100

cart = [("book", 1500, 2), ("pen", 200, 5)]
for name, pricing in strategies.items():
    print(f"{name}: {pricing(cart)} cents")
```

```output for=late-binding
10 percent off: 3200 cents
20 percent off: 3200 cents
```

The FAQ of Python gives the fix: bind the value when the function is made, with a default argument such as `lambda items, p=percent: ...`. `functools.partial` does the same and keeps the argument out of the signature that callers see:[^6][^7]

```python run id=late-binding-fixed
from functools import partial


def regular(items):
    return sum(price * quantity for _, price, quantity in items)


def percent_off(percent, items):
    return regular(items) * (100 - percent) // 100


strategies = {}
for percent in [10, 20]:
    strategies[f"{percent} percent off"] = partial(percent_off, percent)

cart = [("book", 1500, 2), ("pen", 200, 5)]
for name, pricing in strategies.items():
    print(f"{name}: {pricing(cart)} cents")
```

```output for=late-binding-fixed
10 percent off: 3600 cents
20 percent off: 3200 cents
```

A JavaScript `for` loop that declares its variable with `let` makes a fresh copy of the binding for each pass. So the same loop in JavaScript gives the expected values.[^22] A loop with `var` has the Python behaviour, because a `var` binding belongs to the whole function.

## When not to use it

Do not use the pattern for an algorithm that has one form, or for variants that never change after the program is written. A conditional with two or three branches in one place is easier to read than an interface, three classes and a factory. Move to strategies when a second place starts to switch on the same value, or when the variants need separate tests.

Do not use the class form where the language has function values. A one-method interface with one class for each variant adds names and files and gives no benefit over a function. Keep the class form for a strategy with several operations or with named configuration.

Do not use a strategy to vary a small step inside a larger fixed algorithm when only subclasses will ever supply it. Template Method does that with a hook method. Design Patterns states the difference: template methods use inheritance to vary part of an algorithm, and strategies use delegation to vary the entire algorithm.[^2] If the variants must change at run time or combine with other choices, prefer the strategy.

A program that has a strategy for each small decision becomes hard to follow. The reader of the context cannot see which code runs, and must find where the client made its choice.

## Variants and related patterns

**Template Method.** Both patterns fix the structure and let a part vary. Template Method varies a step with an overridden method, chosen when the subclass is written. Strategy varies the algorithm with a delegated object, chosen when the context is built or later.[^2]

**State.** State has the same structure as Strategy: a context that delegates to an object behind an interface. The difference is who changes the object. In State, the context or the state objects choose the next state as the context moves between states.[^21] In Strategy, the client chooses, and the strategy does not replace itself.

**Factory Method and Abstract Factory.** A creation step is also an algorithm, so a function that returns a product is a strategy for creation. The [[factory-method-pattern]] varies that step by subclassing. Passing a factory function to the context, the strategy form, varies it by delegation.

**Decorator.** Design Patterns contrasts the two: a decorator changes the skin of an object, and a strategy changes its guts.[^20] A [[decorator-pattern|decorator]] wraps an object and keeps its interface. A strategy is a part inside the context.

**Flyweight.** Strategies with no state can be shared between contexts, as flyweights.[^1]

**Dependency injection.** Passing a strategy to a constructor is constructor injection, in the terms of Fowler.[^23] A container that injects an implementation of an interface into a service makes the choice of a strategy part of the configuration of the program.

**Higher-order functions.** A function that takes a function argument, such as `map`, `filter` or `sort`, is the context of a strategy in the functional style. Norvig's slide on Strategy makes this point: with first-class functions the pattern is invisible.[^3]

## Known uses

- OpenJDK: `java.util.TreeMap` stores a `Comparator` in a final field that its constructor sets. [getEntryUsingComparator in TreeMap.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/TreeMap.java#L403-L419) walks the tree with `cpr.compare(k, p.key)`, so the order of the map is a strategy that the client chooses.[^18]
- Go: [SortFunc in src/slices/sort.go at go1.27.3](https://github.com/golang/go/blob/go1.27.3/src/slices/sort.go#L30-L33) takes the comparison as a `func(a, b E) int` and passes it to the sorting algorithm.[^19]

## References

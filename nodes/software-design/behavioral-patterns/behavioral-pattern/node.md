---
tag: 4T6Q62
kind: concept
slug: behavioral-pattern
name: Behavioral pattern
aliases: [behavioral patterns, behavioural pattern, behavioral design pattern, class behavioral pattern, object behavioral pattern]
area: software-design/behavioral-patterns
level: core
summary: >-
  A behavioral pattern is a design pattern about algorithms and the communication between objects. Most of them put a behaviour that varies into its own object.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Design Patterns (1994) made a behavioral group of eleven patterns. In 1996 Peter Norvig showed that ten of them are invisible or simpler with first-class types, first-class functions, macros or method combination.
see_also: [strategy-pattern, template-method-pattern, observer-pattern, command-pattern, visitor-pattern, state-pattern, higher-order-functions, structural-pattern]
same_as:
  wikipedia: Behavioral_pattern
sources:
  - url: https://en.wikipedia.org/wiki/Behavioral_pattern
    title: Behavioral pattern, Wikipedia
    locator: Introduction; Examples
  - url: https://en.wikipedia.org/wiki/Design_Patterns
    title: Design Patterns, Wikipedia
    locator: Patterns by type, Behavioral
  - book: isbn:9780201633610
    title: Erich Gamma, Richard Helm, Ralph Johnson and John Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 1, Organizing the Catalog (Table 1.1); Chapter 5, Behavioral Patterns, introduction and Discussion of Behavioral Patterns
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 5, 1996
    locator: slides 9 and 10
  - url: https://doc.rust-lang.org/book/ch06-02-match.html
    title: The match Control Flow Construct, The Rust Programming Language
    locator: Matches Are Exhaustive
  - url: https://kotlinlang.org/docs/sealed-classes.html
    title: Sealed classes and interfaces, Kotlin documentation
    locator: Use sealed classes with when expression
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html
    title: Narrowing, TypeScript Handbook
    locator: Discriminated unions; Exhaustiveness checking
  - url: https://openjdk.org/jeps/441
    title: JEP 441, Pattern Matching for switch
    locator: Summary; Release
  - url: https://docs.python.org/3/tutorial/controlflow.html
    title: More Control Flow Tools, The Python Tutorial
    locator: 4.2 for Statements
---

## Idea

A program is not only a set of objects. The objects work together, and the way they divide the work decides what can change later. A behavioral pattern is a software design pattern for collaboration between objects.[^1] Most behavioral design patterns are specifically concerned with communication between objects.[^2]

Design Patterns names two kinds. A class behavioral pattern uses inheritance to divide the behaviour between classes. An object behavioral pattern uses object composition: one object holds another object that does a part of the work.[^3] Template Method uses inheritance, and Strategy uses composition.[^1]

```python run id=idea
class Checkout:  # class pattern: Template Method
    def total(self, prices):
        return self.discount(sum(prices))

    def discount(self, amount):
        return amount


class SaleCheckout(Checkout):
    def discount(self, amount):  # the subclass gives the step
        return amount * 9 // 10


class Till:  # object pattern: Strategy
    def __init__(self, discount):
        self.discount = discount

    def total(self, prices):
        return self.discount(sum(prices))


prices = [40, 60]
print("class pattern:", SaleCheckout().total(prices))
till = Till(lambda amount: amount * 9 // 10)
print("object pattern:", till.total(prices))
till.discount = lambda amount: amount - 20  # change it while the program runs
print("changed at run time:", till.total(prices))
```

```output for=idea
class pattern: 90
object pattern: 90
changed at run time: 80
```

The class pattern fixes the discount when the program creates the object. The object pattern can change it later, because the discount is a separate value.

## How it works

The eleven behavioral patterns of Design Patterns are Chain of Responsibility, Command, Interpreter, Iterator, Mediator, Memento, Observer, State, Strategy, Template Method and Visitor.[^2] Interpreter and Template Method are class patterns. The other nine are object patterns.[^3]

Most of them use one of a few moves:[^3]

- **Encapsulate what varies.** Put a behaviour that can change into its own object. Strategy holds an algorithm, State holds the behaviour for one state, Iterator holds a way to go through a collection, and Command holds a request.[^2]
- **Pass objects as arguments.** A visitor is passed to each element of a structure. A command or a memento is a value that one part of the program gives to another.[^3]
- **Decouple senders and receivers.** Observer lets a number of observer objects see an event.[^2] Chain of Responsibility passes a request along a chain of processing objects.[^2] Mediator is the only class that has detailed knowledge of the methods of the others.[^2]

Later lists add patterns that are not in the book, such as Blackboard, Null object, Publish-subscribe and Specification.[^1]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | functions are values, so a strategy, a command or a callback is a function; generators for iterators; `match` for visitors |
| JavaScript | native | functions are values; iterators and generators; events for observers |
| TypeScript | native | function types; discriminated unions with exhaustive checks in place of visitors |
| Java | idiom | interfaces and lambdas for functional interfaces; `Iterator`; pattern matching for `switch` on sealed types |
| C# | idiom | delegates and lambdas; events for observers; `IEnumerable` and `yield return`; switch expressions |
| C++ | idiom | virtual functions; `std::function` and lambdas; iterators; `std::variant` with `std::visit` |
| Go | idiom | function values and interfaces; type switches; channels between goroutines |
| Rust | idiom | traits and closures; the `Iterator` trait; enums with exhaustive `match` in place of visitors |
| Kotlin | native | function types and lambdas; sealed classes with exhaustive `when` |
| C | idiom | function pointers with a context pointer; switch on a type tag |
| Bash | idiom | a function name passed as a string and called later |
| PowerShell | idiom | script blocks as values that are invoked later |
| SQL | n/a | a query is declarative, so there are no objects that divide the work |
| Regex | n/a | a pattern has no objects or behaviour to vary |
| HTML | n/a | markup has no behaviour of its own |
| CSS | n/a | style rules have no objects or algorithms |

Many behavioral patterns become simpler in languages with first-class functions. Norvig lists Command, Strategy, Template Method and Visitor among the patterns that first-class functions make invisible or simpler. He lists State and Chain of Responsibility under first-class types, Interpreter and Iterator under macros, and Mediator and Observer under method combination.[^4]

Sum types with exhaustive matching are a second replacement for Visitor. The arms of a Rust `match` must cover all possibilities.[^5] A Kotlin `when` expression with a sealed class lets the compiler check that all possible cases are covered.[^6] TypeScript uses narrowing and the `never` type to do exhaustive checking in a `switch` statement.[^7] Java 21 has pattern matching for `switch` expressions and statements.[^8]

The example files show the program of the Idea section in each language: a class pattern with a subclass, and an object pattern with a function value.

## Variations and related ideas

- **Creational and structural patterns.** Design Patterns sorts its 23 patterns by purpose into creational, structural and behavioral groups.[^3] Structural patterns are about how objects are composed, and behavioral patterns are about how the composed objects divide the work.
- **Concurrency patterns.** The article on behavioral patterns links to concurrency patterns, creational patterns and structural patterns as related groups.[^1]
- **Functional forms.** A strategy or a command can be a function, and a visitor can be a match on a sum type.[^4][^5]

## Pitfalls

- **Pattern for its own sake.** A function value is often enough where a class hierarchy was written for one varying step.[^4]
- **Hidden flow of control.** With Observer or Chain of Responsibility, the code that sends a request does not show which code handles it.
- **Changes during notification.** An observer that unsubscribes while the subject notifies can break the loop. Code that modifies a collection while iterating over that same collection can be tricky to get right.[^9] Here observer `b` is skipped:

```python wrong id=unsubscribe
class Subject:
    def __init__(self):
        self.observers = []

    def notify(self):
        for o in self.observers:  # iterates the list that changes
            o(self)


subject = Subject()


def make(name, once=False):
    def observer(s):
        print(name, "notified")
        if once:
            s.observers.remove(observer)
    return observer


subject.observers += [make("a", once=True), make("b"), make("c")]
subject.notify()
```

```output for=unsubscribe
a notified
c notified
```

The fix is to loop over a copy: `for o in list(self.observers)`.[^9]

## Cost

Each object pattern adds one call through an interface or a function value. In most programs this is small next to the work of the call. The larger cost is in reading: the behaviour is in more places, and a reader must find which object is in use at run time.

## History

Design Patterns, by Gamma, Helm, Johnson and Vlissides, appeared in 1994. It classified 23 patterns by purpose and scope, and its behavioral group has eleven patterns.[^3][^2]

Peter Norvig presented *Design Patterns in Dynamic Programming* at Object World on May 5, 1996. He found that 16 of the 23 patterns are invisible or simpler in Dylan or Lisp, for at least some uses of each pattern.[^4] Ten of the eleven behavioral patterns are in his list. Memento is not.[^4]

## References

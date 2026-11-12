---
tag: V9T0GQ
kind: concept
slug: inversion-of-control
name: Inversion of control
aliases: [IoC, Hollywood principle, don't call us, we'll call you]
area: software-design/design-principles
level: core
summary: >-
  A framework, not the program, holds the main flow of control and calls the code that the program plugs into it; it is a defining feature of a framework, as opposed to a library.
history:
  year: 1988
  people: [Ralph Johnson, Brian Foote, Richard Sweet]
  note: The term appears in the paper Designing Reusable Classes by Johnson and Foote, 1988; the Hollywood principle goes back to a paper on Mesa by Richard Sweet in 1983.
see_also: [dependency-injection, template-method-pattern, service-locator, design-principle, separation-of-concerns]
sources:
  - url: https://martinfowler.com/bliki/InversionOfControl.html
    title: Martin Fowler, InversionOfControl, 26 June 2005
    locator: the whole post, including the etymology
  - url: https://martinfowler.com/articles/injection.html
    title: Martin Fowler, Inversion of Control Containers and the Dependency Injection pattern, 23 January 2004
    locator: the section Inversion of Control
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
    title: MDN Web Docs, Array.prototype.sort()
    locator: the parameter compareFn
  - url: https://docs.python.org/3/howto/sorting.html
    title: Python documentation, Sorting Techniques
    locator: section Key Functions
  - url: https://pkg.go.dev/sort
    title: Go standard library, package sort
    locator: func Slice
---

## Idea

In a plain program, your code is in control. It decides when to ask a question, when to read the answer and when to process it. With inversion of control (IoC), a framework holds that main flow, and your code gives it pieces to call. Martin Fowler: "Inversion of Control is a common phenomenon that you come across when extending frameworks. Indeed it's often seen as a defining characteristic of a framework."[^1]

Fowler uses IoC to explain the difference between a library and a framework. "A library is essentially a set of functions that you can call, these days usually organized into classes. Each call does some work and returns control to the client." A framework is different: "In order to use it you need to insert your behavior into various places in the framework either by subclassing or by plugging in your own classes. The framework's code then calls your code at these points."[^1]

The smallest example uses a line processor. In the first version, the program runs the loop. In the second version, a small framework runs the loop and calls a handler that the program gives it.

```javascript run id=who-calls
const lines = ["alpha", "beta", "gamma"];

// Your code is in control: it runs the loop.
for (const line of lines) console.log(`plain: ${line.length}`);

// Inverted: the framework runs the loop and calls your handler.
function eachLine(source, handler) {
  for (const line of source) handler(line);
}
eachLine(lines, (line) => console.log(`inverted: ${line.length}`));
```

```output
plain: 5
plain: 4
plain: 5
inverted: 5
inverted: 4
inverted: 5
```

## How it works

The framework owns the flow, and the program extends it at points that the framework defines. Fowler lists several ways to plug code in:[^1]

- **A callback or closure.** The program passes a function, and the framework calls it when an event occurs. Fowler's example binds a closure to the FocusOut event of a text field.
- **Events.** The framework defines events, and the client code subscribes to them. Fowler names .NET as a platform with language features to declare events on widgets.
- **An interface.** When one extension needs several methods, "the framework can define an interface that a client code must implement for the relevant calls."
- **A template method.** "The super-class defines the flow of control, subclasses extend this overriding methods or implementing abstract methods to do the extension." In JUnit, "the framework code calls setUp and tearDown methods for you". See [[template-method-pattern|Template method pattern]].

Fowler gives the user interface as the first case of IoC that he met: "Early user interfaces were controlled by the application program." With graphical UIs, "the UI framework would contain this main loop and your program instead provided event handlers for the various fields on the screen."[^2]

## In languages

The smallest case of IoC in each language is a sort function that calls the code of the program to compare or to key the elements. The sort owns the loop; the program decides only the order.

- **JavaScript.** Array.prototype.sort takes compareFn, "A function that determines the order of the elements." The sort calls it with two elements at a time.[^3]
- **Python.** sorted and list.sort take a key parameter "to specify a function (or other callable) to be called on each list element prior to making comparisons."[^4]
- **Go.** "Slice sorts the slice x given the provided less function."[^5] The less function receives two indexes and reports whether the first element goes before the second.

The Python and Go examples of this page show the same line processor as the JavaScript example.

## Variations and related ideas

- **Dependency injection.** Containers of the early 2000s said they used IoC. Fowler asked "what aspect of control are they inverting?" For these containers, "the inversion is about how they lookup a plugin implementation." Fowler wrote that IoC "is too generic a term", and with other IoC advocates settled on the name Dependency Injection.[^2] See [[dependency-injection|Dependency injection]].
- **Service locator.** Fowler gives the service locator as the other way to remove the dependency of a class on the implementation of its plugins.[^2] See [[service-locator|Service locator]].
- **Event-driven programming.** A GUI main loop that calls event handlers is the classic case of inverted control.[^2]

## Pitfalls

- **Confusing the principle with one style.** Fowler notes that "some people confuse the general principle here with the specific styles of inversion of control (such as dependency injection) that these containers use."[^1] He also notes the irony that IoC containers are seen as a competitor to EJB, yet "EJB uses inversion of control just as much (if not more)."[^1]
- **Less control over when code runs.** In the EJB example, the container calls the methods of a session bean at points in its life cycle. "You don't get to control when these methods are called, just what they do."[^1] A handler that assumes an order of calls that the framework does not promise is a bug.

## Cost

The cost is one more level of indirection: a call through a function value, an interface or an overridden method in place of a direct call. The larger cost is in understanding. The flow of the program is in the framework, so a reader must know the framework to know when the program's code runs.

## History

Fowler traces the term to the paper Designing Reusable Classes by Ralph Johnson and Brian Foote, published in the Journal of Object-Oriented Programming in 1988. The term then spread in the object-oriented community and appears in the book Design Patterns.[^1] The synonym Hollywood Principle seems to come from a paper by Richard Sweet on Mesa in 1983, with the design goal: "Don't call us, we'll call you (Hollywood's Law)".[^1] John Vlissides later explained the concept under that name in a column for C++ Report.[^1] In January 2004, Fowler's article on IoC containers named the specific case Dependency Injection.[^2]

## References

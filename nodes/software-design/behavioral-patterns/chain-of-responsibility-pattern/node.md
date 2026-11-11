---
tag: B7VZ5Z
kind: pattern
slug: chain-of-responsibility-pattern
name: Chain of Responsibility pattern
aliases: [Chain of Responsibility, CoR, handler chain, middleware chain]
area: software-design/behavioral-patterns
level: core
summary: >-
  A request passes along a chain of handler objects. Each handler either handles it or passes it to the next one, so the sender does not know which object will handle it.
family: behavioral-design
polarity: positive
intent: "Avoid coupling the sender of a request to its receiver by giving more than one object a chance to handle the request. Chain the receiving objects and pass the request along the chain until an object handles it."
also_known_as: []
refactorings: []
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with context-sensitive help in a graphical user interface as the motivating example.
see_also: [decorator-pattern, composite-pattern, command-pattern, observer-pattern, exception-handling, higher-order-functions, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Chain of Responsibility (Intent, Motivation, Applicability, Participants, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (Chain-Of-Responsibility is one of the six patterns affected by first-class types)
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/urllib/request.py#L457-L466
    title: CPython v3.14.0 source, Lib/urllib/request.py
    locator: method OpenerDirector._call_chain, lines 457 to 466
  - url: https://docs.python.org/3.14/library/urllib.request.html#openerdirector-objects
    title: The Python Standard Library, urllib.request, Python 3.14
    locator: OpenerDirector Objects
  - url: https://docs.python.org/3.14/library/logging.html#logging.Logger.propagate
    title: The Python Standard Library, logging, Python 3.14
    locator: Logger.propagate
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ClassLoader.html
    title: Java SE 25 API, java.lang.ClassLoader
    locator: class description, the delegation model
  - url: https://dom.spec.whatwg.org/#dom-event-stoppropagation
    title: DOM Living Standard, WHATWG
    locator: 2.2 Interface Event, the stopPropagation() method steps
  - url: https://dom.spec.whatwg.org/#concept-event-dispatch
    title: DOM Living Standard, WHATWG
    locator: 2.9 Dispatching events, the dispatch and invoke algorithms
  - url: https://learn.microsoft.com/en-us/aspnet/core/fundamentals/middleware/#create-a-middleware-pipeline-with-webapplication
    title: ASP.NET Core Middleware, Microsoft Learn
    locator: Create a middleware pipeline with WebApplication
  - url: https://learn.microsoft.com/en-us/aspnet/core/fundamentals/middleware/#middleware-order
    title: ASP.NET Core Middleware, Microsoft Learn
    locator: Middleware order
  - url: https://pkg.go.dev/net/http#Handler
    title: Go standard library, package net/http
    locator: type Handler
  - url: https://pkg.go.dev/net/http#StripPrefix
    title: Go standard library, package net/http
    locator: func StripPrefix
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.net.http.delegatinghandler
    title: .NET API reference, System.Net.Http.DelegatingHandler
    locator: class description and InnerHandler property
---

## Intent

Avoid coupling the sender of a request to its receiver, by giving more than one object a chance to handle the request. Chain the receiving objects, and pass the request along the chain until an object handles it.[^1] The sender knows only the first link. Each link knows only the next one.

## Problem

Design Patterns starts from context-sensitive help in a graphical user interface.[^1] A user asks for help on a part of the screen, such as a button. The button may have its own help text. If it does not, the dialog box that holds the button may have help text. If the dialog has none, the application gives a general answer. The object that gets the request is the button, but the object that answers may be any object above it.

The forces are these. The sender does not know which object will answer, and it should not need to know. The set of objects that can answer changes while the program runs, as windows open and close. And the rule for choosing one should not be a long conditional in the sender, because each change to the set of handlers would then change the sender. Design Patterns gives three cases where the pattern applies: more than one object may handle a request and the handler is not known in advance, the sender should not name the receiver, and the set of handlers should be specified at run time.[^1]

## Solution

Design Patterns names three participants.[^1]

- **Handler** defines the interface for handling requests. It may also hold the link to the successor.
- **ConcreteHandler** handles the requests that it is responsible for. If it cannot handle a request, it passes it to its successor.
- **Client** sends the request to the first handler in the chain.

A handler that cannot answer a request passes it on. The request moves along the chain until one handler answers it. If no handler answers, the request falls off the end of the chain with no answer:

```python wrong id=falls-off
class Handler:
    def __init__(self, successor=None):
        self.successor = successor

    def help(self, topic):
        if self.successor is not None:
            return self.successor.help(topic)
        # No successor: the request falls off the end, and help returns None.


class Button(Handler):
    def help(self, topic):
        if topic == "print":
            return "button: prints the page"
        return super().help(topic)


class Dialog(Handler):
    def help(self, topic):
        if topic == "margins":
            return "dialog: sets the page margins"
        return super().help(topic)


button = Button(successor=Dialog())
for topic in ["print", "margins", "shortcuts"]:
    print(f"{topic}: {button.help(topic)}")
```

```output for=falls-off
print: button: prints the page
margins: dialog: sets the page margins
shortcuts: None
```

Design Patterns calls this a consequence of the pattern: receipt is not guaranteed.[^1] The sender cannot tell that no handler took the request, and Python gives back `None` with no error. Put a handler at the end that answers every request, such as an application object with general help, or make the last link raise an error. The Java example answers 404 when no handler is left. The TypeScript example makes the last link a different type from the other links, so it cannot pass a request on.

### Who holds the links

Design Patterns gives two ways to build the chain.[^1] Each handler can hold a field for its successor, as `Handler` does above. Or the chain can use links that the objects already have. In a user interface, each widget already knows its parent, so the parent is the successor. This is why the pattern is often used with Composite. See [[composite-pattern]].

A third form keeps the handlers in a list, and a loop asks each one in turn. No handler knows the next one. The `OpenerDirector` in Python's `urllib.request` works this way. Its `_call_chain` method calls each handler for a kind of request. A handler returns `None` when it cannot answer and another handler could, and the loop returns the first answer that is not `None`.[^3] The handlers are sorted, and the stage of opening a URL ends when a handler returns a response or raises an exception.[^4]

### Representing requests

Design Patterns lists three ways to represent a request.[^1] Each kind of request can be a separate method of the handler, as `help` is above. A single method can take a code that names the kind of request. Or the request can be an object, so that handlers can pass any data along with it. The examples in this node use a request object with a user and a path. A request object is close to a command. See [[command-pattern]].

### Middleware

In the form from Design Patterns, one handler answers and the others only pass the request on. Web servers use a related form, called middleware. Each link can do work before it calls the next link, and work after the next link returns. A link can also answer at once and not call the next link.

ASP.NET Core describes its request pipeline in this way. Calls to `Use` chain the request delegates together, and a parameter `next` represents the next delegate in the pipeline. A delegate that does not pass the request to `next` short-circuits the pipeline.[^9] The documentation also says that the order of the middleware in the program is the order in which they run on a request, the reverse order for the response, and that this order can be critical for security, performance and functionality.[^10]

The order is a dependency that the type system does not check. Here a shared cache, keyed only by the path, runs before the check that the user signed in:

```python wrong id=cache-before-auth
def chain(handlers, last):
    call = last
    for handler in reversed(handlers):
        call = lambda request, handler=handler, next=call: handler(request, next)
    return call


def auth(request, next):
    if request["user"] is None:
        return 401, "sign in first"
    return next(request)


saved = {}


def cache(request, next):
    # A shared cache, keyed by the path only.
    if request["path"] not in saved:
        saved[request["path"]] = next(request)
    return saved[request["path"]]


def app(request):
    return 200, f"report for {request['user']}"


serve = chain([cache, auth], app)
print(serve({"user": "ada", "path": "/report"}))
print(serve({"user": None, "path": "/report"}))
```

```output for=cache-before-auth
(200, 'report for ada')
(200, 'report for ada')
```

The second request has no user, but it gets the report for `ada`. The cache answered it, so `auth` never saw it. With the chain `[auth, cache]`, the second request gets 401. The examples put the authentication check before the cache, and also key the cache by the user.

### Middleware and Decorator

A link that always calls the next link, and only adds work before or after the call, is a decorator: it has the same interface as the object it wraps and adds behaviour to it. The decorator node shows middleware as a chain of decorators. See [[decorator-pattern]]. A link that may answer and not call the next link is a handler in a chain of responsibility. Most middleware stacks hold both kinds. In the examples, `log` is a decorator, and `auth` and `cache` can end the chain.

### Functions in place of handler classes

Norvig lists Chain of Responsibility as one of six patterns of Design Patterns that first-class types make invisible or less complex.[^2] In the languages of the examples, first-class functions do most of this work:

- A handler is a function that takes the request and `next`, the rest of the chain. The chain is a list of these functions, folded from the end. This replaces the abstract class and the successor field. The Python, JavaScript, TypeScript and C++ examples use this form. See [[higher-order-functions]].
- State that a handler needs, such as the map of the cache, lives in a closure. See [[closures]].
- In Go, `http.Handler` is an interface with one method, `ServeHTTP`.[^11] A link is a function that takes the next `http.Handler` and returns an `http.Handler`. The standard library has links of this shape: `StripPrefix` returns a handler that removes a prefix from the path and calls the handler it was given, and it answers 404 itself when the path does not have the prefix.[^12]
- In C#, `DelegatingHandler` is a handler that passes the processing of HTTP response messages to another handler, called the inner handler.[^13] The C# example builds the chain from it.

Exceptions are a chain of responsibility that the language runs. A raised exception moves from a function to its caller, and to the caller of that function, until a `try` block with a matching handler takes it. A handler can also raise it again, to pass it on. See [[exception-handling]].

## Example

All the example files run the same chain on three requests. The chain logs each request, rejects a request with no user, and caches answers by user and path. The first request goes to the application. The second request stops at the check for a user. The third request gets the answer from the cache.

| Language | Chain |
|---|---|
| Python | functions of the request and `next`, folded into a chain |
| JavaScript | functions of the request and `next`, folded with `reduceRight` |
| TypeScript | the same, with types for `Handler` and `Next` |
| Java | an abstract `Handler` with a successor field, as in Design Patterns |
| C# | `DelegatingHandler` subclasses, sent through `HttpMessageInvoker` |
| Go | `net/http` middleware of type `func(http.Handler) http.Handler` |
| Rust | trait objects in a `Vec`, and a `Next` that holds the rest of the slice |
| C++ | a `std::vector` of `std::function`, with an index for the next link |

## Consequences

Design Patterns lists three consequences.[^1] The pattern reduces coupling: the sender and the receiver do not know each other, and a handler knows only its successor, not the whole chain. It gives flexibility in assigning responsibilities: a program can add or change handlers at run time. And receipt is not guaranteed: a request can reach the end of the chain with no handler.

There are other costs. The path of a request is set at run time, so a reader cannot see from the sender which code will answer it. A debugger shows a deep stack of calls through each link. Each request walks the chain until a link answers, so a long chain adds a cost to every request. And the order of the links matters, as the cache before the check shows, but nothing in the code of a link states the order that it needs.

## When not to use it

Do not use a chain when the sender knows the receiver. Call the receiver directly.

Do not use a chain to choose a handler by a key, such as a path or a message type, when each key has one handler. A dictionary from key to handler, or a `match` on the key, finds the handler in one step and shows all the choices in one place. See [[python/match-statement]].

Do not use a chain when every handler must see each request. That is the Observer pattern. See [[observer-pattern]]. Python's `logging` module shows the difference. A record logged to a logger goes to the handlers of that logger and then to the handlers of each ancestor logger. It does not stop at the first handler. Only a logger with `propagate` set to false stops it.[^5]

Do not build a chain of classes with a successor field in a language with first-class functions, unless the handlers need more than one method. A list of functions holds the same chain.

## Variants and related patterns

**Composite.** The parent of a component can act as its successor.[^1] Event bubbling in the DOM is this form. The dispatch algorithm builds a path from the target of an event through each of its parents, and calls the listeners on each object in the path.[^8] A listener that calls `stopPropagation()` sets a flag on the event. The listeners on the current object still run, but the algorithm calls no listeners on the objects later in the path.[^7][^8] See [[composite-pattern]].

**Decorator.** A decorator always passes the call to the object it wraps. A handler may stop the request. See the section on middleware above, and [[decorator-pattern]].

**Command.** A request can be a command object, which the handlers pass along the chain. See [[command-pattern]].

**Observer.** An observer gets every notification. A handler in a chain may stop the request before later handlers see it. See [[observer-pattern]].

**Parent first.** A chain can ask the successor first and answer only when the successor cannot. A Java `ClassLoader` has a parent class loader. When it is asked to find a class or a resource, it usually asks its parent before it tries to find the class or resource itself.[^6]

## Known uses

- Python: `urllib.request.OpenerDirector` asks its handlers in order, and a handler returns `None` to pass a request to the next one. See [the method `_call_chain` at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/urllib/request.py#L457-L466).[^3][^4]
- Go: `net/http` handlers such as `StripPrefix` take the next handler as an argument.[^12]
- .NET: `DelegatingHandler` chains for `HttpClient`, and the middleware pipeline of ASP.NET Core.[^13][^9]
- Java: class loaders delegate to a parent class loader.[^6]
- Web browsers: DOM events bubble from the target to its ancestors, until a listener stops them.[^8]

## References

---
tag: HYDT82
kind: pattern
slug: proxy-pattern
name: Proxy pattern
aliases: [Proxy, Surrogate, stub, smart reference, smart pointer]
area: software-design/structural-patterns
level: core
summary: >-
  A proxy stands in for another object and has the same interface. It controls access to the real object: it can make it on first use, check permissions, or send the call to another process.
family: structural-design
polarity: positive
intent: "Provide a surrogate or placeholder for another object, to control access to it."
also_known_as: [Surrogate]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Surrogate as another name, and an image in a document editor that loads on demand as the motivating example.
see_also: [decorator-pattern, adapter-pattern, facade-pattern, singleton-pattern, garbage-collection, rust/ownership]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Proxy (Intent, Also Known As, Motivation, Applicability, Participants, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (Proxy is among the patterns affected by first-class types) and slide 30 (Other Invisible Patterns, smart pointers)
  - paper: doi:10.1145/2080.357392
    title: Andrew D. Birrell and Bruce Jay Nelson, Implementing Remote Procedure Calls, ACM Transactions on Computer Systems 2(1), 1984
    locator: section 1.5, Structure
  - url: https://scholar.harvard.edu/files/waldo/files/waldo-94.pdf
    title: Jim Waldo, Geoff Wyant, Ann Wollrath and Sam Kendall, A Note on Distributed Computing, Sun Microsystems Laboratories, SMLI TR-94-29, November 1994
    locator: Abstract
  - url: https://tc39.es/ecma262/2025/#sec-proxy-objects
    title: ECMA-262, ECMAScript 2025 Language Specification
    locator: 28.2 Proxy Objects
  - url: https://tc39.es/ecma262/2025/#sec-proxy-object-internal-methods-and-internal-slots
    title: ECMA-262, ECMAScript 2025 Language Specification
    locator: 10.5 Proxy Object Internal Methods and Internal Slots
  - url: https://tc39.es/ecma262/2025/#sec-proxy.revocable
    title: ECMA-262, ECMAScript 2025 Language Specification
    locator: 28.2.2.1 Proxy.revocable
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy#no_private_field_forwarding
    title: MDN Web Docs, Proxy
    locator: No private field forwarding
  - url: https://docs.python.org/3.14/reference/datamodel.html#customizing-attribute-access
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.3.2 Customizing attribute access
  - url: https://docs.python.org/3.14/reference/datamodel.html#special-method-lookup
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.3.13 Special method lookup
  - url: https://docs.python.org/3.14/library/weakref.html#weakref.proxy
    title: The Python Standard Library, weakref, Python 3.14
    locator: weakref.proxy
  - url: https://docs.python.org/3.14/library/multiprocessing.html#proxy-objects
    title: The Python Standard Library, multiprocessing, Python 3.14
    locator: Proxy Objects
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/Proxy.html
    title: Java SE 25 API, java.lang.reflect.Proxy
    locator: class description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.reflection.dispatchproxy
    title: .NET API reference, System.Reflection.DispatchProxy
    locator: class description
  - url: https://pkg.go.dev/net/http/httputil#ReverseProxy
    title: Go standard library, package net/http/httputil
    locator: type ReverseProxy
  - url: https://doc.rust-lang.org/std/ops/trait.Deref.html
    title: Rust standard library, std::ops::Deref
    locator: trait description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.lazy-1
    title: .NET API reference, System.Lazy<T>
    locator: class description and remarks on thread safety
  - url: https://docs.python.org/3.14/library/functools.html#functools.cached_property
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.cached_property
  - url: https://pkg.go.dev/sync#OnceValue
    title: Go standard library, package sync
    locator: func OnceValue
  - url: https://doc.rust-lang.org/std/cell/struct.LazyCell.html
    title: Rust standard library, std::cell::LazyCell
    locator: struct description
  - url: https://doc.rust-lang.org/std/cell/struct.OnceCell.html
    title: Rust standard library, std::cell::OnceCell
    locator: struct description
---

## Intent

Provide a surrogate or placeholder for another object, to control access to it.[^1] The proxy has the same interface as the real object, which Design Patterns calls the subject. A client that holds the proxy cannot tell it from the subject. The proxy decides when and whether to pass each request on.

## Problem

A document editor shows a document with large images. To load each image from disk when the document opens would be slow, and many images are never on the screen. But the document wants to treat each image as an object with a size and a `draw` method. Design Patterns starts from this example.[^1]

The same shape appears in other places. An object lives in another process, but the caller wants to call its methods as if it were local. An object must be shown only to some users. An object is costly to copy, and most copies are never changed. In each case, a client must use an object through its normal interface, and some work must happen between the client and the object.

The forces are these. The client code must not change. The extra work, such as loading, checking or sending, must be in one place. And the real object must not know about it.

## Solution

Design Patterns names three participants.[^1]

- **Subject** is the interface that the real object and the proxy share.
- **RealSubject** is the object that does the work.
- **Proxy** holds a reference to the real subject, or the means to get one. It implements the subject interface and passes requests on.

Design Patterns lists four kinds of proxy.[^1]

- A **virtual proxy** makes a costly object on demand. The image proxy is one.
- A **protection proxy** checks that the caller may make a request.
- A **remote proxy** stands for an object in another address space.
- A **smart reference** does extra work on each access, such as counting references, loading a persistent object, or locking.

The examples build a virtual proxy and a protection proxy for one report. The report prints a line when it loads, so the output shows when the load happens:

| Language | Virtual proxy | Protection proxy |
|---|---|---|
| Python | `__getattr__` forwards each attribute | a class with `render` |
| JavaScript | the built-in `Proxy` with a `get` trap | a `Proxy` over the report |
| TypeScript | a class that implements `Report` | a class that implements `Report` |
| Java | `java.lang.reflect.Proxy` and an `InvocationHandler` | a second handler |
| C# | `Lazy<T>` inside a class | `DispatchProxy` |
| Go | `sync.OnceValue` | a struct with the same method |
| Rust | `OnceCell` | a generic struct over any `Report` |
| C++ | a template with `operator->` | a class that holds a reference |

### Lazy loading

A virtual proxy holds a factory and an empty slot. On the first request it calls the factory, fills the slot, and forwards. Later requests forward at once. Standard libraries hold this logic in one type: `Lazy<T>` in .NET, `sync.OnceValue` in Go, `OnceCell` and `LazyCell` in Rust, and `functools.cached_property` in Python.[^17][^19][^20][^21][^18] With these types, the proxy class has only the forwarding methods.

The lazy object must be safe when threads share it. Two threads can find the slot empty at the same moment and make two objects. `Lazy<T>` is thread safe by default, and `sync.OnceValue` may be called from many goroutines.[^17][^19] Rust's `OnceCell` is for one thread, and `OnceLock` is the thread-safe form.[^21]

### Access control

A protection proxy runs a check before it forwards. The check is in the proxy, so the real object has no code for permissions. Design Patterns gives the example of objects in an operating system that different clients may use in different ways.[^1] The Python, TypeScript, Go, Rust and C++ examples write the check as a method of a class with the same interface. The JavaScript and C# examples put it in a dynamic proxy that sees each call.

ECMAScript has a proxy that a program can turn off. `Proxy.revocable` returns a proxy and a function that revokes it. After the call, each operation on the proxy throws a `TypeError`.[^7] A program can give a proxy to code that it does not trust, and take the access back later.

### Remote proxies

A remote proxy turns each call into a message. It packs the method name and the arguments, sends them, waits for the answer, and unpacks the result. Birrell and Nelson built remote procedure calls this way. A call goes from the user to a user-stub, then through the RPC runtime to a server-stub, which makes a normal local call to the server. A program named Lupine generated both stubs from the interface.[^3] The user-stub is a remote proxy.

This program has a client stub and a server stub, with a transport function in between. A second transport fails, as a network can:

```python run id=remote
import json


class Report:
    def render(self, month):
        return f"report for {month}: 42 orders"


def server_stub(subject, message):
    call = json.loads(message)
    result = getattr(subject, call["method"])(*call["args"])
    return json.dumps({"result": result})


class ReportStub:
    """A remote proxy: it has the interface of Report, and it sends each call as a message."""

    def __init__(self, transport):
        self._transport = transport

    def render(self, month):
        message = json.dumps({"method": "render", "args": [month]})
        print("send:", message)
        return json.loads(self._transport(message))["result"]


def network(message):
    return server_stub(Report(), message)


def broken_network(message):
    raise ConnectionError("no route to the report server")


print(ReportStub(network).render("May"))
try:
    ReportStub(broken_network).render("June")
except ConnectionError as error:
    print("error:", error)
```

```output for=remote
send: {"method": "render", "args": ["May"]}
report for May: 42 orders
send: {"method": "render", "args": ["June"]}
error: no route to the report server
```

The second call shows the limit of the pattern. `Report.render` never raises `ConnectionError`, but the stub does. Waldo, Wyant, Wollrath and Kendall argue that a remote object cannot be made to look like a local one. They name four differences: latency, memory access, partial failure and concurrency.[^4] A remote proxy hides the code that sends a message. It cannot hide that the call is slow, that the argument is a copy and not a reference, or that the network can fail in the middle of a call. An interface for remote objects should show these facts, for example with a timeout and an error in the type of each method.

## Example

The example files share the script of the table above. They make the lazy proxy, call `render` twice, and then guard the lazy proxy for a guest and for an admin. The load message comes after `proxy made` and appears once.

## Consequences

Design Patterns says that a proxy adds a level of indirection, and that the indirection has a different use for each kind of proxy.[^1] A remote proxy hides that the object is in another address space. A virtual proxy can make an object on demand. A protection proxy and a smart reference can do extra work when the object is used. Design Patterns also describes copy-on-write: a proxy can share one subject between copies and copy it only when a client changes it.[^1]

The costs are these. Each call goes through one more object. A proxy with many methods needs a forwarding method for each, unless the language can forward all of them, as Python's `__getattr__`, the JavaScript `Proxy`, Java's `java.lang.reflect.Proxy` and .NET's `DispatchProxy` can.[^9][^5][^13][^14] And identity changes: the proxy is not the real object, so a comparison by identity, a type test, or code that the real object runs on `this` can tell them apart.

**A forwarding proxy does not forward everything.** Python calls `__getattr__` only when the normal lookup fails.[^9] For special methods, such as `__len__`, Python looks on the type of the object and skips the instance, so `__getattr__` does not run.[^10]

```python fails expect=exception:TypeError id=special-methods
class LazyList:
    def __init__(self, factory):
        self._factory = factory
        self._real = None

    def __getattr__(self, name):
        if self._real is None:
            self._real = self._factory()
        return getattr(self._real, name)


items = LazyList(lambda: [1, 2, 3])
print(items.count(2))  # works: count is found through __getattr__
print(len(items))  # fails: len looks up __len__ on the type LazyList
```

The fix is to define each special method that the proxy needs on the proxy class.

A JavaScript `Proxy` has a related limit. A class method that reads a private field `#name` runs with `this` set to the proxy. The proxy does not have the private field, so the read throws a `TypeError`.[^8]

```javascript fails expect=exception:TypeError id=private-fields
class Account {
  #balance = 100;
  balance() {
    return this.#balance;
  }
}

const proxy = new Proxy(new Account(), {});
console.log(proxy.balance());
```

The fix is a `get` trap that binds each method to the target, as the JavaScript example does.

## When not to use it

Do not write a proxy class when the language gives a lazy value. A field of type `Lazy<T>`, `OnceCell` or `sync.OnceValue`, or a `cached_property`, does the work with no extra class.[^17][^21][^19][^18]

Do not hide a remote call behind a local interface when the caller must handle its failures. Give the remote interface its own types, with timeouts and errors that the caller sees.[^4]

Do not use a protection proxy as the only check when other code can reach the real object. The check works only if every path to the object goes through the proxy.

## Variants and related patterns

**Decorator.** A decorator and a proxy have the same structure: an object with the same interface wraps another. Design Patterns separates them by purpose. A decorator adds responsibilities, and a program often stacks several of them. A proxy controls access to the subject, and it often makes or finds the subject itself.[^1] See [[decorator-pattern]].

**Adapter.** An adapter gives an object a different interface. A proxy gives it the same interface.[^1] See [[adapter-pattern]].

**Facade.** A facade gives a smaller interface to a group of objects. A proxy stands for one object. See [[facade-pattern]].

**Smart pointers.** Design Patterns shows a smart reference in C++ that overloads `operator->`.[^1] Norvig lists smart pointers among the patterns that are invisible in dynamic languages, and Proxy among the patterns that first-class types make less complex.[^2] In Rust, `Deref` lets a smart pointer such as `Box` or `Rc` forward method calls to the value it holds.[^16] Reference counting in a smart pointer does the work that a garbage collector does in other languages. See [[garbage-collection]] and [[rust/ownership|ownership]].

**Weak references.** Python's `weakref.proxy` returns a proxy that uses a weak reference to its object. The proxy does not keep the object alive, and it is not hashable.[^11]

## Known uses

- JavaScript: `Proxy` objects are part of the language. A handler object holds traps that the engine calls for the internal methods of the proxy, such as `[[Get]]` and `[[Set]]`.[^5][^6]
- Java: `java.lang.reflect.Proxy` makes at run time an object that implements a list of interfaces, and sends each call to an `InvocationHandler`.[^13]
- .NET: `System.Reflection.DispatchProxy` makes proxy objects and sends their method calls to one `Invoke` method.[^14]
- Python: a `multiprocessing` proxy refers to a shared object that lives, presumably, in a different process. Its methods call the methods of that object.[^12]
- Go: `httputil.ReverseProxy` is an HTTP handler that takes a request and sends it to another server.[^15]

## References

---
tag: NEW-objects-and-messages
kind: concept
slug: objects-and-messages
name: Objects and messages
aliases: [object, message, message passing, receiver, object identity, object state, boxing, everything is an object]
area: objects-and-classes/objects
level: core
summary: >-
  An object is a unit of a running program that holds state, offers behavior through messages or methods, and has an identity that tells it apart from objects with equal state.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1972
  people: [Alan Kay, Dan Ingalls, Adele Goldberg]
  note: Smalltalk made every value an object and all computing the sending of messages between objects, after the objects of Simula 67.
see_also: [object-oriented-programming, classes-and-instances, value-equality-and-identity, pointers-and-references, dynamic-dispatch, encapsulation-and-visibility, prototype-based-programming, actor-model]
same_as:
  wikidata: Q216601
  wikipedia: Object_(computer_science)
sources:
- url: https://docs.python.org/3/reference/datamodel.html#objects-values-and-types
  title: Data model, The Python Language Reference
  locator: 3.1 Objects, values and types
- url: https://doc.rust-lang.org/book/ch18-01-what-is-oo.html
  title: Characteristics of Object-Oriented Languages, The Rust Programming Language
  locator: Objects Contain Data and Behavior
- url: https://www.cs.virginia.edu/~evans/cs655/readings/smalltalk.html
  title: Design Principles Behind Smalltalk, Daniel H. H. Ingalls, BYTE Magazine, August 1981
  locator: Storage Management; Messages
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.3.1
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25 Edition
  locator: 4.3.1 Objects
- url: https://en.cppreference.com/cpp/language/object
  title: Object, cppreference.com
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Working_with_objects
  title: Working with objects, JavaScript Guide, MDN Web Docs
  locator: Introduction
---

## Idea

An object is a unit of a running program that holds state, offers behavior and has an identity. The state is the data in its fields. The behavior is the set of operations that other code can ask it to do, which object-oriented languages call methods or messages. The identity tells one object from another even when their state is equal. Code does not change an object's state directly: it sends the object a message, and the object decides what to do. Python, JavaScript, Java, C#, C++ and Smalltalk are built around objects, and Go, Rust and C write the same pattern with structs, methods and tables of functions.

```python run id=idea
def make_counter(start=0):
    count = start  # state that only receive can reach

    def receive(message, *args):  # behavior: answer a message
        nonlocal count
        if message == "inc":
            count += args[0] if args else 1
            return count
        if message == "value":
            return count
        return f"does not understand: {message}"

    return receive


a = make_counter()
b = make_counter(10)
a("inc")
a("inc", 5)
print(a("value"), b("value"))
print(b("reset"))

c = a  # a second name for the same object
c("inc")
print(a("value"), a is c, a is b)  # identity: c and a are one object
```

```output for=idea
6 10
does not understand: reset
7 True False
```

The program makes objects from closures, with no class at all. Each call to `make_counter` makes a new object: a function that keeps a private `count` and answers the messages `inc` and `value`. Code outside cannot reach `count` except by sending a message, and a message that the object does not know gets a reply, not a crash. `a` and `b` have separate state. `c = a` makes no new object, so a message sent through `c` changes what `a` reports, and `is` shows that `a` and `c` are one object.

## How it works

**State, behavior and identity.** The Python data model states the three parts: "Every object has an identity, a type and a value. An object’s identity never changes once it has been created; you may think of it as the object’s address in memory."[^1] The Rust book quotes the definition of the Design Patterns book: "Object-oriented programs are made up of objects. An object packages both data and the procedures that operate on that data."[^2] Identity matters when state can change: two accounts with the same balance are still two accounts, and a deposit to one must not reach the other. See [value equality and identity](/value-equality-and-identity/).

**Messages and receivers.** Smalltalk took the view that all computing is objects sending messages. Dan Ingalls stated the principle in 1981: "Computing should be viewed as an intrinsic capability of objects that can be uniformly invoked by sending messages."[^3] The object that gets a message is the receiver, and it chooses the method that answers it, "with the understanding that the receiver knows best how to carry out the desired operation".[^3] In most languages, sending a message is a method call such as `a.deposit(50)`, and the choice of method at run time is [dynamic dispatch](/dynamic-dispatch/). Python and Smalltalk also let an object answer a message for which it has no method, through `__getattr__` and `doesNotUnderstand:`.

**References to objects.** In Python, JavaScript, Java and C#, variables do not contain objects; they contain references to objects. The Java Language Specification states it: "An object is a class instance or an array. The reference values (often just references) are pointers to these objects, and a special null reference, which refers to no object."[^4] Assignment copies the reference, so two variables can reach one object, and a change through either one is visible through both. This sharing, called aliasing, is how a program builds graphs of objects, and it is also the source of many bugs. See [pointers and references](/pointers-and-references/).

**Objects as storage.** C and C++ use the word object in a wider sense: any region of storage that holds a value, including a single `int`. cppreference lists the properties: "An object, in C++, has size (can be determined with sizeof); alignment requirement (can be determined with alignof); storage duration (automatic, static, dynamic, thread-local); lifetime (bounded by storage duration or temporary); type; value (which may be indeterminate, e.g. for default-initialized non-class types); optionally, a name."[^5] In this sense objects are values, and assignment copies the state into a second object. The two meanings meet in C++ classes, whose objects have storage and also behavior.

**Objects without classes.** A class is one way to make objects with the same shape, but not the only way. MDN defines a JavaScript object without classes: "An object is a collection of properties, and a property is an association between a name (or key) and a value."[^6] Object literals in JavaScript, anonymous types in C#, anonymous classes in Java, closures that hold private variables, and prototypes all make objects. Go and Rust put methods on any named type, and an interface value or trait object pairs the data with a table of the methods of its type. See [classes and instances](/classes-and-instances/) and [prototype-based programming](/prototype-based-programming/).

**Everything an object, or not.** In Smalltalk, Python and Ruby, every value is an object, including numbers, functions and classes, so every value answers messages. Java and JavaScript keep primitive values, such as `int` and JavaScript numbers, outside the world of objects for speed, and wrap them in objects when code treats them as objects; Java calls this boxing. C# unifies the types, so `int` derives from `object`, but it boxes a value type when code stores it as `object`. Ingalls gave the reason for the uniform design: "Uniform reference is achieved simply by associating a unique integer with every object in the system."[^3]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | every value is an object with an identity, a type and a value |
| JavaScript | native | objects are collections of properties; primitives are wrapped |
| TypeScript | native | as JavaScript, with object types checked by the compiler |
| Java | native | class instances and arrays; primitives are boxed |
| Kotlin | native | every value is an object in the language; `object` declarations |
| C# | native | every type derives from `object`; value types are boxed |
| Go | idiom | methods on any named type; interface values |
| Rust | idiom | structs and enums with `impl`; trait objects |
| C | idiom | structs with tables of function pointers |
| C++ | native | objects as typed storage; classes with virtual functions |
| Bash | n/a | values are strings and arrays of strings |
| PowerShell | native | every pipeline value is a .NET object |
| SQL | idiom | rows hold state and keys give identity |
| Regex | n/a | no objects |
| HTML | idiom | each element is a DOM object |
| CSS | n/a | no objects |

The languages split on two questions. The first is whether every value is an object: Python and Kotlin say yes, C# says yes with boxing for value types, and Java and JavaScript keep primitives apart. The second is whether variables hold objects or references to them: Python, JavaScript and Java always use references, C++ and C hold objects in variables and use pointers to share them, and C#, Go and Rust let the type or the code choose.

Each example file shows how the language makes objects, how a message reaches its receiver, and what an assignment copies. The Python wrong block shows three rows of a grid that are one list object, and the JavaScript wrong block shows a property set on a string that disappears, because a string is not an object.

## Variations and related ideas

- **Actors.** An actor is an object that runs on its own and gets its messages through a mailbox, one at a time. Erlang processes and Akka actors work in this way. See [the actor model](/actor-model/).
- **Value objects.** Some objects have no meaningful identity, such as dates, money amounts and points. They are immutable and compare by value. See [immutability](/immutability/).
- **Entities.** Other objects are defined by their identity, such as a customer or an order, whose state changes over time while the object stays the same.
- **Proxies and wrappers.** An object can stand in for another and forward its messages, as `Proxy` in JavaScript and `__getattr__` in Python allow. See [decorator pattern](/decorator-pattern/).
- **Closures as objects.** A closure that holds private variables is an object with one method, and a closure that dispatches on a message name is an object with many. See [closures](/closures/).
- **Object graphs.** Objects that hold references to other objects form a graph, which a program must copy, compare, serialize or free as a whole. See [reference counting](/reference-counting/).
- **Encapsulation.** An object hides its state behind its messages, so that it can keep its invariants. See [encapsulation](/encapsulation-and-visibility/).

## Pitfalls

- **Aliasing by accident.** Two variables that hold the same reference change one object. The wrong block in the Python file shows a grid whose rows are one list. Copy an object when two parts of a program need separate state.
- **Identity where value is meant.** Comparing objects with `==` in Java, or `is` in Python, tests whether they are the same object, not whether their values are equal. Use `equals` in Java and `==` in Python to compare values.
- **Primitives treated as objects.** A property set on a JavaScript string disappears, as the wrong block in the JavaScript file shows, and a boxed `Integer` in Java can be `null`. Know which values are objects in the language.
- **Objects that are only data.** An object whose fields every caller reads and writes directly has state but no behavior that protects it. Move the operations that change the state into the object.
- **Too many small objects.** Each object costs a header and an allocation in most managed languages. A hot loop that makes millions of short-lived objects can spend more time on allocation and collection than on its work. Use arrays of values, or reuse objects, where profiling shows the cost.
- **Unknown messages that fail late.** In dynamic languages, a message that an object cannot answer fails only when the program runs. Tests, type hints and static checkers find these errors earlier.

## Cost

An object costs the memory of its fields plus a header in most managed runtimes: 8 to 16 bytes in common JVM configurations, 16 bytes on 64-bit .NET, and more in CPython, where every value, even a small integer, is a heap object with a reference count and a pointer to its type. C, C++, Go and Rust objects without virtual functions have no header, so a struct of two `int` fields takes 8 bytes. Objects reached through references add one memory load for each access and scatter data across the heap, which costs cache misses; arrays of values keep data close together. A message send costs a direct call when the compiler knows the receiver's type, and a lookup or an indirect call otherwise.

## History

Ole-Johan Dahl and Kristen Nygaard introduced objects in Simula 67, where each object of a class held its own data and procedures and modelled one part of a simulated system. Alan Kay coined the term object-oriented programming around 1967, as a graduate student at the University of Utah. At Xerox PARC, Kay, Dan Ingalls, Adele Goldberg and others built Smalltalk in the 1970s, in which every value is an object and all computing is message sending.[^3] Carl Hewitt's actor model of 1973 took the idea of objects that communicate only by messages into concurrent computing.

In the 1980s, C++ and Objective-C added objects to C, and in the 1990s, Python, Java and JavaScript made objects the main way to structure programs. Java kept primitive types outside its objects for speed, and Java 5 added automatic boxing in 2004. C# unified its types under `object` in 2002. Go in 2009 and Rust in 2015 kept methods and interfaces but dropped classes and inheritance, which separated the idea of an object from the idea of a class.

## References

---
tag: 4ACQHF
kind: pattern
slug: observer-pattern
name: Observer pattern
aliases: [Observer, dependents, listener pattern]
area: software-design/behavioral-patterns
level: core
summary: >-
  A subject keeps a list of observers and calls each of them when its state changes. The subject knows only a small callback interface, not the types of its observers.
family: behavioral-design
polarity: positive
intent: "When one object changes, tell any number of other objects about the change, and keep the changed object free of their concrete types."
also_known_as: [dependents, publish-subscribe (loosely), listener]
languages_where_built_in: {csharp: csharp/events, javascript: javascript/eventtarget, java: java/property-change-support}
refactorings: [replace-hard-coded-notifications-with-observer]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Smalltalk-80 used a dependents mechanism in its model-view-controller framework. Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994).
see_also: [closures, garbage-collection, async-await]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Observer, page 293 (Also Known As, Consequences, Implementation, Known Uses)
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 8, Generalization, Replace Hard-Coded Notifications with Observer (motivation, benefits and liabilities)
  - paper: doi:10.1145/857076.857078
    title: Eugster, Felber, Guerraoui and Kermarrec, The many faces of publish/subscribe, ACM Computing Surveys 35(2), 2003
    locator: Abstract (decoupling in space, time and synchronization)
  - url: https://dom.spec.whatwg.org/#concept-event-listener-inner-invoke
    title: WHATWG DOM Standard
    locator: 2.9 Dispatching events, invoke and inner invoke
  - url: https://nodejs.org/api/events.html#eventtarget-error-handling
    title: Node.js documentation, Events
    locator: EventTarget error handling; Asynchronous vs. synchronous
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/delegates#216-delegate-invocation
    title: C# language specification, Delegates
    locator: 21.5 Delegate instantiation; 21.6 Delegate invocation
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/classes#1582-field-like-events
    title: C# language specification, Classes
    locator: 15.8.2 Field-like events
  - url: https://learn.microsoft.com/en-us/dotnet/desktop/wpf/events/weak-event-patterns
    title: Weak event patterns (WPF), Microsoft Learn
    locator: Why implement the weak event pattern
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.desktop/java/beans/PropertyChangeSupport.html
    title: Java SE 25 API, java.beans.PropertyChangeSupport
    locator: class description; firePropertyChange
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Observable.html
    title: Java SE 25 API, java.util.Observable
    locator: Deprecated note
  - url: https://docs.python.org/3.14/library/weakref.html#weakref.WeakMethod
    title: The Python Standard Library, weakref, Python 3.14
    locator: weakref.WeakMethod
  - url: https://doc.rust-lang.org/std/rc/struct.Weak.html
    title: Rust standard library, std::rc::Weak
    locator: struct description; method upgrade
  - url: https://go.dev/ref/spec#Send_statements
    title: The Go Programming Language Specification
    locator: Channel types; Send statements
  - url: https://kotlinlang.org/api/kotlinx.coroutines/kotlinx-coroutines-core/kotlinx.coroutines.flow/-shared-flow/
    title: kotlinx.coroutines API, SharedFlow
    locator: interface description
  - url: https://kotlinlang.org/api/kotlinx.coroutines/kotlinx-coroutines-core/kotlinx.coroutines.flow/-state-flow/
    title: kotlinx.coroutines API, StateFlow
    locator: interface description, conflation
  - url: https://github.com/reactive-streams/reactive-streams-jvm/blob/v1.0.4/README.md
    title: Reactive Streams for the JVM, specification 1.0.4
    locator: Goals, Design and Scope; 3. Subscription
  - url: https://openjdk.org/jeps/266
    title: JEP 266, More Concurrency Updates
    locator: Description, interfaces for reactive streams (java.util.concurrent.Flow)
  - url: https://github.com/tc39/proposal-signals
    title: TC39 proposal, JavaScript Signals standard proposal
    locator: README, status (Stage 1)
  - url: https://github.com/nodejs/node/blob/v26.8.1/lib/events.js#L489-L543
    title: Node.js v26.8.1 source, lib/events.js
    locator: EventEmitter.prototype.emit, lines 489 to 543
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.desktop/share/classes/java/beans/PropertyChangeSupport.java#L324-L347
    title: OpenJDK jdk-25+36 source, java/beans/PropertyChangeSupport.java
    locator: firePropertyChange and fire, lines 324 to 347
  - url: https://github.com/nodejs/node/blob/v26.8.1/lib/events.js#L907-L914
    title: Node.js v26.8.1 source, lib/events.js
    locator: ensureMutableListenerArray, lines 907 to 914
  - paper: Glenn E. Krasner and Stephen T. Pope, A cookbook for using the model-view controller user interface paradigm in Smalltalk-80, Journal of Object-Oriented Programming 1(3), 1988, pages 26 to 49
    title: Krasner and Pope, A cookbook for using the model-view controller user interface paradigm in Smalltalk-80, JOOP 1(3), 1988
    locator: the dependency mechanism between a model and its views (changed and update messages)
---

## Intent

When one object changes, tell any number of other objects about the change. The changed object knows only a small callback interface. It does not know the concrete types of the objects that react.[^1]

## Problem

A program often has one object whose state many other parts of the program follow. A temperature sensor gets a reading. A screen must show the value, an alarm must check a limit, and a log must store it. The number of these parts is not fixed: the user opens and closes windows, and a plugin can add a part at run time.

The direct design calls each part from the sensor: `screen.show(value)`, `alarm.check(value)`, `log.write(value)`. The sensor then depends on the screen, the alarm and the log. Each added part needs a change to the sensor and a rebuild of it. The sensor cannot go into a library, because the library would need to know the types of its users. A test of the sensor must build a screen.

The forces are these. The source of the change must stay independent of the code that reacts. The set of parts that react must change at run time. Each part must see a consistent state when it reacts.

## Solution

The sensor becomes a **subject**. It keeps a list of **observers**. An observer is any object (or function) that has one operation, often called `update` or `notify`. The subject has three operations: `subscribe` (also called attach or add listener), `unsubscribe` (detach, remove listener), and an internal `notify` that calls `update` on each observer in the list.[^1]

```text
 Subject                         Observer (interface)
 -------------------------       --------------------
 observers: list of Observer --> update(value)
 subscribe(o)                          ^
 unsubscribe(o)                        |
 set(value): store, then         +-----+------+
   for each o: o.update(value)   |            |
                              Display       Alarm
```

The participants are the subject, the observer interface, and the concrete observers. The subject depends on the interface only. The concrete observers depend on the subject, because they subscribe to it and may read it. So the dependency points from the many to the one, and the one stays reusable.

The sequence of one change is short. A client calls `set` on the subject. The subject stores the value, so that its state is consistent before anyone looks at it. Then it calls `update` on each observer in turn. An observer may call back into the subject to read more state. When the last observer returns, `set` returns to the client.

**Push and pull.** In the push model the subject sends the changed data with the call: `update(21.5)`. In the pull model the subject sends only "I changed", often with a reference to itself, and each observer reads what it needs: `update(sensor)`, then `sensor.value`.[^1] Push is cheaper when every observer needs the same data. Pull keeps the interface stable when observers need different parts of a large state, but each observer must then know the interface of the subject. Many event systems mix the two: the event object carries a small payload and a reference to the source (`sender` in .NET, `event.target` in the DOM).

**Synchronous and asynchronous notification.** In the classic form, `notify` calls each observer in turn on the same thread, and `set` returns only after the last observer returns. DOM `dispatchEvent`, Node.js `EventEmitter.emit`, C# events and `PropertyChangeSupport` all work this way.[^4][^5][^6] In the asynchronous form, the subject puts the change in a queue or a channel, and the observer handles it later or on another thread. The subject then does not wait for slow observers. But an observer can see a value after the subject has changed again, and the program must decide what happens when a queue fills up.

## Example

The example files show one program in each language. A sensor has two observers: a display that prints each reading, and an alarm that prints a warning above 30. The program sends 21.5 and 30.5, removes the display, and sends 31.0. Each language uses its own usual form:

| Language | Form |
|---|---|
| Python | a list of callables, iterated as a copy |
| JavaScript | `EventTarget` with `addEventListener`, removal through an `AbortSignal` |
| TypeScript | a generic subject that returns a `Disposable`, removed by `using` |
| Java | `java.beans.PropertyChangeSupport` with lambdas as `PropertyChangeListener` |
| Kotlin | `Delegates.observable` and a list of function values |
| C# | an `event` of type `EventHandler<double>` with `+=` and `-=` |
| Go | one buffered channel for each subscriber, read by a goroutine |
| Rust | a list of `Weak<dyn Observer>` trait objects |
| C | an array of function pointers, each with a `void *` context |
| C++ | `std::function` slots and a move-only subscription that disconnects in its destructor |
| PowerShell | a list of script blocks |
| Bash | an array of function names |
| SQL | triggers on the table that holds the reading |

## Consequences

The subject and the observers are loosely coupled. The subject can live in a lower layer or a library, and it works with observers written later. Observers can be added and removed at run time. One change reaches any number of receivers with one call.[^1][^2]

The costs come from the indirection. A reader of `set` cannot see which code runs next, and a debugger shows a call through a list. Design Patterns calls this the risk of unexpected updates: an operation on the subject that looks harmless can start a long chain of updates in the observers.[^1] Each implementation must also decide the five questions below.

**The lapsed listener.** The subject holds a reference to each observer. In a language with a garbage collector, that reference keeps the observer reachable. When the program closes a window but does not unsubscribe it, the window stays in memory and still gets updates. Kerievsky lists this memory leak as a liability of the pattern, and Microsoft documents the weak event pattern of WPF for the same reason.[^2][^8] The fixes are: unsubscribe in the close or dispose step of the observer, tie the subscription to a scope (the `using` block in the TypeScript example, the destructor in the C++ example, the `AbortSignal` in the JavaScript example), or let the subject hold weak references (`weakref.WeakMethod` in Python, `Weak<T>` in Rust).[^11][^12] Weak references have a cost: a lambda or a bound method that nothing else holds disappears at once, so the observer stops with no error.

```python wrong id=lapsed
import weakref


class Sensor:
    def __init__(self):
        self.strong = []
        self.weak = []

    def subscribe(self, method):
        self.strong.append(method)

    def subscribe_weak(self, method):
        self.weak.append(weakref.WeakMethod(method))

    def set(self, value):
        self.weak = [ref for ref in self.weak if ref() is not None]
        for method in self.strong + [ref() for ref in self.weak]:
            method(value)


class Window:
    def __init__(self, name):
        self.name = name

    def on_reading(self, value):
        print(f"{self.name} got {value}")


sensor = Sensor()
first = Window("first")
second = Window("second")
sensor.subscribe(first.on_reading)
sensor.subscribe_weak(second.on_reading)
del first, second  # the program closes both windows
sensor.set(1)
print(f"strong: {len(sensor.strong)}, weak: {len(sensor.weak)}")
```

```output for=lapsed
first got 1
strong: 1, weak: 0
```

The program deletes its last name for both windows. The first window still gets the reading, because the bound method in the strong list holds it. The weak entry for the second window is dead, so the subject drops it. CPython frees the second window at once because it counts references. A tracing collector frees it at a later collection.

**The order of notification.** Most implementations call observers in the order of subscription. The DOM Standard keeps the listeners of a target in an ordered list, Node.js documents that `EventEmitter` calls its listeners in the order of registration, and a C# delegate calls its invocation list in order.[^4][^5][^6] Other implementations do not promise an order. The deprecation note of `java.util.Observable` names its unspecified order of notification as one of the reasons to stop using the class.[^10] Code that needs the display to update before the log writes depends on an accident unless the subject documents the order. Make each observer independent of the others, or make the order an explicit part of the contract of the subject.

**Re-entrancy.** An observer can call back into the subject while the subject is notifying. It can unsubscribe itself, subscribe another observer, or change the state of the subject. If the subject iterates over its live list, a removal moves the next observer into the current index, and the loop skips it:

```python wrong id=reentry-wrong
class Subject:
    def __init__(self):
        self.observers = []

    def notify(self, value):
        for observer in self.observers:  # iterates the live list
            observer(self, value)


def once(subject, value):
    print(f"once: {value}")
    subject.observers.remove(once)


def logger(subject, value):
    print(f"logger: {value}")


subject = Subject()
subject.observers += [once, logger]
subject.notify(1)
subject.notify(2)
```

```output for=reentry-wrong
once: 1
logger: 2
```

The logger missed the value 1. The fix is to iterate over a snapshot. The DOM Standard does this: it clones the listener list before it calls the listeners, so a listener added during a dispatch does not run in that dispatch. A removed listener gets a `removed` flag, and the loop skips it.[^4] Node.js `EventEmitter` gets the same effect with copy on write: while an emit runs, a change to the listener array first makes a copy, and the loop in progress keeps the old array.[^21]

```python run id=reentry-fixed
class Subject:
    def __init__(self):
        self.observers = []

    def notify(self, value):
        for observer in list(self.observers):  # iterates a snapshot
            observer(self, value)


def once(subject, value):
    print(f"once: {value}")
    subject.observers.remove(once)


def logger(subject, value):
    print(f"logger: {value}")


subject = Subject()
subject.observers += [once, logger]
subject.notify(1)
subject.notify(2)
```

```output for=reentry-fixed
once: 1
logger: 1
logger: 2
```

A snapshot does not solve the other form of re-entrancy. When an observer changes the subject during a notification, the subject starts a nested notification. The observers later in the list see the second value first, and then the first value, which is out of date:

```python wrong id=nested
class Thermostat:
    def __init__(self):
        self.value = 0
        self.observers = []

    def set(self, value):
        self.value = value
        for observer in list(self.observers):
            observer(self, value)


def clamp(thermostat, value):
    if value > 30:
        thermostat.set(30)  # changes the subject during notification


def display(thermostat, value):
    print(f"display: {value} (subject holds {thermostat.value})")


thermostat = Thermostat()
thermostat.observers += [clamp, display]
thermostat.set(35)
```

```output for=nested
display: 30 (subject holds 30)
display: 35 (subject holds 30)
```

The display ends on 35, but the subject holds 30. The usual fixes are: pull the state instead of trusting the pushed value, queue the nested change until the current notification ends, or move the rule (the clamp) into the subject, so that observers never write to it.

**An exception in one observer.** In a synchronous loop, an exception from one observer leaves the loop, and the observers after it never run. A C# delegate behaves this way: an exception that the handler does not catch stops the invocation, and the methods later in the invocation list are not called.[^6] `EventEmitter.emit` also lets the exception out of the loop.[^19] The DOM takes the other choice: it reports the exception and goes on with the next listener.[^4] Node.js does the same for `EventTarget`, but then raises the error as an uncaught exception on `process.nextTick()`, which ends the process by default.[^5] A subject that must reach every observer catches the exception of each observer:

```python run id=isolate
def broken(value):
    raise ValueError(f"cannot handle {value}")


def logger(value):
    print(f"logger: {value}")


def notify(observers, value):
    for observer in list(observers):
        try:
            observer(value)
        except Exception as error:
            print(f"{observer.__name__} failed: {error}")


notify([broken, logger], 7)
```

```output for=isolate
broken failed: cannot handle 7
logger: 7
```

**Threads.** If one thread subscribes while another notifies, the list needs a lock or a copy-on-write structure. A subject that calls its observers while it holds its own lock can deadlock when an observer calls back into the subject. The Go example holds a lock while it sends on the channels, so a full channel blocks the subject. Each subscriber reads its own channel on its own goroutine.[^13]

## When not to use it

Do not use the pattern when there is one receiver that does not change. A direct call is clearer, and the reader can follow it. Kerievsky lists this as a liability: the pattern complicates a design where a hard-coded notification is enough. He also advises to start with the hard-coded notification and to change to an observer when the need is real.[^2]

Do not use it to build a chain of reactions, where one observer updates a second subject, which notifies a third. Such cascades are hard to trace, can loop, and can deliver values out of order, as the thermostat example shows. Kerievsky names cascading notifications as a liability and suggests a mediator for such code.[^2] A mediator that owns the order of the updates, or a dataflow model with signals, handles chains better.

Do not use it across process or network boundaries in its plain form. A remote observer can fail, be slow or be gone, and a synchronous call to it blocks the subject. Use a message broker with publish-subscribe instead.

An observer list that no one removes entries from becomes the lapsed listener leak. A program with many subjects, where each observer reacts by changing other subjects, has control flow that no one can read from the source code.

## Variants and related patterns

**Event objects and multicast delegates.** C# builds the pattern into the language. An `event` is a delegate field whose invocation list is the observer list, and only the declaring class can raise it.[^7] A delegate instance is immutable, so `+=` makes a second, combined delegate, and a raise that has already read the field calls the old list.[^6] The DOM `EventTarget` and the Node.js `EventEmitter` key their observers by an event name, so one subject has many lists.[^4][^5]

**Property change events.** `java.beans.PropertyChangeSupport` is a helper that a class holds as a field. It sends an event with the name of the property, the old value and the changed value. It sends nothing when the old and the changed values are equal and not null.[^9][^20] Kotlin `Delegates.observable` attaches a callback to one property in the same way.

**Publish-subscribe.** In publish-subscribe, publishers send messages to a topic or a channel, and subscribers register with the channel, not with the publisher. An intermediary decouples the two sides: an event bus in one process, or a broker such as an MQTT or Kafka server across processes. Eugster and others describe three kinds of decoupling: in space (the two sides do not know each other), in time (they need not be active at the same moment), and in synchronization (the publisher does not block).[^3] The classic Observer pattern gives only part of the first kind. The subject knows its observers through the interface, and each observer knows the subject. It gives no decoupling in time, because an observer must exist when the subject notifies.

**Reactive streams.** A stream turns the observer into a subscriber with three callbacks: the next value, an error, and completion. The Reactive Streams specification adds backpressure: the subscriber calls `request(n)`, and the publisher may send at most `n` items.[^16] This bounds the queues that asynchronous notification needs. Java `java.util.concurrent.Flow` holds the same four interfaces (`Publisher`, `Subscriber`, `Subscription` and `Processor`).[^17] Kotlin `SharedFlow` is a hot flow that broadcasts each value to all collectors, and `StateFlow` keeps one current value and drops a value equal to the previous one.[^14][^15] These flows come from the kotlinx.coroutines library, not from the Kotlin standard library, so the Kotlin example uses `Delegates.observable` instead.

**Signals.** A signal is a cell that holds one current value and records which computations read it. A computed signal does not subscribe by hand. It reads other signals, and the runtime records those reads as its dependencies. A write marks the dependent computations as stale, and a computed signal runs again only when someone reads it. This pull step avoids the out-of-date values of a push cascade (a "glitch"), at a cost: two writes with no read between them show only the second value. The TC39 proposal for signals in JavaScript is at Stage 1.[^18]

**Mediator.** A mediator also decouples objects, but it puts the logic of who reacts to what in one place. The Observer pattern spreads that logic over the observers. The two can work together. Design Patterns describes a change manager that sits between subjects and observers and fixes the update order, and it calls this change manager an instance of Mediator.[^1]

**Model-view-controller.** In MVC, the views observe the model. When the model changes, it tells its dependents, and each view asks the model for the data that it shows.[^22] Design Patterns names Smalltalk MVC as the first example of Observer.[^1]

## Known uses

- Node.js: `EventEmitter.prototype.emit` in [lib/events.js at v26.8.1](https://github.com/nodejs/node/blob/v26.8.1/lib/events.js#L489-L543) calls the listeners of one event name in order, synchronously, in a loop over the listener array. A counter marks the array while an emit runs, and [ensureMutableListenerArray](https://github.com/nodejs/node/blob/v26.8.1/lib/events.js#L907-L914) copies it before a change.
- OpenJDK: `firePropertyChange` in [PropertyChangeSupport.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.desktop/share/classes/java/beans/PropertyChangeSupport.java#L324-L347) skips the notification when the old and the changed values are equal and not null. Otherwise it calls each listener in a loop over an array.
- Smalltalk-80: the dependents mechanism of `Object` and the model-view-controller framework, as Design Patterns records in the Known Uses section of Observer, and as Krasner and Pope describe for MVC.[^1][^22]

## References

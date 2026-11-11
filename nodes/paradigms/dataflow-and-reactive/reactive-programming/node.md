---
tag: R0DNQJ
kind: concept
slug: reactive-programming
name: Reactive programming
aliases: [dataflow programming, signals, observables, functional reactive programming, FRP, reactive streams, spreadsheet model]
area: paradigms/dataflow-and-reactive
level: core
summary: >-
  Reactive programming states how values depend on each other, as cells in a spreadsheet do, and a runtime updates the dependent values or streams when their inputs change.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1997
  people: [Conal Elliott, Paul Hudak, Erik Meijer]
  note: Elliott and Hudak introduced functional reactive programming in 1997, and Meijer's Reactive Extensions made observable streams common around 2009.
see_also: [event-driven-programming, functional-programming, declarative-programming, laziness, higher-order-functions, iterators, channels, memory-leaks]
same_as:
  wikidata: Q4354073
  wikipedia: Reactive_programming
sources:
- url: https://github.com/tc39/proposal-signals
  title: JavaScript Signals standard proposal, TC39
  locator: Computed Signals; FAQ on push and pull
- url: https://reactivex.io/intro.html
  title: ReactiveX Introduction
  locator: ReactiveX
- url: https://www.reactive-streams.org/
  title: Reactive Streams
  locator: introduction
---

## Idea

Reactive programming states how values depend on each other, and lets a runtime keep the dependent values up to date when their inputs change. A spreadsheet is the common example: a cell holds a formula such as `=A1*B1`, and when `A1` changes, the cell changes too, with no code to update it. Dataflow programming is the wider idea: a program is a graph of operations, and data flows along the edges, so each operation runs when its inputs are ready. The example below gives signals to Python. A computed signal records which signals it reads, and it computes again only when one of them has changed and someone reads it:

```python run id=idea
class Signal:
    current = None  # the computed signal that is being evaluated now

    def __init__(self, value=None, compute=None):
        self.value, self.compute = value, compute
        self.dirty = compute is not None
        self.dependents = set()

    def get(self):
        if Signal.current is not None:
            self.dependents.add(Signal.current)  # record who read this signal
        if self.dirty:
            outer, Signal.current = Signal.current, self
            try:
                self.value = self.compute()
            finally:
                Signal.current = outer
            self.dirty = False
        return self.value

    def set(self, value):
        self.value = value
        for d in list(self.dependents):
            d.invalidate()

    def invalidate(self):
        if not self.dirty:
            self.dirty = True
            for d in list(self.dependents):
                d.invalidate()


log = []


def computed(name, fn):
    def run():
        log.append(name)
        return fn()

    return Signal(compute=run)


price, qty, shipping = Signal(10), Signal(3), Signal(5)
subtotal = computed("subtotal", lambda: price.get() * qty.get())
total = computed("total", lambda: subtotal.get() + shipping.get())

print(total.get(), log)
qty.set(4)
shipping.set(0)
print(total.get(), log[2:])
print(total.get(), log[4:])
```

```output for=idea
35 ['total', 'subtotal']
40 ['total', 'subtotal']
40 []
```

Nothing in the code tells `total` to update when `qty` changes. `total` read `subtotal`, which read `qty`, so the runtime knows the chain. Two changes in a row cost only one computation of each formula, because a change only marks the computed values as dirty, and the work happens when `total` is read. A read with no change costs no computation at all.

## How it works

**The dependency graph.** A reactive program is a directed graph. Sources, such as input fields, sensors or state signals, are the roots. Derived values and effects, such as computed signals, rendered views and writes to the screen, are the other nodes. An edge goes from each value to each value that uses it. A change at a source must reach every node that depends on it, and only those.

**Automatic dependency tracking.** Many systems find the edges at run time. While a computed value runs, the runtime records each signal it reads, as `Signal.current` does in the example. The TC39 proposal for signals in JavaScript describes the same method: "Computed Signals work by automatically tracking which other Signals are read during their evaluation."[^1] Because the edges come from the actual reads, a value that reads `a` only in one branch of an `if` depends on `a` only when that branch runs.

**Push, pull and glitches.** A change can be pushed: the source calls its dependents at once. Or it can be pulled: the dependents compute only when read. Pure push has a problem in graphs shaped like a diamond. If `d` depends on `b` and `c`, and both depend on `a`, then a push from `a` can update `d` after `b` changes and before `c` does. The value of `d` in between is a glitch, a state that never existed. The wrong block in the Python file shows one. Systems avoid glitches by updating in topological order, or by the push-pull method of the example: push a dirty mark, then pull the values. The signals proposal calls this a "push-pull" construction and requires that computation is "glitch-free".[^1]

**Streams of events.** A second family treats events over time as a collection. ReactiveX describes itself as "a library for composing asynchronous and event-based programs by using observable sequences", which "extends the observer pattern to support sequences of data and/or events".[^2] An observable is like a list that arrives over time, and operators such as `map`, `filter`, `scan`, `merge` and `debounce` make new observables from old ones, the way [higher-order functions](/higher-order-functions/) make new lists.

**Back-pressure.** When a producer is faster than a consumer, the items must wait somewhere, and an unbounded buffer can use all memory. Reactive Streams is "an initiative to provide a standard for asynchronous stream processing with non-blocking back pressure."[^3] A subscriber states how many items it can take, and the publisher sends no more than that. Java put these interfaces in its standard library as `java.util.concurrent.Flow`, which the Java file of this page uses.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | closures and generators; RxPY |
| JavaScript | idiom | `Proxy`, events; RxJS and signal libraries |
| TypeScript | idiom | as JavaScript |
| Java | stdlib | `java.util.concurrent.Flow`, `SubmissionPublisher`; Reactor, RxJava |
| Kotlin | library | `Flow` and `StateFlow` in kotlinx.coroutines |
| C# | stdlib | `IObservable<T>` and `IObserver<T>`; Rx.NET for operators |
| Go | idiom | goroutines connected by channels |
| Rust | idiom | iterators; `Rc<RefCell<T>>` with callbacks; stream crates |
| C | absent | a table of formulas in topological order |
| C++ | idiom | lazy range adaptors; RxCpp, Qt properties |
| Bash | idiom | pipelines of processes |
| PowerShell | idiom | the object pipeline |
| SQL | idiom | views and materialized views |
| Regex | n/a | no values that change |
| HTML | n/a | no derived values |
| CSS | idiom | custom properties and `calc()` |

No core language has reactive values in its syntax. Java and C# have the interfaces of reactive streams in the standard library, and leave the operators to libraries. In JavaScript, reactive programming is mostly the work of user interface frameworks. React computes the view again from state, Vue tracks reads through proxies as in the JavaScript file, and Solid, Svelte, Angular and Preact use signals. The signals proposal aims to give these frameworks one shared base.

Dataflow without reactivity is more common. Go programs connect goroutines with channels into pipelines in which every stage runs at the same time. Iterators in Rust, ranges in C++, streams in Java and generators in Python are pull-based pipelines: each stage asks the one before it for the next value. Unix pipelines connect processes in the same way, and the operating system gives them back-pressure through the fixed size of the pipe buffer.

## Variations and related ideas

- **Spreadsheets.** The most used reactive system. Cells form a dependency graph, and the program recomputes the cells that a change affects.
- **Functional reactive programming (FRP).** Conal Elliott and Paul Hudak defined behaviors, values that vary over continuous time, and events, values at discrete times, as first-class values in Haskell. See [functional programming](/functional-programming/).
- **Observables and Rx.** Push-based streams of events with a rich set of operators.
- **Signals.** Single values that change over time, with automatic dependency tracking and lazy computation.
- **Dataflow languages.** Visual languages such as LabVIEW and Max, and stream languages such as Lustre for control systems, write programs as graphs of operations.
- **Synchronous languages.** Esterel and Lustre assume that each reaction takes no time, which lets a compiler check timing for embedded systems.
- **Incremental computation.** Build systems and compilers keep a graph of derived results and compute again only what a change affects.
- **Event-driven programming.** Reactive systems usually run on an event loop. See [event-driven programming](/event-driven-programming/).
- **Laziness.** Pull-based dataflow computes values only when asked. See [laziness](/laziness/).

## Pitfalls

- **Glitches.** Pushing changes one by one through a diamond-shaped graph can show values that never existed together. The wrong block in the Python file shows one. Use a library that updates in topological order.
- **Cycles.** A value that depends on itself, directly or through others, has no fixed meaning, and a naive runtime loops forever. Most libraries detect cycles and throw an error.
- **Effects that write signals.** An effect that writes to a signal that it also reads runs again and again. Keep effects at the edges of the graph, and derive values with computed signals instead.
- **Leaked subscriptions.** A subscriber that never unsubscribes stays in the source's list, with all that its closure holds. Dispose of subscriptions when their owner goes away. See [memory leaks](/memory-leaks/).
- **Cold and hot observables.** Each subscription to a cold observable runs the source again, which can repeat a network request. Share a single run when more than one subscriber needs it.
- **Hidden control flow.** A change can start a long chain of updates in code far away. Stack traces show the runtime, not the cause, and debugging needs tools that show the graph.

## Cost

A reactive runtime adds memory for the graph: each derived value keeps a list of its sources and a list of its dependents, and automatic tracking rebuilds the lists of a value each time it computes. A change costs time in proportion to the number of nodes it reaches, not the size of the program, which is the main gain over computing the whole view again. Lazy evaluation adds a check of the dirty flag on each read but skips work that no one reads. Stream operators cost an allocation and a function call for each item at each stage, which matters at millions of items per second, and libraries reduce it by fusing operators. Channel pipelines in Go cost a synchronization for each item at each stage, so programs send batches when items are small.

## History

Dataflow ideas came from hardware and from spreadsheets. Dataflow machines were research topics at MIT and Manchester in the 1970s. Dan Bricklin and Bob Frankston released VisiCalc in 1979, the first spreadsheet for personal computers, and made reactive recalculation familiar to millions of users. Synchronous dataflow languages for control systems, Lustre and Esterel, came from France in the 1980s. Conal Elliott and Paul Hudak published "Functional Reactive Animation" at ICFP in 1997, which started functional reactive programming. Erik Meijer and his team at Microsoft built Reactive Extensions for .NET around 2009, and ports such as RxJava and RxJS followed. The Reactive Streams standard came in 2015, and Java 9 added its interfaces as `java.util.concurrent.Flow` in 2017. Knockout and later Vue, Solid, Svelte, Angular and Preact made signals the base of user interface frameworks in JavaScript, and a proposal to add signals to the language was presented to TC39 in 2024.

## References

---
tag: SG4FMR
kind: pattern
slug: iterator-pattern
name: Iterator pattern
aliases: [Iterator, Cursor, enumerator]
area: software-design/behavioral-patterns
level: core
summary: >-
  An iterator object gives the elements of a collection one at a time and keeps the position, so a client can walk any collection without knowing how it stores them.
family: behavioral-design
polarity: positive
intent: "Give sequential access to the elements of an aggregate object without exposing how the aggregate stores them."
also_known_as: [Cursor]
languages_where_built_in: {python: python/iterator-protocol, javascript: javascript/iteration-protocols, java: java/iterable, csharp: csharp/ienumerable, rust: rust/iterator-trait, go: go/range-over-func, cpp: cpp/ranges}
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: CLU had iterators in the 1970s. Gamma, Helm, Johnson and Vlissides named and described the object-oriented pattern in Design Patterns (1994), with Cursor as another name.
see_also: [iterators, composite-pattern, factory-method-pattern, visitor-pattern, memento-pattern, recursion, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Iterator (Intent, Also Known As, Motivation, Participants, Consequences, Implementation, Related Patterns)
  - url: https://docs.python.org/3.14/library/stdtypes.html#iterator-types
    title: The Python Standard Library, Built-in Types, Python 3.14
    locator: Iterator Types, container.__iter__, iterator.__next__
  - url: https://peps.python.org/pep-0255/#motivation
    title: "PEP 255: Simple Generators"
    locator: Motivation
  - url: https://docs.python.org/3.14/reference/expressions.html#yield-expressions
    title: The Python Language Reference, Expressions, Python 3.14
    locator: Yield expressions
  - url: https://tc39.es/ecma262/2026/#sec-iterable-interface
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.1.1.1 The Iterable Interface; 27.1.1.2 The Iterator Interface
  - url: https://tc39.es/ecma262/2026/#sec-generator-objects
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: Generator Objects
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Iterable.html
    title: Java SE 25 API, java.lang.Iterable
    locator: interface description; forEach(Consumer)
  - url: https://go.dev/doc/go1.23#language
    title: Go 1.23 Release Notes
    locator: Changes to the language
  - url: https://pkg.go.dev/iter#hdr-Pulling_Values
    title: Go standard library, package iter
    locator: Iterators; Pulling Values; func Pull
  - url: https://doc.rust-lang.org/std/iter/index.html#implementing-iterator
    title: The Rust Standard Library, Module std::iter
    locator: Implementing Iterator
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/yield#execution-of-an-iterator
    title: C# language reference, yield statement
    locator: Execution of an iterator
  - url: https://timsong-cpp.github.io/cppwp/n4950/range.range
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[range.range], the concept range"
  - url: https://timsong-cpp.github.io/cppwp/n4950/coro.generator
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "26.8 Range generators [coro.generator], 26.8.1 Overview"
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/ArrayList.java#L1036-L1098
    title: OpenJDK jdk-25+36 source, java/util/ArrayList.java
    locator: class Itr, lines 1036 to 1098
  - url: https://github.com/golang/go/blob/go1.23.0/src/maps/iter.go#L22-L33
    title: Go go1.23.0 source, src/maps/iter.go
    locator: func Keys, lines 22 to 33
  - paper: doi:10.1145/154766.155367
    title: Barbara Liskov, A History of CLU, HOPL-II, 1993
    locator: 3.10 Iterators
---

## Intent

Give sequential access to the elements of an aggregate object, such as a list or a tree, without exposing how the aggregate stores them.[^1] The object that walks the aggregate is the **iterator**. It holds the position of one traversal, so the aggregate does not have to.

This page is about the design pattern: the roles, the choices and the trade-offs. The language rules of each iteration protocol (the loop statements, the end signal, the pitfalls of each language) are on [[iterators]].

## Problem

A binary search tree stores its keys in nodes with left and right children. A client wants the keys in ascending order. It does not want to know about nodes, and the tree does not want to expose them, because then it could never change its layout.

The first design puts the traversal in the tree: a method `in_order()` that returns a list. This copies every key before the client sees the first one, and a client that needs only the smallest key pays for all of them. The second design gives the tree a cursor: `first()`, `next()`, `done()`. Now the tree holds the position, so only one traversal can run at a time. A client that compares two positions in the same tree, or a loop inside a loop, breaks.

The forces are these. The client must not depend on the representation of the aggregate. The aggregate must support more than one way to traverse it (in order, by level, in reverse) without a method for each in its own interface. More than one traversal must be able to run at the same time.[^1] And one loop statement, one `sum` and one `take` should work for every kind of collection.

## Solution

Move the position out of the aggregate into a separate object. Design Patterns names four participants.[^1]

- **Iterator** defines the interface to get the elements one at a time.
- **ConcreteIterator** implements that interface for one aggregate and one order of traversal. It keeps track of the current position.
- **Aggregate** defines an operation that makes an iterator.
- **ConcreteAggregate** implements that operation and returns a ConcreteIterator.

```text
  Client
    |  for key in tree
    v
  Aggregate (Tree)                    Iterator (protocol)
  ------------------------            --------------------
  root (private)                      next(): element or end
  make_iterator() ----- creates ----> ^
                                      |
                              InOrderIterator
                              stack of nodes that are left
```

The operation that makes the iterator is a factory method: the aggregate decides which iterator class the client gets.[^1] See [[factory-method-pattern]]. Because each call makes another iterator with its own state, two traversals of the same tree are two objects and do not disturb each other.

Python shows the classic form. `Tree.__iter__` is the factory method, and `InOrderIterator` keeps a stack of the nodes whose keys it has not yet given. Python asks an iterator for `__next__`, which raises `StopIteration` at the end, and for an `__iter__` that returns the iterator itself.[^2]

```python run id=classic
class Node:
    def __init__(self, key, left=None, right=None):
        self.key, self.left, self.right = key, left, right


class InOrderIterator:
    def __init__(self, root):
        self._stack = []
        self._push_left(root)

    def _push_left(self, node):
        while node is not None:
            self._stack.append(node)
            node = node.left

    def __iter__(self):
        return self

    def __next__(self):
        if not self._stack:
            raise StopIteration
        node = self._stack.pop()
        self._push_left(node.right)
        return node.key


class Tree:
    def __init__(self, root):
        self._root = root

    def __iter__(self):  # the factory method
        return InOrderIterator(self._root)


tree = Tree(Node(5, Node(3, Node(1), Node(4)), Node(8, None, Node(9))))
print(list(tree))
print([(x, y) for x in tree for y in tree if x + y == 9])
```

```output
[1, 3, 4, 5, 8, 9]
[(1, 8), (4, 5), (5, 4), (8, 1)]
```

The nested comprehension runs two traversals of one tree at the same time. Each `for` asks the tree for its own iterator.

### Generators write the iterator for you

The iterator class above stores its position by hand, in a stack. A generator stores it in a suspended function call. The body runs up to `yield`, gives a value, and resumes from that point on the next request. PEP 255 gives this as the reason for generators: a producer that must keep state between values otherwise needs a callback or a hand-written state object.[^3][^4] The same tree with a generator needs no iterator class:

```python run id=generator
class Node:
    def __init__(self, key, left=None, right=None):
        self.key, self.left, self.right = key, left, right


class Tree:
    def __init__(self, root):
        self._root = root

    def __iter__(self):
        def walk(node):
            if node is not None:
                yield from walk(node.left)
                yield node.key
                yield from walk(node.right)

        return walk(self._root)


tree = Tree(Node(5, Node(3, Node(1), Node(4)), Node(8, None, Node(9))))
print(list(tree))
```

```output
[1, 3, 4, 5, 8, 9]
```

JavaScript generator functions, C# iterator methods with `yield return`, and the C++ coroutine type `std::generator` do the same.[^6][^11][^13] In C#, the compiler turns the method body into an enumerator class, and the body does not start until the caller asks for the first element.[^11] The pattern did not go away. The compiler writes the ConcreteIterator.

### External and internal iteration

Design Patterns asks who controls the iteration.[^1] With an **external** iterator, the client asks for each element: `next()` in Python, JavaScript and Rust, `hasNext()` and `next()` in Java, `MoveNext()` in C#, `++it` in C++. With an **internal** iterator, the client passes an operation, and the iterator applies it to each element: `Iterable.forEach` in Java, `for_each` in Rust.[^7][^10]

Go makes the internal form the main protocol. A function of type `func(yield func(T) bool)` is a range expression, and the body of the `for` loop becomes the `yield` function.[^8] The iterator then keeps its state on its own call stack, so a recursive walk of a tree needs no explicit stack, as the Go example shows. A `break` in the loop makes `yield` return `false`, and the iterator must stop.

An internal iterator is shorter to write. An external iterator is more flexible. Design Patterns gives the comparison of two collections for equality as a task that external iterators make straightforward and internal iterators make very hard.[^1] The examples compare two trees with the same keys and different shapes. Each one needs two traversals that move in step. Go solves this with `iter.Pull`, which turns a push iterator into a `next` function and a `stop` function.[^9] CLU had iterators that worked like Go's push form, and the history of CLU records that they could not step through two collections at once.[^16]

## Example

Each example file builds a binary search tree from the keys 5, 3, 8, 1, 4, 9 and iterates it in order. It sums the keys, takes the first two, and compares the tree with a tree that has the same keys and another shape, and with a tree that lacks the 9. Each language uses its own protocol, so the tree works with the loop statement and the library functions of the language.

| Language | Protocol | How the example writes the iterator |
|---|---|---|
| Python | `__iter__`, `__next__`, `StopIteration`[^2] | a generator method with an explicit stack |
| JavaScript | `[Symbol.iterator]()`, `next()` returns `{ value, done }`[^5] | a generator method; the iterator helpers `take` and `reduce` |
| TypeScript | as JavaScript, typed as `Iterable<T>` and `Iterator<T>` | a hand-written iterator class |
| Java | `Iterable<T>`, `Iterator<T>`[^7] | an anonymous class with a stack |
| C# | `IEnumerable<T>`, `IEnumerator<T>` | an iterator method with `yield return`; LINQ |
| Go | `iter.Seq[T]`, range over a function[^8] | a recursive push iterator; `iter.Pull` for the comparison |
| Rust | the trait `Iterator`, `IntoIterator`[^10] | a struct that borrows the nodes; `Iterator::eq` |
| C++ | a range: `begin()` and `end()`[^12] | a forward iterator class checked by `static_assert` |
| C | none | a struct with a stack and a `next` function |

## Consequences

Design Patterns lists three consequences.[^1]

- **It supports variations in the traversal.** A tree can give an in-order iterator, a level-order iterator and a reverse iterator. Each one is one more class or one more generator. The tree's interface grows by one factory method, not by a traversal algorithm.
- **It simplifies the interface of the aggregate.** The aggregate does not need `first`, `next` and `done` of its own.
- **More than one traversal can be in progress.** Each iterator keeps its own state.

A common protocol has one more effect. When every collection gives the same iterator interface, the library can supply the algorithms once: `sum`, `zip`, `take`, `filter`, `SequenceEqual`, `std::ranges::equal`. A lazy chain of adapters computes each element when the consumer asks for it, so a search that stops at the first match does not visit the rest. The JavaScript, C#, Rust and C++ examples use these adapters.

The costs are these.

**An iterator can outlive the state it points to.** The iterator holds references into the aggregate. If the aggregate changes during a traversal, the iterator may skip elements, give an element twice, or read freed memory. Design Patterns asks for iterators that insertions and removals do not disturb, and it notes that such an iterator must do this without a copy of the aggregate.[^1] The tree iterator below does not have this property. A key that a traversal inserts behind the current position is missed, and one inserted ahead of it is seen:

```python wrong id=insert-during-loop
class Node:
    def __init__(self, key):
        self.key, self.left, self.right = key, None, None


class Tree:
    def __init__(self, *keys):
        self.root = None
        for key in keys:
            self.insert(key)

    def insert(self, key):
        link = self
        side = "root"
        while getattr(link, side) is not None:
            link = getattr(link, side)
            side = "left" if key < link.key else "right"
        setattr(link, side, Node(key))

    def __iter__(self):
        stack, node = [], self.root
        while stack or node:
            while node:
                stack.append(node)
                node = node.left
            node = stack.pop()
            yield node.key
            node = node.right


tree = Tree(5, 3, 8, 1, 4, 9)
seen = []
for key in tree:
    seen.append(key)
    if key == 4:
        tree.insert(2)  # behind the position: missed
        tree.insert(6)  # ahead of the position: seen
print("seen:", seen)
print("now: ", list(tree))
```

```output
seen: [1, 3, 4, 5, 6, 8, 9]
now:  [1, 2, 3, 4, 5, 6, 8, 9]
```

There are three usual answers. A **fail-fast** iterator detects the change and stops with an error: the iterators of `java.util.ArrayList` compare a modification count on each `next()` and throw `ConcurrentModificationException`.[^14] A **snapshot** iterator walks a copy, at the cost of the copy. And Rust makes the change impossible: the iterator borrows the tree, so the compiler rejects a call of `insert` while the iterator is in use. The per-language behaviour, with executed examples, is on [[iterators]].

**An iterator is one pass.** Code that keeps an iterator and loops over it twice sees nothing the second time. The aggregate is iterable many times, because it makes a fresh iterator for each loop. The iterator itself is not. A class that returns `self` from its factory method mixes the two roles, and a nested loop over it breaks:

```python wrong id=self-iterator
class Countdown:
    def __init__(self, start):
        self.current = start

    def __iter__(self):
        return self  # the aggregate is its own iterator

    def __next__(self):
        if self.current == 0:
            raise StopIteration
        self.current -= 1
        return self.current + 1


countdown = Countdown(3)
print([(x, y) for x in countdown for y in countdown])
```

```output
[(3, 2), (3, 1)]
```

The inner loop used up the shared position, so the outer loop ended after one element. The fix is to make `__iter__` return a fresh iterator (or to make it a generator), as the tree does.

**The traversal state is hidden.** A debugger shows a suspended generator or an iterator object, not the position in the tree. And an iterator that holds a resource, such as an open file or a database cursor, needs a close step when the loop stops early. JavaScript, for example, calls the optional `return` method of the iterator to tell it that the caller will not ask for more elements.[^5]

## When not to use it

Do not write an iterator class in a language that has generators, unless the iterator needs extra operations (`peek`, `remove`, a position that the caller can save). A generator gives the same object with less code and fewer places for the end condition to be wrong.

Do not add an iterator when the collection is already a standard collection. Expose an array, a slice, a list or a read-only view, and the language's own iterator comes with it. A custom iterator over a list that the class could return as a read-only view adds a class and protects nothing more.

Do not use an iterator when the client needs random access. An iterator gives the next element. A client that jumps to index 1000 should get an indexable sequence, a C++ random access iterator, or a direct lookup.

Do not hide expensive work behind `next()` with no sign. An iterator over a remote API that makes one network call for each element looks like a loop over a list in the client. Batch the calls, or make the cost visible in the name of the operation.

## Variants and related patterns

**Cursor.** Design Patterns describes a variant where the aggregate keeps the traversal algorithm and the iterator stores only the position. It calls this iterator a cursor, because it only points to the current position in the aggregate.[^1]

**Null iterator.** An iterator that is done at once. Design Patterns uses it for the leaves of a tree, so that code that walks a structure needs no special case for an element with no children.[^1]

**Push and pull.** An external iterator is pull: the consumer asks. A generator or a Go iterator is push from the inside: the producer runs until it gives a value. Go names its two forms push iterators and pull iterators, and `iter.Pull` converts one into the other.[^9] An observer that receives every value of a stream is push with no pause at all. See [[observer-pattern]].

**Composite.** Iterators are often used on recursive structures such as composites.[^1] The tree in the examples is a composite of nodes, and its iterator turns the recursive structure into a flat sequence. See [[composite-pattern]].

**Visitor.** A visitor and an internal iterator both apply an operation to each element. A visitor dispatches on the type of each element and can do something different for each type. An iterator gives elements of one type in an order. See [[visitor-pattern]].

**Memento.** Design Patterns notes that an iterator can use a memento to capture the state of an iteration, and that the iterator keeps the memento inside itself.[^1] See [[memento-pattern]].

**Factory Method.** The operation that makes the iterator is a factory method, and each aggregate returns the iterator class that fits its structure.[^1] See [[factory-method-pattern]].

## Known uses

- OpenJDK: the class `Itr` in [ArrayList.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/ArrayList.java#L1036-L1098) is a ConcreteIterator. It keeps a cursor index and the modification count it expects, and `next()` throws `ConcurrentModificationException` when the list changed.[^14]
- Go: `maps.Keys` in [src/maps/iter.go at go1.23.0](https://github.com/golang/go/blob/go1.23.0/src/maps/iter.go#L22-L33) is a push iterator. It ranges over the map and returns as soon as `yield` returns `false`.[^15]
- Every standard collection of Python, JavaScript, Java, C#, Rust and C++ gives its elements through the protocol of its language, so the loop statement of the language works on it.[^2][^5][^7][^12]

## References

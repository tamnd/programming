---
tag: 2CAHD7
kind: concept
slug: stack-data-structure
name: Stack
aliases: [stack, LIFO, last in first out, push and pop, std::stack, Stack<T>, ArrayDeque as a stack]
area: data-and-values/stacks-queues-and-trees
level: intro
summary: >-
  A stack is a collection that gives back its items in reverse order: the last item pushed is the first item popped. Most languages use a growable array or a library type as a stack.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1955
  people: [Klaus Samelson, Friedrich L. Bauer]
  note: Samelson and Bauer proposed the stack principle in 1955 under the name Operationskeller and filed a patent in 1957. Turing had used the terms bury and unbury for subroutine calls in 1946.
see_also: [call-stack, recursion, dynamic-array, linked-list, list, abstract-syntax-tree, recursive-descent-parsing, iterators]
same_as:
  wikidata: Q177929
  wikipedia: Stack_(abstract_data_type)
sources:
  - url: https://en.wikipedia.org/wiki/Stack_(abstract_data_type)
    title: Stack (abstract data type), Wikipedia
    locator: the introduction; History; Non-essential operations; Implementation; Applications of stacks
    accessed: 2026-11-16
  - url: https://docs.python.org/3/tutorial/datastructures.html
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1.1 Using Lists as Stacks
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/Stack.html
    title: Stack (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/Deque.html
    title: Deque (Java SE 27 and JDK 27)
    locator: the interface description; removeFirst
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/stack
    title: std::stack, cppreference.com
    locator: the introduction; Template parameters
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.stack-1?view=net-11.0
    title: Stack<T> Class, .NET API reference
    locator: the summary; Examples; Remarks; Methods
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/index.html
    title: Module std::collections, The Rust Standard Library
    locator: When Should You Use Which Collection?
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/ArrayDeque.html
    title: ArrayDeque (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/stack/pop
    title: std::stack::pop, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/vec/struct.Vec.html
    title: Struct Vec, The Rust Standard Library
    locator: pop
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/pop
    title: Array.prototype.pop(), MDN Web Docs
    locator: Return value; Description
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Index expressions; Appending to and copying slices
    accessed: 2026-11-16
---

## Idea

A stack is a pile of items where only the top item is in reach. Wikipedia defines it as "an abstract data type that serves as a collection of elements with two main operations": push, which adds an item, and pop, "which removes the most recently added element".[^1] It adds: "The order in which elements are added to or removed from a stack is described as last in, first out, referred to by the acronym LIFO."[^1]

A stack is the right tool when the most recent open item must close first. Brackets in code are an example: each closing bracket must match the last bracket that is still open.

```python run id=idea
PAIRS = {")": "(", "]": "[", "}": "{"}


def balanced(text: str) -> bool:
    stack = []
    for ch in text:
        if ch in "([{":
            stack.append(ch)
        elif ch in PAIRS:
            if not stack or stack.pop() != PAIRS[ch]:
                return False
    return not stack


for text in ["f(a[0], {b: 1})", "(]", "((x)", "x)"]:
    print(text, "yes" if balanced(text) else "no")
```

```output for=idea
f(a[0], {b: 1}) yes
(] no
((x) no
x) no
```

## How it works

**Operations.** A stack has three usual operations:

- push adds an item on top;
- pop removes the top item and returns it;
- peek (or top) returns the top item and does not remove it.

Wikipedia says that a peek operation "can, without modifying the stack, return the value of the last element added".[^1] Most libraries also give the size and a test for an empty stack.

**Underflow and overflow.** A peek or a pop on an empty stack is an error. Wikipedia: "If the stack is empty, an underflow condition will occur".[^1] Languages report this in different ways: an exception, a special value such as `undefined` or `None`, or undefined behavior. A stack with a fixed capacity can also be full. Wikipedia calls that state "stack overflow".[^1]

**Two implementations.** A stack is usually an array with a count of items, or a linked list:

- In an array, the top is the last used slot. Wikipedia notes that adding items to or removing items from the end of a dynamic array "requires amortized O(1) time".[^1] This is the usual choice.
- In a singly linked list, the top is the head of the list. Wikipedia: "A stack may be implemented as, for example, a singly linked list with a pointer to the top element."[^1] Each push allocates a node.

**Where programs use stacks.** A stack holds the work that is still open: the open brackets in a parser, the steps to undo in an editor, and the places to go back to in a search. Wikipedia names backtracking as an "important application of stacks", and it calls depth-first search "The prototypical example of a backtracking algorithm".[^1] The call stack of a running program is also a stack. Each call pushes a frame and each return pops it. See [the call stack](/call-stack/) and [recursion](/recursion/). A loop with an explicit stack can do the work of a recursive function without deep calls.

## In languages

Few languages have a type that is only a stack. Most programs use a growable array, because pushes and pops at its end are fast.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | a `list`, with `append` and `pop()`[^2] |
| JavaScript | idiom | an array, with `push` and `pop` |
| TypeScript | idiom | an array, as in JavaScript |
| Java | stdlib | `ArrayDeque` through the `Deque` interface; the old `Stack` class[^3][^4] |
| C | idiom | an array and a count of items |
| C++ | stdlib | `std::stack`, an adaptor over `std::deque` by default[^5] |
| C# | stdlib | `Stack<T>`[^6] |
| Go | idiom | a slice, with `append` and a slice expression |
| Rust | stdlib | `Vec`, with `push`, `pop` and `last`[^7] |
| Kotlin | stdlib | `ArrayDeque` or a `MutableList` |
| PowerShell | stdlib | `[System.Collections.Generic.Stack[T]]` from .NET |
| Bash | idiom | an indexed array |
| SQL | idiom | a table with an order column |
| Regex | n/a | a pattern has no data structures |
| HTML/CSS | n/a | markup and style rules have no data structures |

**Python.** The tutorial says that "The list methods make it very easy to use a list as a stack, where the last element added is the first element retrieved".[^2] It continues: "To add an item to the top of the stack, use append() . To retrieve an item from the top of the stack, use pop() without an explicit index."[^2]

**Java.** The `Stack` class "represents a last-in-first-out (LIFO) stack of objects" and extends `Vector`.[^3] Its documentation says: "A more complete and consistent set of LIFO stack operations is provided by the Deque interface and its implementations, which should be used in preference to this class."[^3] When a deque is used as a stack, "elements are pushed and popped from the beginning of the deque".[^4] `ArrayDeque` "is likely to be faster than Stack when used as a stack".[^8]

**C++.** "The std::stack class is a container adaptor that gives the programmer the functionality of a stack - specifically, a LIFO (last-in, first-out) data structure."[^5] It "pushes and pops the element from the back of the underlying container, known as the top of the stack".[^5] Its `pop` has the return type `void`: it removes the top element and returns nothing.[^9] A program reads `top()` first and then calls `pop()`.

**C#.** `Stack<T>` "Represents a variable size last-in-first-out (LIFO) collection of instances of the same specified type."[^6] The remarks say: "Stack<T> is implemented as an array."[^6] `TryPop` and `TryPeek` return a value that "indicates whether there is an object at the top of the Stack<T>".[^6]

**Rust.** The collections guide lists the cases to use a `Vec`, and one of them is "You want a stack."[^7] `Vec::pop` "Removes the last element from a vector and returns it, or None if it is empty."[^10]

**JavaScript.** "If you call pop() on an empty array, it returns undefined ."[^11] The program must check for that value.

**Go.** Go has no stack type. A slice works as one: the built-in `append` pushes, and a slice expression such as `s[:len(s)-1]` pops.[^12] An index outside the slice causes a run-time panic, so `s[len(s)-1]` on an empty slice panics.[^12]

The example files push three items, peek, pop them in reverse order and show what happens on an empty stack.

## Variations and related ideas

- **Bounded stack.** A stack with a fixed capacity, as in embedded code or in a fixed array in C. A push on a full stack fails.
- **Persistent stack.** A singly linked list where push makes a new head and keeps the old list. Several versions of the stack share their tails. See [linked-list](/linked-list/).
- **Deque.** A double-ended queue can work as a stack and as a queue. Java and Rust use deque types as stacks. See [[deque]].
- **Queue.** A queue gives items back in arrival order, first in first out. See [[queue-concept]].
- **Stack machines.** Some virtual machines keep the operands of instructions on a stack. Wikipedia: "Many virtual machines are also stack-oriented, including the p-code machine and the Java Virtual Machine."[^1]

## Pitfalls

### A copy through the constructor reverses a C# stack

A C# `Stack<T>` enumerates from the top. The constructor that takes a collection pushes the items in that order, so the copy has the old top at the bottom. The .NET example shows this: it creates "a copy of the stack with the order of the elements reversed".[^6]

```csharp wrong id=csharp-copy-reverses
var stack = new Stack<string>();
stack.Push("a");
stack.Push("b");
stack.Push("c");

var copy = new Stack<string>(stack);
Console.WriteLine($"{string.Join(",", stack)} | {string.Join(",", copy)}");
Console.WriteLine($"top {stack.Peek()} | top {copy.Peek()}");
```

```output for=csharp-copy-reverses
c,b,a | a,b,c
top c | top a
```

To keep the order, copy the array in reverse: `new Stack<string>(stack.Reverse())`.

### Java's Stack and ArrayDeque print in opposite orders

The old `Stack` class is a `Vector`, so it keeps the bottom item at index 0.[^3] An `ArrayDeque` used as a stack pushes at the front.[^4] Both pop the same item, but iteration and `toString` show the items in opposite orders.

```java run id=java-print-order
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Stack;

public class Main {
    public static void main(String[] args) {
        Stack<Integer> old = new Stack<>();
        Deque<Integer> stack = new ArrayDeque<>();
        for (int i = 1; i <= 3; i++) {
            old.push(i);
            stack.push(i);
        }
        System.out.println("Stack " + old + " peek " + old.peek());
        System.out.println("ArrayDeque " + stack + " peek " + stack.peek());
    }
}
```

```output for=java-print-order
Stack [1, 2, 3] peek 3
ArrayDeque [3, 2, 1] peek 3
```

Code that prints or iterates a stack must not depend on the order when it changes from one type to the other.

### Pop on an empty JavaScript array gives undefined

`pop` on an empty array does not throw. It returns `undefined`, and the error appears later.[^11]

```javascript wrong id=javascript-empty-pop
const stack = [];
const top = stack.pop();
console.log(top, top + 1);
```

```output for=javascript-empty-pop
undefined NaN
```

Check `stack.length` before the pop. In the same case, Python raises `IndexError`, Java's `ArrayDeque.pop` throws `NoSuchElementException`, Rust returns `None` and Go panics.[^2][^4][^10][^12]

## Cost

| Operation | Array stack | Linked-list stack |
|---|---|---|
| push | amortized constant | constant, with one allocation |
| pop | constant | constant |
| peek | constant | constant |
| find an item | linear | linear |
| memory per item | the item, plus spare capacity | the item and one link |

An array stack sometimes grows. The .NET reference says: "If the capacity needs to be increased to accommodate the new element, Push becomes an O(n ) operation".[^6] The average cost of a push stays constant, because the array doubles in size or grows by a similar factor. An array stack also reads its items from one block of memory, so it is fast in practice.

## History

Wikipedia says that "Stacks entered the computer science literature in 1946". In that year, Alan Turing used the terms bury and unbury "as a means of calling and returning from subroutines".[^1] Konrad Zuse's Z4 already had subroutines and a two-level stack in 1945.[^1] Klaus Samelson and Friedrich L. Bauer of the Technical University of Munich proposed a stack called Operationskeller ("operational cellar") in 1955 and filed a patent in 1957.[^1] In March 1988, Bauer received the IEEE Computer Pioneer Award "for the invention of the stack principle".[^1] Charles Leonard Hamblin developed similar ideas in 1954.[^1]

## References

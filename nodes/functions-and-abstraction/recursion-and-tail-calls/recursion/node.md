---
tag: DB5SC0
kind: concept
slug: recursion
name: Recursion
aliases: [recursive function, tail call, tail recursion, tail call elimination]
area: functions-and-abstraction/recursion-and-tail-calls
level: core
summary: >-
  A recursive function calls itself on a smaller part of its input until it reaches a base case. Each active call needs a frame on the stack, unless a tail call is turned into a jump.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John McCarthy, Edsger W. Dijkstra]
  note: McCarthy used conditional expressions to define recursive functions in LISP. In the same year, Dijkstra described the stack that lets a subroutine call itself in an ALGOL 60 implementation.
see_also: [closures, iterators, exception-handling, garbage-collection]
same_as:
  wikidata: Q264164
  wikipedia: Recursion_(computer_science)
sources:
  - paper: doi:10.1145/367177.367199
    title: John McCarthy, Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, Communications of the ACM 3(4), 1960, pages 184 to 195
    locator: section 2d, Recursive Function Definitions
  - paper: doi:10.1145/367177.367199
    title: John McCarthy, Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, Communications of the ACM 3(4), 1960, pages 184 to 195
    locator: section 4e, Representation of S-Functions by Programs (the public push-down list, SAVE and UNSAVE)
  - paper: doi:10.1007/BF01386232
    title: E. W. Dijkstra, Recursive Programming, Numerische Mathematik 2, 1960, pages 312 to 318 (open copy as CWI report MR 33, https://ir.cwi.nl/pub/9253)
    locator: the sections The Aim, The Stack and The Link
  - url: https://dspace.mit.edu/handle/1721.1/5753
    title: Guy L. Steele Jr., Debunking the "Expensive Procedure Call" Myth, MIT AI Memo 443, October 1977
    locator: section A, Procedure Calls as GOTO Statements
  - url: https://standards.scheme.org/official/r7rs.pdf
    title: Revised7 Report on the Algorithmic Language Scheme
    locator: 3.5 Proper tail recursion, and reference [6] (Clinger 1998)
  - url: https://tc39.es/ecma262/2026/#sec-isintailposition
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 15.10.1 Static Semantics, IsInTailPosition
  - url: https://tc39.es/ecma262/2026/#sec-preparefortailcall
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 15.10.3 PrepareForTailCall
  - url: https://v8.dev/blog/modern-javascript#proper-tail-calls
    title: V8 blog, ES2015, ES2016, and beyond, 29 April 2016
    locator: Proper tail calls
  - url: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-foptimize-sibling-calls
    title: Using the GNU Compiler Collection, Options That Control Optimization
    locator: -foptimize-sibling-calls
  - url: https://clang.llvm.org/docs/AttributeReference.html#musttail
    title: Clang documentation, Attributes in Clang
    locator: musttail
  - url: https://docs.python.org/3.14/library/sys.html#sys.setrecursionlimit
    title: The Python Standard Library, sys, Python 3.14
    locator: sys.setrecursionlimit
  - url: https://docs.python.org/3.14/library/functools.html#functools.cache
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.cache
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-2.html#jvms-2.5.2
    title: The Java Virtual Machine Specification, Java SE 25 Edition
    locator: 2.5.2 Java Virtual Machine Stacks
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-2.html#jvms-2.6
    title: The Java Virtual Machine Specification, Java SE 25 Edition
    locator: 2.6 Frames
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/java.html
    title: The java Command, Java SE 25
    locator: Extra Options for Java, -Xss size
  - url: https://kotlinlang.org/docs/functions.html#tail-recursive-functions
    title: Kotlin documentation, Functions
    locator: Tail recursive functions
  - url: https://doc.rust-lang.org/std/thread/index.html#stack-size
    title: The Rust Standard Library, Module std::thread
    locator: Stack size
  - url: https://doc.rust-lang.org/book/ch15-01-box.html#enabling-recursive-types-with-boxes
    title: The Rust Programming Language, Using Box<T> to Point to Data on the Heap
    locator: Enabling Recursive Types with Boxes
  - url: https://pkg.go.dev/runtime/debug#SetMaxStack
    title: Go standard library, package runtime/debug
    locator: func SetMaxStack
  - url: https://go.dev/doc/go1.4#runtime
    title: Go 1.4 Release Notes
    locator: Changes to the runtime
  - url: https://go.dev/doc/devel/release
    title: Go release history
    locator: go1.4 (released 2014-12-10)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.stackoverflowexception#remarks
    title: .NET API reference, StackOverflowException Class
    locator: Remarks
  - url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Functions
    title: Bash Reference Manual
    locator: 3.3 Shell Functions
  - url: https://www.gnu.org/software/bash/manual/bash.html#index-FUNCNEST
    title: Bash Reference Manual
    locator: 5.2 Bash Variables, FUNCNEST
  - url: https://sqlite.org/lang_with.html#recursivecte
    title: SQLite documentation, The WITH Clause
    locator: 3. Recursive Common Table Expressions
  - url: https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html#index-fconstexpr-depth
    title: Using the GNU Compiler Collection, Options Controlling C++ Dialect
    locator: -fconstexpr-depth
  - url: https://www.pcre.org/current/doc/html/pcre2pattern.html#SEC28
    title: PCRE2 documentation, pcre2pattern
    locator: RECURSIVE PATTERNS
---

## Idea

A recursive definition defines a thing in terms of a smaller instance of the same thing. A recursive function calls itself, directly or through other functions. Each call works on a smaller part of the problem. A base case stops the chain: for that input, the function returns an answer and makes no further call. McCarthy used this form to define functions such as the factorial in LISP. He also noted that a recursive definition does not guarantee that the computation ends.[^1]

Recursion fits data that has a recursive shape. A list is empty, or it is an element followed by a list. A tree is a leaf, or it is a node with child trees. A function over such data has one case for each form of the data. It calls itself on each part that has the same form. In Python, `total` adds up the numbers in a nested list.

```python run id=idea
def total(item):
    if isinstance(item, int):
        return item
    result = 0
    for child in item:
        result += total(child)
    return result


print(total([1, [2, 3], [[4], 5]]))
```

```output for=idea
15
```

The base case is a number. The recursive case is a list: the function calls itself on each element and adds the results. The depth of nesting in the input does not matter to the code. A loop can do the same work only if it keeps a stack of its own (see Variations and related ideas).

## How it works

Each call of a function needs its own copy of the parameters and the local variables. It also needs a record of the place in the caller where the work continues. Early implementations gave each subroutine one fixed working space. Dijkstra pointed out that such a subroutine cannot be called again while an earlier activation of it has not ended, because the second call overwrites the data of the first.[^3] His solution was a stack. Each call pushes an activation record, also called a frame. The frame holds the parameters, the local variables and the link, which is the return address in the caller. The return pops the frame. McCarthy's LISP used the same idea. Before a recursive routine calls itself, it saves the values that it still needs on a "public push-down list", and it restores them after the call.[^2]

The trace shows the frames of `factorial(3)`. Each line is indented by the depth of the call.

```python run id=trace
def factorial(n, depth=0):
    print("  " * depth + f"call factorial({n})")
    if n == 0:
        result = 1
    else:
        result = n * factorial(n - 1, depth + 1)
    print("  " * depth + f"return {result}")
    return result


factorial(3)
```

```output for=trace
call factorial(3)
  call factorial(2)
    call factorial(1)
      call factorial(0)
      return 1
    return 1
  return 2
return 6
```

At the deepest point, four frames are on the stack. Each frame waits for the call above it to return, because it still has a multiplication to do.

```text
top     factorial(0)   n = 0   returns 1 to factorial(1)
        factorial(1)   n = 1   waits, then computes 1 * result
        factorial(2)   n = 2   waits, then computes 2 * result
bottom  factorial(3)   n = 3   waits, then computes 3 * result
```

The stack of a thread has a maximum size. A recursion whose depth grows with the input can run out of stack. The result depends on the language.

| Language | What happens when the stack is full | Can the program recover |
|---|---|---|
| Python | `RecursionError` when the depth passes `sys.getrecursionlimit()`[^11] | yes, it is an exception |
| JavaScript (V8) | `RangeError: Maximum call stack size exceeded` (see the JavaScript example) | yes |
| Java, Kotlin | `StackOverflowError`[^13] | yes, a `catch` clause can catch it |
| C# | the runtime ends the process[^22] | no |
| Go | the program crashes when a goroutine passes the maximum stack size[^19] | no |
| Rust | the process stops with a stack overflow (see the Rust example) | no |
| C, C++ | the operating system usually stops the process with a signal | no |

The Python limit exists to protect the interpreter. It stops infinite recursion before it overflows the C stack and crashes Python.[^11]

### Tail calls

A call is in tail position when it is the last thing that the caller does. The caller returns the result of the callee and does no more work. In `n * factorial(n - 1)`, the multiplication runs after the call, so the call is not a tail call. In `return count(n - 1, steps + 1)`, the caller returns the result directly, so the call is a tail call.

Steele showed that a call in tail position can be compiled as a jump.[^4] The frame of the caller is not needed any more, so the callee can reuse it and return straight to the caller of the caller. He wrote: "Such a call is called tail-recursive because the call appears to be recursive, but is not, since it appears at the tail end of the caller."[^4]

```text
each call pushes a frame               tail call compiled as a jump
count(3, 0)                            count(3, 0)
  count(2, 1)                          -> count(2, 1)   same frame
    count(1, 2)                        -> count(1, 2)   same frame
      count(0, 3) returns 3            -> count(0, 3)   returns 3 to the first caller
stack depth grows with n               stack depth stays at one frame
```

With this change, a tail-recursive function runs in constant stack space, like a loop. Languages give three different answers to the question "does a tail call take a frame?"

- **A guarantee.** Scheme requires implementations to be properly tail-recursive. An implementation must support an unbounded number of active tail calls.[^5] ECMAScript defines tail position calls for strict mode code. Before such a call, the abstract operation PrepareForTailCall discards the resources of the current execution context.[^7] Calls in non-strict code, and calls in generator and async function bodies, are never in tail position.[^6]
- **A guarantee for marked calls.** Kotlin turns a function with the `tailrec` modifier into a loop.[^16] Clang's `musttail` attribute on a `return` statement requires the compiler to emit a tail call, even with optimization off.[^10]
- **An optimization.** GCC turns sibling and tail recursive calls into jumps with `-foptimize-sibling-calls`, which `-O2`, `-O3` and `-Os` enable.[^9] The C example counts to 100,000,000 this way. The same code at `-O0` can overflow the stack, so a program that depends on it is fragile.

Other implementations make a frame for each call. The JVM creates a frame each time a method is invoked.[^14] CPython makes a frame for each Python call, and the depth limit counts calls in tail position too (see the Python example). V8 implemented the proper tail calls of ECMAScript but did not ship them, so Node.js gives a stack overflow for a deep tail call.[^8]

Go takes another approach to depth. A goroutine starts with a small stack. When the stack is full, the runtime allocates a larger stack and copies the frames into it.[^20] Deep recursion in Go therefore works up to the maximum stack size, which is 1 GB on 64-bit systems by default.[^19]

## In languages

Every general-purpose language in tier 1 allows a function to call itself. The differences are in the depth limit, in what happens at the limit, and in tail calls. The example files show a recursive function over a tree in each language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | a function that calls itself; a depth limit of `sys.getrecursionlimit()` frames |
| JavaScript | native | a function that calls itself; `RangeError` at the limit |
| TypeScript | native | as JavaScript; type aliases can be recursive |
| Java | native | a method that calls itself; `StackOverflowError` at the limit |
| C | native | a function that calls itself; no check of the depth |
| C++ | native | a function that calls itself; `constexpr` recursion at compile time |
| C# | native | a method or local function; a stack overflow ends the process |
| Go | native | a function that calls itself; goroutine stacks grow |
| Rust | native | an `fn` that calls itself; `Box` for recursive types |
| Kotlin | native | a function that calls itself; `tailrec` for loops |
| PowerShell | native | a function that calls itself |
| Bash | native | a function with `local` variables; `FUNCNEST` limits the depth |
| SQL | native | `WITH RECURSIVE` common table expressions |
| Regex | absent | the common dialects have no recursive patterns |
| HTML/CSS | n/a | markup and style rules have no functions |

Bash functions may be recursive, and Bash places no limit on the depth by default.[^23] The variable `FUNCNEST` sets a maximum nesting level. A call past it aborts the current command.[^24]

SQL has no recursive functions in its core, but a common table expression can refer to itself. In SQLite, the first part of the expression gives the starting rows, and the recursive part computes more rows from them. SQLite does not use a call stack. It keeps the rows in a queue and runs the recursive part on one row at a time, until the queue is empty.[^25]

The regex dialects of ECMAScript, Python, Go and Rust have no way for a pattern to refer to itself, so they cannot match nested parentheses of any depth. PCRE2 adds recursive patterns such as `(?R)` for this case.[^27] A parser is the usual alternative.

## Variations and related ideas

- **Mutual recursion.** Two or more functions call each other, as in a recursive descent parser, where the rule for an expression calls the rule for a term and the term calls the expression again inside parentheses. The stack grows in the same way as for direct recursion.
- **Structural and generative recursion.** Structural recursion follows the shape of the data, as `total` does, and it ends when the data ends. Generative recursion computes other subproblems from the input, as quicksort or Euclid's algorithm do. Its termination needs a separate argument, because a recursive definition alone does not guarantee that the computation ends.[^1]
- **Accumulators.** An extra parameter carries the partial result, so the work happens before the call and the call can be in tail position. `count(n, steps)` in the C and Kotlin examples has this form.
- **An explicit stack.** A loop and a list that holds the work still to do replace the call stack. The list lives on the heap, so the depth of the input is limited only by memory.

```python run id=explicit-stack
def total(item):
    result = 0
    stack = [item]
    while stack:
        item = stack.pop()
        if isinstance(item, int):
            result += item
        else:
            stack.extend(item)
    return result


deep = 1
for _ in range(100_000):
    deep = [deep]
print(total(deep))
print(total([1, [2, 3], [[4], 5]]))
```

```output for=explicit-stack
1
15
```

- **Trampolines.** A function returns a description of the next call instead of making it, and a loop runs the calls one after the other. This gives constant stack depth in a language without tail call elimination (see Pitfalls).
- **Memoization.** A cache keeps the result of each call. It removes repeated work when the same subproblem comes up many times. Python's `functools.cache` wraps a function in a dictionary lookup on its arguments.[^12] Dynamic programming fills the same table in a loop, from the small subproblems to the large ones.
- **Recursive types.** A type that contains itself needs indirection in languages with value types. In Rust, `Box` puts the child on the heap, so the compiler can compute the size of the type.[^18] TypeScript type aliases, Java records and C# records can also refer to themselves.
- **Recursion at compile time.** A C++ `constexpr` function can recurse during constant evaluation. The compiler limits the depth: GCC uses `-fconstexpr-depth`, and the standard asks for at least 512 levels.[^26]
- **Recursive generators.** A generator that walks a tree can delegate to a generator for each child. It gives the elements one at a time instead of building a list (see [[iterators]]).

## Pitfalls

### No base case, or a base case that the input never reaches

Each recursive call must move closer to a base case. Here the function steps by 2 and tests only for zero. An odd argument skips zero and never stops.

```python fails expect=exception:RecursionError id=no-base
def countdown(n):
    if n == 0:
        return "done"
    return countdown(n - 2)


print(countdown(5))
```

The run stops with `RecursionError: maximum recursion depth exceeded`. In C, the same mistake overflows the stack and the process dies with a signal. The fix is a base case that covers every input that ends the recursion, here `n <= 0`.

```python run id=base-fix
def countdown(n):
    if n <= 0:
        return "done"
    return countdown(n - 2)


print(countdown(5))
```

```output for=base-fix
done
```

### Deep input overflows the stack

A recursive function can be correct and still fail on large input, because the depth of the recursion grows with the size of the data. A linked list of a million cells needs a million frames.

```java fails expect=exception:StackOverflowError id=deep-list
public class Main {
    record Cell(int value, Cell next) {}

    static long sum(Cell list) {
        return list == null ? 0 : list.value() + sum(list.next());
    }

    public static void main(String[] args) {
        Cell list = null;
        for (int i = 1; i <= 1_000_000; i++) {
            list = new Cell(i, list);
        }
        System.out.println(sum(list));
    }
}
```

The JVM throws `StackOverflowError`. A larger stack with `-Xss` moves the limit but does not remove it.[^15] The fix for a list is a loop. For a tree whose depth the program does not control, the fix is an explicit stack, as in Variations and related ideas.

```java run id=deep-list-fix
public class Main {
    record Cell(int value, Cell next) {}

    static long sum(Cell list) {
        long result = 0;
        for (Cell cell = list; cell != null; cell = cell.next()) {
            result += cell.value();
        }
        return result;
    }

    public static void main(String[] args) {
        Cell list = null;
        for (int i = 1; i <= 1_000_000; i++) {
            list = new Cell(i, list);
        }
        System.out.println(sum(list));
    }
}
```

```output for=deep-list-fix
500000500000
```

### A tail call still takes a frame

A function in accumulator form looks safe, because the recursive call is in tail position. The ECMAScript specification requires such calls in strict mode code to discard the frame of the caller.[^7] V8 does not ship this, so Node.js still overflows.[^8]

```javascript fails expect=exception:RangeError id=no-tail-calls
"use strict";

function count(n, steps) {
  if (n === 0) return steps;
  return count(n - 1, steps + 1);
}

console.log(count(1_000_000, 0));
```

A trampoline gives constant depth with no help from the engine. The function returns a closure for the next step, and a loop calls the closures until a value comes back.

```javascript run id=trampoline
function count(n, steps) {
  if (n === 0) return steps;
  return () => count(n - 1, steps + 1);
}

function trampoline(result) {
  while (typeof result === "function") result = result();
  return result;
}

console.log(trampoline(count(1_000_000, 0)));
```

```output for=trampoline
1000000
```

CPython and the JVM behave like Node.js here: each call takes a frame.[^14] In Kotlin, mark the function `tailrec`. In other languages, write the loop.

### A call that looks like a tail call is not one

A call is a tail call only if nothing runs after it. These calls are not in tail position:

- `return 1 + depth(n - 1)`, because the addition runs after the call.
- A call inside a `try` block with a `finally` clause, because the `finally` clause runs after the call. Kotlin does not allow `tailrec` on a call inside `try`, `catch` or `finally`.[^16]
- A call in a C++ function that has a local object with a destructor, because the destructor runs after the call. Clang's `musttail` requires that every variable in scope be trivially destructible.[^10]

With GCC, the change is only an optimization. A call that is not in tail position stays an ordinary call, and the program still compiles.[^9]

### Tree recursion repeats work

A function that makes two recursive calls can do an exponential amount of work when the calls share subproblems. The naive Fibonacci function computes `fib(23)` twice, `fib(22)` three times, and so on.

```python wrong id=fib
calls = 0


def fib(n):
    global calls
    calls += 1
    return n if n < 2 else fib(n - 1) + fib(n - 2)


print(fib(25), calls)
```

```output for=fib
75025 242785
```

The answer is right, but the function makes almost 250,000 calls. With `functools.cache`, each value is computed once.[^12]

```python run id=fib-cache
from functools import cache

calls = 0


@cache
def fib(n):
    global calls
    calls += 1
    return n if n < 2 else fib(n - 1) + fib(n - 2)


print(fib(25), calls)
```

```output for=fib-cache
75025 26
```

## Cost

- **Time per call.** A call pushes a frame, jumps to the function and later returns. A loop iteration does none of this. In an interpreter such as CPython or PowerShell, a call does more work than in compiled code.
- **Stack space.** The space is the depth of the recursion times the size of a frame. The default thread stack of the `java` launcher is 1024 KB on Linux x64 and 2048 KB on macOS AArch64.[^15] A Rust thread made with `std::thread` has 2 MiB on tier 1 platforms, and Rust does not set the size of the main thread's stack.[^17] CPython stops at `sys.getrecursionlimit()` frames, which is 1000 in the Python example.
- **Growable stacks.** Go starts each goroutine with a small stack and copies the frames to a larger stack when it is full.[^20] A deep recursion therefore pays for the copies, but a shallow goroutine uses little memory.
- **Tail calls.** When the implementation turns tail calls into jumps, a tail-recursive function uses constant stack space. The cost is in debugging: the frames that the jumps remove are missing from the stack trace. This was one of V8's reasons not to ship proper tail calls.[^8]
- **Repeated subproblems.** Tree recursion with shared subproblems takes exponential time. A cache makes it linear in the number of distinct arguments, at the cost of memory for the cache.[^12]

## History

- **1960.** John McCarthy described LISP. Conditional expressions let him define functions recursively, and the implementation saved values on a public push-down list around each recursive call.[^1][^2]
- **1960.** Edsger W. Dijkstra published "Recursive Programming". It described a stack of activation records with a link to the caller, so that an ALGOL 60 procedure could call itself.[^3]
- **1977.** Guy L. Steele Jr. argued that a procedure call in tail position is a jump that passes arguments, so it needs no extra stack.[^4]
- **1998.** William Clinger gave a formal definition of proper tail recursion. The Scheme report cites this paper for its requirement that implementations be properly tail-recursive.[^5]
- **2014.** Go 1.4, released on 10 December 2014, replaced segmented goroutine stacks with contiguous stacks that the runtime copies when they grow.[^20][^21]
- **2015 and 2016.** ES2015 specified that calls in tail position in strict mode code never cause a stack overflow. In 2016, the V8 team reported that it had implemented proper tail calls but had not shipped them.[^8]

## References

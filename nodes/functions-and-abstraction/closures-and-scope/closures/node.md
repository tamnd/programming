---
tag: GSWN5W
kind: concept
slug: closures
name: Closure
aliases: [lexical closure, function closure]
area: functions-and-abstraction/closures-and-scope
level: intro
summary: >-
  A closure is a function together with the variables of the scope where it was made. The function can still read and change these variables after that scope has ended.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1964
  people: [Peter Landin]
  note: Landin used the word closure for a lambda expression together with the environment that gives values to its free variables, in his description of the SECD machine.
see_also: [garbage-collection]
same_as:
  wikidata: Q535335
  wikipedia: Closure_(computer_programming)
sources:
  - paper: doi:10.1093/comjnl/6.4.308
    title: P. J. Landin, The mechanical evaluation of expressions, The Computer Journal 6(4), 1964
    locator: the definition of a closure as a lambda expression bundled with its environment, in the description of the SECD machine
  - url: https://dspace.mit.edu/handle/1721.1/5854
    title: Joel Moses, The Function of FUNCTION in LISP, or Why the FUNARG Problem Should be Called the Environment Problem, MIT AI Memo 199, 1970
    locator: the FUNARG problem and the binding environment of a functional argument
  - url: https://dspace.mit.edu/handle/1721.1/5794
    title: Gerald J. Sussman and Guy L. Steele Jr., SCHEME, An Interpreter for Extended Lambda Calculus, MIT AI Memo 349, 1975
    locator: the introduction (lexical scoping and closures in a dialect of Lisp)
  - url: https://docs.python.org/3.14/reference/executionmodel.html#resolution-of-names
    title: The Python Language Reference, Execution model, Python 3.14
    locator: 4.2.2 Resolution of names
  - url: https://docs.python.org/3.14/reference/simple_stmts.html#the-nonlocal-statement
    title: The Python Language Reference, Simple statements, Python 3.14
    locator: The nonlocal statement
  - url: https://docs.python.org/3.14/reference/datamodel.html#codeobject.co_cellvars
    title: The Python Language Reference, Data model, Python 3.14
    locator: Code objects, co_cellvars and co_freevars; Functions, __closure__
  - url: https://docs.python.org/3.14/faq/programming.html#why-do-lambdas-defined-in-a-loop-with-different-values-all-return-the-same-result
    title: Python Programming FAQ, Python 3.14
    locator: Why do lambdas defined in a loop with different values all return the same result?
  - url: https://docs.python.org/3.14/faq/programming.html#why-am-i-getting-an-unboundlocalerror-when-the-variable-has-a-value
    title: Python Programming FAQ, Python 3.14
    locator: Why am I getting an UnboundLocalError when the variable has a value?
  - url: https://peps.python.org/pep-0227/#specification
    title: "PEP 227: Statically Nested Scopes"
    locator: Specification
  - url: https://peps.python.org/pep-3104/#proposed-solution
    title: "PEP 3104: Access to Names in Outer Scopes"
    locator: Proposed Solution
  - url: https://tc39.es/ecma262/2026/#sec-createperiterationenvironment
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 14.7.4.4 CreatePerIterationEnvironment
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.27.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.27.2 Lambda Body
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.prim.lambda.capture
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.prim.lambda.capture], paragraph 16, note 10"
  - url: https://doc.rust-lang.org/reference/types/closure.html#capture-modes
    title: The Rust Reference
    locator: Closure types, Capture modes
  - url: https://go.dev/ref/spec#Function_literals
    title: The Go Programming Language Specification
    locator: Function literals
  - url: https://go.dev/doc/go1.22#language
    title: Go 1.22 Release Notes
    locator: Changes to the language
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/lambda-expressions#capture-of-outer-variables-and-variable-scope-in-lambda-expressions
    title: C# language reference, Lambda expressions
    locator: Capture of outer variables and variable scope in lambda expressions
  - url: https://kotlinlang.org/docs/lambdas.html#closures
    title: Kotlin documentation, Higher-order functions and lambdas
    locator: Closures
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.management.automation.scriptblock.getnewclosure
    title: PowerShell SDK, ScriptBlock.GetNewClosure Method
    locator: Remarks and return value
  - url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Functions
    title: Bash Reference Manual
    locator: 3.3 Shell Functions
  - url: https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html
    title: Using the GNU Compiler Collection, Nested Functions
    locator: Nested Functions
  - url: https://clang.llvm.org/docs/BlockLanguageSpec.html
    title: Clang documentation, Language Specification for Blocks
    locator: Block variable declarations and imported variables
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.prim.lambda.closure
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.prim.lambda.closure], paragraph 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/func.wrap.func.con
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[func.wrap.func.con], paragraph 5, recommended practice"
  - url: https://go.dev/doc/faq#stack_or_heap
    title: Go FAQ
    locator: How do I know whether a variable is allocated on the heap or the stack?
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.27.4
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.27.4 Run-Time Evaluation of Lambda Expressions
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Compound statements, Python 3.14
    locator: 8.7 Function definitions
---

## Idea

A function often uses a variable that it does not define. The variable belongs to the scope around the function. When functions are values, the inner function can live longer than that scope: the outer function can return it, store it or pass it to other code. A closure is the function together with the variables that it uses from the outer scope. The closure keeps these variables alive, so the function works the same way after the outer function has returned.[^1]

Closures give a function private state without a class. Callbacks, event handlers, decorators, iterators and partial application all depend on them. In Python, the inner function `increment` below captures the variable `count` of `make_counter`. Each call of `make_counter` makes a fresh `count`, so the two counters do not share state.

```python run id=idea
def make_counter():
    count = 0

    def increment():
        nonlocal count
        count += 1
        return count

    return increment


first = make_counter()
second = make_counter()
print(first(), first(), first())
print(second())
```

```output for=idea
1 2 3
1
```

The statement `nonlocal count` tells Python that `count` is the variable of the enclosing function.[^5] Without it, the assignment `count += 1` makes `count` a local variable of `increment` (see Pitfalls).

## How it works

A free variable of a function is a name that the function uses but does not bind. Under lexical scoping (also called static scoping), the language finds the variable for a free name from the text of the program. It looks in the enclosing blocks, from the inside out, at the place where the function is written.[^4] Under dynamic scoping, the language looks in the calls that are active when the function runs. With lexical scoping, the variable must still exist when the function runs, even if the block that made it has returned. A closure is the mechanism that makes this true.

The environment model explains closures. An environment maps names to variables. Each call of a function makes a fresh environment for its parameters and local variables, with a link to the environment where the function was defined. When the program evaluates a function definition or a lambda expression, it makes a closure: a record of the code and a reference to the current environment. A call of the closure makes a fresh environment whose parent is the stored environment, not the environment of the caller. The diagram shows the state after the example in Idea.

```text
global environment
  make_counter -> function make_counter
  first        -> closure (code: increment, environment: E1)
  second       -> closure (code: increment, environment: E2)

E1, made by the first call of make_counter     E2, made by the second call
  count = 3                                      count = 1
  parent: global environment                     parent: global environment
```

The closure `first` holds a reference to E1, so E1 stays alive after `make_counter` returns. For this reason, a captured variable cannot live in the stack frame of the outer call. The implementation puts it on the heap, or it proves that the closure does not escape the call and keeps the variable on the stack. Moses called the general case, a function that is returned out of the scope of its free variables, the FUNARG problem.[^2]

An implementation does not have to keep the whole environment. CPython keeps only the variables that inner functions use. The compiler marks each such variable of the outer function as a cell variable and stores it in a cell object. The inner function gets a tuple of these cells in its `__closure__` attribute, and both functions read and write the variable through the cell.[^6]

```python run id=cells
def make_counter():
    count = 0

    def increment():
        nonlocal count
        count += 1
        return count

    return increment


counter = make_counter()
counter()
print(make_counter.__code__.co_cellvars)
print(counter.__code__.co_freevars)
cell = counter.__closure__[0]
print(type(cell).__name__, cell.cell_contents)
```

```output for=cells
('count',)
('count',)
cell 1
```

Languages differ in what a closure captures. Some capture the variable itself, so the closure and the outer function see each other's changes. Others copy the value at the time the closure is made.

| Language | What the closure captures | Can the closure assign to the outer variable |
|---|---|---|
| Python | the variable, through a cell | yes, after `nonlocal`[^5] |
| JavaScript, TypeScript | the binding of the variable | yes |
| Go | the variable; the function and the literal share it[^15] | yes |
| C# | the variable[^17] | yes |
| Kotlin | the variable[^18] | yes |
| Java | the value of a local variable that is final or effectively final[^12] | no |
| C++ | per capture: a copy (`[x]`), a reference (`[&x]`) or a fresh member (`[n = expr]`)[^13] | through a reference capture; a copy changes only inside a `mutable` lambda |
| Rust | inferred per variable: a shared borrow, a unique borrow or a move; `move` forces a move[^14] | yes, through a unique borrow or a moved value |
| PowerShell | a copy of the local variables of the caller, in a fresh module[^19] | no, only the copy |

## In languages

The example files show the same counter in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | nested `def` or `lambda`; `nonlocal` to assign |
| JavaScript | native | function expression or arrow function |
| TypeScript | native | as JavaScript, with a function type |
| Java | native | lambda expression or anonymous class; captures effectively final locals only |
| C | absent | a function pointer and a `void *` context argument |
| C++ | native | lambda expression with a capture list |
| C# | native | lambda expression, anonymous method or local function |
| Go | native | function literal |
| Rust | native | closure expression; the traits `Fn`, `FnMut` and `FnOnce` |
| Kotlin | native | lambda expression or anonymous function |
| PowerShell | native | script block with `GetNewClosure()` |
| Bash | absent | no function values; variables have dynamic scope |
| SQL | n/a | a query has no functions as values |
| Regex | n/a | a pattern has no functions |
| HTML/CSS | n/a | markup and style rules have no functions |

ISO C has no nested functions and no closures. The usual form is a callback that takes a function pointer and a `void *` argument, which points to a struct that holds the state. Two compiler extensions exist. GCC allows nested functions, but a call through the address of a nested function after the containing function has returned is not valid.[^21] Clang has blocks (`^{ ... }`), which capture variables and can be copied to the heap.[^22] Neither extension is part of the C standard.

Bash functions are not values, so a function cannot be returned or stored. Variables declared with `local` are visible to the function and to the functions that it calls, which is dynamic scoping.[^20] The usual alternative is a global variable with a name prefix, or a separate process.

## Variations and related ideas

- **Capture of the variable or of the value.** The table in How it works gives the rule of each language. Capture of the value is safe when the closure outlives the variable. Capture of the variable lets the closure and the outer code share state.
- **Escaping and non-escaping closures.** A closure that is called only during the call that made it (for example, a key function given to a sort) does not escape. Its variables can stay on the stack. A closure that is returned or stored escapes. Rust and C++ make the programmer choose: a reference capture or a borrow must not outlive the variable.
- **Closure types in C++ and Rust.** Each lambda expression or closure expression has its own unnamed type, a struct that holds the captures.[^23][^14] Code that must store closures of different types uses type erasure: `std::function` in C++, `Box<dyn Fn()>` in Rust.
- **Closures and objects.** An object with private fields and one method holds state in the same way as a closure. Java code before lambdas used anonymous classes for callbacks, and a Java lambda still becomes an object that implements a functional interface.
- **Function pointer and context.** C libraries pass a function pointer and a context pointer together. This is a closure that the programmer builds by hand, with no help from the compiler for lifetime.
- **Dynamic scoping.** Bash and early Lisp systems resolve a free name in the active calls. A function then sees the variables of its caller, not of its definition, and no closure is needed or possible.[^2]

## Pitfalls

### A loop variable is shared by all closures

A closure made in a loop captures the loop variable, not its value at that time. When all iterations share one variable, every closure sees its last value.[^7]

```python wrong id=late-binding
callbacks = [lambda: i for i in range(3)]
print([f() for f in callbacks])
```

```output for=late-binding
[2, 2, 2]
```

The fix in Python is to copy the value into a parameter with a default value. The default is evaluated when the lambda is made.

```python run id=late-binding-fix
callbacks = [lambda i=i: i for i in range(3)]
print([f() for f in callbacks])
```

```output for=late-binding-fix
[0, 1, 2]
```

In JavaScript, a `var` loop variable has one binding for the whole loop. A `let` loop variable gets a fresh binding for each iteration, so each closure sees its own value.[^11]

```javascript wrong id=var-loop
const callbacks = [];
for (var i = 0; i < 3; i++) {
  callbacks.push(() => i);
}
console.log(callbacks.map((f) => f()).join(", "));
```

```output for=var-loop
3, 3, 3
```

```javascript run id=let-loop
const callbacks = [];
for (let i = 0; i < 3; i++) {
  callbacks.push(() => i);
}
console.log(callbacks.map((f) => f()).join(", "));
```

```output for=let-loop
0, 1, 2
```

C# has the same trap in a `for` loop, while each iteration of `foreach` has its own variable (see the C# example). In Go, each iteration of a `for` loop has its own variable; the Go 1.22 release changed the earlier rule, where all iterations shared one variable.[^16]

### An assignment makes the variable local

In Python, an assignment to a name anywhere in a function makes that name local to the function.[^4] So `count += 1` in the inner function does not change the outer `count`. It reads a local `count` that has no value yet.[^8]

```python fails expect=exception:UnboundLocalError id=no-nonlocal
def make_counter():
    count = 0

    def increment():
        count += 1
        return count

    return increment


make_counter()()
```

The run stops with `UnboundLocalError: cannot access local variable 'count' where it is not associated with a value`. The fix is the statement `nonlocal count` in the inner function, as in Idea. Python 2 had no `nonlocal`; PEP 3104 added it.[^10]

### A reference capture outlives its variable

In C++, a capture by reference does not extend the life of the variable. When the closure runs after the variable has gone, the behaviour is undefined.[^13] Here the lambda captures the local `count` by reference, and the function returns the lambda.

```cpp fails expect=sanitizer:address id=dangling
#include <functional>
#include <print>

std::function<int()> make_counter() {
    int count = 0;
    return [&count] { return ++count; };
}

int main() {
    auto counter = make_counter();
    std::println("{}", counter());
}
```

AddressSanitizer reports `stack-use-after-return`. Without the sanitizer, the program can print a number and exit with status 0, which hides the bug. The fix is a capture by copy in a `mutable` lambda, as in the C++ example: `[count = 0]() mutable { return ++count; }`. Rust rejects the same program at compile time with error E0373, because the borrow of `count` would outlive `count` (see the Rust example).

### Java captures only effectively final variables

A Java lambda may use a local variable of the enclosing method only when the variable is final or effectively final, that is, never assigned after its initialization.[^12]

```java fails expect=compile id=java-capture
public class Main {
    public static void main(String[] args) {
        int count = 0;
        Runnable increment = () -> count++;
        increment.run();
        System.out.println(count);
    }
}
```

`javac` stops with `local variables referenced from a lambda expression must be final or effectively final`. The usual fixes put the state in an object that the lambda can change: a one-element array (as in the Java example), an `AtomicInteger`, or a field of a class.

### A closure keeps its captured objects alive

A captured variable stays reachable as long as the closure is reachable. A handler that is registered and never removed keeps all its captured objects in memory. In this run, the `Report` object lives as long as `handler` does.

```python run id=retain
import weakref


class Report:
    pass


def make_handler():
    report = Report()

    def handle():
        return type(report).__name__

    return handle, weakref.ref(report)


handler, probe = make_handler()
print("alive with handler:", probe() is not None)
del handler
print("alive after del:", probe() is not None)
```

```output for=retain
alive with handler: True
alive after del: False
```

Remove a handler when its work is done, and capture only the values that the closure needs, not a whole large object. [[garbage-collection]] describes when the memory is freed.

## Cost

- **Creation.** In Python, each execution of `def` or `lambda` makes a fresh function object.[^27] A closure made in a hot loop costs one allocation for each pass. Captured variables live in cell objects, and the code reads them through the cell.[^6]
- **Escape to the heap.** In Go, a captured variable that outlives the call moves to the heap. The compiler decides this by escape analysis, and `go build -gcflags=-m` prints the decision.[^25] For the counter, it reports `moved to heap: count` (see the Go example).
- **Size of a closure value.** In C++ and Rust, a closure is a value of its own struct type. It holds the captures and nothing else. The counter closure in the C++ and Rust examples is 4 bytes, the size of one 32-bit integer, and it needs no heap allocation. Each closure has a distinct type, so the compiler knows the called function and can inline it.[^23]
- **Type erasure.** `std::function` and `Box<dyn FnMut()>` hide the type of the closure. A call goes through an indirect jump. `Box` always allocates on the heap. The C++ standard asks implementations of `std::function` to avoid heap allocation for small callables, but it does not require this.[^24]
- **Java.** A lambda that captures values makes an object that holds copies of them. The specification allows an implementation to reuse one object for a lambda that captures nothing.[^26]

## History

- **1964.** Peter Landin used the word "closure" in his description of the SECD machine, an abstract machine that evaluates lambda expressions. A closure there is a lambda expression together with the environment of its free variables.[^1]
- **1970.** Joel Moses described the FUNARG problem in Lisp. Lisp used dynamic binding, so a function that was passed as an argument could see the wrong variables. The `FUNCTION` form of LISP 1.5 made a closure of a function and its binding environment.[^2]
- **1975.** Gerald Sussman and Guy Steele described Scheme, a Lisp with lexical scope where functions are closures by default.[^3]
- **2000 and 2006.** PEP 227 gave Python statically nested scopes, so inner functions can read the variables of outer functions. PEP 3104 added `nonlocal`, so they can also assign to them.[^9][^10]
- **2024.** Go 1.22 gave each iteration of a `for` loop its own variable. This removed the shared-variable trap of the first pitfall below from Go.[^16]

## References

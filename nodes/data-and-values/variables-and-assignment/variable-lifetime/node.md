---
tag: FGQ4Y5
kind: concept
slug: variable-lifetime
name: Variable lifetime
aliases: [variable lifetime, lifetime, extent, storage duration, automatic variable, static variable, static local variable, local lifetime, global lifetime, lifetime versus scope, out of scope]
area: data-and-values/variables-and-assignment
level: core
summary: >-
  The lifetime of a variable is the time that it exists while the program runs. A local variable lives for one call or block, a global one for the whole run, and a closure can make a local live longer.
applies_to: core-languages
viz: optional
formal: none
see_also: [variable-concept, lexical-scope, initialization-concept, uninitialized-variables, binding, pointers-and-references]
same_as:
  wikidata: Q17157128
  wikipedia: Object_lifetime
sources:
- url: https://en.wikipedia.org/wiki/Variable_(high-level_programming_language)
  title: Variable (high-level programming language), Wikipedia
  locator: Scope and extent
- url: https://en.cppreference.com/w/c/language/storage_class_specifiers.html
  title: Storage-class specifiers, cppreference.com (C)
  locator: Storage duration
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25
  locator: 4.12.3 Kinds of Variables
- url: https://go.dev/doc/effective_go
  title: Effective Go
  locator: Composite literals
- url: https://en.wikipedia.org/wiki/Object_lifetime
  title: Object lifetime, Wikipedia
  locator: Introduction; Determinism
- url: https://docs.python.org/3/reference/datamodel.html
  title: Data model, The Python Language Reference
  locator: 3.1 Objects, values and types
- url: https://doc.rust-lang.org/book/ch04-01-what-is-ownership.html
  title: What Is Ownership?, The Rust Programming Language
  locator: Memory and Allocation
---

## Idea

A [[variable-concept]] does not exist for the whole run of a program. It comes into being, holds values, and then ends. "The scope of a variable describes where in a program's text the variable may be used, while the extent (also called lifetime) of a variable describes when in a program's execution the variable has a (meaningful) value."[^1]

In Python, each call of a function makes new local variables, and a global variable lives for the whole run:

```python run id=idea
log = []


def visit(name):
    seen = []
    seen.append(name)
    log.append(name)
    return seen


print(visit("a"), visit("b"))
print(log)
```

```output for=idea
['a'] ['b']
['a', 'b']
```

`seen` is a new list on each call, so it holds one name. `log` lives from the start of the program to the end, so it keeps all the names.

## How it works

**Scope and lifetime.** Scope is about text, and lifetime is about time. "The scope of a variable is actually a property of the name of the variable, and the extent is a property of the storage location of the variable."[^1] The two are often linked: "Entrance into that scope typically begins a variable's lifetime (as it comes into context) and exit from that scope typically ends its lifetime (as it goes out of context)."[^1] See [[lexical-scope]].

**The kinds of lifetime.** C names them storage durations. "Every object has a property called storage duration, which limits the object lifetime."[^2]

- **Automatic.** "The storage is allocated when the block in which the object was declared is entered and deallocated when it is exited by any means (goto, return, reaching the end)."[^2] Local variables and parameters have this lifetime, on the stack. A recursive function gets new variables at each level.
- **Static.** "The storage duration is the entire execution of the program, and the value stored in the object is initialized only once, prior to main function."[^2] Global variables have this lifetime, and so does a local variable marked `static`: its scope is the function, but it keeps its value from one call to the next.
- **Allocated, or dynamic.** "The storage is allocated and deallocated on request, using dynamic memory allocation functions."[^2] The program calls `malloc` and `free`, or `new` and `delete`.
- **Thread.** A thread-local variable lives as long as its thread.

**Variables of objects.** In Java, "A class variable is created when its class or interface is prepared (§12.3.2) and is initialized to a default value (§4.12.5)."[^3] A field of an object lives as long as the object: "The instance variable effectively ceases to exist when the object of which it is a field is no longer referenced, after any necessary finalization of the object (§12.6) has been completed."[^3]

**A lifetime longer than the scope.** "it can be permissible for a variable binding to extend beyond its scope, as occurs in Lisp closures and C static local variables"[^1]. A closure keeps the local variables that it uses alive after the function returns. Go moves a local variable to the heap when the program keeps its address: "unlike in C, it's perfectly OK to return the address of a local variable; the storage associated with the variable survives after the function returns."[^4]

**Variable lifetime and object lifetime.** A variable that holds a reference is not the object. "In some programming contexts, object lifetime coincides with the lifetime of a variable that represents the object. In other contexts – where the object is accessed by reference – object lifetime is not determined by the lifetime of a variable."[^5] In Python and Java, the end of a variable ends only the reference, and a garbage collector frees the object later: "Objects are never explicitly destroyed; however, when they become unreachable they may be garbage-collected."[^6] In C++ and Rust, the end of a variable ends the object that it holds. In Rust, "When a variable goes out of scope, Rust calls a special function for us. This function is called drop"[^7].

## In languages

| Language | Verdict | Lifetime of variables |
|---|---|---|
| Python | native | a local name lives for one call, to the end of the function, not the block; a closure keeps it alive |
| JavaScript | native | `let` and `const` live for a block, `var` for a function; closures keep them alive |
| TypeScript | native | the rules of JavaScript |
| Java | native | locals for a block, fields for their object, static fields while the class is loaded |
| Kotlin | native | the rules of the JVM: locals for a block, properties for their object |
| C# | native | locals for a block, fields for their object; `using` ends a resource at the end of the scope |
| Go | native | each variable lives while the program can reach it; the compiler puts it on the stack or the heap |
| Rust | native | a variable ends at the end of its scope, which runs `drop`; the borrow checker checks the lifetime of references |
| C | native | automatic, static, thread and allocated storage durations |
| C++ | native | the storage durations of C, and a destructor runs at the end of the lifetime of an object |
| Bash | native | variables are global unless `local` in a function, which lives for the call |
| PowerShell | native | a variable lives in its scope: the script, the function or the session |
| SQL | n/a | a query has no variables; a temporary table lives for a session or a transaction |
| Regex | n/a | a pattern has no variables |
| HTML | n/a | HTML is markup, with no variables |
| CSS | n/a | a style sheet has no variables that start and end in time |

## Variations and related ideas

- **Stack and heap.** Automatic variables are on the stack, and the end of a call frees them at once. Values on the heap live until the program, a garbage collector or a destructor frees them.
- **RAII.** In C++ and Rust, the end of a variable runs code that releases a resource such as a file or a lock. Other languages have `with`, `using` and `try` with resources for the same purpose.
- **Lifetimes in Rust types.** Rust checks that a reference does not live longer than the value that it refers to.
- **Escape analysis.** A compiler finds the local variables that outlive their call and puts only those on the heap.

## Pitfalls

- **A variable that lives longer than expected.** A Python variable lives to the end of the function, not the end of a loop, so code after the loop can read the loop variable. With an empty list, the loop never assigns it:

```python wrong id=loop-variable-after-loop
def last_item(items):
    for item in items:
        pass
    return item


print(last_item([1, 2, 3]))
try:
    last_item([])
except UnboundLocalError as e:
    print(type(e).__name__)
```

```output for=loop-variable-after-loop
3
UnboundLocalError
```

Use `items[-1]` with a check for an empty list instead.

- **A pointer to a variable that ended.** In C and C++, the address of a local variable is not valid after the function returns. "A variable whose extent ends before its scope may become a dangling pointer"[^1]. A read through it is undefined behavior.
- **A static local variable is shared.** All calls, and all threads, see the same variable.
- **A closure keeps memory.** A closure that a program keeps also keeps all the variables that it uses, and the objects that they refer to.

## Cost

An automatic variable costs almost nothing to make and to end: the call moves the stack pointer. A static variable costs nothing at run time. A variable on the heap costs an allocation, and later a free or work for the garbage collector. "For space efficiency, a memory space needed for a variable may be allocated only when the variable is first used and freed when it is no longer needed."[^1]

## History

The kinds of lifetime grew with the languages. C has automatic, static and allocated storage, and C11 added thread storage duration[^2]. Languages with a garbage collector, such as Java and Python, end the life of an object some time after no variable refers to it. Rust checks the lifetimes of references in the compiler, with no garbage collector.

## References

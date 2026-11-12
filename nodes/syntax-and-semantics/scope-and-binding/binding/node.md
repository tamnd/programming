---
tag: QXZRCG
kind: concept
slug: binding
name: Binding
aliases: [binding, name binding, variable binding, bind, rebinding, binding time, early binding, late binding, static binding, dynamic binding, shadowing, unbinding, environment]
area: syntax-and-semantics/scope-and-binding
level: core
summary: >-
  A binding is the link between a name and the thing that it names, such as a value, a storage location or a function. Rebinding a name is a different thing from changing the object it names.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1962
  people: [John McCarthy, Paul W. Abrahams, Daniel J. Edwards, Timothy P. Hart]
  note: The LISP 1.5 manual of 1962 described an interpreter that binds variables by pairing them with their arguments on an association list, and finds the value of a variable by searching that list.
see_also: [identifier, lexical-scope, closures, immutability, pointers-and-references, dynamic-dispatch]
same_as:
  wikidata: Q16928028
  wikipedia: Name_binding
sources:
- url: https://en.wikipedia.org/wiki/Name_binding
  title: Name binding, Wikipedia
  locator: Introduction; Rebinding and mutation
- url: https://docs.python.org/3/reference/executionmodel.html
  title: Execution model, The Python Language Reference
  locator: 4.2 Naming and binding
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/declarations
  title: Declaration statements, C# language reference
  locator: Reference variables
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
  title: const, MDN Web Docs
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let
  title: let, MDN Web Docs
  locator: Temporal dead zone (TDZ)
- url: https://docs.python.org/3/faq/programming.html
  title: Programming FAQ, Python documentation
  locator: Why do lambdas defined in a loop with different values all return the same result?
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Short variable declarations; For statements with for clause
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.11.1 Field Access Using a Primary; 15.27.2 Lambda Body
- url: https://en.cppreference.com/w/c/language/storage_class_specifiers.html
  title: Storage-class specifiers, C reference, cppreference.com
  locator: Linkage
- url: https://doc.rust-lang.org/book/ch03-01-variables-and-mutability.html
  title: Variables and Mutability, The Rust Programming Language
  locator: Shadowing
- url: https://www.softwarepreservation.org/projects/LISP/book/LISP%201.5%20Programmers%20Manual.pdf
  title: LISP 1.5 Programmer's Manual, John McCarthy and others, MIT Press, 1962
  locator: Section 2.1, Variables
- url: https://en.cppreference.com/w/cpp/language/reference.html
  title: Reference declaration, C++ reference, cppreference.com
  locator: Rvalue references; Notes
---

## Idea

A name in a program is only text. A binding links the name to something: a value, a storage location, a function or a type. Two different operations can then look the same in the code. To rebind a name is to link it to another thing. To mutate is to change the thing that the name is linked to. When two names are bound to one object, a mutation is visible through both names, and a rebinding is not.

```python run id=idea
# A binding links a name to an object. Assignment makes a binding; it does not copy.
a = [1, 2]
b = a              # b is bound to the same list as a
b.append(3)        # mutation: the object changes, and both names see it
print(a, a is b)

b = [9]            # rebinding: b now names another object, and a is not affected
print(a, b, a is b)

# A function call binds each parameter name to the argument object.
def grow(xs, n):
    xs.append(n)   # changes the caller's object
    n = 0          # rebinds the local name only
    return n

k = 4
print(grow(a, k), a, k)

# A closure keeps the binding, not the value it had when the closure was made.
fs = [lambda: i for i in range(3)]
print([f() for f in fs])
fs = [lambda i=i: i for i in range(3)]   # a default value is bound when the def runs
print([f() for f in fs])
```

```output for=idea
[1, 2, 3] True
[1, 2, 3] [9] False
0 [1, 2, 3, 4] 4
[2, 2, 2]
[0, 1, 2]
```

`a = [1, 2]` binds `a` to a list, and `b = a` binds `b` to the same list. `b.append(3)` mutates the list, so `a` shows the change. `b = [9]` rebinds `b`, and `a` is not affected. A call binds each parameter to its argument, so `grow` can mutate the caller's list but cannot rebind the caller's name `k`. The last two lines show that a closure keeps the binding of `i` and reads it when it runs, after the loop has ended.

## How it works

**Binding and reference.** "Name binding is the association of a data or code entity with an identifier", and "An identifier bound to an entity is said to reference that entity"[^1]. The place in the code that makes a binding is a binding occurrence, and the other uses of the name are references[^1]. Scope decides which binding a reference finds. See [lexical scope](/lexical-scope/).

**Binding constructs.** Each language has a fixed set of constructs that bind names. In Python, "The following constructs bind names: formal parameters to functions, class definitions, function definitions, assignment expressions", and also assignment targets, `for` loop headers, `as` in `with` and `except`, and `import`[^2]. Declarations such as `let`, `var`, `const` and `int x` bind names in other languages, and a pattern can bind many names at once.

**Rebinding, assignment and mutation.** "Rebinding should not be confused with mutation or assignment"[^1]. Languages differ in what a variable is:

- In Python and JavaScript, a variable is a name bound to an object, and assignment rebinds the name. `b = a` makes two names for one object.
- In C, C++, Go and Rust, a variable is a named storage location, and assignment copies a value into it. A pointer or a reference gives a second way to reach the location. C# calls a `ref` local "an alias to its referent"[^3].
- A `const` or `final` binding cannot be rebound, but the object can still change. MDN says that `const` "creates an immutable reference to a value. It does not mean the value it holds is immutable"[^4].

**Where the binding exists.** In Python, "If a name binding operation occurs anywhere within a code block, all uses of the name within the block are treated as references to the current block"[^2]. So a read before the assignment fails with `UnboundLocalError`, even when a global with the same name exists. In JavaScript, a `let` binding exists from the start of its block, but code cannot read it before the declaration runs. This gap is the temporal dead zone[^5].

**Closures.** A function that uses a name from an enclosing scope keeps the binding, not the value. The Python FAQ explains the result of a loop of lambdas: the variable is read when the lambda is called, not when it is defined[^6]. Languages give a new binding to each iteration of a loop to avoid this problem: `let` in a JavaScript `for` loop, `foreach` in C#, and `for` in Go[^7]. Java requires that a local variable that a lambda uses is "final or effectively final"[^8], so the binding can never change.

**Static and dynamic binding.** Binding also means the choice of which entity a use refers to, made at compile time (static or early binding) or at run time (dynamic or late binding). In Java, a call of an overridden method binds at run time to the class of the object, but for a field "only the type of the Primary expression, not the class of the actual object referred to at run time, is used in determining which field to use"[^8]. See [dynamic dispatch](/dynamic-dispatch/).

**Linkage.** In C, the linker binds names across files. `static` at file scope gives "internal linkage", so the name is bound only in its file, and `extern` gives external linkage, so all the files refer to one object[^9].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | assignment, `def`, `import` and other statements bind names; `global`, `nonlocal`, `del` |
| JavaScript | native | `let`, `const`, `var`, function declarations; `bind` for `this` |
| TypeScript | native | as JavaScript; `readonly` for properties |
| Java | native | declarations; `final`; static binding of fields, dynamic binding of methods |
| Kotlin | native | `val` and `var`; destructuring declarations |
| C# | native | declarations; `ref` locals; `readonly`; `virtual` and `new` |
| Go | native | `var`, `const`, `:=`; a new loop variable for each iteration |
| Rust | native | `let`, `let mut`, shadowing, patterns, `@` |
| C | native | declarations; linkage; block scope |
| C++ | native | references, structured bindings, lambda captures |
| Bash | native | `name=value`; `local`; `declare -n` name references |
| PowerShell | native | `$name = value`; scope modifiers such as `$script:` |
| SQL | native | column aliases, `WITH` names; variables in procedural SQL |
| Regex | native | capture groups bind text to a number or a name |
| HTML | n/a | no names bound to values |
| CSS | idiom | custom properties bind a name to a value on an element |

Every general-purpose language binds names, so the verdict is native. The languages differ in whether a variable is a name for an object or a named location, in which constructs can rebind a name, and in when a binding is made: at compile time, at link time or at run time.

The example files show these differences. Python shows `global`, `nonlocal`, the `UnboundLocalError` rule and `del`. JavaScript shows `const`, the temporal dead zone, the loop bindings of `var` and `let`, and the binding of `this`. Java compares the binding of methods, fields and static methods. C# shows a `ref` local and the captures of `for` and `foreach`. Go shows `:=` and shadowing. Rust shows shadowing with a new type, a move and pattern bindings. C shows linkage and block scope. C++ shows a reference that cannot be rebound and a structured binding.

## Variations and related ideas

- **Binding time.** A binding can be fixed when the language is designed (the meaning of `+`), when the program is compiled (the type of a variable in C), when it is linked (an external name), when it is loaded, or when it runs (the method for a virtual call).
- **Lexical and dynamic scope.** With lexical scope, a reference finds the binding in the text that encloses it. With dynamic scope, it finds the most recent binding in the calls that are active. Early LISP, Bash `local` and Emacs Lisp special variables use dynamic scope. See [lexical scope](/lexical-scope/).
- **Shadowing.** An inner declaration with the same name hides the outer binding. Rust uses this often to change the type of a value and keep the name[^10]. Many linters warn about it in other languages.
- **Single assignment.** In Haskell, Erlang and Elixir patterns, and with `const`, `final` and `val`, each binding is made once and never changed. See [immutability](/immutability/).
- **Pattern binding.** Destructuring in JavaScript, tuple unpacking in Python, patterns in Rust and structured bindings in C++ bind several names from one value.

## Pitfalls

- **Aliasing by mistake.** `b = a` with a list or an object in Python, JavaScript or Java does not copy it. Use `list(a)`, `[...a]` or a copy method when a separate object is needed.
- **Late binding in closures.** Closures made in a loop can all see the last value. Use a binding per iteration, or pass the value as a parameter or a default argument.
- **Reading a name before the binding.** `UnboundLocalError` in Python and `ReferenceError` in JavaScript come from a binding later in the same block. Use `global` or `nonlocal`, or rename the local.
- **Shadowing an error variable in Go.** `x, err := f()` in an inner block makes a new `err`, and the outer `err` stays unchanged. The bug passes the compiler.
- **`const` that looks deep.** A `const` array or a `final` list can still change. Freeze the object or use an immutable type when the content must not change.
- **Field hiding.** A field in a subclass with the same name as a field of its superclass does not override it, and the declared type decides which field a read uses.

## Cost

Most bindings cost nothing at run time: the compiler turns a local name into a register or a stack slot. A binding that the program looks up by name, such as a global in Python, costs a hash table lookup. A closure that keeps bindings after its function returns must store them on the heap. Dynamic binding of a method costs an indirect call through a table, which can stop the compiler from inlining the call.

## History

The LISP 1.5 manual of 1962 describes how its interpreter binds variables. "The part of the interpreter that binds variables is called apply. When apply encounters a function beginning with LAMBDA, the list of variables is paired with the list of arguments and added to the front of the a-list." To evaluate a variable, the interpreter searches the a-list, and "If a variable has been bound several times, the last or most recent value is used"[^11]. John McCarthy and his co-authors based this on "the Church lambda notation"[^11]. This search gave dynamic scope, and later Lisp dialects moved to lexical scope.

Modern languages separate the binding of a name from the storage of a value. Python describes all of its names as bindings[^2], C and C++ bind names to objects and use linkage to bind names across files[^9][^12], and Rust makes each `let` a new binding that can shadow the last one[^10]. C++ adds references, which are names bound to an existing object: "References are not objects", and a reference that is bound to a temporary can extend its lifetime[^12]. Go changed its `for` loops so that "Each iteration has its own separate declared variable"[^7].

## References

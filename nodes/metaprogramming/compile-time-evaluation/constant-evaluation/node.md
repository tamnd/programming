---
tag: MQEEPJ
kind: concept
slug: constant-evaluation
name: Constant evaluation
aliases: [compile-time evaluation, compile-time function execution, constant expression, constexpr, const fn]
area: metaprogramming/compile-time-evaluation
level: core
summary: >-
  The compiler computes the value of an expression while it translates the program, so the value can size an array, label a case or fail the build.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2007
  people: [Gabriel Dos Reis, Bjarne Stroustrup, Jens Maurer]
  note: The C++ paper N2235, Generalized Constant Expressions, extended constant expressions from fixed operators to calls of constexpr functions; C++11 adopted the keyword constexpr.
see_also: [syntactic-macros, parametric-polymorphism, undefined-behavior, integer-overflow, immutability, recursion, cpp, rust]
same_as:
  wikidata: Q5156374
  wikipedia: Compile-time_function_execution
sources:
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 7.7 [expr.const] paragraphs 5, 15, 18 and 19; 8.5.2 [stmt.if] paragraphs 4 and 5; 9.1 [dcl.pre] paragraph 10; 9.2.6 [dcl.constexpr] paragraphs 1 to 6; Annex B [implimits] paragraph 2, items 2.38 and 2.39; C.2.2 paragraph 3; C.5.2 paragraph 3
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2235.pdf
    title: Gabriel Dos Reis, Bjarne Stroustrup, Jens Maurer, Generalized Constant Expressions, Revision 5, N2235, 17 April 2007
    locator: Abstract; 1 Introduction
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 6.6 Constant expressions, paragraphs 2, 3, 7 and 8; 6.7.2 Storage-class specifiers, paragraphs 2, 5, 16 and 18 with its example; 6.7.12 Static assertions; 6.9.2 Function definitions, paragraph 4; M.2 Fifth Edition; M.4 Third Edition
  - url: https://doc.rust-lang.org/reference/const_eval.html
    title: The Rust Reference, Constant evaluation
    locator: const-eval.intro; const-eval.const-expr.evaluation; const-eval.const-expr.runtime-context; const-eval.const-expr.error; const-eval.const-expr.loop; const-eval.const-context; const-eval.const-fn.const-context; const-eval.const-fn.outside-context; const-eval.const-fn.async
  - url: https://blog.rust-lang.org/2018/12/06/Rust-1.31-and-rust-2018/
    title: The Rust Core Team, Announcing Rust 1.31 and Rust 2018, The Rust Blog, 6 December 2018
    locator: const fn
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Constants (including the implementation restriction); Constant expressions; Length and capacity
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.29
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.29 Constant Expressions
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.12.4
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.12.4 final Variables (the definition of a constant variable)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-13.html
    title: The Java Language Specification, Java SE 25 Edition
    locator: 13.1 The Form of a Binary; 13.4.9 final Fields and static Constant Variables
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/const
    title: C# language reference, The const keyword
    locator: the introduction; the paragraph on interpolated strings; the paragraph on readonly
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked
    title: C# language reference, The checked and unchecked statements
    locator: the first paragraph; the paragraph on constant expressions
  - url: https://kotlinlang.org/docs/properties.html
    title: Kotlin documentation, Properties
    locator: Compile-time constants
  - url: https://www.typescriptlang.org/docs/handbook/enums.html
    title: TypeScript Handbook, Enums
    locator: const enums
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
    title: MDN Web Docs, const
    locator: Description
  - url: https://peps.python.org/pep-0008/
    title: PEP 8, Style Guide for Python Code
    locator: Constants
  - url: https://port70.net/~nsz/c/c89/c89-draft.html
    title: Draft of ANSI X3.159-1989, Programming Language C
    locator: 3.4 Constant expressions
---

## Idea

A compiler reads a program and translates it. During the translation, it can also compute some values. `60 * 60 * 24` has the same value each time the program runs, so the compiler can compute 86400 once and put the number in the output. This is constant evaluation. The C standard says it in one sentence: "A constant expression can be evaluated during translation rather than runtime, and accordingly may be used in any place that a constant may be."[^3]

The second half of that sentence is the important part. Some places in a program need a value before the program runs. The size of an array that lives in static memory, a `case` label, the width of a bit field, a template argument in C++ and a const generic argument in Rust all need a value that the compiler knows. A language that can compute such values lets the programmer write `ROWS * COLS` and not `12`.

A computed constant also lets the compiler check a fact about the program. `static_assert(sizeof(int) >= 4)` in C and C++ stops the build if the condition is false.[^1][^3] A check of this kind costs nothing at run time and fails before any user runs the program.

The languages differ in how much code the compiler will run. Two positions mark the ends of the range:

- **A fixed set of operators.** C, Go, Java and C# allow literals, names of constants and built-in operators. A constant expression cannot call a function that the programmer wrote.[^3][^6][^7][^10]
- **Ordinary functions, with limits.** C++ `constexpr` functions and Rust `const fn` functions are ordinary functions that the compiler can also run. A large part of the language is available inside them, and the rest is forbidden.[^1][^4]

The cpp example `both-times` shows one `constexpr` function used by a `static_assert`, an array bound and a call at run time:

```cpp norun reason=fragment
constexpr long factorial(int n) {
    long r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

static_assert(factorial(5) == 120, "5! is 120");
```

## How it works

### An interpreter inside the compiler

To evaluate `factorial(5)` at compile time, the compiler must run the function. It does not run machine code. It interprets the program in its own internal form, with its own model of memory. The Rust Reference says that a const function called from a const context "is interpreted by the compiler at compile time".[^4]

This interpreter must give the answer that the target machine would give. Rust says so directly: "The interpretation happens in the environment of the compilation target and not the host. So usize is 32 bits if you are compiling against a 32 bit system".[^4] C requires that a floating expression evaluated during translation has at least the range and precision of the execution environment.[^3]

The interpreter also checks each step. A processor executes a signed overflow or an access out of bounds and continues, and the result is undefined behavior in C and C++. The compiler's interpreter sees the operation and stops. In C++, an operation "that would have undefined behavior" in the core language makes the expression not a core constant expression.[^1] In Rust, out of bounds indexing and overflow "are compiler errors if the value must be evaluated at compile time".[^4] So in a constant context, a C++ compiler reports undefined behavior of the core language as an error. See [[undefined-behavior]].

### Contexts that require a constant

Each language names the places that need a constant. In Rust, a const context is one of these: the length of an array type, the length in an array repeat expression, the initializer of a constant, a static or an enum discriminant, a const generic argument, and a `const` block.[^4] "Expressions in const contexts are always evaluated at compile time."[^4]

Outside these contexts, the rules are different. In Rust, constant expressions elsewhere "may be, but are not guaranteed to be, evaluated at compile time".[^4] A `const fn` called from ordinary code "behaves the same as if it did not have the const qualifier".[^4] C++ has the same split. A `constexpr` function call produces the same result as a call to a function that is not `constexpr`, except that it can also appear in a constant expression.[^1]

So `constexpr` and `const fn` grant permission. They do not force evaluation at compile time. The place of the call decides. An optimizing compiler can also fold a constant expression in ordinary code, but that is an optimization, and the program cannot depend on it.

### Forcing evaluation

C++20 added `consteval`.[^1] A function declared `consteval` is an immediate function, and each call of it must produce a constant.[^1] The cpp example `consteval-runtime` passes `argc` to such a function, and the compiler rejects it. A `constexpr` variable also forces evaluation, because its initializer must be a constant expression.[^1]

A function sometimes needs two implementations: one that the compiler can interpret and a faster one for run time. C++ has `if consteval` for this. If the statement is evaluated in a context that is manifestly constant-evaluated, the first branch runs. Otherwise the `else` branch runs.[^1]

### Static assertions

A static assertion takes a constant condition and a message. C11 added static assertions, and C23 made `static_assert` a keyword and added a form with one argument.[^3] In C the condition is an integer constant expression.[^3] In C++ the condition is a constant expression converted to `bool`. If it is false, the program is ill-formed, and the diagnostic should include the message.[^1] Rust has no special keyword. A panic during constant evaluation is a compile error, so `const _: () = assert!(...);` does the same work. The rust example `assert` shows it.

## In languages

| Language | Verdict | Form |
|---|---|---|
| C++ | native | `constexpr` variables and functions, `consteval`, `if consteval`, `static_assert` |
| Rust | native | `const` items, `const fn`, `const` blocks, `assert!` in a `const` item |
| C | native | operators on constants; C23 `constexpr` objects and `static_assert`; no constant functions |
| Go | native | exact untyped constants, `iota`, `len` of constants; no constant functions |
| Java | native | constant expressions and constant variables; no method calls |
| C# | native | `const` fields and locals; no method calls |
| Kotlin | native | `const val` of a primitive type or `String` |
| TypeScript | idiom | `const enum` members, inlined by the compiler |
| JavaScript | absent | `const` is a binding computed at run time |
| Python | absent | capital names are constants by convention |
| PowerShell | absent | no compile-time constants |
| Bash | absent | no compile-time constants |
| SQL | n/a | no user-defined compile-time computation |
| Regex | n/a | no expressions |
| HTML/CSS | n/a | no compile step |

**C++** allows a large part of the language in a constant evaluation. It can even allocate memory with `new`, if the storage is deallocated within the same evaluation.[^1] The cpp example `heap` builds a `std::vector` inside a constant evaluation. The list of forbidden operations includes a call to a function that is not `constexpr`, an operation with undefined behavior in the core language, `reinterpret_cast`, a `throw` expression, an `asm` declaration and a `goto` statement.[^1] For undefined behavior in the library, the standard leaves it unspecified whether the expression is a core constant expression.[^1] The standard also allows a `constexpr` function for which no call is a constant expression: "It is possible to write a constexpr function for which no invocation satisfies the requirements of a core constant expression."[^1]

**Rust** grew `const fn` step by step. Rust 1.31 allowed "a minimal subset of operations" in a `const fn`: arithmetic on integers, construction of arrays, structs, enums and tuples, calls to other `const fn` functions, index expressions and field access.[^5] The Reference lists `loop` and `while` as constant expressions.[^4] It does not list `for`, because a `for` loop calls trait methods. The rust example `for-loop` shows the error. A const function cannot be `async`.[^4]

**C** had constant expressions from C89: "Constant expressions shall not contain assignment, increment, decrement, function-call, or comma operators".[^16][^3] C23 added the `constexpr` storage class for objects.[^3] Such an object "has its value permanently fixed at translation-time".[^3] Its initializer must give a value that the type represents exactly. The standard example marks `constexpr unsigned int minusOne = -1;` as a constraint violation and `constexpr double onethird = 1.0/3.0;` as a possible constraint violation.[^3] `constexpr` applies to objects only. A function definition can have only `extern` or `static` as a storage class.[^3]

**Go** constants are exact. "Numeric constants represent exact values of arbitrary precision and do not overflow."[^6] A compiler must represent integer constants with at least 256 bits.[^6] A constant can be untyped, and it takes a type only when the program uses it. At that point the value must fit: `int32(1) << 33` is illegal.[^6] "Constant expressions may contain only constant operands and are evaluated at compile time."[^6] The built-in `len` gives a constant for a string constant or an array without function calls, but a call to a function of the program is never constant.[^6]

**Java** defines a constant expression as "an expression denoting a value of primitive type or a String that does not complete abruptly", built from literals, casts, most operators, the conditional operator and the names of constant variables.[^7] A constant variable is "a final variable of primitive type or type String that is initialized with a constant expression".[^8] Method calls are not in the list, so `Math.max(10, 20)` is not a constant expression even though its result never changes. String constant expressions are interned, and constant expressions are used as `case` labels.[^7]

**C#** constants "can be numbers, Boolean values, strings, or a null reference".[^10] "A constant expression is an expression that the compiler can fully evaluate at compile time."[^10] An interpolated string can be a constant "if all expressions used are also constant strings".[^10] The compiler evaluates constant expressions in a checked context, so an overflow is a compile-time error.[^11] A value that is known only at run time, such as the current time, goes in a `static readonly` field.[^10]

**Kotlin** marks a compile-time constant with `const val`. It must be a top-level property or a member of an object declaration or a companion object, its value must be of type `String` or a primitive type, and it cannot have a custom getter. "Compile-time constants are inlined at compile time, so each reference is replaced with its actual value."[^12] No Kotlin example runs on this page, because kotlinc is not installed on the machine that recorded the examples.

**TypeScript** compiles `const enum` declarations away. "Const enums can only use constant enum expressions and unlike regular enums they are completely removed during compilation. Const enum members are inlined at use sites."[^13] No TypeScript example runs, because the examples run under Node.js type stripping, which rejects enums. **JavaScript** `const` is not a compile-time constant. It "creates an immutable reference to a value" and the value is computed when the declaration runs.[^14] **Python** has no constant evaluation in the language. PEP 8 says that "Constants are usually defined on a module level and written in all capital letters".[^15]

## Variations and related ideas

- **Constant folding.** An optimizer computes constant subexpressions in ordinary code. The program cannot see whether this happened, and the language does not require it. Constant evaluation in the sense of this page is part of the language and has rules that every compiler follows.
- **Macros.** A macro also does work at compile time, but it produces code, not values. C programs that need a constant computed from parts often use a macro, because C has no constant functions. See [[syntactic-macros]].
- **Generics.** C++ templates and Rust const generics take constant values as arguments, so the compiler must evaluate them before it can instantiate the code. See [[parametric-polymorphism]].
- **Immutability.** `const` in JavaScript, `final` in Java and `readonly` in C# stop a variable from changing after it is set. They do not move the computation to compile time. See [[immutability]].
- **Precomputed tables.** A constant evaluation can fill an array at compile time. The program then reads the table at run time with no setup cost.

## Pitfalls

### Expecting constexpr to mean compile time

A call to a `constexpr` function in ordinary code is an ordinary call. Only a constant context forces the compiler to evaluate it.[^1] Rust has the same rule for `const fn`.[^4] If the evaluation at compile time matters, assign the result to a `constexpr` variable or a Rust `const` item, or use `consteval` in C++.

### Overflow in a constant

The cpp example `overflow` computes 13! in a 32-bit `int`. In a constant context, the compiler finds the overflow and rejects the program. The same call at run time compiles, and its overflow is undefined behavior.[^1] The C# example `overflow` and the rust example `overflow` show the same check. A Go typed constant must fit its type, so the go example `overflow` also fails to compile.[^6] This is an advantage of constant evaluation, but only for the values that the compiler evaluates.

### Limits of the compiler

A constant evaluation runs inside the compiler, and the compiler has limits. The C++ standard recommends at least 512 recursive `constexpr` calls and 1 048 576 full-expressions in one core constant expression. These numbers are "only guidelines and do not determine compliance".[^1] An expression that would exceed the limits of the implementation is not a core constant expression.[^1] The cpp example `depth` recurses 2000 times and fails to compile. Write deep computations as loops, and keep the work of a constant evaluation small, because the compiler does it again in each build.

### A method call that looks constant

In Java, C# and Go, a call to a library function such as `Math.max` is never a constant expression, even when the arguments are constants.[^7][^10][^6] The java example `method-call` and the csharp example `call` fail for this reason. Write the value, or compute it at run time in a `static final` or `static readonly` field.

### An abrupt completion in Java

A Java constant expression must not complete abruptly.[^7] `1 / zero`, where `zero` is a constant variable with the value 0, is therefore not a constant expression. The compiler accepts it as ordinary code, and the division throws `ArithmeticException` when it runs. The java example `abrupt` shows this. Go rejects a constant division by zero at compile time.[^6]

### Constants copied into other binaries

The compiler puts the value of a constant into the code that uses it. In Java, a reference to a constant variable "must be resolved at compile time to the value", and a class that was compiled against an old value "will not see any new value for a usage of the field unless they are recompiled".[^9] The specification calls these "inconstant constants" and recommends "very sparing use of static constant variables".[^9] C# warns about the same effect for public constants in libraries: other code "needs to be recompiled to see the changes".[^10] Use a constant only for a value that will not change, and use a `static final` or `static readonly` field for a value such as a version number.

### Lossy conversion in a C constexpr

`const unsigned int x = -1;` is valid C and gives the maximum value. With `constexpr`, the same declaration is a constraint violation, because the value -1 is not representable in `unsigned int`.[^3] The c example `minus-one` shows the error. Write `-1U`.

## Cost

- **Run time.** A value computed by the compiler costs nothing when the program runs. The program reads it as a literal.
- **Compile time.** The compiler interprets the code, so heavy constant evaluation makes each build slower.
- **Language surface.** C++ and Rust split their functions into those that can run at compile time and those that cannot. Each library function must decide whether it is `constexpr` or `const`, and a change of that decision can break callers.[^1][^4]
- **Binary coupling.** Inlined constants tie compiled clients to the old value, as described under Pitfalls.[^9]

## History

- **1989.** The ANSI C standard defined a constant expression as one that "can be evaluated during translation rather than runtime", and it forbade function calls in it.[^16]
- **2007.** Dos Reis, Stroustrup and Maurer proposed generalized constant expressions for C++ in N2235. The paper extended constant expressions to "constant-expression functions" and user-defined literals, to support generic programming, systems programming and library building.[^2]
- **C++11.** `constexpr` and `static_assert` became keywords.[^1]
- **C11.** C added static assertions.[^3]
- **2018.** Rust 1.31 shipped `const fn` with "a minimal subset of operations".[^5]
- **C++20.** C++ added the keyword `consteval` for immediate functions.[^1]
- **C23.** C added `constexpr` for objects, made `static_assert` a keyword and added its form with one argument.[^3]

## References

---
tag: R151YT
kind: concept
slug: function-signature
name: Function signature
aliases: [signature, function prototype, method signature, type signature, call signature]
area: functions-and-abstraction/functions-and-procedures
level: intro
summary: >-
  A function signature is the outside of a function: its name, its parameters with their types and its result type. Callers need only the signature, and a typed language checks each call against it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1989
  people: []
  note: Early C declared functions without the types of their parameters. The C standard of 1989 added the new style of declaration with parameter types, so that the compiler could check the number and the types of the arguments of each call.
see_also: [function, function-call, return-value, parameter-passing, procedure-concept, method-concept, parametric-polymorphism, higher-order-functions]
sources:
- url: https://doc.rust-lang.org/reference/items/functions.html
  title: Functions, The Rust Reference
  locator: items.fn.intro; items.fn.signature
- url: https://en.cppreference.com/w/c/language/function_declaration.html
  title: Function declarations, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/function.html
  title: Function declaration, C++ reference, cppreference.com
  locator: Function signature
- url: https://docs.python.org/3/reference/compound_stmts.html#function-definitions
  title: Compound statements, The Python Language Reference
  locator: 8.7 Function definitions
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Function Type Expressions; Call Signatures
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.2
  title: Classes, The Java Language Specification, Java SE 25 Edition
  locator: 8.4.2 Method Signature
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/methods
  title: Methods, C# Programming Guide
  locator: Method signatures
- url: https://go.dev/ref/spec#Function_types
  title: The Go Programming Language Specification
  locator: Function types
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map
  title: Array.prototype.map(), MDN Web Docs
  locator: Using parseInt() with map()
- url: https://en.wikipedia.org/wiki/Function_prototype
  title: Function prototype, Wikipedia
  locator: the introduction; Uses
- url: https://en.cppreference.com/w/c/language/function_definition.html
  title: Function definitions, C reference, cppreference.com
  locator: Syntax
---

## Idea

A function has an outside and an inside. The outside is the signature: the name, the parameters and their types, and the type of the result. The inside is the body. A caller needs to know only the signature to call the function correctly.

```typescript run id=idea
function repeatText(text: string, times: number): string {
  return text.repeat(times);
}

console.log(repeatText("ab", 3));
```

```output for=idea
ababab
```

The first line is the signature. It says that `repeatText` takes a string and a number, and gives back a string. A reader can use the function from this line alone, and the compiler rejects a call such as `repeatText(3, "ab")`.

The Rust Reference says that a function can declare "a set of input variables as parameters, through which the caller passes arguments into the function, and the output type of the value the function will return to its caller on completion"[^1]. These parts are the signature.

## How it works

1. **The declaration.** The program writes the signature as the first part of the function definition. In C and C++, the program can also write the signature alone, without a body, as a declaration.
2. **The check at the call.** A compiler that knows the signature checks each call. In C, a declaration with parameter types "serves as a function prototype for any future function call expressions, forcing conversions from argument expressions to the declared parameter types and compile-time checks for the number of arguments"[^2].
3. **The check of the body.** The compiler also checks that the body agrees with the signature, for example that each `return` gives a value of the result type.
4. **Identity.** A language with overloading uses the signature to tell two functions with the same name apart. "The signature of a function consists of its name and parameter-type-list" in C++[^3].

## In languages

- **Python.** The parameters have no required types. "Parameters may have an annotation of the form “: expression” following the parameter name."[^4] "Functions may have “return” annotation of the form “-> expression” after the parameter list."[^4] Tools such as type checkers read these annotations, but Python does not check them when it runs.
- **JavaScript.** A signature has only the name and the parameter names. The language does not check the number or the types of the arguments.
- **TypeScript.** TypeScript adds types to the JavaScript signature. A function type expression such as `(a: string) => void` means "“a function with one parameter, named a, of type string, that doesn’t have a return value”"[^5]. An object type can also have a call signature.
- **Java.** The JLS defines the signature without the result type: "Two methods or constructors, M and N, have the same signature if they have the same name, the same type parameters (if any)" and the same formal parameter types[^6].
- **C#.** The C# guide lists "the access level such as public or private, optional modifiers such as abstract or sealed, the return value, the name of the method, and any method parameters. These parts together are the signature of the method."[^7] But "A return type of a method is not part of the signature of the method for the purposes of method overloading."[^7]
- **C.** A declaration such as `int sum(int a, int b);` is a prototype. A declaration with no parameter list "does not introduce a prototype and any future function call expressions will perform default argument promotions and will invoke undefined behavior if the number of arguments doesn't match the number of parameters"[^2].
- **C++.** "Every function has a signature. The signature of a function consists of its name and parameter-type-list."[^3] For a member function, the signature "contains the class of which the function is a member"[^3].
- **Go.** The grammar calls the parameters and the results the signature. "A function type denotes the set of all functions with the same parameter and result types."[^8]
- **Rust.** Each parameter and the result must have a type. If the output type is not written, "it is the unit type"[^1].
- **Bash.** A Bash function has no signature. It is declared as "fname () compound-command [ redirections ]"[^9], and the parentheses are always empty. The body reads its arguments as `$1`, `$2` and so on.

## Variations and related ideas

- **Signatures as contracts.** A signature is a small contract between the function and its callers: the callers give arguments of the parameter types, and the function gives a result of the result type. It says nothing about the effects of the function or the rules on the values, so documentation and tests must give the rest.
- **Function types.** In Go, TypeScript and Rust, the signature without the name is a type. A variable or a parameter of that type can hold any function with the same parameter and result types[^8]. See [[higher-order-functions]].
- **Signatures and overloading.** Java, C#, and C++ allow several functions with one name if the signatures differ. The result type does not tell them apart[^6][^7].
- **Generic signatures.** A signature can have type parameters, such as `fn first<T>(items: &[T]) -> &T` in Rust. See [[parametric-polymorphism]].
- **Method signatures.** A method signature also names the type that the method belongs to. See [[method-concept]].
- **Prototypes and headers.** C and C++ put the signatures of a library in header files, so other files can call the functions and the compiler can check the calls.

## Pitfalls

### Type annotations are not checked at run time in Python

A Python signature with annotations looks like a typed signature, but "The presence of annotations does not change the semantics of a function."[^4]

```python wrong id=python-annotations-unchecked
def repeat_text(text: str, times: int) -> str:
    return text * times


print(repeat_text(3, 4))
```

```output for=python-annotations-unchecked
12
```

The call passes two integers, and Python runs it with no error. Run a type checker such as mypy on the code to find this mistake before the program runs.

### A callback with a different signature

JavaScript does not compare the signature of a callback with the arguments that the caller gives. MDN shows a well-known case[^10]:

```javascript wrong id=javascript-map-parseint
console.log(["1", "2", "3"].map(parseInt));
```

```output for=javascript-map-parseint
[ 1, NaN, NaN ]
```

"Array.prototype.map passes 3 arguments: the element, the index, and the array."[^10] `parseInt` takes two: the text and the radix. "The third argument is ignored by parseInt — but not the second one!"[^10] So the second call is `parseInt('2', 1)`, which gives `NaN`. Pass a function with the right signature: `['1', '2', '3'].map((s) => parseInt(s, 10))`.

## Cost

A signature costs nothing at run time. It is information for the compiler and for the reader. In a typed language, the compiler uses it to find wrong calls before the program runs.

## History

Early C declared a function without the types of its parameters. Wikipedia says: "In early versions of C, if a function was not previously declared and its name occurred in an expression followed by a left parenthesis, it was implicitly declared as a function that returns an int and nothing was assumed about its arguments."[^11]

The C standard of 1989 added parameter types to declarations and definitions. cppreference calls the form with typed parameters the "New-style (C89) function definition", and the form without them the "Old-style (K&R) function definition"[^12]. A declaration with parameter types is a prototype, and the compiler checks each call against it[^2].

## References

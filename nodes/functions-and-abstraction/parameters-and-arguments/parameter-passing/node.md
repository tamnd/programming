---
tag: W3XADK
kind: concept
slug: parameter-passing
name: Parameter passing
aliases: [evaluation strategy, call by value, call by reference, call by sharing, call by name, pass by value, pass by reference, named arguments, default arguments, variadic functions]
area: functions-and-abstraction/parameters-and-arguments
level: core
summary: >-
  Parameter passing is how the arguments of a call reach the parameters of a function: as copies, as references to variables, as shared objects or as unevaluated expressions.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Peter Naur]
  note: ALGOL 60 defined value parameters and name parameters.
see_also: [closures, higher-order-functions, lexical-scope, order-of-evaluation, value-equality-and-identity]
same_as:
  wikidata: Q1410440
  wikipedia: Parameter_(computer_programming)
sources:
- url: https://docs.python.org/3.14/faq/programming.html#what-is-the-difference-between-arguments-and-parameters
  title: Programming FAQ, Python 3.14
  locator: What is the difference between arguments and parameters? How do I write a function with output parameters (call by reference)?
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language ALGOL 60, P. Naur (ed.), Communications of the ACM 6(1), 1963
  locator: 4.7.3 Semantics of procedure statements
- url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
  title: ISO/IEC 9899:201x Committee Draft N1570
  locator: 6.5.2.2 Function calls, paragraph 4 and footnote 93
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.1
  title: The Java Language Specification, Java SE 25 Edition
  locator: 8.4.1 Formal Parameters
- url: https://go.dev/doc/faq#pass_by_value
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: When are function parameters passed by value?
- url: https://en.cppreference.com/w/cpp/language/reference
  title: Reference declaration, cppreference.com
  locator: Lvalue references
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/method-parameters
  title: Method parameters and modifiers, C# reference, Microsoft Learn
  locator: ref parameter modifier, out parameter modifier
- url: https://publications.csail.mit.edu/lcs/pubs/pdf/MIT-LCS-TR-561.pdf
  title: A History of CLU, Barbara Liskov, MIT LCS TR-561, 1992
  locator: 3.5 Semantic Model
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Functions
  title: Functions, MDN Web Docs
  locator: Function parameters
- url: https://doc.rust-lang.org/book/ch04-01-what-is-ownership.html
  title: The Rust Programming Language, What Is Ownership?
  locator: Ownership and Functions
- url: https://www.postgresql.org/docs/current/sql-syntax-calling-funcs.html
  title: PostgreSQL Documentation, Calling Functions
  locator: 4.3 Calling Functions
- url: https://peps.python.org/pep-3102/
  title: PEP 3102, Keyword-Only Arguments
  locator: Abstract, Specification
- url: https://peps.python.org/pep-0570/
  title: PEP 570, Python Positional-Only Parameters
  locator: Abstract, Specification
- url: https://docs.python.org/3.14/reference/compound_stmts.html#function-definitions
  title: The Python Language Reference, Compound statements, Python 3.14
  locator: 8.7 Function definitions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/Default_parameters
  title: Default parameters, MDN Web Docs
  locator: Evaluated at call time
- url: https://kotlinlang.org/docs/functions.html
  title: Kotlin documentation, Functions
  locator: Parameters with default values, Named arguments
---

## Idea

A function declares parameters, and a call supplies arguments. The Python FAQ states the difference: "Parameters are defined by the names that appear in a function definition, whereas arguments are the values actually passed to a function when calling it."[^1] Parameter passing is the rule that connects the two. It answers three questions. What does the parameter get: a copy of the value, the variable of the caller, or an expression that is not evaluated yet? Which argument goes to which parameter: by position, by name, or both? What happens when the call has fewer or more arguments than the function has parameters?

The first question decides what a function can change. In this Python example, the function changes the list of the caller but not the variable `total`:

```python run id=idea
def update(items, count):
    items.append(4)
    items = [0]
    count += 1


numbers = [1, 2, 3]
total = 1
update(numbers, total)
print(numbers, total)
```

```output for=idea
[1, 2, 3, 4] 1
```

`items` and `numbers` refer to the same list, so `append` changes it. The assignments to `items` and `count` only change the local names. Most modern languages behave like this, and programmers often argue whether to call it "by value" or "by reference". It is neither in the old sense of those words.

## How it works

**Call by value.** The parameter is a new local variable, and the value of the argument is copied into it before the body runs. The ALGOL 60 report describes the effect "as though an additional block embracing the procedure body were created" with an assignment to each value parameter.[^2] C works this way: "each parameter is assigned the value of the corresponding argument", and the standard adds that a function "may change the values of its parameters, but these changes cannot affect the values of the arguments".[^3] To let a function change a variable of the caller, C passes a pointer to it. The pointer is copied, but the copy points to the same object. Java states the same rule for methods: "the values of the actual argument expressions initialize newly created parameter variables".[^4] The Go FAQ is short: "everything in Go is passed by value", and "passing a pointer value makes a copy of the pointer, but not the data it points to".[^5]

**Call by reference.** The parameter is another name for the variable of the caller. An assignment to the parameter is an assignment to that variable. Fortran compilers used this method, and Pascal has it with `var` parameters. In C++, a parameter of type `T&` is a reference, and cppreference shows a `double_string(std::string& s)` that changes the string of `main`.[^6] C# has the modifiers `ref`, `out`, `in` and `ref readonly`. Both the declaration and the call must write `ref` or `out`, so a reader of the call can see that the variable can change. An `out` argument needs no value before the call, and the method must assign it before it returns.[^7] Call by reference needs an argument that is a variable. A call such as `f(x + 1)` has no variable to refer to.

**Call by sharing.** In Python, Java, JavaScript, Ruby and many other languages, a variable holds a reference to an object. Passing an argument copies the reference, so the caller and the function share the object, but not the variable. Barbara Liskov described this model for CLU: there "is no call-by-reference. Instead arguments are passed 'by object'; the (pointer to the) object resulting from evaluating the actual argument expression is assigned to the formal. (Thus passing a parameter is just doing an assignment to the formal.)"[^8] The Python FAQ says the same: "arguments are passed by assignment in Python", and "consequently no call-by-reference".[^1] MDN describes JavaScript in the same way. A new value assigned to a parameter "is not reflected globally or in the code which called that function", but a change to the properties of an object argument is visible outside.[^9] So the rule for these languages is simple. Mutation of the shared object is visible. Rebinding of the parameter is not.

**Moves and borrows.** Rust passes by value, but a value of a type that owns resources moves into the function. "Passing a variable to a function will move or copy, just as assignment does."[^10] After the move, the caller cannot use the variable. To lend a value without a move, the caller passes a shared reference `&T` or a mutable reference `&mut T`. The compiler checks that a borrow does not outlive the value and that a mutable borrow is the only access at that time. C++ has a similar idea with rvalue references and `std::move`, but the compiler does not stop the use of a moved-from object.

**Call by name.** In ALGOL 60, a parameter that is not in the value list is "replaced, throughout the procedure body, by the corresponding actual parameter".[^2] The argument expression is evaluated again at each use of the parameter, in the environment of the caller. An implementation passes a small procedure, called a thunk, that computes the expression. Call by name lets a procedure take an expression such as `a[i]` and see a new element each time that it changes `i`. It is hard to implement efficiently and hard to reason about, and later languages dropped it. Lazy evaluation in Haskell is call by need: the expression is evaluated at the first use, and the result is kept for later uses.

**Copy-in, copy-out.** A parameter can also get a copy of the value at the call and copy its final value back to the variable of the caller at the return. This is call by value-result. Ada defines `in`, `out` and `in out` modes and lets the compiler pass a scalar `in out` parameter by copy. The difference from call by reference can be seen only when the same variable is passed twice or is also visible as a global.

**Matching arguments to parameters.** Most languages match arguments by position. Named arguments, also called keyword arguments, match by name: Python, C#, Kotlin, Swift and PostgreSQL have them. PostgreSQL states the reason: named notation "makes the associations between parameters and actual arguments more explicit and reliable".[^11] Python can force a style. Parameters after `*` are keyword-only, as PEP 3102 defines them, and parameters before `/` are positional-only, as PEP 570 defines them.[^12][^13]

**Default values.** A parameter with a default value can be left out of the call. The language must say when the default is evaluated. Python evaluates it once: "Default parameter values are evaluated from left to right when the function definition is executed", so a mutable default is shared by all the calls.[^14] JavaScript evaluates the default at each call, and MDN contrasts this with Python.[^15] C++ evaluates a default argument at each call too. Kotlin has default values and named arguments, which replace many overloads.[^16]

**Variadic parameters.** A function can take any number of arguments: `*args` in Python, `...rest` in JavaScript, `int... values` in Java, `params int[]` in C#, `...int` in Go and `...` with `<stdarg.h>` in C. In the typed languages other than C, the extra arguments arrive as an array or a slice of one type. The C form has no type check of the extra arguments.

## In languages

| Language | Verdict | Default mode | By-reference form | Named arguments |
|---|---|---|---|---|
| Python | native | sharing | none | yes |
| JavaScript | native | sharing | none | no, use an object |
| TypeScript | native | sharing | none | no, use an object |
| Java | native | value, references for objects | none | no |
| Kotlin | native | value, references for objects | none | yes |
| C# | native | value | `ref`, `out`, `in` | yes |
| Go | native | value | pointers | no |
| Rust | native | move or copy | `&`, `&mut` | no |
| C | native | value | pointers | no |
| C++ | native | value | `T&`, `const T&`, `T&&` | no |
| Bash | native | strings by value | a variable name | no |
| PowerShell | native | value, references for objects | `[ref]` | yes |
| SQL | native | value | `OUT`, `INOUT` | yes, in PostgreSQL |
| Regex, HTML | n/a | | | |
| CSS | absent | | | |

**Python** passes by assignment. Default values are evaluated once. `*` and `/` in the parameter list make keyword-only and positional-only parameters.

**JavaScript** passes each value by value, and an object value is a reference. Defaults are evaluated at each call. The language does not check the number of arguments, so a function can get more or fewer than it declares. **TypeScript** adds a check of the number and the types.

**Java** passes all arguments by value and has varargs. It has no default values or named arguments. **Kotlin** has the same model on the JVM and adds default values and named arguments.

**C#** passes by value by default and has explicit by-reference modifiers that the call must repeat. It has optional parameters, named arguments and `params`.

**Go** passes by value. A pointer, a slice, a map or a channel is a small value that refers to shared data. Go has variadic parameters but no defaults and no named arguments.

**Rust** moves or copies each argument. References are explicit at the call: `&x` or `&mut x`.

**C** passes by value. Arrays become pointers, so the length is a separate argument.

**C++** adds references, default arguments and variadic templates to the C model.

**Bash** passes strings as positional parameters. A function returns a string by printing it, or by assigning to a variable whose name the caller passes.

## Variations and related ideas

- **Receivers.** A method call `obj.m()` passes `obj` as a hidden first parameter. Go and Rust make the mode of the receiver explicit: `func (p *T)` or `&mut self`.
- **Currying and partial application.** A function of two parameters can be a function of one parameter that returns a function of one parameter. Some languages fix some arguments of a function to make a new function.
- **Closures.** A closure captures variables from the scope around it. The capture is another way to give data to a function, and it has the same choices of copy or reference. See [closures](/closures/).
- **Out parameters and multiple results.** Languages with tuples or multiple return values, such as Python, Go and Rust, rarely need out parameters. C# keeps `out` for the `TryParse` pattern.
- **Calling conventions.** At the machine level, the ABI decides which arguments go in registers and which go on the stack. This is how call by value is implemented, not a different rule.
- **Higher-order functions.** A parameter can be a function. See [higher-order functions](/higher-order-functions/).

## Pitfalls

- **Rebinding is not changing.** In Python, Java and JavaScript, an assignment to a parameter does not change the variable of the caller. A swap function that only assigns its parameters has no effect.
- **Mutation of a shared object.** A function that changes an argument object changes it for the caller too. Copy the object first if the caller does not expect the change.
- **A mutable default in Python.** `def f(items=[])` shares one list between all the calls that use the default.
- **A slice header in Go.** `append` inside a function changes the copy of the slice header, and the length of the caller's slice does not change.
- **A struct copy in C# or C++.** A method that changes a struct parameter changes a copy.
- **Array size in C.** `sizeof` of an array parameter is the size of a pointer.
- **Extra arguments in JavaScript.** `["10", "10", "10"].map(parseInt)` passes the index as the radix.
- **Boolean arguments by position.** `connect(host, true, false)` is hard to read. Use named arguments, an options object or an enum.
- **Unquoted arguments in shell scripts.** `$@` without quotes splits arguments with spaces into many words.

## Cost

Call by value copies the argument. For a small value, such as an integer or a pointer, the copy is a register move and costs nothing. For a large struct or a container in C++, the copy can allocate memory and take time proportional to the size. Pass a `const T&` in C++, a `&T` in Rust or a pointer in Go and C to avoid it. Call by sharing never copies the object, only the reference.

Call by reference makes aliasing possible. If a function has two reference parameters, the compiler cannot assume that they refer to different objects, so it must reload values from memory after each write. C has the `restrict` qualifier to promise that they do not overlap. Rust gets the same guarantee from the borrow rules.

Call by name and lazy evaluation make each argument a small closure. The closure costs an allocation, and in call by name the expression is evaluated at each use.

## History

The ALGOL 60 report of 1960 defined two modes: value parameters, assigned before the body runs, and name parameters, replaced in the body by the text of the argument.[^2] Fortran compilers of the same period passed the address of the argument, which is call by reference. Pascal, in 1970, kept value parameters and replaced call by name with `var` parameters, which are passed by reference. C, in 1972, passed everything by value and used pointers for the rest. CLU, designed at MIT from 1973, passed arguments "by object", and the same model is used by Smalltalk, Python, Java, JavaScript and Ruby.[^8] Ada, in 1983, described parameters by their direction, `in`, `out` and `in out`, and not by the mechanism. C++ added references in the 1980s and rvalue references in C++11. Rust, in 2015, made moves the default and made borrows explicit and checked.

## References

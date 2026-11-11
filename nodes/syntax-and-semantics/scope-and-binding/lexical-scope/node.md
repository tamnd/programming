---
tag: CWHGPD
kind: concept
slug: lexical-scope
name: Lexical scope
aliases: [static scope, lexical scoping, block scope, name resolution, shadowing]
area: syntax-and-semantics/scope-and-binding
level: core
summary: >-
  The scope of a declaration is the part of the program text where its name refers to it. With lexical scope, the text decides what each name means, not the calls at run time.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Peter Naur]
  note: ALGOL 60 made each block a new level of naming, with local and non-local identifiers.
see_also: [closures, recursion, higher-order-functions, value-equality-and-identity, pattern-matching]
same_as:
  wikidata: Q1034415
  wikipedia: Scope_(computer_programming)
sources:
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language ALGOL 60, P. Naur (ed.), Communications of the ACM 6(1), 1963
  locator: 4.1.3 Semantics of blocks
- url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
  title: ISO/IEC 9899:201x Committee Draft N1570
  locator: 6.2.1 Scopes of identifiers
- url: https://eel.is/c++draft/basic.scope
  title: Working Draft, Programming Languages C++
  locator: 6.4 Scope [basic.scope]
- url: https://doc.rust-lang.org/reference/names/scopes.html
  title: The Rust Reference, Scopes
  locator: Pattern binding scopes, Pattern binding shadowing
- url: https://docs.python.org/3.14/reference/executionmodel.html#resolution-of-names
  title: The Python Language Reference, Execution model, Python 3.14
  locator: 4.2.2 Resolution of names
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let
  title: let, MDN Web Docs
  locator: Temporal dead zone (TDZ), Redeclarations
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/var
  title: var, MDN Web Docs
  locator: Scope, Hoisting
- url: https://learn.microsoft.com/en-us/dotnet/csharp/misc/cs0136
  title: Compiler Error CS0136, Microsoft Learn
  locator: description and examples
- url: https://go.dev/ref/spec#Declarations_and_scope
  title: The Go Programming Language Specification
  locator: Declarations and scope, Blocks
- url: https://docs.python.org/3.14/reference/compound_stmts.html#the-for-statement
  title: The Python Language Reference, Compound statements, Python 3.14
  locator: 8.3 The for statement
- url: https://peps.python.org/pep-3104/
  title: PEP 3104, Access to Names in Outer Scopes
  locator: Abstract, Proposal
- url: https://docs.python.org/3.14/faq/programming.html#why-am-i-getting-an-unboundlocalerror-when-the-variable-has-a-value
  title: Programming FAQ, Python 3.14
  locator: Why am I getting an UnboundLocalError when the variable has a value?
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-6.html#jls-6.4
  title: The Java Language Specification, Java SE 25 Edition
  locator: 6.4 Shadowing and Obscuring
- url: https://go.dev/ref/spec#Short_variable_declarations
  title: The Go Programming Language Specification
  locator: Short variable declarations
- url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  title: GCC Manual, Options to Request or Suppress Warnings
  locator: -Wall, -Wshadow
- url: https://kotlinlang.org/docs/functions.html#local-functions
  title: Kotlin documentation, Functions
  locator: Local functions
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_scopes?view=powershell-7.6
  title: about_Scopes, PowerShell 7.6
  locator: Basic rules of scope, Scope modifiers
- url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Functions
  title: Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://www.postgresql.org/docs/current/functions-subquery.html
  title: PostgreSQL Documentation, Subquery Expressions
  locator: 9.23.1 EXISTS
- url: https://peps.python.org/pep-0227/
  title: PEP 227, Statically Nested Scopes
  locator: Abstract
---

## Idea

A name in a program refers to a variable, a function or a type. The scope of a declaration is the part of the program text where the name refers to that declaration. With lexical scope, also called static scope, the scope follows the structure of the text. To find what a name means, start at the use and read outward, block by block, to the nearest declaration of the name. The answer is fixed before the program runs, and the calls that the program makes at run time do not change it.

The other rule is dynamic scope. With dynamic scope, a name refers to the most recent binding that is still active at run time, which is usually a variable of the caller. The same program gives different results under the two rules. Python uses lexical scope, so `reads_level` always sees the module variable:

```python run id=idea
level = "global"


def reads_level():
    return level


def caller():
    level = "caller"
    return reads_level()


print(reads_level())
print(caller())
```

```output for=idea
global
global
```

Bash uses dynamic scope, so the same function sees the variable of its caller:

```bash run id=idea-dynamic
level="global"

reads_level() {
  echo "$level"
}

caller() {
  local level="caller"
  reads_level
}

reads_level
caller
```

```output for=idea-dynamic
global
caller
```

Most languages use lexical scope for their variables. A reader can understand a function from its text and the text around it, with no knowledge of the callers. A compiler can find each variable at compile time. Dynamic scope is still the rule for variables in command shells, such as Bash and PowerShell.

## How it works

**Regions nest.** A block, a function body, a class body and a module are regions of the text. The Revised Report on ALGOL 60 states the rule for blocks: "Every block automatically introduces a new level of nomenclature." An identifier that is declared in the block is local to it, and the other identifiers are non-local.[^1] To resolve a name, the compiler looks in the innermost region that contains the use, then in the region around it, and so on to the outermost region. The first declaration that it finds is the one that the name refers to.

**An inner declaration hides an outer one.** When an inner region declares a name that an outer region also declares, the inner declaration hides the outer one inside the inner region.[^2] This is also called shadowing. After the inner region ends, the name refers to the outer declaration again.

**Where the scope starts.** Languages do not agree on the first point of a scope.

- In C, the scope of an identifier in a block starts just after its declarator and ends at the end of the block.[^2] C++ has the same rule, so in `const int i = 2; { int i[i]; }` the inner array has two elements, because the size `i` is still the outer `i`.[^3]
- In Rust, a `let` binding is in scope from just after the `let` statement to the end of the block.[^4]
- In Python, a binding anywhere in a block makes the name local to the whole block.[^5] A read before the assignment is an error, not a read of the outer variable.
- In JavaScript, a `let` or `const` declaration is in scope in its whole block, but a read before the declaration throws a `ReferenceError`. The part of the block before the declaration is the temporal dead zone.[^6] A `var` declaration is hoisted: it acts as if it were at the top of its function, with the value `undefined`.[^7]
- In C#, the compiler rejects a local variable that has the name of a local variable in an enclosing block, whatever the order of the two declarations in the text.[^8]

**Which regions make a scope.** In C, C++, Java, C#, Go and Rust, each block is a scope. Go also treats each `if`, `for` and `switch` statement as an implicit block, so a variable that the statement header declares ends with the statement.[^9] In JavaScript, `let`, `const` and `class` have block scope, and `var` has function scope.[^6][^7] Python has no block scope. A function, a class body, a module and a comprehension are scopes, and a `for` loop or an `if` statement is not. The loop variable of a `for` loop keeps its last value after the loop.[^10] The names of a class body are not visible in the methods of the class.[^5]

**Assignment to an outer variable.** Most languages let an inner function assign to a variable of an outer function with a normal assignment. In Python, an assignment makes a new local variable, so the inner function must first declare the name with `nonlocal`, or with `global` for a module variable.[^5][^11]

**Nested functions.** In a language where a function can be a value, an inner function can live longer than the call of the outer function that made it. Lexical scope says that the inner function still sees the variables of the outer function. The run time keeps those variables alive as long as the inner function, and the pair of the function and its variables is a [closure](/closures/).

**The compiler resolves the names.** Because the binding of each name is known from the text, a compiler resolves each name once. The running program does not search for names. It reads a local variable from a known slot in the stack frame or from a register. A variable that a closure captures moves to a place that lives as long as the closure. With dynamic scope, the binding depends on the calls, so an interpreter such as Bash finds a variable by its name at run time.

## In languages

| Language | Unit of scope | An inner local can hide an outer local | Verdict |
|---|---|---|---|
| Python | function, class body, module, comprehension | yes | native |
| JavaScript | block for `let`, `const` and `class`, function for `var` | yes | native |
| TypeScript | as JavaScript | yes | native |
| Java | block | no, a compile error | native |
| C# | block | no, error CS0136 | native |
| Go | block, and the implicit block of `if`, `for` and `switch` | yes | native |
| Rust | block | yes, also in the same block | native |
| C | block | yes, with no warning by default | native |
| C++ | block, namespace, class | yes | native |
| Kotlin | block | yes | native |
| PowerShell | script and function, at run time | yes | absent |
| Bash | function with `local`, at run time | yes | absent |
| SQL | query and subquery | yes | native |

**Python.** A name resolves in the nearest enclosing scope that binds it, and a name that no function binds resolves in the module and then in the builtins.[^5] An assignment anywhere in a function makes the name local to the whole function, which is the cause of `UnboundLocalError`.[^5][^12] `global` and `nonlocal` change this for one name.[^11]

**JavaScript and TypeScript.** `let`, `const` and `class` declarations have block scope and a temporal dead zone. `let` cannot declare the same name twice in one scope.[^6] `var` has function scope and is hoisted, so a `var` in a block is visible in the whole function.[^7] TypeScript does not change these rules, because its output is JavaScript.

**Java.** A local variable is in scope from its declaration to the end of its block. A local variable can hide a field. It is a compile-time error to declare a local variable or a parameter with the name of another local variable or parameter that is still in scope.[^13] So the inner `count` in the Java example does not compile.

**C#.** The rule is the same as in Java and is stronger: the compiler gives error CS0136 also when the outer declaration comes later in the text than the inner block.[^8]

**Go.** "Go is lexically scoped using blocks." An identifier that is declared in a block can be declared again in an inner block, and the inner declaration hides the outer one.[^9] A short variable declaration `:=` can reuse a variable only when the variable is declared earlier in the same block and at least one other variable on the left is new.[^14] In an inner block, `:=` always declares new variables, which is the source of a common bug.

**Rust.** A `let` binding is in scope from the end of the `let` statement to the end of the block. A new binding can shadow any earlier binding with the same name, also in the same block.[^4] Code in Rust uses this to give a value a new type under the same name, as in `let input = input.trim();`.

**C and C++.** Each block is a scope, and an inner declaration hides an outer one with no error.[^2][^3] GCC reports the hiding only with `-Wshadow`, and `-Wall` does not turn that option on.[^15] C++ adds namespace scope, class scope and the scope of the init-statement of `if`, `switch` and `for`.[^3] A qualified name, such as `config::level` or `this->x`, reaches a hidden declaration.

**Kotlin.** Blocks and functions nest. A local function can read the local variables of the functions around it.[^16] There is no Kotlin example file, because `kotlinc` is not installed on the machine that recorded the examples.

**PowerShell.** When a script or a function runs, PowerShell makes a new child scope. An item is visible in the scope where it was made and in all child scopes, unless it is private.[^17] A function therefore sees the variables of its caller, as in Bash. Scope modifiers such as `$script:` and `$global:` select a scope by name.[^17] There is no PowerShell example file, because `pwsh` is not installed on the machine that recorded the examples.

**Bash.** "Ordinarily, variables and their values are shared between the function and its caller." The `local` builtin gives a variable "a visible scope restricted to that function and its children".[^18] The children are the functions that it calls, so the scope follows the calls and not the text.

**SQL.** A subquery is inside the query that contains it, and it can refer to the columns of that query.[^19] An unqualified column name in a subquery binds to a table of the outer query when no table of the subquery has a column with that name. The SQL example shows this in SQLite.

## Variations and related ideas

- **Dynamic scope.** The binding follows the calls. Bash and PowerShell use it for their variables.[^18][^17] A reader must know the callers of a function to know what its names mean.
- **Block scope and function scope.** Most languages make each block a scope. Python and the `var` declarations of JavaScript make only the function a scope.[^10][^7]
- **Shadowing as a tool.** Rust and Go allow an inner declaration to hide an outer one, and code in Rust shadows on purpose to change the type of a value.[^4]
- **Shadowing as an error.** Java and C# reject a local variable that hides another local variable, because a programmer seldom means it.[^13][^8]
- **Closures.** A function value that keeps the variables of the scope where it was made. Lexical scope and functions as values together need closures. See [closures](/closures/).
- **Modules and namespaces.** A module or a namespace is the outermost named region. Its names are reached from other modules through an import or a qualified name.

## Pitfalls

- **An assignment makes the name local.** In Python, `count += 1` in a function makes `count` local to the whole function, and the read on the same line raises `UnboundLocalError`.[^5][^12] The Python example shows the error and the fix.
- **The loop variable stays.** In Python, the variable of a `for` loop exists after the loop with its last value.[^10] A loop in a long function can change a variable with the same name that the function uses later.
- **`var` in a block.** In JavaScript, a `var` in a block is a variable of the whole function.[^7] Use `let` or `const`.
- **The temporal dead zone.** In JavaScript, a read of a `let` variable before its declaration in the same block throws a `ReferenceError`, also when an outer variable has the same name.[^6]
- **`:=` in an inner block.** In Go, `x, err := f()` inside an `if` block declares a new `x` and a new `err`. The outer variables keep their old values, and the function returns them.[^14] The Go example returns `8080` after it parses `9090`.
- **Silent hiding in C and C++.** An inner `int total = 0;` hides the outer `total`, and the compiler says nothing unless `-Wshadow` is on.[^15] The C example prints `total: 0`.
- **A parameter that hides a field.** In a Java or C++ constructor, `x = x` assigns the parameter to itself when the parameter has the name of the field. Use `this.x = x` in Java, or a member initializer list in C++.
- **A column from the outer query.** In SQL, a column name that is wrong in a subquery can bind to the outer query, and the query runs with a wrong result and no error.[^19] Qualify each column with the alias of its table.
- **A function that changes its caller.** In Bash, a function that uses a variable with no `local` changes the variable of its caller.[^18] The Bash example prints one row in place of three.

## Cost

Lexical scope costs nothing at run time. The compiler resolves each name once, and the program reads each variable from a known place. A variable that a closure captures can need a heap allocation, because it must live as long as the closure. Dynamic scope needs a lookup at run time, or a stack of saved values for each name, because the binding depends on the calls.

## History

ALGOL 60 made the block the unit of naming. The Revised Report, edited by Peter Naur, says that each block introduces a new level of nomenclature, with local and non-local identifiers.[^1]

Python started with three namespaces for each name: the local, the global and the built-in namespace. A nested function could not see the variables of the function around it. PEP 227 added statically nested scopes, as an option in Python 2.1 with `from __future__ import nested_scopes`, and as the default in Python 2.2.[^20] PEP 3104 added the `nonlocal` statement in Python 3.0, so that an inner function can assign to a variable of an outer function.[^11]

## References

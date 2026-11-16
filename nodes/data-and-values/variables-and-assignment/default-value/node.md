---
tag: 9A6ECK
kind: concept
slug: default-value
name: Default value
aliases: [default value, default, zero value, default initialization, default parameter, default argument, fallback value, null default, undefined]
area: data-and-values/variables-and-assignment
level: core
summary: >-
  A default value is the value that a variable, a field or a parameter gets when the program gives none, such as 0, false or null. Some languages give one to each variable, and others only to fields.
applies_to: core-languages
viz: none
formal: none
see_also: [initialization-concept, variable-concept, value, constructors]
same_as:
  wikidata: Q281782
  wikipedia: Default_(computer_science)
sources:
- url: https://en.wikipedia.org/wiki/Default_(computer_science)
  title: Default (computer science), Wikipedia
  locator: Introduction
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: The zero value
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25
  locator: 4.12.5 Initial Values of Variables
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/default-values
  title: Default values of C# types, C# reference
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/undefined
  title: undefined, MDN Web Docs
  locator: Description
- url: https://doc.rust-lang.org/std/default/trait.Default.html
  title: Trait Default, Rust standard library
  locator: Description; Derivable
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/Default_parameters
  title: Default parameters, MDN Web Docs
  locator: Description; Evaluated at call time
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8.7 Function definitions
---

## Idea

A program often needs a value where nobody gave one: a field of a new object, a parameter that the caller left out, a key that is not in a map. A default value fills the gap. In general use, "A default, in computer science, refers to the preexisting value of a user-configurable setting"[^1], and "When programmers or users failed to provide a specific value or setting, the system would revert to a pre-programmed or standard value."[^1]

In Python, a parameter and a map lookup can have a default value:

```python run id=idea
def greet(name="world"):
    return "Hello, " + name


print(greet())
print(greet("Ada"))
settings = {"width": 80}
print(settings.get("height", 24))
```

```output for=idea
Hello, world
Hello, Ada
24
```

## How it works

**Default values of variables.** Some languages put a fixed value into each new variable that has no initializer, see [[initialization-concept]]. The value depends on the type, and it is the value with all bits zero in most cases. Go calls it the zero value: each element "is set to the zero value for its type: false for booleans, 0 for numeric types, "" for strings, and nil for pointers, functions, interfaces, slices, channels, and maps"[^2]. Java gives default values to fields and array elements: "For type int, the default value is zero, that is, 0."[^3] "For type boolean, the default value is false."[^3] "For all reference types (§4.3), the default value is null."[^3] C# has a table of the same kind: "Any reference type null"[^4], and for a struct, "The value produced by setting all value-type fields to their default values and all reference-type fields to null."[^4]

**undefined in JavaScript.** JavaScript has a special value for a variable with no value yet. "A variable that has not been assigned a value is of type undefined."[^5] The same value comes back from a missing property: "Accessing a property that does not exist also returns undefined."[^5]

**A default that the type defines.** In Rust, a variable has no default value, but a type can have one. The trait `Default` is "A trait for giving a type a useful default value."[^6] `#[derive(Default)]` makes one for a struct, and "When derived, it will use the default value for each field’s type."[^6] C++ has default constructors and default member initializers for the same purpose.

**Default parameters.** A function can give a parameter a value for a call that leaves it out. "Default function parameters allow named parameters to be initialized with default values if no value or undefined is passed."[^7] Python, JavaScript, TypeScript, Kotlin, C# and C++ have default parameters. Java, Go and C do not: the program uses overloads, a variadic parameter, or a struct of options.

**Defaults for lookups.** A map lookup for a missing key needs a value too. Python has `dict.get(key, default)` and `collections.defaultdict`, Java has `getOrDefault`, and a Go map gives the zero value of its value type.

## In languages

| Language | Verdict | Default value |
|---|---|---|
| Python | idiom | no default for variables; default parameters, `dict.get`, `defaultdict` and fields of a dataclass |
| JavaScript | native | `undefined` for a variable, a property or a parameter with no value; default parameters and `??` |
| TypeScript | native | the rules of JavaScript, and optional parameters and properties |
| Java | native | 0, `false`, `'\u0000'` or `null` for fields and array elements |
| Kotlin | native | no implicit default; default parameter values and property initializers |
| C# | native | `default` and `default(T)`; 0, `false` or `null` for fields and array elements; default parameters |
| Go | native | the zero value of the type for each variable |
| Rust | library | no implicit default; the trait `Default` and `#[derive(Default)]` |
| C | native | zero for static objects and `calloc`; none for automatic variables |
| C++ | native | value initialization with `{}`, default constructors and default member initializers |
| Bash | idiom | an unset variable is empty; `${name:-default}` gives a default |
| PowerShell | idiom | an unassigned variable is `$null`; default values for parameters |
| SQL | native | the `DEFAULT` clause of a column, else NULL |
| Regex | n/a | a pattern has no variables |
| HTML | n/a | HTML is markup, with no variables |
| CSS | native | each property has an initial value |

## Variations and related ideas

- **Zero values that work.** In Go, a type can make its zero value ready to use: an empty `strings.Builder` or `bytes.Buffer` needs no constructor.
- **null and Option.** A default of `null` means "no value", and a read of a field of it fails. Rust and other languages use an option type, and the program must say what to do when the value is missing.
- **Default arguments evaluated once or at each call.** Python evaluates a default parameter once, when it defines the function. In JavaScript, "The default argument is evaluated at call time."[^7]
- **Configuration defaults.** A program reads settings from a file or the command line, and a default for each missing setting.

## Pitfalls

- **A mutable default parameter in Python.** "Default parameter values are evaluated from left to right when the function definition is executed. This means that the expression is evaluated once, when the function is defined, and that the same “pre-computed” value is used for each call."[^8] So a list as a default is shared by all calls:

```python wrong id=mutable-default
def add_item(item, items=[]):
    items.append(item)
    return items


print(add_item("a"))
print(add_item("b"))
```

```output for=mutable-default
['a']
['a', 'b']
```

Write `items=None` and `if items is None: items = []` in the body.

- **A default that hides a bug.** A zero value looks like real data. A count of 0 from a field that nobody set is wrong, but the program does not see it.
- **0 and false as a missing value.** `value || fallback` in JavaScript replaces 0, `""` and `false` too. Use `value ?? fallback`, which replaces only `null` and `undefined`.
- **null as a default.** A default of `null` defers the error to the first use of the value.

## Cost

A default value for each variable costs a write of zeros, which the program can do in bulk for an array or an object. A default parameter costs the evaluation of its expression on each call that leaves it out, or once in Python.

## History

The word comes from the failure to make a choice: "This "failure" to explicitly define a choice led to that preset value being called the "default"."[^1] Java gave default values to fields and array elements, and Go extended the idea to each variable with the zero value.

## References

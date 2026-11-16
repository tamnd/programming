---
tag: 1VVHH0
kind: concept
slug: operator-overloading
name: Operator overloading
aliases: [overloaded operators, operator functions, operator methods, special methods for operators, ad hoc operator polymorphism]
area: objects-and-classes/standard-object-methods
level: core
summary: Operator overloading lets a type define what operators such as +, == and [] do for its values, so user types such as vectors and money can use the same syntax as numbers.
applies_to: core-languages
viz: optional
history:
  year: 1968
  note: ALGOL 68 let programs define operators and their priority; Ada 83 and C++ allowed new meanings for the existing operators, C# followed, and Java and Go chose to leave the feature out.
see_also: [operator, overload-an-operator, universal-object-methods, implement-equality-for-a-type, natural-ordering]
sources:
  - url: https://en.cppreference.com/w/cpp/language/operators
    title: Operator overloading, cppreference.com
    locator: Restrictions; Binary arithmetic operators
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.8 Emulating numeric types
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/operator-overloading
    title: Operator overloading, C# reference
    locator: Overloadable operators; Non overloadable operators
  - url: https://doc.rust-lang.org/std/ops/index.html
    title: std::ops, Rust standard library
    locator: Description
  - url: https://kotlinlang.org/docs/operator-overloading.html
    title: Operator overloading, Kotlin documentation
    locator: Introduction; Equality and inequality operators
  - url: https://go.dev/doc/faq
    title: Frequently Asked Questions, The Go Programming Language
    locator: Why does Go not support overloading of methods and operators?
  - url: https://docs.oracle.com/javase/specs/jls/se21/html/jls-15.html
    title: "Chapter 15: Expressions, The Java Language Specification, Java SE 21"
    locator: 15.18 Additive Operators
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/toPrimitive
    title: Symbol.toPrimitive, JavaScript reference, MDN
    locator: Description
  - url: https://en.wikipedia.org/wiki/Operator_overloading
    title: Operator overloading, Wikipedia
    locator: Criticisms; ALGOL 68; Ada; C#; Java
---

## Idea

The built-in operators work on built-in types: `+` adds numbers and joins strings, and `==` compares numbers. Operator overloading lets a programmer give an operator a meaning for a new type. A vector type can define `+` as the sum of coordinates, a money type can define `<`, and a matrix type can define `*` and `[]`. Code that uses such types then reads like arithmetic: `a + b * c` in place of `a.add(b.multiply(c))`.

An overloaded operator is a function with a special name or a special mark. The compiler or the interpreter turns `a + b` into a call of that function. The operator keeps its syntax: the precedence and the number of operands do not change. In C++, "It is not possible to change the precedence, grouping, or number of operands of operators."[^1]

## How it works

Each language maps an operator to a function. Python calls special methods: "to evaluate the expression x + y, where x is an instance of a class that has an __add__() method, type(x).__add__(x, y) is called".[^2] When the method cannot handle the other operand, it returns `NotImplemented`, and Python tries the reflected method of the right operand, such as `__rmul__`:

```python run id=add-and-reflected-mul
class Vec:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __add__(self, other):
        if not isinstance(other, Vec):
            return NotImplemented
        return Vec(self.x + other.x, self.y + other.y)

    def __mul__(self, k):
        if not isinstance(k, int):
            return NotImplemented
        return Vec(self.x * k, self.y * k)

    __rmul__ = __mul__

    def __repr__(self):
        return f"Vec({self.x}, {self.y})"


v = Vec(1, 2)
print(v + Vec(3, 4))
print(v * 3)
print(3 * v)
try:
    v + 1
except TypeError as e:
    print("TypeError:", e)
```

```output for=add-and-reflected-mul
Vec(4, 6)
Vec(3, 6)
Vec(3, 6)
TypeError: unsupported operand type(s) for +: 'Vec' and 'int'
```

`3 * v` works because `int.__mul__` returns `NotImplemented` for a `Vec`, and Python then calls `v.__rmul__(3)`. `v + 1` fails, because neither side knows the other.

The other languages that allow overloading use one of three forms:

| Form | Languages | `a + b` becomes |
|---|---|---|
| a special method name | Python, Kotlin | `a.__add__(b)`, `a.plus(b)` |
| a function named with a keyword | C++, C# | `operator+(a, b)` or `a.operator+(b)` |
| a trait or an interface | Rust | `Add::add(a, b)` |

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | special methods such as `__add__`, `__radd__`, `__eq__` and `__getitem__` |
| JavaScript | absent | no user operators; `valueOf` and `Symbol.toPrimitive` convert objects to primitives |
| TypeScript | absent | as JavaScript |
| Java | absent | no user operators; `+` joins strings |
| Kotlin | native | member or extension functions marked `operator`, such as `plus` and `get` |
| C# | native | `public static` methods declared with `operator` |
| Go | absent | no user operators; `==` works on comparable structs |
| Rust | stdlib | traits of `std::ops`, such as `Add`, `Mul`, `Index` and `Neg`; `PartialEq` for `==` |
| C | absent | no user operators |
| C++ | native | functions named `operator+`, `operator==`, `operator[]` and more |
| Bash | n/a | no user types |
| PowerShell | idiom | .NET operators of a type, such as `op_Addition`, work in PowerShell expressions |
| SQL | n/a | no user operators in standard SQL; PostgreSQL has `CREATE OPERATOR` |
| Regex | n/a | no user types |
| HTML | n/a | no operators |
| CSS | n/a | `calc()` has fixed operators |

- **C++** allows most operators. Exceptions are `::`, `.`, `.*` and `?:`, and "New operators such as **, <>, or &| cannot be created."[^1] Binary operators are often free functions: "Binary operators are typically implemented as non-members to maintain symmetry".[^1]
- **C#** declares operators with the `operator` keyword, and some come in pairs: "== and !=, < and >, <= and >=".[^3] `&&` and `||` cannot be overloaded; a type overloads `true`, `false`, `&` and `|` instead.
- **Rust** ties each operator to a trait: "Implementing these traits allows you to overload certain operators."[^4] Only operators with a trait can change: "since the assignment operator (=) has no backing trait, there is no way of overloading its semantics".[^4]
- **Kotlin** maps each operator to a function with a fixed name: "To overload an operator, mark the corresponding function with the operator modifier".[^5] `a == b` always calls `equals`, and `a < b` calls `compareTo`.
- **Go** leaves the feature out on purpose: "Regarding operator overloading, it seems more a convenience than an absolute requirement. Again, things are simpler without it."[^6]
- **Java** gives `+` a meaning only for strings and numbers; any other operand type is "a compile-time error".[^7] Types such as `BigInteger` use methods: `a.add(b)`.
- **JavaScript** has no user operators, but it converts an object to a primitive for `+` and `<`. A class can define `Symbol.toPrimitive`; otherwise the language calls "the valueOf() and toString() methods in different orders".[^8]

## Variations and related ideas

- **New operators.** ALGOL 68, Haskell, Scala and Swift let a program define new operator symbols with their own precedence. C++, C#, Rust, Kotlin and Python allow only the existing symbols.[^9]
- **Equality and comparison.** `==` and `<` are operators, but their rules come from the [equality](/implement-equality-for-a-type/) and [ordering](/natural-ordering/) contracts: equal objects need equal hash codes, and an order must be consistent. Records, data classes and Rust's `#[derive(PartialEq, Ord)]` generate them.
- **Indexing and calling.** `a[i]` and `f(x)` are operators in some languages: Python's `__getitem__` and `__call__`, C++'s `operator[]` and `operator()`, Rust's `Index`, and C#'s indexers.
- **Conversions.** C++ conversion operators and C#'s `implicit` and `explicit` operators let a type change into another type. Implicit conversions can make an overloaded operator apply where the reader does not expect it.
- **Function overloading.** Operator overloading is a special case of overloading by type: the compiler picks the operator function from the types of the operands. See [operator](/operator/) for the built-in operators.

## Pitfalls

- **A surprising meaning.** Wikipedia notes a common criticism: overloading lets a programmer change the usual meaning of an operator, which can surprise the next reader.[^9] C++'s `<<` both shifts bits and writes to a stream. Use an operator only when its meaning for the type is the usual one, such as `+` for a sum.
- **Lost short-circuit evaluation.** The built-in `&&` and `||` skip the right operand when the left one decides the result. In C++, "The overloads of operators && and || lose short-circuit evaluation."[^1] Rust and C# do not let a type overload them directly.[^4][^3]
- **Asymmetric operands.** A C++ member `operator+` accepts `v + 2` but not `2 + v`, and in Python `__add__` alone does not handle `2 + v`. Use a non-member function in C++, and a reflected method such as `__radd__` in Python.
- **Primitive conversions in JavaScript.** A class with `valueOf` makes `+` and `<` work, but the result is a number, not an object of the class, and a template literal calls `toString` in place of `valueOf`:

```javascript run id=valueof-is-not-overloading
class Money {
  constructor(cents) {
    this.cents = cents;
  }
  valueOf() {
    return this.cents;
  }
}
const a = new Money(150);
const b = new Money(250);
console.log(a + b);
console.log(a < b);
console.log(`${a}`);
```

```output for=valueof-is-not-overloading
400
true
[object Object]
```

- **`==` that is not an equivalence.** An overloaded `==` must be reflexive, symmetric and transitive, and it must agree with the hash. An `==` that compares with a tolerance, or that accepts other types, breaks hash tables and sets.

## Cost

An overloaded operator is a function call, so it costs the same as the method that it replaces. In C++, Rust and C# the compiler can inline small operator functions, so `a + b` on a small struct costs as much as the code for the fields. An operator that returns a new object on each step, such as `+` on large matrices or strings in a loop, allocates a temporary each time; compound operators such as `+=` that change an operand in place avoid it.

## History

ALGOL 68 allowed operator overloading, and programs could also define new operators and set their priority.[^9] Ada had overloading of the existing operators from Ada 83, but not new operators. C++ took the idea from ALGOL 68 with changes, and C# added it in 2001.[^9] The designers of Java left it out, and Go also chose not to have it.[^9][^6] Python, Kotlin and Rust later tied operators to methods or traits with fixed names.

## References

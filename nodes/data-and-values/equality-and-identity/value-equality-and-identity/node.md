---
tag: EYF876
kind: concept
slug: value-equality-and-identity
name: Equality and identity
aliases: [value equality, reference equality, object identity, structural equality, sameness]
area: data-and-values/equality-and-identity
level: core
summary: >-
  Two values are equal when they hold the same data. Two references are identical when they point to the same object. Each language decides which of the two tests its operators make.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John McCarthy]
  note: The first LISP paper defined eq, which compares two atoms, and equal, which compares the structure of two symbolic expressions.
see_also: [pointers-and-references, immutability, floating-point-arithmetic, null-references, sum-types, interfaces, type-conversion]
same_as:
  wikidata: Q5988266
  wikipedia: Identity_(object-oriented_programming)
sources:
  - url: https://docs.python.org/3.14/reference/expressions.html#is-not
    title: "The Python Language Reference, Expressions, Python 3.14"
    locator: 6.10.3 Identity comparisons
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Comparison operators
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Record.html#equals(java.lang.Object)
    title: "Java SE 25 API, java.lang.Record"
    locator: equals(Object)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/equality-operators
    title: "C# language reference, Equality operators"
    locator: "Value types equality, Record types equality, String equality"
  - url: https://docs.python.org/3.14/library/dataclasses.html#dataclasses.dataclass
    title: "dataclasses, Python 3.14"
    locator: "@dataclasses.dataclass, the eq and frozen parameters"
  - url: https://kotlinlang.org/docs/equality.html
    title: "Kotlin documentation, Equality"
    locator: "Structural equality, Referential equality, Floating-point numbers equality"
  - url: https://doc.rust-lang.org/std/cmp/trait.PartialEq.html
    title: "Rust standard library, Trait PartialEq"
    locator: description
  - url: https://docs.python.org/3.14/reference/expressions.html#value-comparisons
    title: "The Python Language Reference, Expressions, Python 3.14"
    locator: 6.10.1 Value comparisons
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html
    title: "Java SE 25 API, java.lang.Object"
    locator: equals(Object) and hashCode()
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/statements-expressions-operators/equality-comparisons
    title: "C# programming guide, Equality comparisons"
    locator: "value equality and reference equality, Object.ReferenceEquals"
  - url: https://doc.rust-lang.org/std/cmp/trait.Eq.html
    title: "Rust standard library, Trait Eq"
    locator: description
  - url: https://docs.python.org/3.14/reference/datamodel.html#object.__hash__
    title: "The Python Language Reference, Data model, Python 3.14"
    locator: "3.3.1 Basic customization, object.__hash__"
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Equality_comparisons_and_sameness
    title: "Equality comparisons and sameness, MDN"
    locator: "Loose equality, strict equality, same-value-zero equality, same-value equality"
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Double.html#fpNumericalEq
    title: "Java SE 25 API, java.lang.Double"
    locator: "Floating-point Equality, Equivalence, and Comparison"
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.double.equals
    title: "Double.Equals Method, .NET API reference"
    locator: Remarks
  - url: https://docs.python.org/3.14/reference/expressions.html#membership-test-operations
    title: "The Python Language Reference, Expressions, Python 3.14"
    locator: 6.10.2 Membership test operations
  - url: https://eel.is/c++draft/class.eq
    title: "Working Draft, Programming Languages: C++"
    locator: "[class.eq] Equality operator"
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.9
    title: "N1570, Committee Draft of ISO/IEC 9899:2011 (C11)"
    locator: "6.5.9 Equality operators, paragraph 2"
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.2.6.1p6
    title: "N1570, Committee Draft of ISO/IEC 9899:2011 (C11)"
    locator: "6.2.6.1 General, paragraph 6"
  - url: https://doi.org/10.1145/367177.367199
    title: "J. McCarthy, Recursive functions of symbolic expressions and their computation by machine, Part I, Communications of the ACM 3(4), 184-195, 1960"
    locator: "section 3, the elementary functions eq and equal"
  - url: https://docs.python.org/3/whatsnew/3.8.html#changes-in-python-behavior
    title: "What's New In Python 3.8"
    locator: Changes in Python behavior
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.21.3
    title: "The Java Language Specification, Java SE 25 Edition"
    locator: "15.21.3 Reference Equality Operators == and !="
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.7
    title: "The Java Language Specification, Java SE 25 Edition"
    locator: 5.1.7 Boxing Conversion
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Integer.html#valueOf(int)
    title: "Java SE 25 API, java.lang.Integer"
    locator: valueOf(int)
  - url: https://doc.rust-lang.org/std/ptr/fn.eq.html
    title: "Rust standard library, Function std::ptr::eq"
    locator: description
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_comparison_operators
    title: "about_Comparison_Operators, PowerShell documentation"
    locator: Equality
  - url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
    title: Bash Reference Manual
    locator: "3.2.5.2 Conditional Constructs, [[...]]"
  - url: https://www.sqlite.org/lang_expr.html#isisnot
    title: "SQLite, SQL Language Expressions"
    locator: The IS and IS NOT operators
  - url: https://www.postgresql.org/docs/18/functions-comparison.html
    title: PostgreSQL 18 Documentation
    locator: 9.2 Comparison Functions and Operators
  - url: https://www.sqlite.org/lang_select.html
    title: "SQLite, The SELECT statement"
    locator: "Generation of the set of result rows, and Removal of duplicate rows"
  - url: https://doi.org/10.1145/165593.165596
    title: "H. G. Baker, Equal rights for functional objects or, the more things change, the more they are the same, ACM SIGPLAN OOPS Messenger 4(4), 2-27, 1993"
    locator: section 2 and section 3
---

## Idea

A program often asks if two values are the same. The question has two meanings. Two values can be equal: they hold the same data, so the program can use one in place of the other. Or two references can be identical: they point to the same object in memory, so a change through one is visible through the other. Two copies of the point (1, 2) are equal and not identical. A second variable that holds the same reference as the first is identical to it, and so also equal.

Each language gives one operator, or two, and decides which question each one asks. The decision is different from language to language, and sometimes from type to type in one language. This is the source of many bugs: a test that compares identity where the programmer meant equality, or the reverse.

```python run id=idea
from dataclasses import dataclass


@dataclass(frozen=True)
class Point:
    x: int
    y: int


a = Point(1, 2)
b = Point(1, 2)
c = a
print(a == b, a is b)
print(a == c, a is c)
print(len({a, b, c}))
```

```output for=idea
True False
True True
1
```

In Python, `==` asks if the values are equal and `is` asks if the two names refer to the same object.[^1] `a` and `b` are two objects with the same fields, so they are equal and not identical. `c` is a second name for the object `a`, so it is both. A set keeps one copy of equal values, so the three names give a set of one element.

## How it works

### Identity

Identity is a property of objects that live at an address. Two references are identical when they refer to the same object. A language can test this in one step, because it compares two addresses. The test does not depend on the type and needs no code from the programmer.

Identity matters for mutable objects. If `a` and `c` are identical, a change made through `a` is visible through `c`. If `a` and `b` are only equal, a change through `a` makes them different. [[pointers-and-references]] shows aliasing in detail.

Some values have no identity that a program can see. In C, an `int` is a value in a variable, and two variables that hold `3` are two variables. The question "is this the same 3?" has no meaning. Many languages make small immutable values work like this, even when the run time stores them as objects.

### Equality

Equality is a question about the data. Its meaning depends on the type, so somebody must define it. There are three common sources of a definition:

- The language. Numbers, strings in most languages, and structs in Go have a built-in equality.[^2]
- The compiler, from the fields. A Java record, a C# record, a Python data class, a Kotlin data class and a Rust type with `#[derive(PartialEq)]` compare their fields in order.[^3][^4][^5][^6][^7]
- The programmer, in a method such as `equals`, `Equals`, `__eq__` or `operator==`.

When a class defines nothing, most object-oriented languages use identity as the default equality. Python's default `__eq__` compares identity, and so do `Object.equals` in Java and `Object.Equals` for a C# class.[^8][^9][^10] So two objects of a plain class with equal fields are not equal until the class says how to compare them.

### The rules of an equality

An equality method must behave like equality in mathematics. The Java documentation of `Object.equals` gives the rules:[^9]

| Rule | Meaning |
|---|---|
| Reflexive | `x.equals(x)` is true. |
| Symmetric | `x.equals(y)` is true exactly when `y.equals(x)` is true. |
| Transitive | If `x.equals(y)` and `y.equals(z)` are true, `x.equals(z)` is true. |
| Consistent | Many calls give the same result if no data used in the comparison changes. |
| Not null | `x.equals(null)` is false. |

Collections depend on these rules. A list search, a set and a map key all call the equality of the elements. A method that breaks a rule makes a collection give answers that look random. A common break of symmetry is an `equals` in a subclass that compares more fields than the `equals` of its superclass. Then a base object is equal to a derived object, but the derived object is not equal to the base object.

Rust puts the rules in the type system. `PartialEq` is the trait for `==`, and it requires symmetry and transitivity. `Eq` adds reflexivity.[^7][^11] Floating point types implement `PartialEq` and not `Eq`, because `NaN != NaN`.[^7]

### Equality and hashing

A hash table finds a key in two steps. It computes the hash code of the key and goes to the bucket for that code. Then it compares the key with each entry in the bucket by equality. This only works if equal values have equal hash codes. Python and Java state this rule in their documentation.[^12][^9]

So equality and hashing are one design decision. A class that redefines equality must redefine the hash too, from the same fields. Python enforces part of this: a class that defines `__eq__` and not `__hash__` gets `__hash__` set to `None`, so its objects cannot be set elements or dictionary keys.[^12] Java does not enforce it, and a class that overrides only `equals` gives a `HashSet` that keeps duplicates. The pitfalls below show both.

A second rule follows. The hash code of a key must not change while the key is in a table. If a program changes a field that equality uses, the table looks in the wrong bucket. Immutable keys avoid the problem. [[immutability]] has more.

### Four kinds of sameness

Real languages have more than two tests. JavaScript has four algorithms, and the differences are in the details of numbers:[^13]

| Test | Converts types | `NaN` and `NaN` | `0` and `-0` | Used by |
|---|---|---|---|---|
| `==` | yes | not equal | equal | the loose equality operator |
| `===` | no | not equal | equal | `===`, `indexOf`, `switch` |
| SameValueZero | no | equal | equal | `includes`, `Set`, `Map` |
| `Object.is` | no | equal | not equal | `Object.is` |

Floating point is the reason for the split. IEEE 754 says that `NaN` is not equal to any value, itself included, and that `0` equals `-0`.[^14] [[floating-point-arithmetic]] explains why. This numerical equality is not reflexive, so it cannot serve as the equality of a set. Java solves this with two definitions. The `==` operator on `double` follows IEEE 754. `Double.equals` uses representation equivalence: it treats `NaN` as equal to `NaN` and `0.0` as not equal to `-0.0`, so a `Double` can be a `HashSet` element or a `HashMap` key.[^14] .NET makes the same choice for `Double.Equals`.[^15] Kotlin uses IEEE 754 when the operands are statically known to be floating point numbers, and the `equals` rule otherwise.[^6]

Python takes a third way. A list, a set and a dictionary test `x is e or x == e` for each element, so a container finds the same `NaN` object that it holds, although `NaN == NaN` is false.[^16]

### Equality of composite values

For a composite value, equality is usually defined from the parts: two structs are equal when their fields are equal, and two lists are equal when they have the same length and equal elements in the same order. Go defines `==` this way for structs and arrays, and it compares the fields in source order and stops at the first difference.[^2] C++ does the same for a defaulted `operator==`.[^17]

Not every type has a useful structural equality. Go does not allow `==` on slices, maps and functions, except a comparison with `nil`.[^2] The standard library has `slices.Equal` and `maps.Equal` for contents. C does not allow `==` on structs at all. The operands of `==` must be numbers or pointers.[^18] A program must compare the fields one by one, and `memcmp` is not a correct shortcut, because the padding bytes of a struct have unspecified values.[^19]

Deep equality also has a cost. It visits each part, so the time grows with the size of the value. Identity takes one comparison. Some programs keep one copy of each distinct value, and then compare identity in place of equality. This is called interning or hash consing. The atoms of LISP work this way: two atoms with the same name are the same symbol, so `eq` can compare them.[^20]

## In languages

| Language | Value equality | Identity | Verdict |
|---|---|---|---|
| Python | `==`, through `__eq__` | `is` | native |
| JavaScript | a function that the program writes | `===` on objects | idiom |
| TypeScript | as JavaScript | `===` on objects | idiom |
| Java | `equals` | `==` on references | native |
| C# | `==` and `Equals` on records, `string` and structs | `ReferenceEquals`, and `==` on plain classes | native |
| Go | `==` on comparable types | `==` on pointers | native |
| Rust | `==`, through `PartialEq` | `std::ptr::eq` | native |
| C | a function that the program writes | `==` on pointers | idiom |
| C++ | `operator==`, which can be defaulted | `==` on addresses | native |
| Kotlin | `==`, through `equals` | `===` | native |
| PowerShell | `-eq` | `[object]::ReferenceEquals` | native |
| Bash | `==` and `-eq` inside `[[ ]]` | none | idiom |
| SQL | `=`, and `IS NOT DISTINCT FROM` for `NULL` | none | native |

**Python.** `==` calls `__eq__`, and `is` compares identity.[^8][^1] PEP 8 says to compare with `None` by `is`, because `None` is a single object.[^8] Python shares some small integers and some strings in CPython, so `is` on numbers and strings sometimes works by accident. Since Python 3.8 the compiler gives a `SyntaxWarning` for `is` with a literal.[^21]

**Java.** On references, `==` is identity, also for `String`.[^22] `equals` is value equality when the class overrides it. A record overrides `equals` and `hashCode` from its components.[^3] Boxing converts an `int` to an `Integer` object. The language guarantees that boxing gives the same object only for small values, from -128 to 127, so `==` on boxed numbers works in tests with small numbers and fails later with large ones.[^23][^24]

**C#.** A plain class compares references with both `==` and `Equals`. A record compares its fields. A struct compares its fields through `Equals`, but it has no `==` until it declares one.[^10][^4] `string` overloads `==` with an ordinal, case-sensitive comparison.[^4] `Object.ReferenceEquals` always compares references.[^10] The compiler selects `==` by the static types, so two boxed `int` values typed as `object` compare by reference.

**Kotlin.** `==` calls `equals` and handles `null`: `a == b` means `a?.equals(b) ?: (b === null)`. `===` compares references. A data class gets `equals` from its properties.[^6] There is no Kotlin example file, because `kotlinc` is not installed on the machine that recorded the examples.

**JavaScript and TypeScript.** `===` compares primitives by value and objects by identity, and a class cannot change this.[^13] Value equality of objects is a function, often a comparison of the fields or of a key string. `==` converts the types of its operands first, and its rules surprise most readers.

**Go.** `==` compares values for all comparable types. A struct is comparable if its fields are, and two pointers are equal when they point to the same variable.[^2] So a struct value compares by value, a pointer compares by identity, and a comparable struct can be a map key with no extra code. Interface values compare their dynamic types and values, and a comparison panics at run time if the dynamic type is not comparable.[^2]

**Rust.** `==` calls `PartialEq::eq`. `#[derive(PartialEq, Eq, Hash)]` gives field-by-field equality and a matching hash. `std::ptr::eq` compares two pointers by address.[^25] A type has no `==` until it implements `PartialEq`.

**C and C++.** In C, `==` works only on numbers and pointers.[^18] Two pointers to the same object are equal, so pointer comparison is identity. Strings are arrays of `char`, and `strcmp` compares them. In C++, a class can declare `operator==` as defaulted, and the compiler compares the subobjects in order.[^17] `std::string` compares characters, but two `const char*` operands still compare addresses.

**PowerShell.** `-eq` compares values, and string comparisons are case-insensitive unless the script uses `-ceq`.[^26] There is no PowerShell example file, because `pwsh` is not installed on the machine that recorded the examples.

**Bash.** All values are strings. Inside `[[ ]]`, `==` matches the left side against a pattern on the right side, and a quoted right side matches as text.[^27] `-eq` compares integers.

**SQL.** `=` compares values, but a comparison with `NULL` gives `NULL`, not true or false. `IS NOT DISTINCT FROM` treats two `NULL` values as equal. SQLite also accepts the short form `IS`.[^28][^29] `DISTINCT` and `GROUP BY` treat `NULL` values as equal to each other.[^30] A row has no identity apart from its key.

## Variations and related ideas

- Structural and nominal equality. Some languages compare only values of the same class. A Python data class compares only with an instance of the identical type.[^5] Others compare any two values with the same shape.
- Ordering and equality. A sorted set in C++ or Java uses an order, not `equals`. Two elements are the same when neither is less than the other. If the order and `equals` disagree, a sorted set and a hash set can hold different elements for the same input. The Java `Double` documentation notes that its `compareTo` is consistent with `equals`.[^14]
- Equality of functions. Two functions are equal if they give the same result for every input. A program cannot decide this in general, so languages compare functions by identity or not at all. Go does not allow `==` on functions.[^2]
- Equality of errors and enums. [[sum-types]] compare by their case and their fields. [[errors-as-values]] are often compared with a sentinel value, which is an identity test.
- Equality up to case, Unicode form or locale. Two strings with different bytes can be the same text for a reader. [[utf-8]] covers normalization.
- Approximate equality. Floating point code often compares with a tolerance, `abs(a - b) <= eps`. This test is not transitive, so it cannot be the equality of a set.

## Pitfalls

### Identity where equality was meant

The most common mistake is `==` on references in a language where `==` means identity.

```java wrong id=string-identity
public class Main {
    public static void main(String[] args) {
        String s = "hello";
        String t = new StringBuilder("hel").append("lo").toString();
        System.out.println(s == t);
        System.out.println(s.equals(t));
    }
}
```

```output for=string-identity
false
true
```

`t` is a new `String` with the same characters, so `==` is false. The JLS says that `==` on two `String` operands tests if they refer to the same object, and that `equals` compares the contents.[^22] String literals in one program are often the same object, so the bug can hide in tests that use only literals and appear with text from input. C has the same bug with `==` on two `char` arrays.

### Boxed values

Boxing turns a primitive value into an object. Then `==` compares references.

```java wrong id=boxed
public class Main {
    public static void main(String[] args) {
        Integer small1 = Integer.valueOf(127);
        Integer small2 = Integer.valueOf(127);
        Integer large1 = Integer.valueOf(128);
        Integer large2 = Integer.valueOf(128);
        System.out.println(small1 == small2);
        System.out.println(large1 == large2);
        System.out.println(large1.equals(large2));
    }
}
```

```output for=boxed
true
false
true
```

`Integer.valueOf` always caches the values from -128 to 127.[^24] The two `127` objects are one object, and the two `128` objects are two objects on this JVM. The language does not fix the result outside the cache range.[^23] So a test with small numbers passes and the program fails with larger ones. Compare boxed values with `equals`, or unbox them. C# has the same trap when two numbers are typed as `object`.

### Equal values with different hash codes

```java wrong id=no-hashcode
import java.util.HashSet;
import java.util.Set;

public class Main {
    static final class Point {
        final int x;
        final int y;

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public boolean equals(Object o) {
            return o instanceof Point p && p.x == x && p.y == y;
        }
    }

    public static void main(String[] args) {
        Set<Point> seen = new HashSet<>();
        seen.add(new Point(1, 2));
        seen.add(new Point(1, 2));
        System.out.println(new Point(1, 2).equals(new Point(1, 2)));
        System.out.println(seen.size());
        System.out.println(seen.contains(new Point(1, 2)));
    }
}
```

```output for=no-hashcode
true
2
false
```

The class overrides `equals` and not `hashCode`. As far as is reasonably practical, `Object.hashCode` returns distinct integers for distinct objects.[^9] So two equal points go to different buckets, and the set never compares them. The fix is a `hashCode` from the same fields, such as `Objects.hash(x, y)`, or a record. Python stops this mistake at the first use:

```python fails expect=exception:TypeError id=eq-without-hash
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return isinstance(other, Point) and (self.x, self.y) == (other.x, other.y)


seen = {Point(1, 2)}
print(len(seen))
```

The class defines `__eq__` and no `__hash__`, so Python sets `__hash__` to `None`, and the set raises `TypeError: unhashable type`.[^12]

### A key that changes

```java wrong id=mutated-key
import java.util.HashSet;
import java.util.Set;

public class Main {
    static final class Point {
        int x;
        int y;

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public boolean equals(Object o) {
            return o instanceof Point p && p.x == x && p.y == y;
        }

        @Override
        public int hashCode() {
            return 31 * x + y;
        }
    }

    public static void main(String[] args) {
        Set<Point> seen = new HashSet<>();
        Point p = new Point(1, 2);
        seen.add(p);
        p.x = 5;
        System.out.println(seen.contains(p));
        System.out.println(seen.size());
        for (Point q : seen) {
            System.out.println(q == p);
        }
    }
}
```

```output for=mutated-key
false
1
true
```

The set holds the object `p`, but `contains(p)` is false. The set stored `p` in the bucket for the hash code of `(1, 2)`, and now looks in the bucket for `(5, 2)`. Do not change a field that equality uses while the object is a key. Use immutable keys.

### Loose equality

```javascript wrong id=loose
console.log(0 == "");
console.log("0" == false);
console.log(null == undefined);
console.log(null == 0);
console.log([] == false);
console.log("1,2" == [1, 2]);
```

```output for=loose
true
true
true
false
true
true
```

`==` converts its operands before the comparison, and the rules differ for each pair of types.[^13] An empty string, the string `"0"`, `false` and an empty array are all loosely equal to `0` or to `false`. Use `===`.

### Objects as map keys in JavaScript

```javascript wrong id=object-key
const visits = new Map();
visits.set([1, 2], "first");
console.log(visits.get([1, 2]));
console.log(visits.size);
```

```output for=object-key
undefined
1
```

A `Map` compares keys with SameValueZero, which is identity for objects.[^13] The second `[1, 2]` is a new array, so it is a different key. Use a string key, such as `"1,2"`, or keep the key object.

### NULL in SQL

```sql wrong id=null-equals
CREATE TABLE users (name TEXT, email TEXT);
INSERT INTO users VALUES ('ann', 'ann@example.com'), ('bob', NULL);

SELECT count(*) FROM users WHERE email = NULL;
SELECT count(*) FROM users WHERE email IS NULL;
```

```output for=null-equals
0
1
```

`email = NULL` is `NULL` for every row, so the `WHERE` clause keeps no row. The PostgreSQL documentation warns not to write `expression = NULL`, because `NULL` is not equal to `NULL`.[^29] Use `IS NULL`, or `IS NOT DISTINCT FROM` to compare two columns that can be `NULL`.

### A comparison that fails at run time

```go fails expect=exit:2 id=uncomparable
package main

import "fmt"

func main() {
	var x, y any = []int{1}, []int{1}
	fmt.Println(x == y)
}
```

Interface values are comparable, so the program compiles. Both dynamic types are `[]int`, which is not comparable, so the comparison panics at run time.[^2] The same panic can come from a struct with an interface field or from a map key of interface type. Use `reflect.DeepEqual` or a comparison that knows the types.

### A pattern in place of a string

```bash wrong id=pattern
expected='*'
actual="anything"
if [[ $actual == $expected ]]; then
  echo "match"
else
  echo "no match"
fi
```

```output for=pattern
match
```

In `[[ ]]`, the right side of `==` is a pattern unless it is quoted.[^27] `*` matches any string. Quote the right side: `[[ $actual == "$expected" ]]`.

## Cost

An identity test compares two addresses. It takes constant time and does not touch the objects.

A value equality visits the parts of the values. For a string or a list, the time grows with the length of the shorter value, and the test stops at the first difference. For a tree of objects, it visits each node. A cycle in the objects makes a naive recursive equality loop forever. Python's built-in containers assume that identical objects are equal, so they skip the equality test for an element that is the same object.[^8]

A hash table needs one hash computation and, on average, a small number of equality tests for each lookup. A slow `hashCode` or `__hash__` makes every lookup slow. A hash function that gives many equal codes puts many keys in one bucket, and then each lookup does many equality tests.

Interning trades memory and a table lookup at creation time for constant-time equality later. It works best for values that are created once and compared many times, such as identifiers in a compiler.

## History

McCarthy's 1960 paper on LISP defined two predicates. `eq` is defined only for atoms, and is true when the two atoms are the same symbol. `equal` is true when two symbolic expressions have the same structure, and McCarthy defines it by recursion over `car` and `cdr` with `eq` at the atoms.[^20] In later Lisps, `EQ` is the identity test for any object and `EQUAL` is the structural test.[^31]

In 1993, Henry Baker argued that neither `EQ` nor `EQUAL` is wrong. `EQ` is correct for mutable objects and `EQUAL` is correct for immutable ones, and the mistake of Lisp was to not separate the two kinds of objects. He proposed one predicate, `EGAL`, for both.[^31] Languages with value types and records follow this idea: a mutable object compares by identity, and an immutable value compares by its contents.

Kotlin names the two tests with two operators: `==` for equality and `===` for identity.[^6] Java records, C# records and Python data classes write the equality and the hash from the fields, so the two stay consistent.[^3][^4][^5] Python 3.8 started to warn about `is` with a literal.[^21]

## References

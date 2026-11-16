---
tag: N257JN
kind: concept
slug: universal-object-methods
name: Universal object methods
aliases: [standard object methods, object protocol, equals hashCode toString, java.lang.Object methods, System.Object methods, dunder methods, magic methods, special methods, derivable traits, Any methods]
area: objects-and-classes/standard-object-methods
level: core
summary: >-
  Universal object methods are the small set of methods, such as equality, hash code and string form, that a language gives every object and that its libraries call, with defaults that a type overrides.
applies_to: core-languages
viz: optional
history:
  year: 1996
  note: JDK 1.0 shipped java.lang.Object with equals, hashCode and toString; Python 2.1 added the rich comparison methods, and Rust and C++20 later generated these methods from the fields.
see_also: [classes-and-instances, value-equality-and-identity, hash-codes, method-concept, record-type, comparator, immutability, class-inheritance]
sources:
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Object (Java SE 21 & JDK 21), API documentation
    locator: Class Object; equals; hashCode; toString
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.1 Basic customization
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.equals
    title: Object.Equals Method, .NET API reference
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.tostring
    title: Object.ToString Method, .NET API reference
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.gethashcode
    title: Object.GetHashCode Method, .NET API reference
    locator: Remarks
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin/-any/
    title: Any, Kotlin standard library API
    locator: Any
  - url: https://doc.rust-lang.org/book/appendix-03-derivable-traits.html
    title: Appendix C, Derivable Traits, The Rust Programming Language
    locator: whole appendix
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Comparison operators
  - url: https://pkg.go.dev/fmt
    title: fmt package, Go standard library
    locator: type Stringer
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Equality_comparisons_and_sameness
    title: Equality comparisons and sameness, MDN
    locator: Introduction
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/toString
    title: Object.prototype.toString(), JavaScript reference, MDN
    locator: Description
  - url: https://en.cppreference.com/w/cpp/language/default_comparisons
    title: Default comparisons, cppreference.com
    locator: Introduction
  - url: https://en.wikipedia.org/wiki/Java_version_history
    title: Java version history, Wikipedia
    locator: Release table
  - url: https://peps.python.org/pep-0207/
    title: PEP 207, Rich Comparisons
    locator: Header; Abstract
---

## Idea

A program compares objects, puts them in hash tables, sorts them and prints them. The code of a hash map or of `print` does not know the types that it will get, so the language agrees on a few method names that every object has. The library calls these methods, and each type decides what they mean.

In Java the agreement is the class `Object`: "Class Object is the root of the class hierarchy. Every class has Object as a superclass."[^1] Its methods include `equals`, `hashCode` and `toString`. Python gives every class `__eq__`, `__hash__`, `__repr__` and `__str__` from `object`,[^2] C# gives `Equals`, `GetHashCode` and `ToString` from `System.Object`, and Kotlin gives `equals`, `hashCode` and `toString` from `Any`. Rust and Go have no root class, but they have the same roles as traits and interfaces that a type implements.

The default versions use the identity of the object: two objects are equal only if they are the same object, and the string form shows the type and an address or a hash. A type that holds a value, such as a point, an amount of money or a date, overrides them so that two objects with the same fields are equal, hash the same and print their fields.

## How it works

The usual roles are:

| Role | Java | Python | C# | Rust | Go |
|---|---|---|---|---|---|
| equality | `equals` | `__eq__` | `Equals`, `==` | `PartialEq`, `Eq` | `==` (built in) |
| hash code | `hashCode` | `__hash__` | `GetHashCode` | `Hash` | none; map keys use `==` |
| string form | `toString` | `__str__`, `__repr__` | `ToString` | `Display`, `Debug` | `String()` (`fmt.Stringer`) |
| ordering | `Comparable.compareTo` | `__lt__` and the others | `IComparable<T>.CompareTo` | `PartialOrd`, `Ord` | `cmp.Compare`, or a function |
| copy | `clone` | `copy.copy` | `MemberwiseClone` | `Clone` | assignment copies a struct |

1. **The default equality is identity.** Python says that with the default methods "all objects compare unequal (except with themselves)".[^2] In C#, for a reference type, "the Equals(Object) method tests for reference equality".[^3] Java's default `equals` and `hashCode` work in the same way.
2. **The default string form names the type.** Java's default `toString` returns "the name of the class of which the object is an instance, the at-sign character `@', and the unsigned hexadecimal representation of the hash code of the object."[^1] C#: "Default implementations of the Object.ToString method return the fully qualified name of the object's type."[^4]
3. **A type overrides the methods together.** Equality and the hash code are one contract: "If two objects are equal according to the equals method, then calling the hashCode method on each of the two objects must produce the same integer result."[^1] C# has the same rule,[^5] and Python says that "The only required property is that objects which compare equal have the same hash value".[^2] See [the equality and hashing contract](/equality-and-hashing-contract/).
4. **Libraries call the methods.** A `HashMap` calls `hashCode` to find a bucket and `equals` to find the key in it, `print` calls the string form, and a sort calls the ordering. A type that does not override them works, but with identity semantics.

The Python version of a point shows the difference. `Plain` keeps the defaults of `object`, and `Point` overrides equality, the hash and the string form:

```python run id=defaults-and-overrides
class Plain:
    def __init__(self, x, y):
        self.x, self.y = x, y


class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __eq__(self, other):
        if not isinstance(other, Point):
            return NotImplemented
        return (self.x, self.y) == (other.x, other.y)

    def __hash__(self):
        return hash((self.x, self.y))

    def __repr__(self):
        return f"Point({self.x}, {self.y})"


a, b = Plain(1, 2), Plain(1, 2)
print(a == b, a == a, repr(a).startswith("<__main__.Plain object at"))

p, q = Point(1, 2), Point(1, 2)
print(p == q, p is q, len({p, q}))
print(p, [p])
```

```output for=defaults-and-overrides
False True True
True False 1
Point(1, 2) [Point(1, 2)]
```

`p` and `q` are two objects (`p is q` is false), but they are equal, and the set keeps only one of them. `print` uses `__repr__` because the class has no `__str__`.

In Java, a record gets the three methods from its fields, while a plain class keeps the methods of `Object`:

```java run id=class-and-record
import java.util.HashSet;
import java.util.List;

public class Main {
    static final class Plain {
        final int x, y;

        Plain(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }

    record Point(int x, int y) {}

    public static void main(String[] args) {
        Plain a = new Plain(1, 2), b = new Plain(1, 2);
        System.out.println(a.equals(b) + " " + a.toString().startsWith("Main$Plain@"));

        Point p = new Point(1, 2), q = new Point(1, 2);
        System.out.println(p.equals(q) + " " + (p == q) + " " + new HashSet<>(List.of(p)).contains(q));
        System.out.println(p);
    }
}
```

```output for=class-and-record
false true
true false true
Point[x=1, y=2]
```

In Java, `==` on two objects is always identity, so `p == q` is false even for the record. Value equality needs `equals`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `object` gives `__eq__`, `__ne__`, `__hash__`, `__repr__`, `__str__` and `__format__`; the ordering methods return `NotImplemented` |
| JavaScript | absent | `Object.prototype` gives `toString` and `valueOf`; `===` on objects is identity, and no hook changes it |
| TypeScript | absent | as JavaScript |
| Java | native | `java.lang.Object`: `equals`, `hashCode`, `toString`, `clone`, `getClass` |
| Kotlin | native | `Any`: `equals`, `hashCode`, `toString`; `==` calls `equals` |
| C# | native | `System.Object`: `Equals`, `GetHashCode`, `ToString`, `MemberwiseClone`, `GetType` |
| Go | idiom | no root type; `==` compares comparable structs field by field; `fmt.Stringer` for the string form |
| Rust | stdlib | traits `PartialEq`, `Eq`, `Hash`, `PartialOrd`, `Ord`, `Debug`, `Display`, `Clone`, most of them derivable |
| C | absent | no methods; a library takes function pointers, such as the comparison function of `qsort` |
| C++ | idiom | no root class; `operator==` and `operator<=>`, which a class can default, `operator<<` on streams, a `std::hash` specialization |
| Bash | n/a | no objects |
| PowerShell | native | objects are .NET objects, with the members of `System.Object` |
| SQL | n/a | rows and values compare by value; there are no user methods on rows |
| Regex | n/a | no objects |
| HTML | n/a | no objects |
| CSS | n/a | no objects |

- **Java, C# and Kotlin** put the methods on the root class, so every object has them, and a class overrides them with `@Override`, `override` or `override fun`. Kotlin's `Any` is "The root of the Kotlin class hierarchy."[^6]
- **Python** calls the methods through operators and built-in functions: `x==y calls x.__eq__(y)`, `hash()` calls `__hash__`, and `repr()` calls `__repr__`, which computes "the “official” string representation of an object".[^2]
- **Rust** uses one trait for each role. The derive attribute writes most of them from the fields, but not `Display`: "An example of a trait that can’t be derived is Display, which handles formatting for end users."[^7]
- **Go** has no hook for equality or hashing. The spec says: "Struct types are comparable if all their field types are comparable."[^8] For the string form, a `String` method makes a type a `fmt.Stringer`, and "The String method is used to print values passed as an operand to any format that accepts a string or to an unformatted printer such as Print."[^9]
- **JavaScript** compares objects by identity, and MDN notes that "JavaScript does not provide a general deep comparison operator".[^10] The string form is `toString`: "JavaScript automatically invokes it when encountering an object where a primitive value is expected."[^11]
- **C++** has no root class, but "Comparison operator functions can be explicitly defaulted to request the compiler to generate the corresponding default comparison for a class."[^12]

## Variations and related ideas

- **Generated methods.** Records in Java and C#, data classes in Kotlin, `@dataclass` in Python and `#[derive]` in Rust write equality, hashing and the string form from the fields. Most value types should use them and not code by hand.
- **Display form and debug form.** Python has `__str__` and `__repr__`, and Rust has `Display` and `Debug`: one form is for users, the other for developers. Java and C# have one `toString`; see [string representation of an object](/string-representation/).
- **Ordering.** Ordering is not on `Object` in Java or C#; a type opts in with `Comparable` or `IComparable<T>`. Most types have equality, but only some have a natural order. See [natural ordering](/natural-ordering/) and [comparator](/comparator/).
- **Identity equality.** Each language also keeps an identity test: `is` in Python, `==` on references in Java, `ReferenceEquals` in C#, `===` in JavaScript. See [value equality and identity](/value-equality-and-identity/).
- **Methods for the run-time.** Java's `Object` also has `getClass`, `wait` and `notify`, and C# has `GetType`. They are universal, but a type does not override them.

## Pitfalls

- **Equality without a hash.** A Java class that overrides `equals` and not `hashCode` breaks `HashMap` and `HashSet`: two equal keys go to different buckets. Python protects the programmer here: "A class that overrides __eq__() and does not define __hash__() will have its __hash__() implicitly set to None."[^2] The objects then cannot go in a set.
- **A hash of mutable fields.** If a field that the hash uses changes after the object goes into a hash set, the set looks in the wrong bucket and cannot find the object. Python says that for mutable objects a class "should not implement __hash__()".[^2]
- **The wrong parameter type.** In Java, `public boolean equals(Point other)` is an overload, not an override, so a `HashMap` still calls `Object.equals`. Use `@Override`, and take `Object`.
- **Equality across a class hierarchy.** If `ColorPoint` extends `Point` and adds a color to `equals`, `point.equals(colorPoint)` and `colorPoint.equals(point)` can give different results, which breaks the rule that equality is symmetric. See [the equality and hashing contract](/equality-and-hashing-contract/).
- **Logic that reads the string form.** Java says of `toString`: "The string output is not necessarily stable over time or across JVM invocations."[^1] Do not parse it, and do not use it as a key.
- **`==` that does not call the method.** In Java and JavaScript, `==` and `===` on objects compare identity, and in C# a class must define `operator ==` to make it call `Equals`. Kotlin's `==` and Python's `==` call the equality method.

## Cost

The default methods are cheap: identity equality compares two references, and the default hash comes from the identity of the object, so it does not read the fields. Value equality compares each field, so its cost grows with the size of the fields; for a list field it compares every element. A hash of a large object is also computed from all the fields that equality uses. A hash map calls the hash once for each lookup and calls equality only for the keys in the same bucket, so a good hash keeps the number of equality calls small.

## History

Java made the methods part of the root class from its first version. JDK 1.0 was released on January 23, 1996,[^13] and `Object` is marked "Since: 1.0", with `equals`, `hashCode` and `toString`.[^1] Python 2.1 added the rich comparison methods `__lt__`, `__eq__` and the others; PEP 207, by Guido van Rossum and David Ascher, proposed to "Allow separately overloading of <, >, <=, >=, ==, !=, both in classes and in C extensions."[^14] Later languages generate the methods: Rust derives them from the fields, and C++20 added defaulted comparison operators.[^12]

## References

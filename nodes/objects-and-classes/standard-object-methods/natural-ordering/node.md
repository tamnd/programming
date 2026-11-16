---
tag: RF4SM8
kind: concept
slug: natural-ordering
name: Natural ordering
aliases: [natural order, Comparable, Ord trait, IComparable, rich comparisons, __lt__, spaceship operator, default ordering]
area: objects-and-classes/standard-object-methods
level: core
summary: >-
  The natural ordering of a type is the default order that the type defines for its values, such as Java's compareTo or Rust's Ord, which sorts and sorted collections use when no other order is given.
applies_to: core-languages
viz: optional
history:
  year: 1998
  note: J2SE 1.2 added the Comparable and Comparator interfaces to Java with the Collections Framework, and the documentation of Comparable named the order of a class its natural ordering; Python 2.1 added the rich comparison methods, and C++20 added a defaulted three-way comparison.
see_also: [universal-object-methods, equality-and-hashing-contract, comparator, implement-ordering-for-a-type, value-equality-and-identity]
sources:
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Comparable.html
    title: Comparable (Java SE 21 & JDK 21), API documentation
    locator: Interface Comparable
  - url: https://pkg.go.dev/cmp
    title: cmp package, Go standard library
    locator: func Compare
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.1 Basic customization, rich comparison methods
  - url: https://docs.python.org/3/library/functools.html
    title: functools, Python standard library
    locator: total_ordering; cmp_to_key
  - url: https://doc.rust-lang.org/std/cmp/trait.Ord.html
    title: Ord trait, Rust standard library
    locator: Description; Derivable
  - url: https://en.wikipedia.org/wiki/Total_order
    title: Total order, Wikipedia
    locator: Introduction
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/Comparator.html
    title: Comparator (Java SE 21 & JDK 21), API documentation
    locator: Interface Comparator
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.icomparable-1
    title: IComparable<T> Interface, .NET API reference
    locator: Remarks
  - url: https://en.cppreference.com/w/cpp/language/default_comparisons
    title: Default comparisons, cppreference.com
    locator: Three-way comparison; Comparison category types
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
    title: Array.prototype.sort(), JavaScript reference, MDN
    locator: Description
  - url: https://doc.rust-lang.org/std/cmp/trait.PartialOrd.html
    title: PartialOrd trait, Rust standard library
    locator: Strict and non-strict partial orders
  - url: https://en.wikipedia.org/wiki/Java_version_history
    title: Java version history, Wikipedia
    locator: J2SE 1.2
  - url: https://peps.python.org/pep-0207/
    title: PEP 207, Rich Comparisons
    locator: Header
---

## Idea

Many types have one order that most code expects: numbers from small to large, strings in dictionary order, dates from early to late. A type that defines this order for its own values gives it a natural ordering. Java's `Comparable` says: "This interface imposes a total ordering on the objects of each class that implements it. This ordering is referred to as the class's natural ordering".[^1] Sorting, sorted sets, sorted maps, `min` and `max` then work on the type with no extra argument.

A type has at most one natural ordering. Other orders, such as "by price" or "newest first", are separate comparison functions that a caller passes in, which is the job of a [comparator](/comparator/).

## How it works

A natural ordering is a method that compares this object with another object of the same type. It has one of two forms.

- **A three-way comparison** returns a negative number, zero or a positive number, or an enum such as Rust's `Ordering`. Java's `compareTo`, C#'s `CompareTo`, Go's `cmp.Compare` and C++'s `operator<=>` use this form. Go's `Compare` "returns -1 if x is less than y, 0 if x equals y, +1 if x is greater than y."[^2]
- **A less-than method** answers one question. Python's rich comparisons map each operator to a method: "x<y calls x.__lt__(y)".[^3] `functools.total_ordering` writes the rest: "Given a class defining one or more rich comparison ordering methods, this class decorator supplies the rest."[^4]

For a type with several fields, the usual order is lexicographic: compare the most important field, and compare the next field only when the first fields are equal. Rust's derived `Ord` does this: "When derived on structs, it will produce a lexicographic ordering based on the top-to-bottom declaration order of the struct’s members."[^5]

The method must define a real order, or a sort can give wrong results. Rust states the rules for `Ord`: for all values, "exactly one of a < b, a == b or a > b is true; and < is transitive".[^5] This is a total order, "a partial order in which any two elements are comparable."[^6] Java adds one more rule that is strongly recommended, but not required: the order should be consistent with equals, which means that "e1.compareTo(e2) == 0 has the same boolean value as e1.equals(e2)".[^1]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `__lt__`, `__le__`, `__gt__`, `__ge__`; `functools.total_ordering`; `sorted` uses `<` |
| JavaScript | absent | no natural order for objects; `sort` without a function compares strings |
| TypeScript | absent | as JavaScript |
| Java | native | `Comparable<T>.compareTo`; `Comparator<T>` for other orders |
| Kotlin | native | `Comparable<T>.compareTo`, and the operators `<` and `>` call it |
| C# | native | `IComparable<T>.CompareTo`; `IComparer<T>` for other orders |
| Go | idiom | a `Compare` method by convention; `cmp.Ordered` covers built-in types only |
| Rust | stdlib | traits `PartialOrd` and `Ord`, derivable; `Ordering` enum |
| C | absent | a comparison function passed to `qsort` and `bsearch` |
| C++ | native | `operator<=>`, which a class can default; `operator<` for older code |
| Bash | n/a | `sort` and `[[ < ]]` compare strings; no user types |
| PowerShell | native | .NET objects that implement `IComparable` |
| SQL | n/a | `ORDER BY` orders rows by column values; no user methods |
| Regex | n/a | no values to order |
| HTML | n/a | no values to order |
| CSS | n/a | no values to order |

- **Java and C#** keep the natural ordering on the type and other orders outside it. `Comparator` is "A comparison function, which imposes a total ordering on some collection of objects."[^7] In C#, `CompareTo` returns less than zero when "This object precedes the object specified by the CompareTo method in the sort order."[^8]
- **Rust** splits the order into two traits. `PartialOrd` allows values that cannot be compared, and `Ord` is the "Trait for types that form a total order."[^5] Floats are only `PartialOrd`, so `Vec<f64>::sort` does not compile.
- **C++** can default the three-way comparison, and the compiler rewrites `<`, `<=`, `>` and `>=` to use it. The result type gives the strength of the order: "There are three comparison category types: std::strong_ordering std::weak_ordering std::partial_ordering".[^9]
- **JavaScript** has no hook for objects. "If compareFn is not supplied, all non-undefined array elements are sorted by converting them to strings and comparing strings".[^10] Code passes a comparison function to every sort.
- **Go** has no operator overloading, so `<` works only on built-in ordered types. A struct gets an order from a `Compare` method, which code passes to `slices.SortFunc`.

## Variations and related ideas

- **Total, weak and partial orders.** A total order compares every pair and gives zero only for equal values. A weak order allows different values to be equivalent, such as strings compared without regard to case. A partial order allows pairs that cannot be compared, such as floats with `NaN`: Rust shows that for `f64::NAN`, `a <= a` is false.[^11]
- **Key functions.** In place of a comparison, a sort can take a function that maps each item to a key with a natural order, such as a tuple of fields. Python's `sorted(..., key=...)` works this way, and `functools.cmp_to_key` turns an old comparison function into a key function.[^4]
- **Comparators.** A [comparator](/comparator/) is an order that lives outside the type, so a type can have many. The natural ordering is the one that the type chooses as the default.
- **Equality.** The order and [the equality and hashing contract](/equality-and-hashing-contract/) must agree. When `compare(a, b)` is zero exactly when `a` equals `b`, a sorted set and a hash set hold the same items.

## Pitfalls

- **An order that is not consistent with equals.** Java's `BigDecimal` is the example in the documentation. Its natural ordering "equates BigDecimal objects with equal numerical values and different representations (such as 4.0 and 4.00)", but `equals` does not:[^1]

```java run id=compare-inconsistent-with-equals
import java.math.BigDecimal;
import java.util.HashSet;
import java.util.List;
import java.util.TreeSet;

public class Main {
    public static void main(String[] args) {
        BigDecimal a = new BigDecimal("4.0");
        BigDecimal b = new BigDecimal("4.00");
        System.out.println(a.equals(b) + " " + a.compareTo(b));
        System.out.println(new HashSet<>(List.of(a, b)).size() + " " + new TreeSet<>(List.of(a, b)).size());
    }
}
```

```output for=compare-inconsistent-with-equals
false 0
2 1
```

  A hash set keeps both values and a tree set keeps one. Java warns that such sorted sets "behave "strangely"".[^1]
- **Subtraction as comparison.** `return a.x - b.x;` overflows when the values have different signs and are large, and the sign of the result is then wrong. Use `Integer.compare`, `cmp.Compare` or `<=>`.
- **An order that is not transitive.** A comparison that uses a tolerance, such as "equal if the difference is less than 0.01", is not transitive. A sort can then give a wrong order.
- **`NaN` in the data.** With floats, `NaN < x` and `NaN > x` are both false, so a comparison built from `<` treats `NaN` as equal to every number. Use a total order for floats, such as Rust's `total_cmp` or Go's `cmp.Compare`, which considers `NaN` "less than any non-NaN".[^2]
- **The default sort in JavaScript.** `[10, 9, 1, 2].sort()` gives `[1, 10, 2, 9]`, because the items are compared as strings.

## Cost

A comparison method is called O(n log n) times when a sort orders n items, and O(log n) times for each insertion or lookup in a balanced tree. Its own cost grows with the number of fields that it must read before two values differ. A three-way comparison costs one call for each step, while a less-than method can need two calls to find that two values are equal. A key function computes each key once, which helps when the key is expensive, such as a lower-case copy of a string.

## History

J2SE 1.2, released in December 1998,[^12] added `Comparable` and `Comparator` to Java as part of the Collections Framework, and `Comparable` is marked "Since: 1.2".[^1] The documentation named the order of a class its natural ordering. Python 2.1 added the rich comparison methods with PEP 207, by Guido van Rossum and David Ascher.[^13] C++20 added the three-way comparison operator `<=>`, which a class can default, and Rust's `derive` generates `PartialOrd` and `Ord` from the fields.

## References

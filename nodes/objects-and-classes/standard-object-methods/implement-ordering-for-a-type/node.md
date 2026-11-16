---
tag: 8C752V
kind: task
slug: implement-ordering-for-a-type
name: Implement ordering for a type
aliases: [implement compareTo, implement Comparable, implement Ord, implement __lt__, sort custom objects, order objects by fields, implement IComparable, default operator<=>]
area: objects-and-classes/standard-object-methods
level: core
summary: Give a Version type with three numbers a natural ordering, with compareTo, __lt__, Ord, IComparable or the spaceship operator, and use it to sort version strings by number and not by text.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [natural-ordering, comparator, implement-equality-for-a-type, equality-and-hashing-contract, universal-object-methods]
sources:
  - url: https://docs.python.org/3/library/functools.html
    title: functools, Python standard library
    locator: total_ordering
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/Comparator.html
    title: Comparator (Java SE 21 & JDK 21), API documentation
    locator: comparingInt; thenComparingInt
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Comparable.html
    title: Comparable (Java SE 21 & JDK 21), API documentation
    locator: Interface Comparable; compareTo
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.icomparable-1
    title: IComparable<T> Interface, .NET API reference
    locator: Remarks; Notes to Implementers
  - url: https://pkg.go.dev/cmp
    title: cmp package, Go standard library
    locator: func Compare; func Or
  - url: https://doc.rust-lang.org/std/cmp/trait.Ord.html
    title: Ord trait, Rust standard library
    locator: Derivable; How can I implement Ord?
  - url: https://en.cppreference.com/w/cpp/language/default_comparisons
    title: Default comparisons, cppreference.com
    locator: Three-way comparison
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
    title: Array.prototype.sort(), JavaScript reference, MDN
    locator: Description
  - url: https://docs.python.org/3/library/dataclasses.html
    title: dataclasses, Python standard library
    locator: Module contents, order
---

## Task

Write a type `Version` with three numbers: major, minor and patch. Give it a natural ordering: compare the major numbers first, then the minor numbers, then the patch numbers, each as a number. Then use the order. The function `sort_versions(versions)` parses each string such as `"1.10.0"` into a `Version`, sorts the values with the natural ordering of the type, and returns them as strings again.

For example, `sort_versions(["2.0.0", "1.9.9", "1.10.0"])` is `["1.9.9", "1.10.0", "2.0.0"]`. A sort of the strings as text gives a different answer, because the character `1` comes before `9`:

```python run id=text-order-is-wrong
print(sorted(["1.10.0", "1.9.9", "1.2.0"]))
```

```output for=text-order-is-wrong
['1.10.0', '1.2.0', '1.9.9']
```

The list has 0 to 10,000 strings. Each number is from 0 to 1,000,000, with no sign and no leading zeros, so two equal versions are also equal strings. The tests also check the order directly: `a < b` must give the opposite answer of `b < a`, and a version compares equal to itself.

## Solutions

| Language | Natural ordering | Sort call |
|---|---|---|
| Python | `__lt__` and `__eq__`, with `@functools.total_ordering` | `sorted(...)` |
| JavaScript | a static `compare(a, b)` by convention | `array.sort(Version.compare)` |
| TypeScript | as JavaScript, with types | `array.sort(Version.compare)` |
| Java | `record Version ... implements Comparable<Version>` | `stream.sorted()` |
| C# | `record Version : IComparable<Version>`, with `<`, `>`, `<=` and `>=` | `Order()` |
| Go | a `Compare` method by convention | `slices.SortFunc(vs, Version.Compare)` |
| Rust | `#[derive(PartialEq, Eq, PartialOrd, Ord)]` | `vec.sort()` |
| C | a function `version_compare` | `qsort` |
| C++ | `auto operator<=>(const Version&) const = default;` | `std::sort` |

- **Compare the fields in order of importance.** Each solution compares the major numbers and goes to the next field only when they are equal. This is a lexicographic order of the fields, the same order as tuples in Python and the derived order in Rust.
- **Python** compares two tuples of the fields in `__lt__`, and tuples compare item by item. `total_ordering` writes `<=`, `>` and `>=`. Its documentation says: "The class must define one of __lt__(), __le__(), __gt__(), or __ge__()."[^1] Both methods return `NotImplemented` for other types, so `Version(1, 0, 0) < "1.0.0"` raises `TypeError`.
- **Java** builds the order from `Comparator.comparingInt(Version::major).thenComparingInt(...)`, which returns a lexicographic-order comparator.[^2] The order is consistent with equals: `compareTo` returns 0 only when all three fields are equal, which is what the record's `equals` compares. `Comparable` says that "e.compareTo(null) should throw a NullPointerException even though e.equals(null) returns false",[^3] and the comparator does.
- **C#** implements `IComparable<Version>` and the four comparison operators. The documentation says that a type that implements `IComparable<T>` "should overload the op_GreaterThan, op_GreaterThanOrEqual, op_LessThan, and op_LessThanOrEqual operators to return values that are consistent with CompareTo(T)."[^4] By convention, any version is greater than `null`.
- **Go** has no operator overloading, so `<` does not work on a struct. The `Compare` method chains `cmp.Compare` for the three fields with `cmp.Or`, which returns the first result that is not zero.[^5] `slices.SortFunc` takes the method expression `Version.Compare` as its comparison function.
- **Rust** derives the order. "When derived on structs, it will produce a lexicographic ordering based on the top-to-bottom declaration order of the struct’s members."[^6] The fields are declared as `major`, `minor`, `patch`, so the derived order is the right one with no code.
- **C++** defaults `operator<=>`. The compiler compares the members in the order of their declaration and rewrites `a < b` as `(a <=> b) < 0`, so `std::sort` and `std::set` work. The defaulted `<=>` also gives a defaulted `==`.[^7]
- **C** has no methods. `version_compare` returns a negative number, zero or a positive number, as `qsort` expects. It uses `(x > y) - (x < y)` for each field, which cannot overflow.
- **JavaScript and TypeScript** have no natural order for objects, and "If compareFn is not supplied, all non-undefined array elements are sorted by converting them to strings and comparing strings".[^8] `Version.compare` subtracts the fields and uses `||` to go to the next field when the result is zero. The subtraction is safe here because JavaScript numbers are exact up to 2^53.

### Alternatives

- **Generate the order.** Python's `@dataclass(order=True)` generates the four ordering methods, which "compare the class as if it were a tuple of its fields, in order."[^9] Rust's `derive` and C++'s defaulted `<=>` do the same, and Kotlin's `compareValuesBy(a, b, { it.major }, { it.minor }, { it.patch })` builds the comparison from the fields.
- **A key function in place of an order.** `sorted(versions, key=lambda s: tuple(map(int, s.split("."))))` sorts the strings with no `Version` type. This is short, but other code cannot use the order, for example in a sorted set.
- **A comparator outside the type.** When the type has no single obvious order, give no natural ordering, and pass a [comparator](/comparator/) to each sort.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `single`: a sort of zero or one item returns it unchanged.
- `numeric-not-text`: `10.0.0` comes after `9.0.0`. A comparison of the strings gives the opposite order.
- `duplicates`: equal versions stay in the list, so the solutions use a sort and not a set.
- `zero`: `0.0.0` is a valid version and is the smallest one.
- `limit`: `1000000.0.0` is larger than `999999.1000000.1000000`, because the major number decides before the other fields count.

## Pitfalls

- **Subtraction in a comparison.** `return a - b;` is a common short form, but with 32-bit integers it overflows when the values are far apart, and the sign of the result is then wrong:

```java run id=subtraction-overflows
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        Comparator<Integer> bySubtraction = (x, y) -> x - y;
        System.out.println(bySubtraction.compare(Integer.MIN_VALUE, 1));

        List<Integer> xs = new ArrayList<>(List.of(5, Integer.MIN_VALUE, -5, Integer.MAX_VALUE));
        xs.sort(bySubtraction);
        System.out.println(xs);
        xs.sort(Integer::compare);
        System.out.println(xs);
    }
}
```

```output for=subtraction-overflows
2147483647
[5, 2147483647, -2147483648, -5]
[-2147483648, -5, 5, 2147483647]
```

  `Integer.MIN_VALUE - 1` wraps around to the largest positive `int`, so the comparator says that the smallest number is greater than 1. Use `Integer.compare`, `cmp.Compare`, `<=>` or `(x > y) - (x < y)`.
- **An order that disagrees with equality.** If `compareTo` compares only the major and minor numbers, `1.2.0` and `1.2.5` are equal for a `TreeSet`, and the set keeps only one of them, although `equals` says that they differ. Compare every field that equality compares.
- **The wrong field order.** A comparison that starts with the patch number sorts `1.0.9` after `2.0.0`. The derived orders of Rust and C++ follow the order of the declarations, so a change to the field order of the type changes the sort.
- **Only `<` in C#.** A class that implements `IComparable<T>` and not the operators gives `CompareTo` to sorts, but `a < b` does not compile. Define all four operators, and make them call `CompareTo`.

## Related tasks

- [[implement-equality-for-a-type]] gives a type the equality that this order must agree with.
- [[sort-the-lines-of-a-file]] sorts strings by their code points, which is the text order that this task must not use.
- [[define-a-record-type]] gets equality from the language; some languages also derive an order for records.

## References

---
tag: ZX1AWQ
kind: task
slug: implement-equality-for-a-type
name: Implement equality for a type
aliases: [implement equals, override equals, value equality for a class, define equality, __eq__, IEquatable, PartialEq, operator==, equals and hashCode]
area: objects-and-classes/standard-object-methods
level: core
summary: Give a Money class with an amount and a currency value equality and a hash that agrees with it, so that equal values are equal from both sides and a hash set keeps only one of them.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [universal-object-methods, equality-and-hashing-contract, value-equality-and-identity, hash-codes, record-type, immutability]
sources:
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Object (Java SE 21 & JDK 21), API documentation
    locator: equals; hashCode
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.1 Basic customization
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/statements-expressions-operators/how-to-define-value-equality-for-a-type
    title: How to define value equality for a class or struct, C# programming guide
    locator: Value types, reference types, and equality defaults; Use records for value equality
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Comparison operators
  - url: https://doc.rust-lang.org/std/cmp/trait.Eq.html
    title: Eq trait, Rust standard library
    locator: Description
  - url: https://doc.rust-lang.org/std/hash/trait.Hash.html
    title: Hash trait, Rust standard library
    locator: Hash and Eq
  - url: https://en.cppreference.com/w/cpp/language/default_comparisons
    title: Default comparisons, cppreference.com
    locator: Introduction
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Equality_comparisons_and_sameness
    title: Equality comparisons and sameness, MDN
    locator: Introduction
---

## Task

Write a class `Money` with two fields: an amount in cents, and a currency code of 3 letters. Two `Money` values are equal when their amounts are equal and their currency codes are equal; `"eur"` and `"EUR"` are the same currency. Give the class an equality that follows this rule, and a hash code that agrees with it, so that a `Money` value works as a key of a hash map and as an item of a hash set.

Then use it. The function `count_distinct(amounts, currencies)` makes the value `Money(amounts[i], currencies[i])` for each index, puts the values in a hash set and returns the number of different values. For example, `count_distinct([500, 500, 500, 700], ["EUR", "USD", "EUR", "EUR"])` is 3. The lists have 0 to 10,000 items, each amount has an absolute value of 10^12 or less, and the tests also check the rules of equality directly.

The rules come from Java's `Object.equals`, which says that equality "implements an equivalence relation on non-null object references".[^1] It must be reflexive, symmetric and transitive, and Java adds: "For any non-null reference value x, x.equals(null) should return false." The hash code must agree: "If two objects are equal according to the equals method, then calling the hashCode method on each of the two objects must produce the same integer result."

## Solutions

| Language | Equality | Hash | `==` on two objects |
|---|---|---|---|
| Python | `__eq__` | `__hash__` | calls `__eq__` |
| JavaScript | an `equals` method by convention | a string `key()` for `Set` and `Map` | identity, cannot change |
| TypeScript | as JavaScript, `equals(other: unknown)` | as JavaScript | identity, cannot change |
| Java | override `equals(Object)` | override `hashCode()` | identity, cannot change |
| C# | `IEquatable<Money>.Equals` and an override of `Equals(object)` | override `GetHashCode()` | calls `Equals` after the class defines `operator ==` |
| Go | built-in `==` on a struct with comparable fields; an `Equal` method | built in for map keys | compares the fields |
| Rust | `#[derive(PartialEq, Eq)]` | `#[derive(Hash)]` | calls `PartialEq::eq` |
| C | a function `money_equal` | a function `money_hash` | not defined for structs |
| C++ | `operator==` with `= default` | a specialization of `std::hash` | calls `operator==` |

- **Normalize in the constructor.** Each solution converts the currency code to upper case when it makes the value. Equality and the hash can then compare the fields directly, and they cannot disagree about the case.
- **Python** returns `NotImplemented` when the other operand is not a `Money`, and Python then tries the other operand's method and at last identity. `__hash__` hashes a tuple of the same two fields; the data model advises "packing them into a tuple and hashing the tuple".[^2] The fields are read-only properties, because a hash of a field that changes breaks a set.
- **Java** checks for the same object first, then uses `instanceof` with a pattern, which also returns false for `null`. `Objects.hash(cents, currency)` uses the same fields as `equals`. The class is `final`, so no subclass can add a field to `equals` and break symmetry.
- **C#** implements `IEquatable<Money>`, which gives a typed `Equals`, and the override of `Equals(object)` calls it. A class must also define `==` and `!=`, because "A plain class uses reference equality, so == and Equals test whether two variables point to the same object."[^3] `HashCode.Combine` mixes the fields.
- **Go** needs no method for equality. The spec says that "Struct types are comparable if all their field types are comparable. Two struct values are equal if their corresponding non-blank field values are equal."[^4] A comparable struct can be a map key, so a `map[Money]struct{}` is the hash set. The fields are not exported, so other packages must use the constructor `New`, which makes the code upper case.
- **Rust** derives `PartialEq`, `Eq` and `Hash` from the two fields. `Eq` adds the promise that the value is equal to itself, and "This property cannot be checked by the compiler, and therefore Eq is a trait without methods."[^5] The `Hash` documentation gives the rule that the derive keeps: "k1 == k2 -> hash(k1) == hash(k2)".[^6]
- **C++** defaults `operator==`, which compares the members in the order of their declaration, and C++20 rewrites `a != b` as `!(a == b)`. The default order of the members is "The non-static data members of C, in declaration order".[^7] `std::unordered_set` needs a specialization of `std::hash` that uses the same members.
- **C** has no operators for structs. `money_equal` compares the fields, and `money_hash` is an FNV-1a hash of the same fields. `count_distinct` uses a small hash set with open addressing.
- **JavaScript and TypeScript** cannot change equality. MDN says that for two distinct objects with the same structure, all the equality operators "will evaluate to false".[^8] The class has an `equals` method for code that calls it, and a `key()` that turns the value into a string, because a `Set` of strings compares them by value.

### Alternatives

- **Generate the methods.** A Java or C# `record`, a Kotlin `data class`, a Python `@dataclass(frozen=True)` and Rust's `#[derive]` write equality and the hash from the fields. Use them when the default rule, which compares all the fields, is the right rule. Here the currency must be normalized first, which a record does in its constructor. The C# guide says: "Choose record types for implementing value equality, and the compiler generates all the necessary equality members for you."[^3]
- **Compare with `getClass()`.** In Java, `getClass() != o.getClass()` in place of `instanceof` makes an object equal only to objects of the same class, which keeps symmetry when a subclass adds a field. A subclass that adds no field is then never equal to its base class, though.
- **A typed equality method only.** In JavaScript and in C, a function `equals(a, b)` is enough when no hash set needs the values.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no values give a count of 0.
- `case`: `gbp`, `GBP` and `Gbp` are the same currency. Without the normalization, the count is 3.
- `other-currency`: three values with the amount 0 in different currencies are different, so the hash and the equality must both use the currency.
- `negative`: -5 and 5 are different amounts.
- `limit`: amounts of 10^12 need a 64-bit integer; a 32-bit `int` overflows. JavaScript numbers hold them exactly, because they are smaller than 2^53.

## Pitfalls

- **A hash that disagrees with equality.** If `equals` normalizes the currency and `hashCode` hashes the raw string, two equal values can go to different buckets, and the set keeps both. Normalize once, in the constructor, and use the same fields in both methods.
- **Equality across a class hierarchy.** If a subclass adds a field to `equals` and checks for its own type, a base object can be equal to a subclass object, but not the other way round:

```java run id=subclass-breaks-symmetry
public class Main {
    static class Point {
        final int x, y;

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public boolean equals(Object o) {
            return o instanceof Point p && x == p.x && y == p.y;
        }

        @Override
        public int hashCode() {
            return 31 * x + y;
        }
    }

    static class ColorPoint extends Point {
        final String color;

        ColorPoint(int x, int y, String color) {
            super(x, y);
            this.color = color;
        }

        @Override
        public boolean equals(Object o) {
            return o instanceof ColorPoint cp && super.equals(cp) && color.equals(cp.color);
        }
    }

    public static void main(String[] args) {
        Point p = new Point(1, 2);
        ColorPoint red = new ColorPoint(1, 2, "red");
        System.out.println(p.equals(red) + " " + red.equals(p));
    }
}
```

```output for=subclass-breaks-symmetry
true false
```

  `p.equals(red)` is true but `red.equals(p)` is false, so equality is not symmetric, and a collection can give different answers for the same two objects. Make the class final, or give `ColorPoint` a `Point` field in place of the subclass.
- **An overload in place of an override.** In Java, `public boolean equals(Money other)` does not override `equals(Object)`, so `HashSet` still uses identity. In C#, the same mistake leaves `Equals(object)` unchanged. Use `@Override` and `override`, which make the compiler check the signature.
- **`==` that stays identity.** In Java and JavaScript, `a == b` and `a === b` compare references even after the class has `equals`. In C#, a class that overrides `Equals` and not `operator ==` gives `true` for `a.Equals(b)` and `false` for `a == b`.
- **`memcmp` on structs in C.** The compiler can add padding bytes between the fields, and their values are not defined, so two equal structs can have different bytes. Compare the fields one by one.
- **Mutable fields.** A value whose fields change after it goes into a hash set is in the wrong bucket. Python says that if a class "defines mutable objects and implements an __eq__() method, it should not implement __hash__()".[^2] The solutions make the fields read-only.

## Related tasks

- [[define-a-method-on-a-type]] defines a value type with methods; its record gets equality from the language.
- [[implement-ordering-for-a-type]] adds an order that must be consistent with this equality.
- [[define-a-record-type]] gets equality, the hash and the string form from a record or a data class.

## References

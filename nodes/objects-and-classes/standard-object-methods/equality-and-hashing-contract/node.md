---
tag: X2FZ6P
kind: concept
slug: equality-and-hashing-contract
name: The equality and hashing contract
aliases: [equals and hashCode contract, hashCode contract, equals contract, Eq and Hash, hash consistency, equal objects equal hashes]
area: objects-and-classes/standard-object-methods
level: core
summary: >-
  The equality and hashing contract is the rule that equality must be an equivalence relation and that equal objects must have equal hash codes, which hash tables need to find their keys.
applies_to: core-languages
viz: optional
history:
  year: 1996
  note: JDK 1.0 put equals and hashCode on java.lang.Object, and its API documentation states the contract that equal objects must have the same hash code; later languages state the same rule for their own hash tables.
see_also: [universal-object-methods, implement-equality-for-a-type, hash-codes, value-equality-and-identity, immutability, natural-ordering]
sources:
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Object (Java SE 21 & JDK 21), API documentation
    locator: equals; hashCode
  - url: https://doc.rust-lang.org/std/hash/trait.Hash.html
    title: Hash trait, Rust standard library
    locator: Hash and Eq
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.gethashcode
    title: Object.GetHashCode Method, .NET API reference
    locator: Remarks; Notes to Inheritors
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.1 Basic customization, object.__hash__
  - url: https://kotlinlang.org/docs/equality.html
    title: Equality, Kotlin documentation
    locator: Structural equality
  - url: https://docs.python.org/3/library/dataclasses.html
    title: dataclasses, Python standard library
    locator: Module contents, unsafe_hash
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Map types
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Equality_comparisons_and_sameness
    title: Equality comparisons and sameness, MDN
    locator: Same-value-zero equality
  - url: https://doc.rust-lang.org/std/cmp/trait.Eq.html
    title: Eq trait, Rust standard library
    locator: Description
  - url: https://doc.rust-lang.org/std/cmp/trait.PartialEq.html
    title: PartialEq trait, Rust standard library
    locator: Description
  - url: https://en.wikipedia.org/wiki/Java_version_history
    title: Java version history, Wikipedia
    locator: Release table
---

## Idea

A hash table finds a key in two steps. It computes the hash code of the key to choose a bucket, and then it calls equality on the keys in that bucket. The two steps give the same answer only if the type keeps one rule: equal objects have equal hash codes. Java states the rule for `hashCode`: "If two objects are equal according to the equals method, then calling the hashCode method on each of the two objects must produce the same integer result."[^1] Rust states it as "k1 == k2 -> hash(k1) == hash(k2)".[^2]

The rule is one part of a contract. Equality must also be an equivalence relation: reflexive, symmetric and transitive. The hash code must not change while the object is in a table. A type that breaks any part of the contract compiles and runs, but a hash set can then hold duplicates, or lose an object that it holds.

## How it works

The contract has these parts. The words come from Java's `Object`, and the other languages state the same rules.[^1]

| Part | Rule | What breaks without it |
|---|---|---|
| Reflexive | `x.equals(x)` is true | a lookup cannot find the key that went in |
| Symmetric | `x.equals(y)` if and only if `y.equals(x)` | the answer depends on which object the table holds |
| Transitive | `x` equals `y` and `y` equals `z`, so `x` equals `z` | a set keeps different items for each insertion order |
| Consistent | the result does not change while the fields do not change | a key found once is not found again |
| Not null | `x.equals(null)` is false | code that compares an object with null throws an exception |
| Hash agrees | equal objects give equal hash codes | equal keys go to different buckets, and the set keeps both |
| Hash stable | the hash code does not change while the fields that equality reads do not change | the table looks in the wrong bucket |

The rule goes in one direction only. Java says: "It is not required that if two objects are unequal according to the equals method, then calling the hashCode method on each of the two objects must produce distinct integer results."[^1] .NET says the same: "equal hash codes do not imply object equality, because different (unequal) objects can have identical hash codes."[^3] Two unequal keys with the same hash code are a collision. A collision makes the table slower, because equality must compare more keys, but the result stays correct. A hash code that is the same constant for every object keeps the contract and makes every lookup a linear search.

The simplest way to keep the contract is to compute the hash from the same fields that equality compares, and from no others. Python advises this: "The only required property is that objects which compare equal have the same hash value; it is advised to mix together the hash values of the components of the object that also play a part in comparison of objects".[^4] If equality ignores a difference, such as the case of a string, the hash must ignore it too, usually by hashing a normalized form.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `__eq__` and `__hash__`; a class that defines only `__eq__` gets `__hash__ = None` |
| JavaScript | absent | `Map` and `Set` use SameValueZero, which is identity for objects; no user hash |
| TypeScript | absent | as JavaScript |
| Java | native | `equals` and `hashCode` on `Object`, with the contract in their documentation |
| Kotlin | native | `equals` and `hashCode` on `Any`; a `data class` generates both |
| C# | native | `Equals` and `GetHashCode` on `Object`; `IEqualityComparer<T>` gives a separate pair |
| Go | native | map keys use the built-in `==` and hash of a comparable type; no user hash |
| Rust | stdlib | traits `PartialEq`, `Eq` and `Hash`; `HashMap` needs `Eq + Hash` |
| C | absent | no hash tables in the standard library; a library takes a hash function and an equality function |
| C++ | stdlib | `std::unordered_set` takes a hash type and an equality type; `std::hash` and `operator==` by default |
| Bash | n/a | associative arrays have string keys only |
| PowerShell | native | .NET objects with `Equals` and `GetHashCode`; hashtables use them |
| SQL | n/a | a database compares and hashes column values; no user types with methods |
| Regex | n/a | no objects |
| HTML | n/a | no objects |
| CSS | n/a | no objects |

- **Java, Kotlin and C#** put the two methods on the root class, so each override of one needs an override of the other. Kotlin says: "When overriding the equals() function, you should also override the hashCode() function to keep consistency between equality and hashing".[^5] .NET says that derived classes that override one method "must also override Equals(Object) to guarantee that two objects considered equal have the same hash code".[^3]
- **Python** enforces half of the rule. "A class that overrides __eq__() and does not define __hash__() will have its __hash__() implicitly set to None", so its objects cannot go into a set.[^4] A `@dataclass` with `eq=True` and `frozen=True` generates a hash; with `eq=True` and no `frozen`, "__hash__() will be set to None, marking it unhashable (which it is, since it is mutable)."[^6]
- **Rust** makes the contract part of the types. `HashSet<T>` needs `T: Eq + Hash`, and `#[derive(PartialEq, Eq, Hash)]` uses the same fields for both. The compiler cannot check that a hand-written `Hash` agrees with a hand-written `PartialEq`; the documentation calls a mismatch a logic error.
- **Go** does not let code define equality or hashing for map keys. The key type must be comparable: "The comparison operators == and != must be fully defined for operands of the key type; thus the key type must not be a function, map, or slice."[^7] The built-in hash always agrees with the built-in `==`.
- **JavaScript** has no hook. MDN says that "JavaScript does not provide a general deep comparison operator",[^8] and a `Map` with object keys compares references. Code that needs value keys builds a string or a number from the fields.

## Variations and related ideas

- **A separate comparer.** C#'s `IEqualityComparer<T>` and the hash and equality template parameters of C++'s `std::unordered_set` let a collection use an equality and a hash that the type does not define. The two still must agree.
- **Partial equality.** IEEE 754 floating-point numbers break reflexivity, because `NaN` is not equal to itself. Rust records this in the types: "Floating point types such as f32 and f64 implement only PartialEq but not Eq because NaN != NaN."[^9] Rust's `PartialEq` exists "for types that do not have a full equivalence relation."[^10]
- **Identity equality.** The default `equals` and hash code of a Java or C# object use identity, which keeps the contract trivially. A type needs the contract only when it changes equality to compare values. See [value equality and identity](/value-equality-and-identity/).
- **Ordering.** A sorted map such as Java's `TreeMap` uses `compareTo` in place of `equals` and `hashCode`. The ordering should be consistent with equality, which is part of [natural ordering](/natural-ordering/).

## Pitfalls

- **Override one and not the other.** In Java, a class that overrides `equals` and keeps the identity hash code of `Object` gives `false` for `set.contains(copy)`, although `copy.equals(original)` is true. Java's note: "It is generally necessary to override the hashCode method whenever this method is overridden".[^1]
- **A mutable key.** If a field that the hash reads changes after the object goes into a hash set, the object stays in the bucket of its old hash code:

```python run id=mutable-key-lost
class Key:
    def __init__(self, name):
        self.name = name

    def __eq__(self, other):
        return isinstance(other, Key) and self.name == other.name

    def __hash__(self):
        return hash(self.name)


k = Key("a")
s = {k}
print(Key("a") in s)
k.name = "b"
print(k in s, Key("b") in s, len(s))
```

```output for=mutable-key-lost
True
False False 1
```

  The set still holds one object, but no lookup finds it. Python's advice is that a class that "defines mutable objects and implements an __eq__() method, it should not implement __hash__()".[^4] Use immutable fields for keys, or take the object out of the set before the change.
- **A hash that reads more than equality.** If equality ignores the case of a string and the hash does not, `"Rust"` and `"RUST"` are equal but go to different buckets. Hash the normalized form that equality compares.
- **Equality across subclasses.** If a subclass adds a field to `equals`, equality is not symmetric any more, and the hash codes of the base and the subclass can differ for objects that one side calls equal. [Implement equality for a type](/implement-equality-for-a-type/) shows the case with a run block.
- **Floating-point fields.** A key with a `NaN` field is not equal to itself, so a Go map or a hand-written equality can store it many times and never find it again.
- **Hash codes that persist.** .NET does not guarantee the default hash code, and the value "might differ between .NET implementations and platforms".[^3] Java says that the hash code "need not remain consistent from one execution of an application to another".[^1] Do not store hash codes in files or send them over a network.

## Cost

Equality and hashing should cost about the same: each reads the fields that define the value once. A lookup in a hash table computes the hash of the key once, then calls equality on each key in the bucket with the same hash code, so a good hash keeps the number of equality calls near one. A bad hash, which puts many keys in one bucket, makes each lookup grow with the number of keys. An immutable type with large fields, such as long strings or lists, can compute its hash code once and keep it.

## History

JDK 1.0, released on January 23, 1996,[^11] put `equals` and `hashCode` on `java.lang.Object`, and the documentation of `hashCode` states the contract that later languages copied. Python's data model and .NET's `GetHashCode` state the same rule. Later languages moved the rule closer to the compiler: Rust's `HashMap` needs both `Eq` and `Hash`, and `derive` generates both from the same fields.

## References

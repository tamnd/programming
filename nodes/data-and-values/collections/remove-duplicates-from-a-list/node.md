---
tag: NEW-remove-duplicates-from-a-list
kind: task
slug: remove-duplicates-from-a-list
name: Remove duplicates from a list
aliases: [deduplicate a list, unique values, distinct elements, remove repeated items, dedupe by key]
area: data-and-values/collections
level: core
summary: Remove the repeated items from a list and keep the first copy of each item in its input order, and remove records that have the same key.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/stdtypes.html#dict.fromkeys
    title: The Python Standard Library, Built-in Types
    locator: Mapping types, dict (fromkeys; dictionaries preserve insertion order)
  - url: https://docs.python.org/3/library/stdtypes.html#set-types-set-frozenset
    title: The Python Standard Library, Built-in Types
    locator: Set Types, set, frozenset
  - url: https://docs.python.org/3/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Expressions
    locator: 6.10.1 Value comparisons (not-a-number values; built-in containers and identical objects)
  - url: https://docs.python.org/3/using/cmdline.html#envvar-PYTHONHASHSEED
    title: The Python documentation, Command line and environment
    locator: PYTHONHASHSEED
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set
    title: MDN Web Docs, Set
    locator: Description (Value equality); Iterating sets
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/indexOf
    title: MDN Web Docs, Array.prototype.indexOf()
    locator: Description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html#distinct()
    title: Java SE 25 API, java.util.stream.Stream
    locator: distinct()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/HashSet.html#add(E)
    title: Java SE 25 API, java.util.HashSet
    locator: class description (iteration order); add(E)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html#equals(java.lang.Object)
    title: Java SE 25 API, java.lang.Object
    locator: equals(Object), hashCode()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Record.html#equals(java.lang.Object)
    title: Java SE 25 API, java.lang.Record
    locator: equals(Object), hashCode()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html#toLowerCase()
    title: Java SE 25 API, java.lang.String
    locator: toLowerCase(Locale), toLowerCase()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.distinct?view=net-10.0
    title: .NET API reference, Enumerable.Distinct
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.distinctby?view=net-10.0
    title: .NET API reference, Enumerable.DistinctBy
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.hashset-1.add?view=net-10.0
    title: .NET API reference, HashSet<T>.Add
    locator: Returns
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.string.tolowerinvariant?view=net-10.0
    title: .NET API reference, String.ToLowerInvariant
    locator: Remarks
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause; Map types
  - url: https://pkg.go.dev/slices#Compact
    title: Go standard library, package slices
    locator: Compact, CompactFunc
  - url: https://doc.rust-lang.org/std/collections/struct.HashSet.html#method.insert
    title: The Rust Standard Library, struct HashSet
    locator: insert; the type documentation (Eq and Hash)
  - url: https://doc.rust-lang.org/std/primitive.str.html#method.to_ascii_lowercase
    title: The Rust Standard Library, primitive type str
    locator: to_ascii_lowercase
  - url: https://en.cppreference.com/w/cpp/container/unordered_set/insert
    title: cppreference.com, std::unordered_set::insert
    locator: Return value
  - url: https://en.cppreference.com/w/cpp/string/byte/tolower
    title: cppreference.com, std::tolower
    locator: Description; Notes
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/strcmp.html
    title: The Open Group, POSIX.1-2024, strcmp
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/strcasecmp.html
    title: The Open Group, POSIX.1-2024, strcasecmp
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
    title: The Open Group, POSIX.1-2024, awk
    locator: Variables and Special Variables; Overall Program Structure; String Functions (tolower)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/uniq.html
    title: The Open Group, POSIX.1-2024, uniq
    locator: Description; Application Usage
---

## Task

Remove the repeated items from a list of strings. Keep the first copy of each item, and remove the later copies. The items that stay keep their input order. So `["apple", "pear", "apple", "fig", "pear"]` becomes `["apple", "pear", "fig"]`.

Two items are equal only if they have the same code points. The function does not fold case, does not normalize Unicode and does not trim spaces. So `"a"` and `"A"` are two items, and `" a"` and `"a"` are two items. The precomposed `é` (U+00E9) and `e` followed by U+0301 look the same, but they are two items too.

A second function removes records that have the same key. Each user is a name and an email, and the emails are ASCII. The function keeps the first user for each email, and it compares the emails with the letters A to Z changed to lower case.

## Solutions

Each solution reads the list one time. It keeps a set of the items that it saw. An item that is not in the set goes to the result and into the set. The order comes from the list, not from the set.

- **Python.** `list(dict.fromkeys(items))`. A dict keeps its keys in insertion order, and `fromkeys` adds each key one time. `unique_by` uses a set of keys and a loop, and `unique_by_email` gives it the key `u.email.lower()`.
- **JavaScript and TypeScript.** `[...new Set(items)]`. A `Set` visits its values in insertion order. A value that is already in the set does not move. `uniqueBy` keeps a `Set` of keys and a result array.
- **Java.** `items.stream().distinct().toList()`. For an ordered stream, `distinct` keeps the first copy of each item. `uniqueBy` uses `HashSet.add`, which returns `false` when the set already has the key. The email key is `toLowerCase(Locale.ROOT)`.
- **C#.** A loop with `HashSet<TKey>.Add`, which returns `false` for a key that is already present. The Remarks of `Distinct` and `DistinctBy` say that their result is an unordered sequence. The loop gives the input order from its design. The email key is `ToLowerInvariant()`.
- **Go.** A generic `UniqueBy[T any, K comparable]` with a `map[K]struct{}` and a result slice. `Unique` calls it with the item as its own key. The map does not give the order, because the order of a `range` over a map is not specified.
- **Rust.** `items.iter().filter(|&x| seen.insert(x)).cloned().collect()`. `HashSet::insert` returns `true` only if the set did not have the value. The set holds references, so it copies only the items that stay. The email key is `to_ascii_lowercase()`.
- **C++.** A template `unique_by` with a `std::unordered_set` of keys. `insert(key).second` is `true` only if the insertion took place. `unique` uses `std::string_view` keys that point into the input.
- **C.** The function works in place on an array of `const char *` and returns the new length. It compares each item with `strcmp` against the items that it kept, so the time grows with n². The email compare changes A to Z by hand, because `strcasecmp` uses the current locale.
- **Bash.** `awk '!seen[$0]++'` reads one item on each line. An uninitialized awk value is zero, so `!seen[$0]++` is true only for the first copy. A pattern with no action prints the line. For users, `LC_ALL=C awk -F '\t' '!seen[tolower($2)]++'` uses field 2 as the key. The items cannot hold a newline.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. The cases of the users are in `cases/by-key.jsonl`.

- `empty` and `single`: the result is the input.
- `all-same`: three copies of `"x"` become one.
- `no-duplicates`: the result is the input, in the input order. A solution that sorts fails here.
- `not-adjacent` and `adjacent`: copies apart and copies side by side. A solution that removes only adjacent copies fails `not-adjacent`, and it keeps two `"a"` in `adjacent`.
- `empty-string`: `""` is an item, and its later copies go.
- `case`, `whitespace` and `number-like`: `"a"` and `"A"`, `" a"` and `"a "`, and `"1"`, `"01"` and `"1.0"` are all different items.
- `normalization`: U+00E9 and `e` with U+0301 are two items.
- `non-ascii`: CJK text, an emoji above U+FFFF and `ü`.
- In `cases/by-key.jsonl`: `ADA@Example.com` is the same email as `ada@example.com`, the first user wins, two users with the same name and two emails both stay, and `a.b@example.com` is not `ab@example.com`.

## Pitfalls

- **A set does not keep the order.** In Python, `list(set(items))` removes the copies, but a set is an unordered collection. The order can change from one run to the next for strings, because Python gives the hashes of `str` objects a random seed. Use `dict.fromkeys`:

```python wrong id=py-set
print(list(set([30, 10, 30, 20, 10])))
```

```output
[10, 20, 30]
```

```python run id=py-fromkeys
print(list(dict.fromkeys([30, 10, 30, 20, 10])))
```

```output
[30, 10, 20]
```

The same problem is in Java `HashSet`, Go maps and Rust `HashSet`. Their iteration order is not specified.

- **Removing only adjacent copies.** `uniq` compares adjacent lines only. `sort -u` removes all copies, but it changes the order. Go `slices.Compact` also replaces only consecutive runs of equal elements:

```bash wrong id=sh-uniq
printf '%s\n' b a b a | uniq
echo ---
printf '%s\n' b a b a | sort -u
```

```output
b
a
b
a
---
a
b
```

```bash run id=sh-awk
printf '%s\n' b a b a | awk '!seen[$0]++'
```

```output
b
a
```

- **NaN in JavaScript.** A `Set` compares values with SameValueZero, so `NaN` is the same as `NaN`. The idiom with `filter` and `indexOf` uses strict equality, and `indexOf(NaN)` is always -1. So it removes every `NaN`. It also compares each item with all the items before it:

```javascript wrong id=js-indexof-nan
console.log([NaN, 1, NaN].filter((v, i, a) => a.indexOf(v) === i));
```

```output
[ 1 ]
```

```javascript run id=js-set-nan
console.log([...new Set([NaN, 1, NaN])]);
```

```output
[ NaN, 1 ]
```

- **NaN in Python.** A NaN is not equal to itself. But the built-in containers assume that an object is equal to itself, so they skip the test for the same object. So two references to one NaN become one key, and two NaN objects stay two keys:

```python run id=py-nan
nan = float("nan")
print(list(dict.fromkeys([nan, nan])))
print(list(dict.fromkeys([float("nan"), float("nan")])))
```

```output
[nan]
[nan, nan]
```

- **A class with no equality.** In Java, the `equals` of `Object` is true only for the same object. So `distinct` and `HashSet` keep two objects with the same fields. A record gets `equals` and `hashCode` from its components. If you write `equals` yourself, also write `hashCode`, because equal objects must have equal hash codes:

```java wrong id=java-no-equals
import java.util.List;

public class Main {
    static final class Point {
        final int x;
        final int y;

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public String toString() {
            return "(" + x + ", " + y + ")";
        }
    }

    public static void main(String[] args) {
        System.out.println(List.of(new Point(1, 2), new Point(1, 2)).stream().distinct().toList());
    }
}
```

```output
[(1, 2), (1, 2)]
```

```java run id=java-record
import java.util.List;

public class Main {
    record Point(int x, int y) {}

    public static void main(String[] args) {
        System.out.println(List.of(new Point(1, 2), new Point(1, 2)).stream().distinct().toList());
    }
}
```

```output
[Point[x=1, y=2]]
```

- **A key that depends on the locale.** Java `toLowerCase()` uses the default locale. In a Turkish locale, `I` becomes a dotless `ı` (U+0131), so `BILL@EXAMPLE.COM` and `bill@example.com` get different keys. Use `toLowerCase(Locale.ROOT)`. In C#, use `ToLowerInvariant()`. In C and C++, `tolower` uses the current C locale, and POSIX `strcasecmp` uses the current locale too:

```java wrong id=java-turkish
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

public class Main {
    public static void main(String[] args) {
        Locale.setDefault(Locale.forLanguageTag("tr")); // as on a machine with a Turkish locale
        Set<String> seen = new HashSet<>();
        for (String email : new String[] {"bill@example.com", "BILL@EXAMPLE.COM"}) {
            System.out.println(email + " is new: " + seen.add(email.toLowerCase()));
        }
    }
}
```

```output
bill@example.com is new: true
BILL@EXAMPLE.COM is new: true
```

```java run id=java-root
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

public class Main {
    public static void main(String[] args) {
        Locale.setDefault(Locale.forLanguageTag("tr"));
        Set<String> seen = new HashSet<>();
        for (String email : new String[] {"bill@example.com", "BILL@EXAMPLE.COM"}) {
            System.out.println(email + " is new: " + seen.add(email.toLowerCase(Locale.ROOT)));
        }
    }
}
```

```output
bill@example.com is new: true
BILL@EXAMPLE.COM is new: false
```

- **A key that is too loose.** Case folding, trimming and Unicode normalization make more items equal. Do them only when the data needs them, and do them in the key, so that the result keeps the original items. An email address is an example: the solution here lowers only the letters A to Z, and it keeps the dots.

## Related tasks

- [[sort-a-list-of-records]]: sort the records after you remove the copies, or sort them by the same key.
- [[split-a-string]]: split a line into the items of a list.
- [[read-a-file-line-by-line]]: read the items from a file, one on each line.
- [[read-from-standard-input]]: read the lines that `awk '!seen[$0]++'` gets.
- [[reverse-a-string]]: another job where code points and visible characters are not the same.

## References

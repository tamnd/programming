---
tag: WCQ6N9
kind: task
slug: sort-a-list-of-records
name: Sort a list of records
aliases: [sort objects by a field, sort by multiple keys, sort structs, stable sort by key]
area: data-and-values/collection-operations
level: core
summary: Sort a list of records by one field, then by a second field in the opposite direction, with a stable sort so that records with equal keys keep their input order.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/howto/sorting.html
    title: The Python HOWTOs, Sorting Techniques
    locator: Sort Stability and Complex Sorts; Odds and Ends (the reverse parameter)
  - url: https://docs.python.org/3/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Expressions
    locator: 6.10.1 Value comparisons (strings, sequences)
  - url: https://tc39.es/ecma262/multipage/indexed-collections.html#sec-array.prototype.sort
    title: Ecma International, ECMA-262, ECMAScript Language Specification
    locator: 23.1.3.30 Array.prototype.sort, 23.1.3.30.1 SortIndexedProperties, 23.1.3.30.2 CompareArrayElements, 23.1.3.34 Array.prototype.toSorted
  - url: https://tc39.es/ecma262/multipage/abstract-operations.html#sec-islessthan
    title: Ecma International, ECMA-262, ECMAScript Language Specification
    locator: 7.2.12 IsLessThan (strings compare by code unit)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Collator
    title: MDN Web Docs, Intl.Collator
    locator: Using locales
  - url: https://nodejs.org/api/intl.html#embed-the-entire-icu-full-icu
    title: Node.js documentation, Internationalization support
    locator: Embed the entire ICU (full-icu)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html#sorted(java.util.Comparator)
    title: Java SE 25 API, java.util.stream.Stream
    locator: sorted(Comparator)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Comparator.html
    title: Java SE 25 API, java.util.Comparator
    locator: compare, comparingInt, thenComparing(Comparator), reversed
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.18.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.18.2 Additive Operators (+ and -) for Numeric Types
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html#compareTo(java.lang.String)
    title: Java SE 25 API, java.lang.String
    locator: compareTo(String)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/Collator.html
    title: Java SE 25 API, java.text.Collator
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.orderby?view=net-10.0
    title: .NET API reference, Enumerable.OrderBy
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.sort?view=net-10.0
    title: .NET API reference, List<T>.Sort
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.stringcomparer.ordinal?view=net-10.0
    title: .NET API reference, StringComparer.Ordinal
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/best-practices-strings
    title: .NET documentation, Best practices for comparing strings in .NET
    locator: String comparisons that use the current culture; String.CompareTo
  - url: https://learn.microsoft.com/en-us/dotnet/core/extensions/globalization-icu
    title: .NET documentation, Globalization and ICU
    locator: Globalization setup in .NET apps, Linux
  - url: https://pkg.go.dev/slices#SortFunc
    title: Go standard library, package slices
    locator: SortFunc, SortStableFunc
  - url: https://pkg.go.dev/cmp#Or
    title: Go standard library, package cmp
    locator: Compare, Or
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Comparison operators
  - url: https://doc.rust-lang.org/std/primitive.slice.html#method.sort_by
    title: The Rust Standard Library, primitive type slice
    locator: sort_by, sort_unstable_by
  - url: https://en.cppreference.com/w/cpp/algorithm/stable_sort
    title: cppreference.com, std::stable_sort
  - url: https://en.cppreference.com/w/cpp/string/char_traits/cmp
    title: cppreference.com, std::char_traits::eq, std::char_traits::lt
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/qsort.html
    title: The Open Group, POSIX.1-2024, qsort
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/strcmp.html
    title: The Open Group, POSIX.1-2024, strcmp
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sort.html
    title: The Open Group, POSIX.1-2024, sort
    locator: Description; Options (-k, -n, -r, -t); Rationale
  - url: https://www.gnu.org/software/coreutils/manual/html_node/sort-invocation.html
    title: GNU Coreutils manual, sort invocation
    locator: the last-resort comparison and --stable; -k; footnote 1 (LC_ALL)
---

## Task

Sort a list of employee records. Each record has a name, a department and a salary. Sort by department in ascending order. In one department, sort by salary from high to low. If two records have the same department and the same salary, keep them in their input order. A sort that does this is a **stable** sort.

Compare the department names by their characters, not by the rules of a language. So `"Sales"` comes before `"eng"`, because `S` (U+0053) is before `e` (U+0065). The order must be the same on every machine. A sort for people to read is a different job. The section on locales below shows it.

The salaries are integers that fit in 32 bits, and they can be negative. The names contain no tab or newline. They contain only characters below U+D800, so the byte order of UTF-8, the code unit order of UTF-16 and the code point order are the same.

## Solutions

Each solution gives the sort one comparison of two keys. The first key is the department. If the departments are equal, the second key is the salary, in reverse.

- **Python.** `sorted(employees, key=lambda e: (e.dept, -e.salary))`. Tuples compare item by item, and strings compare by code point. `-e.salary` puts high salaries first. Python sorts are always stable.
- **JavaScript and TypeScript.** `toSorted` with a comparison that uses `<` and `>` on the department, then `b.salary - a.salary`. The `||` operator goes to the second key only when the first comparison gives 0. The standard requires a stable sort, and `<` compares UTF-16 code units, so the result does not depend on the locale. The subtraction is safe here, because the numbers are doubles and the salaries are small.
- **Java.** `Comparator.comparing(Employee::dept).thenComparing(Comparator.comparingInt(Employee::salary).reversed())` on `stream().sorted(...)`. For an ordered stream, `sorted` is stable. `String.compareTo` compares UTF-16 values and does not use the locale.
- **C#.** `OrderBy(e => e.Dept, StringComparer.Ordinal).ThenByDescending(e => e.Salary)`. `OrderBy` is a stable sort. Give `StringComparer.Ordinal`, because the default comparer of strings uses the current culture.
- **Go.** `slices.SortStableFunc` on a copy, with `cmp.Or(strings.Compare(a.Dept, b.Dept), cmp.Compare(b.Salary, a.Salary))`. `cmp.Or` returns the first value that is not 0. Go compares strings byte by byte.
- **Rust.** `sort_by` with `a.dept.cmp(&b.dept).then_with(|| b.salary.cmp(&a.salary))`. `sort_by` is stable. `sort_unstable_by` is not.
- **C++.** `std::ranges::stable_sort` with `std::tie(a.dept, b.salary) < std::tie(b.dept, a.salary)`. The salaries change sides, so high salaries come first. `std::string` compares each `char` as `unsigned char`, so UTF-8 sorts by code point.
- **C.** `qsort` does not keep equal elements in order. So the code copies each record with its input position into a new array, and uses the position as the last key. The comparison uses `strcmp`, which compares bytes as `unsigned char`, and `(x > y) - (x < y)` for the numbers.
- **Bash.** `LC_ALL=C sort -s -t $'\t' -k2,2 -k3,3nr` on lines of tab-separated fields. `-k2,2` is field 2 only. `-k3,3nr` is field 3 as a number, in reverse. `-s` keeps the input order of equal keys. `LC_ALL=C` compares bytes on every machine.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `single`: the result is the input.
- `sorted` and `reversed`: input that is already in order, and input in the reverse order.
- `ties`: three records with the same department and salary, with other records between them. They must keep their input order.
- `many-ties`: 40 records with two departments in turn and the same salary. An unstable sort can change their order. On a short list, an unstable sort often gives the stable order by chance, so this case is long.
- `case`: `"Sales"` sorts before `"eng"` in code point order.
- `non-ascii`: `"équipe"` sorts after `"zoo"`, because U+00E9 is after `z`.
- `negative`: 5, 0 and -5 sort as numbers, high to low.
- `overflow`: the salaries 2000000000 and -2000000000. Their difference does not fit in 32 bits.

## Pitfalls

- **Reversing the result instead of the comparison.** A reversed ascending sort is in descending order, but it also reverses the order of equal keys. Python `reverse=True` keeps equal keys in their input order:

```python wrong id=py-reverse-ties
staff = [("Zoe", 100), ("Al", 100), ("Bo", 90)]
print(sorted(staff, key=lambda e: e[1])[::-1])
```

```output
[('Al', 100), ('Zoe', 100), ('Bo', 90)]
```

```python run id=py-reverse-flag
staff = [("Zoe", 100), ("Al", 100), ("Bo", 90)]
print(sorted(staff, key=lambda e: e[1], reverse=True))
```

```output
[('Zoe', 100), ('Al', 100), ('Bo', 90)]
```

- **The default sort of JavaScript compares strings.** With no comparison function, `sort` changes each value to a string first:

```javascript wrong id=js-default-sort
console.log([100, 90, 1000].sort());
```

```output
[ 100, 1000, 90 ]
```

```javascript run id=js-numeric-sort
console.log([100, 90, 1000].sort((a, b) => a - b));
```

```output
[ 90, 100, 1000 ]
```

- **Subtraction in a comparison of 32-bit integers.** `b - a` can overflow. In Java the result keeps only the low-order bits, so its sign is wrong, and the order is wrong. Use `Integer.compare`, `Comparator.comparingInt`, `cmp.Compare` or `(x > y) - (x < y)`:

```java wrong id=java-subtract-overflow
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> salaries = new ArrayList<>(List.of(-2_000_000_000, 2_000_000_000, 0));
        salaries.sort((a, b) -> b - a); // high to low, but b - a overflows
        System.out.println(salaries);
    }
}
```

```output
[-2000000000, 2000000000, 0]
```

```java run id=java-compare
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> salaries = new ArrayList<>(List.of(-2_000_000_000, 2_000_000_000, 0));
        salaries.sort((a, b) -> Integer.compare(b, a));
        System.out.println(salaries);
    }
}
```

```output
[2000000000, 0, -2000000000]
```

- **Code unit order is not code point order.** A character above U+FFFF is two UTF-16 code units, and the first unit is in the range D800 to DBFF. So JavaScript, Java and C# `Ordinal` put U+1F600 before U+FF5E. Python, Go, Rust, C and C++ compare code points or UTF-8 bytes, and they put U+FF5E first:

```javascript run id=js-code-units
const hex = (s) => "U+" + s.codePointAt(0).toString(16).toUpperCase();
console.log(["\u{FF5E}", "\u{1F600}"].sort().map(hex));
```

```output
[ 'U+1F600', 'U+FF5E' ]
```

```python run id=py-code-points
print([f"U+{ord(s):04X}" for s in sorted(["\uFF5E", "\U0001F600"])])
```

```output
['U+FF5E', 'U+1F600']
```

- **An order for people is a different sort.** Code point order puts all capital letters before all small letters, and it puts `é` after `z`. To sort names for people to read, use the collation rules of a locale. `Intl.Collator` in JavaScript and `java.text.Collator` in Java do this:

```javascript run id=js-collator
const depts = ["eng", "Sales", "équipe", "ops"];
console.log(depts.toSorted());
console.log(depts.toSorted(new Intl.Collator("en").compare));
```

```output
[ 'Sales', 'eng', 'ops', 'équipe' ]
[ 'eng', 'équipe', 'ops', 'Sales' ]
```

The rules change from one language to the next. In Swedish, `å`, `ä` and `ö` come after `z`. In Danish, `aa` sorts as `å`, at the end of the alphabet. The cases in `cases/locale.jsonl` test this for the JavaScript, TypeScript and Java solutions, because their results do not depend on the machine. Official Node.js binaries include all of the ICU data, and the JDK supplies its own `RuleBasedCollator`. On Linux, .NET gets its collation rules from the ICU library of the system, so the result of a culture comparison can change from one machine to the next.

- **A default comparer that uses the culture.** In C#, `OrderBy(e => e.Dept)` with no comparer, `List<string>.Sort()` and `string.CompareTo` use the current culture. So the same program can give a different order on a different machine. Give `StringComparer.Ordinal` for a fixed order, or a culture-specific comparer for an order that people read. Also, `List<T>.Sort` is not stable.

- **A sort by two passes in the wrong order.** With a stable sort you can sort by the second key first, then by the first key. The last sort is the main key. If you sort by department first and then by salary, the result is ordered by salary.

- **The last comparison of `sort`.** GNU `sort` compares the whole lines when all the keys are equal. So without `-s`, records with equal keys come out in the order of their names, not in their input order. POSIX does not define `-s`, but its rationale says that a stable option is usually `-s`. With `-k2`, the key goes to the end of the line. Write `-k2,2` for one field.

## Related tasks

- [[split-a-string]]: split a line into the fields of a record.
- [[read-a-file-line-by-line]]: read the records from a file.
- [[reverse-a-string]]: another job where code units and characters are not the same.

## References

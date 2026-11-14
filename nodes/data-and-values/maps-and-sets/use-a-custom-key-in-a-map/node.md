---
tag: VFSMT7
kind: task
slug: use-a-custom-key-in-a-map
name: Use a custom key in a map
aliases: [user type as a map key, struct as a dictionary key, composite key, tuple key, hashable class]
area: data-and-values/maps-and-sets
level: core
summary: Use a point type as the key of a map, with equality and a hash that agree, so that two equal points find one entry.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html
  title: Java SE 25 API, Class Object
  locator: hashCode()
- url: https://doc.rust-lang.org/std/hash/trait.Hash.html
  title: Rust standard library, trait Hash
  locator: Hash and Eq
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Record.html
  title: Java SE 25 API, Class Record
  locator: Class description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record
  title: C# reference, Records
  locator: Value equality
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Rust standard library, struct HashMap
  locator: Struct description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Comparison operators; Map types
- url: https://docs.python.org/3/reference/datamodel.html#object.__hash__
  title: The Python Language Reference, Data model
  locator: object.__hash__
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, Map
  locator: Key equality
- url: https://learn.microsoft.com/en-us/dotnet/api/system.object.gethashcode?view=net-10.0
  title: .NET API reference, Object.GetHashCode method
  locator: Notes to inheritors
- url: https://en.cppreference.com/w/cpp/container/map
  title: cppreference.com, std::map
  locator: Description
- url: https://en.cppreference.com/w/c/string/byte/strcmp
  title: cppreference.com, strcmp (C)
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Count points with a map whose key is a point. The function `count_points(points)` takes a list of points `(x, y)`. It returns one triple `(x, y, count)` for each distinct point, where `count` is the number of times the point is in the list. The triples are in ascending order of `x`, then of `y`. For example, `count_points([(1, 2), (3, 4), (1, 2)])` is `[(1, 2, 2), (3, 4, 1)]`.

Each coordinate fits in a signed 32-bit integer. The list can be empty.

## Solutions

| Language | Key type | Equality and hash |
|---|---|---|
| Python | `@dataclass(frozen=True)` class `Point` | made from the fields |
| JavaScript, TypeScript | the string `"x,y"` | string equality |
| Java | `record Point(int x, int y)` | made from the fields |
| C# | `readonly record struct Point(int X, int Y)` | made from the fields |
| Go | `struct { X, Y int }` | `==` on each field; the map hashes the fields |
| Rust | `struct Point` with `#[derive(PartialEq, Eq, Hash)]` | made from the fields |
| C | `struct point` in a sorted array | a compare function |
| C++ | `struct Point` with `operator==` and a hash type | written by hand |
| Bash | the string `"x,y"` | string equality |

- **Equality and hash together.** A hash map first finds a bucket with the hash of the key, then compares the keys in it for equality. Thus two equal keys must have equal hashes. In Java, "If two objects are equal according to the equals method, then calling the hashCode method on each of the two objects must produce the same integer result."[^1] In Rust, "When implementing both Hash and Eq, it is important that the following property holds: k1 == k2 -> hash(k1) == hash(k2)"[^2]. Two unequal keys can have one hash. That makes the map slower, but not wrong.
- **Let the language make them.** A record type gets equality and a hash from all of its fields, so the two agree. For a Java record, "The implicit declaration of the Object.equals(Object), Object.hashCode(), and Object.toString() methods are derived from all of the component fields."[^3] For a C# record, "two objects are equal if they are of the same type and store the same values."[^4] Rust keys must implement `Eq` and `Hash`, "although this can frequently be achieved by using #[derive(PartialEq, Eq, Hash)]."[^5] In Go, "Struct types are comparable if all their field types are comparable."[^6] Python `dataclass(frozen=True)` makes `__eq__` and `__hash__` from the fields.
- **Keys must not change.** If a key changes while it is in a hash map, its hash changes, and the map looks in the wrong bucket. The frozen dataclass, the Java record and the `readonly` record struct have fields that cannot change. Another way is a tuple key, such as `(x, y)` in Python and Rust.
- **JavaScript and Bash.** These languages have no hash for a user type. A JavaScript `Map` compares objects by identity, and a Bash associative array has string keys. Both solutions use the string `"x,y"` as the key. The comma is necessary: without it, `(1, 23)` and `(12, 3)` both give `123`.
- **C++.** `std::unordered_map` needs a hash type for a user key, and `std::hash` has no specialization for `Point`. The solution puts `x` and `y` in one 64-bit value and hashes it. A `std::map` needs only an order, for example `std::tie(a.x, a.y) < std::tie(b.x, b.y)`.
- **C.** C has no map type and no `==` for structs. The solution sorts the points with `qsort`, so that equal points are next to each other, then counts each run in one pass.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `swap`: `(1, 2)` and `(2, 1)` are two points. A hash that adds `x` and `y` gives both the same hash, which is slow but correct.
- `empty`: no points give no triples.
- `negative`: negative coordinates sort before 0.
- `joined-digits`: `(1, 23)` and `(12, 3)` are two points. A string key without a separator makes them one key.
- `all-same`: three equal points give one triple with the count 3.

## Pitfalls

- **Python `__eq__` without `__hash__` makes a class unhashable.** "A class that overrides __eq__() and does not define __hash__() will have its __hash__() implicitly set to None."[^7] Then instances "will raise an appropriate TypeError when a program attempts to retrieve their hash value"[^7]. Define `__hash__` from the same fields as `__eq__`:

```python run id=python-eq-without-hash
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __eq__(self, other):
        return (self.x, self.y) == (other.x, other.y)


class Fixed(Point):
    def __hash__(self):
        return hash((self.x, self.y))


try:
    counts = {Point(1, 2): 1}
except TypeError as e:
    print(type(e).__name__)
print({Fixed(1, 2): 1}[Fixed(1, 2)])
```

```output for=python-eq-without-hash
TypeError
1
```

- **JavaScript a Map compares object keys by identity.** "for object keys, equality is based on object identity."[^8] Two object literals with the same fields are two keys, so a read with a new object finds nothing. Use a string key:

```javascript run id=javascript-object-key
const counts = new Map();
counts.set({ x: 1, y: 2 }, 1);
console.log(counts.get({ x: 1, y: 2 }));
counts.set("1,2", 1);
console.log(counts.get("1,2"));
```

```output for=javascript-object-key
undefined
1
```

- **Java `equals` without `hashCode` loses keys.** The class below overrides `equals` but keeps the `hashCode` of `Object`, so two equal points have two hashes, and `HashMap` looks in the wrong bucket. A record gets both from its fields[^3]:

```java run id=java-equals-without-hashcode
import java.util.HashMap;
import java.util.Map;

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

  record Fixed(int x, int y) {}

  public static void main(String[] args) {
    Map<Point, Integer> a = new HashMap<>();
    a.put(new Point(1, 2), 1);
    System.out.println(a.get(new Point(1, 2)));
    Map<Fixed, Integer> b = new HashMap<>();
    b.put(new Fixed(1, 2), 1);
    System.out.println(b.get(new Fixed(1, 2)));
  }
}
```

```output for=java-equals-without-hashcode
null
1
```

- **C# a key that changes is lost.** Override `GetHashCode` for a mutable type only if "You can ensure that the hash code of a mutable object does not change while the object is contained in a collection that relies on its hash code. Otherwise, you might think that the mutable object is lost in the hash table."[^9] Below, the key changes after the `Add`. The dictionary still has 1 entry, but no point finds it:

```csharp run id=csharp-mutable-key
using System;
using System.Collections.Generic;

var p = new Point { X = 1, Y = 2 };
var counts = new Dictionary<Point, int> { [p] = 1 };
p.X = 5;
Console.WriteLine(counts.ContainsKey(p) + " " + counts.ContainsKey(new Point { X = 1, Y = 2 }));
Console.WriteLine(counts.Count);

class Point
{
    public int X { get; set; }
    public int Y { get; set; }

    public override bool Equals(object? obj) => obj is Point q && q.X == X && q.Y == Y;

    public override int GetHashCode() => HashCode.Combine(X, Y);
}
```

```output for=csharp-mutable-key
False False
1
```

- **Go a pointer field compares the address.** "Two pointer values are equal if they point to the same variable or if both have value nil."[^6] A struct key with a pointer field is equal to another key only if both point to one variable, not if the two values are equal. Use the value as the field:

```go run id=go-pointer-field-key
package main

import "fmt"

type Key struct {
	name *string
}

func main() {
	a, b := "ann", "ann"
	counts := map[Key]int{}
	counts[Key{&a}]++
	counts[Key{&b}]++
	fmt.Println(len(counts))
	byName := map[string]int{}
	byName[a]++
	byName[b]++
	fmt.Println(len(byName))
}
```

```output for=go-pointer-field-key
2
1
```

- **Rust derive uses all of the fields.** The derived `PartialEq` and `Hash` agree, but they use each field. If two points with different labels must be one key, the label must not be in the key. Use a key type with only `x` and `y`, or write `PartialEq` and `Hash` by hand from the same fields:

```rust run id=rust-derive-all-fields
use std::collections::HashMap;

#[derive(PartialEq, Eq, Hash)]
struct Place {
    x: i32,
    y: i32,
    label: &'static str,
}

fn main() {
    let mut counts = HashMap::new();
    *counts.entry(Place { x: 1, y: 2, label: "home" }).or_insert(0) += 1;
    *counts.entry(Place { x: 1, y: 2, label: "work" }).or_insert(0) += 1;
    println!("{}", counts.len());
    let mut by_point: HashMap<(i32, i32), i32> = HashMap::new();
    *by_point.entry((1, 2)).or_insert(0) += 1;
    *by_point.entry((1, 2)).or_insert(0) += 1;
    println!("{}", by_point.len());
}
```

```output for=rust-derive-all-fields
2
1
```

- **C++ a `std::map` order that skips a field joins keys.** A `std::map` has no `==`. Two keys are equivalent "if neither compares less than the other"[^10], that is, if `!comp(a, b) && !comp(b, a)`. An order on `x` only makes `(1, 2)` and `(1, 5)` one key, and the map keeps the first key:

```cpp run id=cpp-map-order-one-field
#include <iostream>
#include <map>

struct Point {
    int x;
    int y;
};

struct ByX {
    bool operator()(const Point &a, const Point &b) const { return a.x < b.x; }
};

struct ByXY {
    bool operator()(const Point &a, const Point &b) const {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    }
};

int main() {
    std::map<Point, int, ByX> a;
    a[{1, 2}]++;
    a[{1, 5}]++;
    std::cout << a.size() << ' ' << a.begin()->first.y << ' ' << a.begin()->second << '\n';
    std::map<Point, int, ByXY> b;
    b[{1, 2}]++;
    b[{1, 5}]++;
    std::cout << b.size() << '\n';
}
```

```output for=cpp-map-order-one-field
1 2 2
2
```

- **C a string key needs a separator.** A C program that keeps keys as text, for example for `hsearch`, often joins the fields with `snprintf`. `strcmp` "Compares two null-terminated byte strings lexicographically."[^11] It sees only the text, so the joined keys of `(1, 23)` and `(12, 3)` are equal. Put a separator between the fields:

```c run id=c-joined-key
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[32];
    char b[32];
    snprintf(a, sizeof a, "%d%d", 1, 23);
    snprintf(b, sizeof b, "%d%d", 12, 3);
    printf("%s %s %d\n", a, b, strcmp(a, b) == 0);
    snprintf(a, sizeof a, "%d,%d", 1, 23);
    snprintf(b, sizeof b, "%d,%d", 12, 3);
    printf("%s %s %d\n", a, b, strcmp(a, b) == 0);
    return 0;
}
```

```output for=c-joined-key
123 123 1
1,23 12,3 0
```

- **Bash keys compare as text.** Indexed arrays use numbers, but "associative arrays use arbitrary strings."[^12] Thus `1,2` and `01,2` are two keys, though they are one point. Put each coordinate in one form before you make the key. `10#` reads a number in base 10, so a leading 0 does not make it octal:

```bash run id=bash-text-keys
declare -A seen=()
for p in 1,2 01,2 1,2; do
  seen[$p]=1
done
echo "${#seen[@]}"
seen=()
for p in 1,2 01,2 1,2; do
  x=${p%%,*} y=${p#*,}
  seen[$((10#$x)),$((10#$y))]=1
done
echo "${#seen[@]}"
```

```output for=bash-text-keys
2
1
```

## Related tasks

- [[create-a-map]]: add, replace and delete keys.
- [[use-a-map-with-a-default-value]]: count with the default 0.
- [[count-the-items-of-each-kind]]: count items with a map.
- [[create-a-set]]: a set of points uses the same rules for its items.

## References

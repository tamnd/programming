---
tag: Z8JRWK
kind: task
slug: define-an-enumeration
name: Define an enumeration
aliases: [define an enum, enum type, enumerated type, list the values of an enum, enum to string, string to enum, enum to int]
area: types/algebraic-data-types
level: intro
summary: Define an enum of the seven weekdays, convert a name to a value and back, and step through the values by their positions, in each tier 1 language.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/enum.html
    title: The Python Standard Library, enum
    locator: the introduction; EnumType; StrEnum
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Enum.html
    title: Java SE 27 API, Class Enum
    locator: ordinal(); valueOf(Class, String)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/enum
    title: C# reference, Enumeration types
    locator: the introduction; Conversions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Iota
    title: The Go Programming Language Specification
    locator: Iota
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/items/enumerations.html
    title: The Rust Reference, Enumerations
    locator: Casting
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.2.2
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.7.2.2 Enumeration specifiers
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/language/enum
    title: cppreference.com, Enumeration declaration
    locator: Scoped enumerations
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/freeze
    title: MDN Web Docs, Object.freeze()
    locator: Description
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/enums.html
    title: TypeScript Handbook, Enums
    locator: Objects vs Enums
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: 6.7 Arrays
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.enum.tryparse?view=net-10.0
    title: .NET API reference, Enum.TryParse method
    locator: Remarks
    accessed: 2026-11-16
---

## Task

Define an enumeration `Day` with the seven values `monday` to `sunday`, in that order. Then use it in the function `cycle(start, steps)`. The function converts the name `start` to a `Day`, and returns `steps` names that begin at that day and go on in the order of the enumeration. After `sunday` comes `monday` again. For example, `cycle("friday", 4)` is `["friday", "saturday", "sunday", "monday"]`.

A name must be one of the seven names, exactly and in lower case. For any other text, such as `"Friday"`, `"funday"` or `"3"`, the result is the empty list. `steps` is from 0 to 100.

The task shows the four things that a program does with an enumeration: it lists the values in order, converts a name to a value, converts a value to its position and back, and converts a value to its name. For the idea behind enumerations, see [[enumeration]].

## Solutions

| Language | Definition | All values in order | Value to position |
|---|---|---|---|
| Python | `class Day(Enum)` with `MONDAY = "monday"` | `list(Day)` | the index in `list(Day)` |
| JavaScript | `Object.freeze({ MONDAY: "monday", ... })` | `Object.values(Day)` | `indexOf` |
| TypeScript | an object with `as const`, and `type Day` from its values | `Object.values(Day)` | `indexOf` |
| Java | `enum Day { MONDAY, ... }` | `Day.values()` | `d.ordinal()` |
| C# | `enum Day { Monday, ... }` | `Enum.GetValues<Day>()` | `(int)d` |
| Go | `type Day int` and constants with `iota` | a loop from `Monday` to `numDays - 1` | the value itself |
| Rust | `enum Day { Monday, ... }` | a constant array `Day::ALL` | `d as usize` |
| C | `enum day { MONDAY, ..., DAY_COUNT }` | a loop from `MONDAY` to `DAY_COUNT - 1` | the value itself |
| C++ | `enum class Day { Monday, ... }` | a loop over `0` to `6` | `static_cast<int>(d)` |
| Bash | the indexed array `DAYS=(monday ... sunday)` | `"${DAYS[@]}"` | the array index |

- **Python.** An enumeration "is a set of symbolic names (members) bound to unique values"[^1]. The members come in order: iteration "Returns each member in cls in definition order"[^1]. `Day(value)` looks up a member by value, and `Day[name]` looks up a member by name. For a name that is not in the class, the index form "raises a KeyError"[^1].
- **Java.** Each enum class has a static `values()` method. "All the constants of an enum class can be obtained by calling the implicit public static T[] values() method of that class."[^2] The position comes from `ordinal()`, which "Returns the ordinal of this enumeration constant (its position in its enum declaration, where the initial constant is assigned an ordinal of zero)."[^2] The Java API also says: "Most programmers will have no use for this method."[^2] Do not store an ordinal in a file or a database, because it changes when someone puts a new constant in the middle.
- **C#.** "An enumeration type (or enum type) is a value type defined by a set of named constants of the underlying integral numeric type."[^3] The members "start with zero and increase by one following the definition text order."[^3] Casts go both ways: "For any enumeration type, explicit conversions exist between the enumeration type and its underlying integral type."[^3]
- **Go.** Go has no enum keyword. The solution defines a named integer type and a block of constants: "Within a constant declaration, the predeclared identifier iota represents successive untyped integer constants."[^4] A `String` method gives the names. The last constant `numDays` counts the days.
- **Rust.** A Rust enum has no built-in list of its variants, so the solution writes the array `Day::ALL` and a `match` for the names. The position is a cast: "If an enumeration is unit-only (with no tuple and struct variants), then its discriminant can be directly accessed with a numeric cast"[^5]. There is no cast from an integer to an enum, so the solution indexes `Day::ALL`.
- **C.** "The identifiers in an enumerator list are declared as constants that have type int and may appear wherever such are permitted."[^6] C keeps no names at run time, so the solution has a table of names with one entry for each constant. The last constant, `DAY_COUNT`, is the size of the table.
- **C++.** The solution uses a scoped enumeration, `enum class`. "There are no implicit conversions from the values of a scoped enumerator to integral types, although static_cast may be used to obtain the numeric value of the enumerator."[^7]
- **JavaScript and TypeScript.** JavaScript has no enum. A frozen object takes its place, because "A frozen object can no longer be changed"[^8]. TypeScript has an `enum` keyword, but its handbook gives an object with `as const` as an alternative. Its main reason is that this form "keeps your codebase aligned with the state of JavaScript"[^9].
- **Bash.** Bash has no enum. An indexed array keeps the names in order, and the index of a name is its value. "Bash provides one-dimensional indexed and associative array variables."[^10]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `wrap`: after `sunday` comes `monday`. The solutions use the position modulo 7.
- `zero-steps`: 0 steps give an empty list, also for a valid name.
- `more-than-a-week`: 9 steps from `sunday` go round the week once and give `sunday` and `monday` twice.
- `unknown`, `empty-name`: a name that is not a day gives an empty list.
- `wrong-case`: `"Friday"` is not a name. A parse that ignores case, such as `valueOf(start.toUpperCase())` in Java, is wrong here.
- `number`: `"3"` is not a name. In C#, `Enum.TryParse` accepts it as the value 3 (see Pitfalls).

## Pitfalls

- **Python an `Enum` member is not equal to its value.** A member is not a string, so a comparison with the string value is false. Compare with `.value`, or use `StrEnum`, which "is the same as Enum , but its members are also strings and can be used in most of the same places that a string can be used."[^1]

```python run id=python-member-not-value
from enum import Enum, StrEnum


class Day(Enum):
    MONDAY = "monday"


class Day2(StrEnum):
    MONDAY = "monday"


print("yes" if Day.MONDAY == "monday" else "no")
print("yes" if Day.MONDAY.value == "monday" else "no")
print("yes" if Day2.MONDAY == "monday" else "no")
```

```output for=python-member-not-value
no
yes
yes
```

- **JavaScript an object that is not frozen can change.** Any code can give a new value to a property of a plain object, so the "enum" changes for the whole program. `Object.freeze` stops that[^8]. Outside strict mode, a write to a frozen object fails with no error, so `Reflect.set` shows the result:

```javascript run id=javascript-unfrozen-enum
const Loose = { MONDAY: "monday" };
Loose.MONDAY = "tuesday";
console.log(Loose.MONDAY);
const Day = Object.freeze({ MONDAY: "monday" });
console.log(Reflect.set(Day, "MONDAY", "tuesday") ? "yes" : "no", Day.MONDAY);
```

```output for=javascript-unfrozen-enum
tuesday
no monday
```

- **Java `valueOf` is case-sensitive.** "The name must match exactly an identifier used to declare an enum constant in this class."[^2] For any other text, it throws an exception and does not return null:

```java run id=java-valueof-case
public class Main {
  enum Day {
    MONDAY,
    TUESDAY
  }

  public static void main(String[] args) {
    System.out.println(Day.valueOf("MONDAY"));
    try {
      Day.valueOf("monday");
    } catch (IllegalArgumentException e) {
      System.out.println("IllegalArgumentException");
    }
  }
}
```

```output for=java-valueof-case
MONDAY
IllegalArgumentException
```

- **C# `Enum.TryParse` accepts any number.** "If value is the string representation of an integer that does not represent an underlying value of the TEnum enumeration, the method returns an enumeration member whose underlying value is value converted to an integral type."[^11] Thus `"9"` parses to a `Day` that is not a day. Check the result with `Enum.IsDefined`:

```csharp run id=csharp-tryparse-number
using System;

bool ok = Enum.TryParse<Day>("9", out var d);
Console.WriteLine((ok ? "yes" : "no") + " " + d);
Console.WriteLine(Enum.IsDefined(d) ? "yes" : "no");

enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }
```

```output for=csharp-tryparse-number
yes 9
no
```

- **Go any integer is a `Day`.** The constants are integers, and a `Day` can hold any `int` value. The compiler does not check that a value is one of the constants. Check the range before you use the value as an index:

```go run id=go-any-int-day
package main

import "fmt"

type Day int

const (
	Monday Day = iota
	Tuesday
	numDays
)

func main() {
	var d Day = 9
	fmt.Println(int(d))
	if d < 0 || d >= numDays {
		fmt.Println("not a day")
	}
}
```

```output for=go-any-int-day
9
not a day
```

- **C an enum variable can hold other numbers.** "Each enumerated type shall be compatible with char, a signed integer type, or an unsigned integer type."[^6] A variable of the type can hold a number that is not one of the constants, and a `switch` on it then goes to `default`:

```c run id=c-enum-other-number
#include <stdio.h>

enum day { MONDAY, TUESDAY, DAY_COUNT };

int main(void) {
    enum day d = 42;
    switch (d) {
    case MONDAY:
        printf("monday\n");
        break;
    case TUESDAY:
        printf("tuesday\n");
        break;
    default:
        printf("not a day: %d\n", (int)d);
    }
    return 0;
}
```

```output for=c-enum-other-number
not a day: 42
```

- **C++ an unscoped enum converts to `int`.** The values of a plain `enum` take part in arithmetic and comparisons with numbers with no cast, so a mistake compiles. A scoped `enum class` needs a `static_cast`[^7]:

```cpp run id=cpp-unscoped-enum
#include <iostream>

enum Fruit { apple, pear };
enum class Day { monday, tuesday };

int main() {
    int n = pear + 1;
    std::cout << n << ' ' << (pear == 1 ? "yes" : "no") << '\n';
    std::cout << static_cast<int>(Day::tuesday) + 1 << '\n';
}
```

```output for=cpp-unscoped-enum
2 yes
2
```

- **Bash an index past the end gives empty text.** A read of `${DAYS[7]}` gives an empty string and no error. A negative index counts from the end: "an index of -1 refers to the last element"[^10]. Use the index modulo the length:

```bash run id=bash-index-past-end
DAYS=(monday tuesday wednesday thursday friday saturday sunday)
echo "[${DAYS[7]}] [${DAYS[-1]}]"
i=6
echo "${DAYS[(i + 1) % ${#DAYS[@]}]}"
```

```output for=bash-index-past-end
[] [sunday]
monday
```

## Related tasks

- [[use-an-enum-with-data]]: an enum whose cases carry values.
- [[define-a-record-type]]: a type with named fields.
- [[use-a-tuple]]: group values by position.

## References

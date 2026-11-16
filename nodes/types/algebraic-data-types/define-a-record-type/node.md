---
tag: 8HNCXT
kind: task
slug: define-a-record-type
name: Define a record type
aliases: [define a struct, define a data class, define a record class, make a record, struct type, plain data type]
area: types/algebraic-data-types
level: intro
summary: Define a person record with a name and an age, build it, read its fields and make a changed copy, and get equality and printing from the language.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/dataclasses.html
    title: The Python Standard Library, dataclasses
    locator: Module contents; replace(); Mutable default values
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Record.html
    title: Java SE 27 API, Class Record
    locator: Class description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record
    title: C# reference, Records
    locator: Nondestructive mutation; Built-in formatting for display
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/appendix-03-derivable-traits.html
    title: The Rust Programming Language, Appendix C, Derivable Traits
    locator: Debug for Programmer Output; PartialEq and Eq for Equality Comparisons
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/objects.html#readonly-properties
    title: TypeScript Handbook, Object Types
    locator: readonly Properties
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch05-01-defining-structs.html
    title: The Rust Programming Language, 5.1 Defining and Instantiating Structs
    locator: Creating Instances with Struct Update Syntax
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Spread_syntax
    title: MDN Web Docs, Spread syntax
    locator: Spread in object literals
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Struct types; Comparison operators; Slice types
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.16.1
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.5.16.1 Simple assignment; 6.5.9 Equality operators
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/language/default_comparisons
    title: cppreference.com, Default comparisons
    locator: Equality comparison
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: 3.5.7 Word Splitting; 6.7 Arrays
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/freeze
    title: MDN Web Docs, Object.freeze()
    locator: What is shallow freeze?
    accessed: 2026-11-16
---

## Task

Define a record type `Person` with two fields, `name` (a string) and `age` (an integer). Then use it in the function `birthday(people, name)`. The function takes a list of people and a name. It returns a new list with the people in the same order. Each person whose name is equal to `name`, character for character, has the age plus 1. The other people do not change. The function must not change the input list or its records. For example, `birthday([("Ada", 36), ("Alan", 41)], "Ada")` is `[("Ada", 37), ("Alan", 41)]`.

The task shows the four things that a program does with a record: it builds one, reads a field by name, makes a copy with one field changed, and compares or prints records. For the idea behind records, see [[record-type]].

## Solutions

| Language | Record type | Copy with a change | Equality and printing |
|---|---|---|---|
| Python | `@dataclass(frozen=True) class Person` | `replace(p, age=p.age + 1)` | made from the fields |
| JavaScript | a frozen object `{ name, age }` | `{ ...p, age: p.age + 1 }` | by reference; write your own |
| TypeScript | `type Person = { readonly name: string; readonly age: number }` | `{ ...p, age: p.age + 1 }` | by reference; write your own |
| Java | `record Person(String name, int age)` | `new Person(name, age + 1)` | made from the components |
| C# | `record Person(string Name, int Age)` | `p with { Age = p.Age + 1 }` | made from the properties |
| Go | `type Person struct { Name string; Age int }` | assign, then change the copy | `==` on each field; `%+v` |
| Rust | `struct Person` with `#[derive(Clone, Debug, PartialEq, Eq)]` | `Person { age: p.age + 1, ..p.clone() }` | from the derives |
| C | `struct person { const char *name; int age; }` | assign, then change the copy | none; write your own |
| C++ | `struct Person` with `operator==` set to `= default` | copy, then change the copy | `==` from the members; no printing |
| Bash | the string `"name\|age"` | split, change, join | string equality |

- **The language writes the methods.** Python's `dataclasses` module "provides a decorator and functions for automatically adding generated special methods such as __init__() and __repr__() to user-defined classes."[^1] The generated `__eq__` "compares the class by comparing each field in order."[^1] For a Java record, "The implicit declaration of the Object.equals(Object) , Object.hashCode() , and Object.toString() methods are derived from all of the component fields."[^2] C# records "have a compiler-generated ToString method that displays the names and values of public properties and fields."[^3] In Rust, the derives do the same work: "The Debug trait enables debug formatting in format strings"[^4], and "When PartialEq is derived on structs, two instances are equal only if all fields are equal, and the instances are not equal if any fields are not equal."[^4]
- **Fields that cannot change.** The solutions keep each record as it is and make a new one for the older person. With `frozen=True`, "assigning to fields will generate an exception."[^1] A Java record class "is a shallowly immutable, transparent carrier for a fixed set of values, called the record components."[^2] "A positional record class and a positional readonly record struct declare init-only properties."[^3] In TypeScript, `readonly` is a check at compile time only. The handbook says that it does not change the behavior at run time[^5]. The JavaScript solution thus also calls `Object.freeze`.
- **A copy with one change.** Python's `replace` "Creates a new object of the same type as obj, replacing fields with values from changes."[^1] In C#, "A with expression creates a new record instance that's a copy of an existing record instance, but with specified properties and fields modified."[^3] In Rust, the struct update syntax fills the other fields: "The syntax .. specifies that the remaining fields not explicitly set should have the same value as the fields in the given instance."[^6] It moves the fields that are not `Copy`, so the solution writes `..p.clone()`: "we can no longer use user1 after creating user2 because the String in the username field of user1 was moved into user2"[^6]. JavaScript uses spread syntax: "You can use spread syntax to merge multiple objects into one new object."[^7]
- **Go and C copy on assignment.** A Go struct "is a sequence of named elements, called fields, each of which has a name and a type."[^8] Assignment copies all of the fields, so the Go solution changes the copy `p` in the loop and not `people[i]`. "Struct types are comparable if all their field types are comparable. Two struct values are equal if their corresponding non-blank field values are equal."[^8] C struct assignment copies all of the members too: the left operand can have "an atomic, qualified, or unqualified version of a structure or union type compatible with the type of the right"[^9]. But C has no `==` for structs. The operands of `==` must be numbers or pointers, for example "both operands are pointers to qualified or unqualified versions of compatible types"[^9], so the C test compares the fields one at a time.
- **C++.** "An operator== for a class type can be defined as defaulted with return type bool ."[^10] "The default equality comparison between x and y is performed by comparing corresponding subobjects x_i and y_i with increasing i order."[^10] C++ has no generated printing for a struct.
- **Bash.** Bash has no record type. Its arrays hold strings: "Bash provides one-dimensional indexed and associative array variables."[^11] The solution keeps each person as one string with a `|` between the fields, so a name must not contain `|`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no people give an empty list.
- `not-found`: a name that is not in the list changes nothing.
- `duplicate-name`: two people can have one name. Both get older, and the person between them does not change.
- `zero-age`: an age of 0 is a valid age.
- `non-ascii`: `Zoë` and `Zoe` are two names. The comparison is on the characters, with no change to the accents.
- `case`: `ada` is not `Ada`. The comparison is case-sensitive.

## Pitfalls

- **Python a mutable default is shared.** A default value in a class body is one object for all instances. "Instead, the @dataclass decorator will raise a ValueError if it detects an unhashable default parameter."[^1] Use `field(default_factory=list)`, so that each record gets a new list:

```python run id=python-mutable-default
from dataclasses import dataclass, field

try:

    @dataclass
    class Team:
        members: list = []

except ValueError as e:
    print(type(e).__name__)


@dataclass
class Fixed:
    members: list = field(default_factory=list)


a, b = Fixed(), Fixed()
a.members.append("Ada")
print(b.members)
```

```output for=python-mutable-default
ValueError
[]
```

- **JavaScript spread and freeze are shallow.** "A single spread creates a shallow copy of the original object"[^7], and Object.freeze does not freeze the objects inside the object "(freeze is shallow)"[^12]. A copy of a frozen record shares its arrays with the original. Copy each nested array or object that the code will change:

```javascript run id=javascript-shallow-copy
const ada = Object.freeze({ name: "Ada", tags: ["math"] });
const copy = { ...ada, age: 36 };
copy.tags.push("code");
console.log(ada.tags.join(","));
const deep = { ...ada, tags: [...ada.tags] };
deep.tags.push("chess");
console.log(ada.tags.join(","));
```

```output for=javascript-shallow-copy
math,code
math,code
```

- **Java a record with a list component can change.** A record is only shallowly immutable[^2]. Its field keeps a reference to the list that the caller gave, and the caller can still change that list. Copy the list in a compact constructor:

```java run id=java-shallow-record
import java.util.ArrayList;
import java.util.List;

public class Main {
  record Team(String name, List<String> members) {}

  record Fixed(String name, List<String> members) {
    Fixed {
      members = List.copyOf(members);
    }
  }

  public static void main(String[] args) {
    List<String> list = new ArrayList<>(List.of("Ada"));
    Team t = new Team("a", list);
    Fixed f = new Fixed("a", list);
    list.add("Alan");
    System.out.println(t.members());
    System.out.println(f.members());
  }
}
```

```output for=java-shallow-record
[Ada, Alan]
[Ada]
```

- **C# `with` copies references.** "The result of a with expression is a shallow copy."[^3] The copy and the original share an array property, so a change through one shows in the other:

```csharp run id=csharp-with-shallow
using System;

var a = new Person("Ada", new[] { "math" });
var b = a with { Name = "Alan" };
b.Tags[0] = "code";
Console.WriteLine(a.Tags[0]);
Console.WriteLine(ReferenceEquals(a.Tags, b.Tags) ? "yes" : "no");

record Person(string Name, string[] Tags);
```

```output for=csharp-with-shallow
code
yes
```

- **Go a slice field is shared after a copy.** Struct assignment copies the slice, but "A slice is a descriptor for a contiguous segment of an underlying array"[^8], so the copy and the original use one array. Clone the slice before you change it:

```go run id=go-slice-field
package main

import (
	"fmt"
	"slices"
)

type Person struct {
	Name string
	Tags []string
}

func main() {
	a := Person{"Ada", []string{"math"}}
	b := a
	b.Tags[0] = "code"
	fmt.Println(a.Tags[0])
	c := a
	c.Tags = slices.Clone(a.Tags)
	c.Tags[0] = "chess"
	fmt.Println(a.Tags[0])
}
```

```output for=go-slice-field
code
code
```

- **C a pointer member copies the address, not the text.** Struct assignment copies each member[^9]. For a `char *` member, that is the address, so two records point to one buffer. An array member is copied with the struct:

```c run id=c-pointer-member
#include <stdio.h>
#include <string.h>

struct person {
    char *name;
    int age;
};

struct fixed {
    char name[16];
    int age;
};

int main(void) {
    char buf[16] = "Ada";
    struct person a = {buf, 36};
    struct person b = a;
    strcpy(b.name, "Bob");
    printf("%s\n", a.name);
    struct fixed c = {"Ada", 36};
    struct fixed d = c;
    strcpy(d.name, "Bob");
    printf("%s\n", c.name);
    return 0;
}
```

```output for=c-pointer-member
Bob
Ada
```

- **C++ a defaulted `==` compares pointers by address.** The defaulted `operator==` uses `==` on each member[^10]. For a `const char *` member, two equal names in two buffers are not equal. Use `std::string`:

```cpp run id=cpp-pointer-member
#include <iostream>
#include <string>

struct Raw {
    const char *name;
    int age;
    bool operator==(const Raw &) const = default;
};

struct Fixed {
    std::string name;
    int age;
    bool operator==(const Fixed &) const = default;
};

int main() {
    char a[] = "Ada";
    char b[] = "Ada";
    std::cout << (Raw{a, 36} == Raw{b, 36} ? "yes" : "no") << '\n';
    std::cout << (Fixed{a, 36} == Fixed{b, 36} ? "yes" : "no") << '\n';
}
```

```output for=cpp-pointer-member
no
yes
```

- **Bash an unquoted expansion splits a record.** "The shell scans the results of parameter expansion, command substitution, and arithmetic expansion that did not occur within double quotes for word splitting."[^11] A name with a space then becomes two words. Put the array expansion in double quotes:

```bash run id=bash-word-splitting
people=('Mary Ann|30' 'Ada|36')
for p in ${people[@]}; do
  echo "[$p]"
done
for p in "${people[@]}"; do
  echo "[$p]"
done
```

```output for=bash-word-splitting
[Mary]
[Ann|30]
[Ada|36]
[Mary Ann|30]
[Ada|36]
```

## Related tasks

- [[use-a-tuple]]: group values by position, not by name.
- [[define-an-enumeration]]: a type with a fixed set of named values.
- [[use-an-enum-with-data]]: a type whose cases are records.
- [[use-a-custom-key-in-a-map]]: a record as the key of a map.

## References

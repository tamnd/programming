---
tag: NEW-immutability
kind: concept
slug: immutability
name: Immutability
aliases: [immutable object, immutable value, read-only]
area: data-and-values/mutability
level: core
summary: >-
  An immutable value cannot change after the program makes it. A program that needs a different value makes a fresh one, so every holder of the value sees the same state.
applies_to: core-languages
viz: optional
formal: none
see_also: [pointers-and-references, closures, garbage-collection]
same_as:
  wikidata: Q23828
  wikipedia: Immutable_object
sources:
  - url: https://docs.python.org/3.14/glossary.html#term-immutable
    title: Python Glossary, Python 3.14
    locator: immutable
  - url: https://docs.python.org/3.14/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.1 Objects, values and types
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
    title: Java SE 25 API, java.lang.String
    locator: class description
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.4.5p7
    title: "N1570, Committee Draft of ISO/IEC 9899:2011 (C11)"
    locator: 6.4.5 String literals, paragraph 7
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-17.html#jls-17.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 17.5 final Field Semantics
  - url: https://docs.python.org/3.14/glossary.html#term-hashable
    title: Python Glossary, Python 3.14
    locator: hashable
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record#immutability
    title: "C# language reference, Records"
    locator: Immutability; Nondestructive mutation
  - url: https://doc.rust-lang.org/book/ch03-01-variables-and-mutability.html#variables-and-mutability
    title: The Rust Programming Language
    locator: 3.1 Variables and Mutability
  - url: https://tc39.es/ecma262/2026/#sec-blockdeclarationinstantiation
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 14.2.3 BlockDeclarationInstantiation
  - url: https://tc39.es/ecma262/2026/#sec-declarative-environment-records-setmutablebinding-n-v-s
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 9.1.1.1.5 SetMutableBinding
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.12.4
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.12.4 final Variables
  - url: https://kotlinlang.org/docs/basic-syntax.html#variables
    title: Kotlin documentation, Basic syntax
    locator: Variables
  - url: https://www.gnu.org/software/bash/manual/html_node/Bourne-Shell-Builtins.html#index-readonly
    title: Bash Reference Manual
    locator: 4.1 Bourne Shell Builtins, readonly
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/set-variable?view=powershell-7.6#-option
    title: PowerShell documentation, Set-Variable
    locator: parameters -Option and -Force
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.type.cv
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[dcl.type.cv], paragraphs 3 and 4"
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Collection.html#unmodview
    title: Java SE 25 API, java.util.Collection
    locator: Unmodifiable Collections; Unmodifiable View Collections
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.objectmodel.readonlycollection-1
    title: .NET API, ReadOnlyCollection<T> Class
    locator: Remarks
  - url: https://kotlinlang.org/docs/collections-overview.html#read-only-and-mutable-collections
    title: Kotlin documentation, Collections overview
    locator: Read-only and mutable collections
  - url: https://www.typescriptlang.org/docs/handbook/2/objects.html#readonly-properties
    title: TypeScript Handbook, Object Types
    locator: readonly Properties; The ReadonlyArray Type
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#unmodifiable
    title: Java SE 25 API, java.util.List
    locator: Unmodifiable Lists
  - url: https://tc39.es/ecma262/2026/#sec-setintegritylevel
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 7.3.15 SetIntegrityLevel
  - url: https://doc.rust-lang.org/reference/interior-mutability.html
    title: The Rust Reference
    locator: Interior mutability, rules interior-mut.shared-ref, interior-mut.unsafe-cell and interior-mut.ref-cell
  - url: https://doc.rust-lang.org/std/cell/index.html
    title: "The Rust Standard Library, module std::cell"
    locator: module description
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.prim.this
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.prim.this], paragraph 3"
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.stc
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[dcl.stc], paragraph 9, note 5"
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/readonly#readonly-field-example
    title: "C# language reference, readonly"
    locator: the field declaration context
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/init
    title: "C# language reference, init"
    locator: introduction
  - url: https://tc39.es/ecma262/2026/#sec-object.freeze
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 20.1.2.6 Object.freeze
  - url: https://tc39.es/ecma262/2026/#sec-putvalue
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 6.2.5.6 PutValue
  - url: https://docs.python.org/3.14/library/dataclasses.html#frozen-instances
    title: Python Standard Library, dataclasses, Python 3.14
    locator: Frozen instances
  - url: https://go.dev/ref/spec#String_types
    title: The Go Programming Language Specification
    locator: String types
  - url: https://go.dev/ref/spec#Constants
    title: The Go Programming Language Specification
    locator: Constants
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.3p6
    title: "N1570, Committee Draft of ISO/IEC 9899:2011 (C11)"
    locator: 6.7.3 Type qualifiers, paragraph 6
  - paper: doi:10.1016/0022-0000(89)90034-2
    title: James R. Driscoll, Neil Sarnak, Daniel D. Sleator and Robert E. Tarjan, Making data structures persistent, Journal of Computer and System Sciences 38(1), 1989, pp. 86-124
    locator: the abstract on p. 86, the definitions of persistence in Section 1 on p. 87, and the footnote on p. 86 about the 1986 preliminary version
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.immutable
    title: .NET API, System.Collections.Immutable Namespace
    locator: namespace description
  - url: https://doc.rust-lang.org/std/rc/struct.Rc.html#method.make_mut
    title: "The Rust Standard Library, std::rc::Rc"
    locator: method make_mut
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#literal-inference
    title: TypeScript Handbook, Everyday Types
    locator: Literal Inference, the as const suffix
  - url: https://kotlinlang.org/docs/data-classes.html#copying
    title: Kotlin documentation, Data classes
    locator: Copying
  - url: https://doc.rust-lang.org/reference/types/pointer.html
    title: The Rust Reference
    locator: Pointer types, rule type.pointer.reference.shared.constraint-mutation
  - url: https://openjdk.org/jeps/269
    title: JEP 269, Convenience Factory Methods for Collections
    locator: header (Release 9); Goals; Non-Goals
  - url: https://docs.python.org/3.14/faq/programming.html#why-does-a-tuple-i-item-raise-an-exception-when-the-addition-works
    title: Python Programming FAQ, Python 3.14
    locator: "Why does a_tuple[i] += ['item'] raise an exception when the addition works?"
  - url: https://doc.rust-lang.org/error_codes/E0384.html
    title: Rust error codes index
    locator: E0384
  - url: https://openjdk.org/projects/jdk9/
    title: OpenJDK, JDK 9
    locator: introduction and schedule (General Availability on 21 September 2017)
  - url: https://www.python.org/downloads/release/python-370/
    title: Python Release Python 3.7.0
    locator: release date
  - url: https://peps.python.org/pep-0557/#specification
    title: PEP 557, Data Classes
    locator: header (Python-Version 3.7); Specification, the frozen parameter
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history#c-version-9
    title: "The history of C#"
    locator: "C# version 9"
  - url: https://openjdk.org/projects/jdk/16/
    title: OpenJDK, JDK 16
    locator: introduction and schedule (General Availability on 16 March 2021)
  - url: https://openjdk.org/jeps/395
    title: JEP 395, Records
    locator: header (Release 16); Summary
---

## Idea

An object is immutable when its value cannot change after the program makes it. A program that needs a different value makes a fresh object. The Python glossary uses this definition: an immutable object has a fixed value, and a different value needs a fresh object.[^1] The opposite is a mutable object, such as a Python list, whose contents change in place.[^2]

The difference matters when two parts of a program hold the same object. In this run, a function changes the list that the caller gave it. The caller's list changes too, because both names refer to one object.

```python wrong id=shared-list
def add_tax(prices):
    for i, price in enumerate(prices):
        prices[i] = round(price * 1.2, 2)
    return prices


base = [10.0, 20.0]
with_tax = add_tax(base)
print(base, with_tax)
```

```output for=shared-list
[12.0, 24.0] [12.0, 24.0]
```

Without immutability, the usual guard is a defensive copy: the function or the caller copies the list before the change. With a tuple, the function cannot change its argument. It must return a fresh tuple, and the caller keeps the original.

```python run id=shared-tuple
def add_tax(prices):
    return tuple(round(price * 1.2, 2) for price in prices)


base = (10.0, 20.0)
with_tax = add_tax(base)
print(base, with_tax)
```

```output for=shared-tuple
(10.0, 20.0) (12.0, 24.0)
```

Immutability gives these properties:

- **Sharing without copies.** Two holders can share one immutable object, because neither of them can change it for the other. The Java `String` class says that strings can be shared because they are immutable.[^3] In C, string literals with the same characters can share one array.[^4]
- **Safe concurrent reads.** The Python glossary calls immutable objects inherently thread-safe, because no thread can change their state after creation.[^1] In Java, `final` fields let a programmer write thread-safe immutable objects without synchronization.[^5]
- **Stable hash keys.** A hash table finds a key by its hash value. That value must not change while the key is in the table. In Python, a hashable object has a hash value that never changes during its lifetime, and most immutable built-in objects are hashable.[^6] The C# documentation gives a hash code that stays the same in a hash table as one reason to use immutable records.[^7]
- **Local reasoning.** A reader who sees an immutable value knows that no other code can change it. The Rust book gives the bug that this prevents. One part of the code assumes that a value never changes, and another part sometimes changes it.[^8]
- **No defensive copies.** A function can keep an immutable argument as it is. It does not need to copy the argument to protect its own state.

## How it works

### A binding or a value

A binding links a name to a value. An immutable binding cannot get a different value by assignment, but the value that it names can still be mutable. A mutable binding can name an immutable value. The two ideas are independent.

Many languages give immutable bindings. In JavaScript, a `const` declaration makes a strict immutable binding.[^9] An assignment to such a binding throws `TypeError`, in strict code and in sloppy code.[^10] A Java `final` variable can be assigned only once.[^11] A Kotlin `val` is a read-only variable that cannot get a different value after initialization.[^12] A Rust `let` binding is immutable unless it has the keyword `mut`.[^8] The Bash builtin `readonly` marks names whose values cannot be changed by assignment or `unset`.[^13] In PowerShell, `Set-Variable -Option ReadOnly` makes a variable that cannot be changed without `-Force`. With `-Option Constant`, the variable cannot be changed or deleted.[^14]

None of these bindings makes the value immutable. A JavaScript `const` array still accepts `push` (see Pitfalls).

### A read-only view or an immutable object

A read-only view is a reference that has no operations that change the object. Other references to the same object can still change it. The C++ standard says that a `const` access path cannot change an object, even when another access path can change it.[^15] The Java collection documentation describes an unmodifiable view of a backing collection. Changes to the backing collection are visible through the view.[^16] `ReadOnlyCollection<T>` in .NET is a wrapper of the same kind.[^17] In Kotlin, each collection type has a read-only interface, such as `List`, and a mutable interface, such as `MutableList`. A read-only interface prevents changes only through that interface.[^18] A TypeScript `readonly` property can change through an alias of a type without `readonly`.[^19]

An immutable object has no access path that changes it. In Java, `List.of` and `List.copyOf` return unmodifiable lists. Their mutator methods always throw `UnsupportedOperationException`.[^20] The Kotlin documentation makes the same distinction: an immutable collection does not allow changes to its elements after creation.[^18]

### Shallow or deep

Shallow immutability fixes the fields of one object. The objects that the fields refer to can still change. Deep immutability needs every object that is reachable from the value to be immutable too.

Most language features are shallow. A Python tuple that holds a list is still an immutable container, because the set of objects that it contains cannot change. Its value changes when the list changes.[^2] `Object.freeze` in JavaScript acts on the own properties of the object only, so an object inside it can still change.[^21] The init-only properties of a C# record have shallow immutability: the data that a reference-type property refers to can change.[^7] The Java documentation says that an unmodifiable collection is not necessarily immutable, because its elements can be mutable. An unmodifiable collection of immutable elements can be considered effectively immutable.[^16]

### Interior mutability

Some types change their internal state behind an interface that looks immutable. In Rust, a type has interior mutability when its internal state can change through a shared reference. `UnsafeCell<T>` is the only allowed way to do this, and types such as `Cell<T>` and `RefCell<T>` are built on it. `RefCell<T>` checks the borrow rules at run time.[^22] The `std::cell` documentation contrasts interior mutability with inherited mutability, where a value can change only through a unique `&mut` reference.[^23]

In C++, a `const` member function sees `this` as a pointer to a `const` object.[^24] A member with the `mutable` specifier cancels the `const` of the object that contains it, so a `const` member function can change it.[^25] The C++ example uses a `mutable` counter in a `const` object.

### Enforcement

A language can enforce immutability in three ways.

- **At compile time.** The compiler rejects the change. C and C++ `const`, Java `final`, Kotlin `val`, Rust bindings without `mut`, and C# `readonly` fields work this way. In C#, a `readonly` field can be assigned only in its declaration or in a constructor of the same class.[^26] An `init` accessor assigns a property only during object construction.[^27] TypeScript checks `readonly` at compile time only. The modifier does not change any behaviour at run time.[^19]
- **At run time.** A check stops the change when it happens. `Object.freeze` makes the properties of an object read-only, and later writes fail.[^28] A failed write throws `TypeError` only in strict mode code. In sloppy mode, it does nothing and gives no error.[^29] A Python frozen dataclass raises `FrozenInstanceError` on an assignment to a field.[^30] A Java unmodifiable list throws `UnsupportedOperationException` from its mutator methods.[^20]
- **By convention.** No check exists, and the programmer keeps the rule. The Python documentation says that it is not possible to create truly immutable Python objects; `frozen=True` only emulates immutability.[^30] Go has immutable strings,[^31] and `const` declares boolean, rune, numeric and string constants only.[^32] For other Go types, the usual forms are unexported fields and copies (see the Go example).

The compile-time checks of C and C++ do not hold at run time. A cast can remove `const`. A change to an object that was defined `const` is undefined behaviour in C and in C++.[^33][^15] A change to a C string literal is also undefined behaviour.[^4] In the C example, AddressSanitizer stops such a write.

### Persistent data structures and structural sharing

A change to an immutable collection gives a fresh collection. A full copy costs time and memory in proportion to the size. Persistent data structures avoid most of this cost.

Driscoll, Sarnak, Sleator and Tarjan describe ordinary data structures as ephemeral: a change destroys the old version. A persistent structure allows access to every version at any time. It is partially persistent when only the last version can change, and fully persistent when every version can change.[^34]

The usual technique is structural sharing. The fresh version reuses the unchanged parts of the old version. In this stack, each cell is a tuple of a value and the rest of the stack. A push makes one fresh cell. The stacks `b` and `c` share all the cells of `a`.

```python run id=sharing
def push(stack, value):
    return (value, stack)


def to_list(stack):
    values = []
    while stack is not None:
        value, stack = stack
        values.append(value)
    return values


a = push(push(None, 1), 2)
b = push(a, 3)
c = push(a, 4)
print(b[1] is a, c[1] is a)
print(to_list(a), to_list(b), to_list(c))
```

```output for=sharing
True True
[2, 1] [3, 2, 1] [4, 2, 1]
```

The old version stays valid because no cell ever changes. The four authors give general techniques that make linked data structures persistent. They use the techniques to make persistent binary search trees.[^34] The Kotlin documentation points to the kotlinx-collections-immutable library for immutable and persistent collections.[^18] In .NET, the namespace `System.Collections.Immutable` contains immutable collections.[^35]

### Copy-on-write

Copy-on-write lets several holders share one copy until one of them wants to change it. Then that holder gets its own copy, and the others keep the shared one. In Rust, `Rc::make_mut` gives a mutable reference to the value in an `Rc`. If other `Rc` pointers share the allocation, it first clones the value. The Rust documentation calls this clone-on-write.[^36]

```rust run id=clone-on-write
use std::rc::Rc;

fn main() {
    let first = Rc::new(vec![1, 2, 3]);
    let mut second = Rc::clone(&first);
    println!("shared: {}", Rc::ptr_eq(&first, &second));
    Rc::make_mut(&mut second).push(4);
    println!("shared: {}", Rc::ptr_eq(&first, &second));
    println!("{first:?} {second:?}");
}
```

```output for=clone-on-write
shared: true
shared: false
[1, 2, 3] [1, 2, 3, 4]
```

Before the change, both pointers refer to one vector. The call to `make_mut` clones the vector, so `first` keeps the old value.

## In languages

The example files show the immutable forms of each language, and the matching read-only or shallow forms. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | immutable built-in types such as `str`, `tuple` and `frozenset`; `@dataclass(frozen=True)` |
| JavaScript | native | immutable primitive values; `const` bindings; `Object.freeze` |
| TypeScript | native | `readonly`, `ReadonlyArray<T>` and `as const`, checked at compile time only |
| Java | native | `final`; records; `List.of` and `List.copyOf` |
| C | native | the `const` qualifier |
| C++ | native | `const` objects and member functions; `constexpr`; `mutable` |
| C# | native | `readonly`, `init` and records; `System.Collections.Immutable` |
| Go | idiom | immutable strings; `const` for basic types; copies for the rest |
| Rust | native | immutable bindings by default; `mut`; `Cell` and `RefCell` |
| Kotlin | native | `val`; read-only collection interfaces; `copy` of a data class |
| PowerShell | native | `Set-Variable -Option ReadOnly` or `-Option Constant` |
| Bash | native | the `readonly` builtin for variables |
| SQL | n/a | a query has no objects that code shares and changes |
| Regex | n/a | a pattern has no values to change |
| HTML/CSS | n/a | markup and style rules have no program values |

In TypeScript, `as const` makes the type checker give each property its literal type, as `const` does for a binding.[^37] Like `readonly`, it has no effect on the emitted JavaScript.

Bash has no example file. The `readonly` builtin marks a variable, so a later assignment or `unset` fails.[^13] The protection belongs to the name, as with `const` in JavaScript.

## Variations and related ideas

- **Value semantics.** When an assignment copies the value, each holder has its own copy. A change by one holder is not visible to the others. Go and C copy a struct on assignment, as the examples show. [[pointers-and-references]] describes the other case, where holders share one object.
- **Value types and reference types.** In C#, a `readonly` field of a value type is immutable. A `readonly` field of a reference type always refers to the same object, but that object can still change.[^26]
- **Constants.** A constant gets its value from a literal or a constant expression. Go constants are an example.[^32] C++ `constexpr` variables are another (see the C++ example). A constant is a stricter form of an immutable binding.
- **Nondestructive update.** A copy with some fields changed takes the place of a change. C# has the `with` expression for records.[^7] Kotlin data classes have `copy`.[^38] Python has `dataclasses.replace`.[^30] JavaScript has spread syntax.
- **Freeze after build.** A program builds a value with a mutable object, and then gives out an immutable copy. `List.copyOf` in Java and `ImmutableList.CreateRange` in C# make such a copy.[^20][^35]
- **Borrowing.** Rust does not forbid all change. It forbids a direct change while a shared reference to the value exists.[^39] A program can change a value when it holds the only reference.
- **Closures.** A closure that captures only immutable values gives the same result each time its captures are read. [[closures]] describes captures that change.

## Pitfalls

### A const binding does not freeze the value

A JavaScript `const` declaration makes the binding immutable.[^9] The array that it names is still mutable.

```javascript wrong id=const-array
const items = [1, 2];
items.push(3);
console.log(items);
```

```output for=const-array
[ 1, 2, 3 ]
```

Use `Object.freeze(items)` when the array must not change. Use a copy such as `[...items, 3]` when the program needs a changed array.

### Object.freeze is shallow

`Object.freeze` acts on the own properties of the object.[^21] An array or an object in a property can still change. In sloppy mode code, a write to a frozen property also fails with no error.[^29]

```javascript wrong id=shallow-freeze
const config = Object.freeze({ name: "app", tags: ["a"] });
config.tags.push("b");
config.name = "other";
console.log(config.name, config.tags, Object.isFrozen(config));
```

```output for=shallow-freeze
app [ 'a', 'b' ] true
```

The array in `tags` changed, and the write to `name` was lost without a message. The fix is a function that also freezes each nested object. The directive `"use strict"` turns the lost write into a `TypeError` (see the JavaScript example).

### An unmodifiable view shows changes to the original

`Collections.unmodifiableList` returns a view. Its mutator methods throw, but changes to the backing list are visible through it.[^16]

```java wrong id=view
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> source = new ArrayList<>(List.of("a"));
        List<String> view = Collections.unmodifiableList(source);
        source.add("b");
        System.out.println(view);
    }
}
```

```output for=view
[a, b]
```

A class that gives out such a view of an internal list still lets its own code change the list. When the caller must see a fixed value, use `List.copyOf(source)`, which copies.[^20] C# `ReadOnlyCollection<T>` and Kotlin read-only interfaces behave the same way.[^17][^18]

### An unmodifiable list fails only at run time

The lists that `List.of` returns have the type `List`, which has the method `add`. JEP 269 states that it does not try to show unmodifiability in the type system.[^40] So the call compiles, and it fails when it runs.[^20]

```java fails expect=exception:UnsupportedOperationException id=list-of-add
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> names = List.of("ada", "alan");
        names.add("grace");
        System.out.println(names);
    }
}
```

The run stops with `java.lang.UnsupportedOperationException`, with no message. A method that receives a `List` cannot know from the type whether `add` works. Write in the documentation of a method when it returns an unmodifiable list. Make a mutable copy with `new ArrayList<>(names)` when a change is necessary.

### A tuple that holds a list

A Python tuple is immutable, but a list in it is not. The tuple is hashable only when all its elements are hashable, so it cannot be a dictionary key.[^6]

```python fails expect=exception:TypeError id=tuple-key
key = (1, [2, 3])
cache = {key: "value"}
print(cache)
```

The run stops with `TypeError: cannot use 'tuple' as a dict key (unhashable type: 'list')`. Use a tuple of immutable elements, such as `(1, (2, 3))`, as the key.

An augmented assignment to such an element both raises an error and changes the list. `t[1] += [4]` first calls the list's in-place addition, which changes the list. Then it assigns the result to `t[1]`, and the tuple rejects the assignment.[^41]

```python wrong id=tuple-iadd
t = (1, [2, 3])
try:
    t[1] += [4]
except TypeError as error:
    print("TypeError:", error)
print(t)
```

```output for=tuple-iadd
TypeError: 'tuple' object does not support item assignment
(1, [2, 3, 4])
```

The fix is a method call on the list, `t[1].append(4)` or `t[1].extend([4])`, which does not assign to the tuple.

### An assignment to an immutable Rust binding

A Rust `let` binding without `mut` cannot get a second value.[^8]

```rust fails expect=compile id=reassign
fn main() {
    let count = 0;
    count += 1;
    println!("{count}");
}
```

`rustc` stops with error E0384, `cannot assign twice to immutable variable`.[^42] The fix is `let mut count = 0;`, or a fresh binding such as `let count = count + 1;`, which shadows the first one.

### A read-only interface is not an immutable list

A Kotlin function that takes a `List` cannot change the list through that interface.[^18] But a `List` value can be a `MutableList` at run time. A cast gets the mutable interface back, and the change reaches the caller's list.

```kotlin wrong id=cast timeout=30
fun register(names: List<String>) {
    (names as MutableList<String>).add("grace")
}

fun main() {
    val team = mutableListOf("ada", "alan")
    register(team)
    println(team)
}
```

```output for=cast
[ada, alan, grace]
```

The caller gave a read-only view, and the function changed the list. Give out a copy, such as `team.toList()`, when the caller must keep its list as it is.

## Cost

- **A fresh object for each change.** A change to an immutable value makes a fresh object. A loop that builds a value from many changes makes many objects. Mutable builders avoid this. The Java `String` documentation names string buffers as the mutable form of strings.[^3]
- **Persistent structures.** Structural sharing avoids a full copy for each change. Driscoll, Sarnak, Sleator and Tarjan give persistent binary search trees with logarithmic time for access, insertion and deletion. Each insertion and deletion takes constant space.[^34]
- **Run-time checks.** A frozen Python dataclass has a small cost: its `__init__` must call `object.__setattr__` to set the fields, not a plain assignment.[^30]
- **Compile-time checks.** TypeScript `readonly` has no cost at run time, because it does not change the emitted code. For the same reason, it gives no protection at run time.[^19]
- **Optimization.** A Java compiler can keep the value of a `final` field in a register. A field that is not `final` must be read again in some of these places.[^5]
- **Copy-on-write.** `Rc::make_mut` clones only when other pointers share the value. A value with one owner changes in place.[^36]

## History

- **1986 and 1989.** Driscoll, Sarnak, Sleator and Tarjan studied persistent data structures. They presented a first version at the ACM Symposium on Theory of Computing in 1986, and the journal version appeared in 1989.[^34]
- **2017.** JDK 9 reached General Availability on 21 September 2017.[^43] It added the factory methods `List.of`, `Set.of` and `Map.of`, which return unmodifiable collections (JEP 269).[^40]
- **2018.** Python 3.7 was released on 27 June 2018.[^44] It added the `dataclasses` module of PEP 557, with the parameter `frozen`.[^45]
- **2020.** C# 9 was released in November 2020, with records and init-only setters.[^46]
- **2021.** JDK 16 reached General Availability on 16 March 2021.[^47] It made records a final feature of Java. JEP 395 describes them as classes that act as transparent carriers for immutable data.[^48]

## References

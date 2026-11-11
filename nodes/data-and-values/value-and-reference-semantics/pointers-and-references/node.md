---
tag: VX3T75
kind: concept
slug: pointers-and-references
name: Pointers and references
aliases: [pointer, reference, address, indirection, alias]
area: data-and-values/value-and-reference-semantics
level: core
summary: >-
  A reference is a value that leads to another value, so several parts of a program can share one object without copies. A pointer is a reference that holds a memory address.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1964
  people: [Harold Lawson]
  note: Lawson invented the pointer variable at IBM in 1964 and 1965 and put it into PL/I, so that a general-purpose high-level language could handle linked lists.
see_also: [null-references, immutability, garbage-collection, closures, recursion]
same_as:
  wikidata: Q1334113
  wikipedia: Reference_(computer_science)
sources:
  - url: https://www.computer.org/profiles/harold-lawson
    title: IEEE Computer Society, Harold W. (Bud) Lawson, 2000 Computer Pioneer Award
    locator: the award citation and the biography (the pointer variable, PL/I, 1964-65, "PL/I List Processing" in CACM, June 1967)
  - url: https://www.bell-labs.com/usr/dmr/www/chist.html
    title: Dennis M. Ritchie, The Development of the C Language, HOPL-II, 1993
    locator: sections Origins, the languages; Problems of B; Embryonic C
  - url: https://www.stroustrup.com/bs_faq2.html#pointers-and-references
    title: Bjarne Stroustrup, C++ Style and Technique FAQ
    locator: Why does C++ have both pointers and references?
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.2.5
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.2.5 Types, paragraph 20 (pointer types)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.3.2
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.5.3.2 Address and indirection operators, paragraphs 3 and 4
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.6
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.5.6 Additive operators, paragraph 8
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.2.1
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.5.2.1 Array subscripting, paragraph 2
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.2.8
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.2.8 Alignment of objects, paragraph 1
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.2.4
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.2.4 Storage durations of objects, paragraph 2
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.2.2
    title: ISO/IEC 9899:201x, Programming languages, C (N1570, the C11 draft)
    locator: 6.5.2.2 Function calls, paragraph 4
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.ref
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[dcl.ref], paragraphs 4 and 5"
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.init.ref
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[dcl.init.ref], paragraphs 1 and 2"
  - url: https://timsong-cpp.github.io/cppwp/n4950/vector.modifiers
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[vector.modifiers], paragraph 2"
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.3.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.3.1 Objects
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.4.1 Formal Parameters
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.21.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.21.3 Reference Equality Operators == and !=
  - url: https://docs.python.org/3.14/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.1 Objects, values and types
  - url: https://docs.python.org/3.14/faq/programming.html#how-do-i-write-a-function-with-output-parameters-call-by-reference
    title: Python Programming FAQ, Python 3.14
    locator: How do I write a function with output parameters (call by reference)?
  - url: https://docs.python.org/3.14/faq/programming.html#how-do-i-create-a-multidimensional-list
    title: Python Programming FAQ, Python 3.14
    locator: How do I create a multidimensional list?
  - url: https://docs.python.org/3.14/library/stdtypes.html#common-sequence-operations
    title: The Python Standard Library, Built-in Types, Python 3.14
    locator: Common Sequence Operations, note 2
  - url: https://tc39.es/ecma262/2026/#sec-samevaluenonnumber
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 7.2.11 SameValueNonNumber
  - url: https://go.dev/ref/spec#Pointer_types
    title: The Go Programming Language Specification
    locator: Pointer types
  - url: https://go.dev/ref/spec#Address_operators
    title: The Go Programming Language Specification
    locator: Address operators
  - url: https://go.dev/doc/faq#pass_by_value
    title: Go FAQ
    locator: When are function parameters passed by value?
  - url: https://go.dev/doc/faq#no_pointer_arithmetic
    title: Go FAQ
    locator: Why is there no pointer arithmetic?
  - url: https://go.dev/doc/faq#stack_or_heap
    title: Go FAQ
    locator: How do I know whether a variable is allocated on the heap or the stack?
  - url: https://doc.rust-lang.org/reference/types/pointer.html
    title: The Rust Reference
    locator: Pointer types (rules type.pointer.reference.shared, type.pointer.reference.mut, type.pointer.raw, type.pointer.smart)
  - url: https://doc.rust-lang.org/reference/unsafety.html
    title: The Rust Reference
    locator: Unsafety (rule safety.unsafe-deref)
  - url: https://doc.rust-lang.org/book/ch04-02-references-and-borrowing.html#the-rules-of-references
    title: The Rust Programming Language, References and Borrowing
    locator: The Rules of References
  - url: https://doc.rust-lang.org/error_codes/E0502.html
    title: Rust Compiler Error Index
    locator: E0502
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/method-parameters#pass-by-value-and-pass-by-reference
    title: C# language reference, Method parameters
    locator: Pass by value and pass by reference
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/method-parameters#ref-parameter-modifier
    title: C# language reference, Method parameters
    locator: ref parameter modifier
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-types
    title: C# language reference, Value types
    locator: the introduction (a variable of a value type contains an instance of the type)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/unsafe-code#pointer-types
    title: C# language reference, Unsafe code, pointer types, and function pointers
    locator: Pointer types
  - url: https://kotlinlang.org/docs/equality.html#referential-equality
    title: Kotlin documentation, Equality
    locator: Referential equality
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_ref?view=powershell-7.5
    title: PowerShell documentation, about_Ref
    locator: Long description; Writing functions to accept reference parameters
  - url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Parameters
    title: Bash Reference Manual
    locator: 3.4 Shell Parameters (the nameref attribute)
  - url: https://www.gnu.org/software/bash/manual/bash.html#index-printf
    title: Bash Reference Manual
    locator: 4.2 Bash Builtin Commands, printf
---

## Idea

A program often needs to reach one value from several places. It can copy the value into each place. It can also store, in each place, a small value that leads to one shared copy. That small value is a reference. A pointer is a reference that holds the address of the value in memory.[^4]

References serve four common needs:

- **Sharing without copies.** A function can read or change a large object, and only a reference moves.
- **Linked data structures.** A node of a list, a tree or a graph holds references to other nodes.
- **Out parameters.** A function can change a variable of its caller through a reference to that variable.
- **Dynamic allocation.** A program makes objects at run time and keeps a reference to each one.

Two names that refer to one object are aliases. A change through one name shows through the other. In Python, every variable holds a reference, so an assignment makes an alias, not a copy.[^17]

```python run id=idea
scores = [10, 20, 30]
alias = scores
alias.append(40)
print(scores)
print(alias is scores)
```

```output for=idea
[10, 20, 30, 40]
True
```

## How it works

### Addresses and indirection

The memory of a machine is a sequence of cells, and each cell has a number, its address. A load instruction reads the value at an address. A store instruction writes a value to an address. A pointer is a variable whose value is an address. To follow the pointer to the value is called indirection, or dereferencing.

In C, the unary operator `&` gives the address of an object, and the unary operator `*` gives the object that a pointer points to.[^5] The diagram shows a variable `x` and a pointer `p` to it. The addresses are illustrative.

```text
name   address   contents
x      1000      42
p      1008      1000     p points to x, so *p is 42
```

A read of `*p` is two loads: one to get the address in `p`, one to get the value at that address. A write `*p = 7` is a load of `p` and a store to the address. When `p` does not hold the address of a valid object, the behaviour of `*p` is undefined.[^5]

A pointer has a fixed size, whatever the size of the object it points to. That size depends on the machine and the compiler; the C example prints it. Each object type also has an alignment. Its address must be a multiple of a number of bytes that the implementation defines.[^8]

C allows arithmetic on pointers into an array. `p + 1` points to the next element, not the next byte. A pointer may also point one past the last element, but a dereference there is undefined.[^6] The subscript `p[i]` is defined as `*(p + i)`.[^7]

A pointer is valid only while its object exists. When the lifetime of the object ends, the value of the pointer becomes indeterminate. A use of the object after that point is undefined.[^9] Such a pointer is a dangling pointer. The C pitfall below shows one.

### Value semantics and reference semantics

Under value semantics, an assignment copies the value. The two variables are then independent. Under reference semantics, a variable holds a reference, and an assignment copies the reference. Both variables then refer to one object.

Most languages mix the two:

- **Java.** Primitive types such as `int` hold values. Class, interface and array types hold references, and there can be many references to one object.[^14]
- **C#.** A variable of a struct type contains an instance of the type. A variable of a class type contains a reference.[^33]
- **Go.** An array is a value, and an assignment copies its elements. A slice refers to an underlying array, so a copy of a slice shares the elements (see the Go example).
- **C and C++.** An assignment of a struct or a class object copies it. Sharing needs an explicit pointer or, in C++, a reference.
- **Python, JavaScript, Kotlin.** Every object is reached through a reference. Objects that cannot change, such as Python integers, make the difference hard to see.

Reference semantics adds a question that value semantics does not need: are two references the same object, or only equal objects? Each language has a separate test for identity.

| Language | Identity of objects | Equality of contents |
|---|---|---|
| Python | `a is b`[^17] | `a == b` |
| Java | `a == b`[^16] | `a.equals(b)` |
| JavaScript, TypeScript | `a === b` on objects[^21] | written by hand |
| Kotlin | `a === b`[^35] | `a == b` |
| C# | `ReferenceEquals(a, b)` | `a.Equals(b)` |
| Go, C, C++ | `p == q` on pointers | `*p == *q` |
| Rust | `std::ptr::eq(a, b)` | `a == b` on references |

### Pass by value and pass by reference

Under pass by value, the parameter is a fresh variable that gets a copy of the argument. Under pass by reference, the parameter is a second name for the variable of the caller. An assignment to the parameter then changes the variable of the caller.

Many languages pass by value only. C assigns the value of each argument to the parameter.[^10] Java initializes fresh parameter variables with the argument values.[^15] Python passes arguments by assignment, so it has no call by reference.[^18] Go passes everything by value; a map or slice value behaves like a pointer, so the callee can change the shared contents.[^24]

When the argument is a reference, these languages copy the reference. The callee can change the object, because the caller and the callee share it. The callee cannot make the variable of the caller refer to a different object. The examples show both cases with a function `rebind`, which has no effect, and a function `append`, whose change the caller sees.

A few languages also have true pass by reference:

- **C++.** A parameter of type `T&` is a reference. Argument passing is initialization, so the parameter becomes an alias for the argument.[^12]
- **C#.** A `ref` parameter is an alias for the variable of the caller. Both the declaration and the call must use the keyword `ref`.[^32] Without it, a class parameter gets a copy of the reference, and a struct parameter gets a copy of the value.[^31]
- **PowerShell.** A parameter of type `[ref]` holds a `PSReference` to the variable of the caller. The function reads and writes the variable through its `Value` property.[^36]

Rust and C get the same effect with an explicit pointer: the caller writes `&mut x` or `&x`, and the callee writes `*r = 42`.

### References in C++

A C++ reference is an alias for an object, not an object of its own. The standard does not say whether a reference needs storage.[^11] It has three rules that pointers do not have:

- A reference must be initialized when it is declared. The exceptions are parameters, return types, class members and `extern` declarations.[^11][^12]
- A reference cannot be changed to refer to another object after its initialization. `r = b` assigns the value of `b` to the object that `r` refers to.[^12]
- A null reference cannot exist in a well-defined program. There are no references to references, no arrays of references and no pointers to references.[^11]

The C++ example shows the second rule: after `r = b`, the address of `r` is still the address of `a`.

### Managed references and raw pointers

In Java, Python, JavaScript and Kotlin, and in the safe parts of Go and C#, a reference can only be made from an existing object. The program cannot do arithmetic on it and cannot convert an integer into it. The runtime keeps an object alive while a reference to it exists ([[garbage-collection]]). So a managed reference cannot dangle. Go has pointers but no pointer arithmetic. The Go FAQ gives safety as the reason. It adds that a loop with array indices can be as efficient, and that the absence of pointer arithmetic helps the implementation of the garbage collector.[^25] When the address of a local variable leaves the function, the Go compiler puts the variable on the heap, so the pointer stays valid.[^26]

C and C++ have raw pointers with arithmetic and casts. The compiler does not track lifetimes, so a dangling pointer is possible. C# has pointer types too, but only inside an unsafe context, and the project must allow unsafe blocks.[^34]

### Borrows in Rust

Rust has two kinds of reference. A shared reference `&T` allows reading, and any number of them may exist at one time. A mutable reference `&mut T` is the only way to reach its value while it exists.[^27] The borrow checker enforces the rule at compile time: either one mutable reference or any number of shared references. References must always be valid, so a reference cannot outlive its value.[^29]

Rust also has raw pointers `*const T` and `*mut T`, with no guarantees of safety or liveness.[^27] To dereference a raw pointer is an unsafe operation, so it must be inside an `unsafe` block.[^28]

## In languages

The example files show the same small program in each language: a function that tries to change its argument, and two names for one object.

| Language | Verdict | Form |
|---|---|---|
| Python | native | every variable holds a reference; `is` compares identity |
| JavaScript | native | objects are reached through references; `===` compares identity |
| TypeScript | native | as JavaScript |
| Java | native | reference types; `==` on references compares identity |
| C | native | pointer types `T *`, operators `&` and `*`, pointer arithmetic |
| C++ | native | pointers `T*` and references `T&` |
| C# | native | class types, `ref` parameters, pointers in unsafe code |
| Go | native | pointer types `*T`; no pointer arithmetic |
| Rust | native | references `&T` and `&mut T` checked by the borrow checker; raw pointers |
| Kotlin | native | references to objects; `===` compares identity |
| PowerShell | native | references to .NET objects; `[ref]` parameters |
| Bash | idiom | the name of a variable, used through `printf -v` or a nameref |
| SQL | n/a | a row refers to another row by the value of a key |
| Regex | n/a | a pattern has no variables in memory |
| HTML | n/a | markup has no variables in memory |
| CSS | n/a | style rules have no variables in memory |

Bash has strings and arrays, but no reference to a value. The idiom passes the name of a variable instead. The builtin `printf -v name` assigns its output to the variable called `name`.[^38] The `nameref` attribute, set with `declare -n` or `local -n`, makes a variable that refers to another variable by name.[^37]

```bash run id=bash-name
set_to_42() {
  printf -v "$1" '%s' 42
}

count=1
set_to_42 count
echo "after set_to_42: $count"
```

```output for=bash-name
after set_to_42: 42
```

## Variations and related ideas

- **Null.** Many languages let a reference refer to nothing. Java has a special null reference that refers to no object.[^14] [[null-references]] covers the errors that this causes and the types that exclude it.
- **Smart pointers.** A smart pointer is a type that acts as a pointer and also decides when to free the object. The Rust standard library has smart pointer types beyond references and raw pointers.[^27] Reference counting is one way to decide when to free the object ([[garbage-collection]]).
- **Immutable objects.** Aliasing causes no surprise when no one can change the shared object. Shared references to immutable values are as safe as copies ([[immutability]]).
- **Capture by reference.** A closure that captures a variable keeps a reference to it ([[closures]]). The same rules of lifetime apply.
- **Recursive data.** A list or a tree is a node with references to smaller nodes, and a recursive function walks it ([[recursion]]).
- **Indices as references.** An index into an array also leads to a value. BCPL and B treated a pointer as an integer index into the array of memory cells.[^2]

## Pitfalls

### Repetition copies references, not objects

In Python, `s * n` does not copy the items of `s`. It refers to them `n` times.[^20] So `[[0] * 3] * 3` is a list of three references to one inner list.[^19]

```python wrong id=grid
grid = [[0] * 3] * 3
grid[0][0] = 1
print(grid)
```

```output for=grid
[[1, 0, 0], [1, 0, 0], [1, 0, 0]]
```

The fix makes a separate inner list for each row: `[[0] * 3 for _ in range(3)]`. The inner `[0] * 3` is safe, because integers cannot change.

### A swap of two references does not swap

A Java method gets copies of the references of its caller.[^15] A swap of the parameters exchanges only the copies.

```java wrong id=swap
public class Main {
    static void swap(StringBuilder a, StringBuilder b) {
        StringBuilder t = a;
        a = b;
        b = t;
    }

    public static void main(String[] args) {
        StringBuilder first = new StringBuilder("first");
        StringBuilder second = new StringBuilder("second");
        swap(first, second);
        System.out.println(first + " " + second);
    }
}
```

```output for=swap
first second
```

The variables `first` and `second` still refer to the same objects. A method can swap two elements of an array or a list, because it changes the shared container. Python, JavaScript, Go and Kotlin behave in the same way. C++ and C# can swap with reference parameters, and C can swap with pointers (see the C example).

### A pointer to a local outlives the local

A local variable of a C function lives until the function returns. A pointer to it is then dangling, and a read through it is undefined.[^9]

```c fails expect=sanitizer:address flags="-fsanitize-address-use-after-return=always" id=dangling
#include <stdio.h>

int *make_value(void) {
    int value = 42;
    return &value;
}

int main(void) {
    int *p = make_value();
    printf("%d\n", *p);
    return 0;
}
```

The compiler warns `address of stack memory associated with local variable 'value' returned`, and AddressSanitizer reports `stack-use-after-return`. In a build without the sanitizer, the program printed 42 and exited with status 0, which hides the bug. The fix returns the value itself, or memory from `malloc` that the caller frees.

### A reference into a vector dangles after growth

When a `std::vector` grows past its capacity, it moves its elements to a larger buffer. Then all references, pointers and iterators to the elements are invalid.[^13]

```cpp fails expect=sanitizer:address timeout=30 id=invalidated
#include <print>
#include <vector>

int main() {
    std::vector<int> scores = {10, 20, 30};
    int& first = scores[0];
    scores.push_back(40);
    first += 1;
    std::println("{}", scores[0]);
}
```

AddressSanitizer reports `heap-use-after-free` at the line `first += 1`. In a build without the sanitizer, the program printed 10, so the increment went to freed memory. The fix uses an index after the growth, as in `scores[0] += 1`.

### Rust rejects the same program

The Rust version of the vector pitfall does not compile. `push` needs a mutable borrow of `scores`, while the shared borrow `first` is still in use.[^29]

```rust fails expect=compile id=borrow
fn main() {
    let mut scores = vec![10, 20, 30];
    let first = &scores[0];
    scores.push(40);
    println!("{first}");
}
```

The compiler stops with error E0502, "cannot borrow `scores` as mutable because it is also borrowed as immutable".[^30] The fix ends the use of `first` before the `push`, or copies the value with `let first = scores[0];`.

### A shallow copy shares nested objects

In JavaScript, the spread syntax copies the properties of an object. A property that holds an object is copied as a reference, so the copy and the original share it.

```javascript wrong id=shallow
const defaults = { name: "report", tags: [] };
const report = { ...defaults };
report.tags.push("draft");
console.log(defaults.tags);
```

```output for=shallow
[ 'draft' ]
```

The fix copies the nested array too, as in `{ ...defaults, tags: [...defaults.tags] }`.

## Cost

- **Indirection.** A read through a pointer needs the address before the value, so it costs at least one extra load. A chain of pointers costs one load for each step.
- **Storage.** Each reference costs one pointer of storage, and the C example prints that size.
- **Copy against share.** To pass a reference costs the same for any size of object. To pass a value costs a copy of the whole value, but the callee then reads it with no indirection.
- **Locality.** The nodes of a linked structure can lie anywhere in memory. The elements of an array lie next to each other, so a walk over an array touches fewer cache lines than a walk over a linked list.
- **Heap allocation.** A value that a reference must keep alive often lives on the heap. In Go, the compiler moves a variable to the heap when its address escapes the function.[^26] Managed heaps add the cost of [[garbage-collection]].

## History

- **1964 and 1965.** Harold Lawson invented the pointer variable at IBM and put it into PL/I. It gave a general-purpose high-level language a flexible way to handle linked lists.[^1]
- **Mid-1960s.** Martin Richards designed BCPL. In BCPL, memory is an array of cells, and a pointer is an integer index into it. The indirection operator was `rv`, later `!`. B used the unary `*`.[^2]
- **1967.** Lawson described the pointer variable in "PL/I List Processing", in the June issue of Communications of the ACM.[^1]
- **1969 to 1973.** Dennis Ritchie and others built C. Pointers in C hold byte addresses, so a dereference needs no scaling. Pointer arithmetic scales the integer by the size of the object instead.[^2]
- **C++.** Bjarne Stroustrup kept the pointers of C for compatibility. He added references directly to support operator overloading. He compares the design with Simula, which has separate operators for assignment to the object and to the reference.[^3]

## References

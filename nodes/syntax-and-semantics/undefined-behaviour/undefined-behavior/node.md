---
tag: CJ0J7P
kind: concept
slug: undefined-behavior
name: Undefined behavior
aliases: [UB, undefined behaviour]
area: syntax-and-semantics/undefined-behaviour
level: core
summary: >-
  Behavior for which the standard imposes no requirements. In C and C++ it covers errors such as signed overflow, and compilers optimize on the assumption that it never happens.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1989
  people: [ANSI X3J11 committee]
  note: The draft of the ANSI C standard defined undefined behavior, unspecified behavior and implementation-defined behavior as separate terms, with integer overflow as its example of undefined behavior.
see_also: [integer-overflow, null-pointer-dereference, use-after-free, out-of-bounds-write, data-races, c, cpp, rust]
same_as:
  wikidata: Q1964707
  wikipedia: Undefined_behavior
sources:
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 3.5.3 undefined behavior, notes 1 and 3 and the example
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 3.65 [defns.undefined] and its note
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 3.5.1 implementation-defined behavior; 3.5.4 unspecified behavior; Annex J
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 3.26 [defns.impl.defined]; 3.66 [defns.unspecified]; 4.1.2 [intro.abstract], paragraphs 1 to 5 and footnote 6
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 4 Conformance, paragraphs 2 and 5
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf
    title: Rationale for International Standard, Programming Languages, C, Revision 5.10, April 2003
    locator: 3 Terms, definitions, and symbols, page 11
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 6.5.1 General, paragraphs 2 and 5; 6.5.4.2 Address and indirection operators, paragraph 4 and footnote 101; 6.5.8 Bitwise shift operators, paragraphs 3 to 5
  - url: https://blog.llvm.org/2011/05/what-every-c-programmer-should-know.html
    title: Chris Lattner, What Every C Programmer Should Know About Undefined Behavior #1/3, LLVM Project Blog, May 13, 2011
    locator: Signed integer overflow; Oversized Shift Amounts; Dereferencing a NULL Pointer; Violating Type Rules
  - url: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
    title: Using the GNU Compiler Collection, Options That Control Optimization
    locator: -fdelete-null-pointer-checks; -fstrict-aliasing
  - url: https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
    title: Using the GNU Compiler Collection, Options for Code Generation Conventions
    locator: -fwrapv
  - url: https://blog.llvm.org/2011/05/what-every-c-programmer-should-know_14.html
    title: Chris Lattner, What Every C Programmer Should Know About Undefined Behavior #2/3, LLVM Project Blog, May 14, 2011
    locator: the contains_null_check example (Dead Code Elimination and Redundant Null Check Elimination)
  - url: https://lwn.net/Articles/342330/
    title: Jonathan Corbet, Fun with NULL pointers, part 1, LWN.net, July 20, 2009
    locator: the tun_chr_poll() code and the paragraph on the NULL test that GCC removes
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 6.9.2.3 [intro.progress], paragraph 1 and note 1
  - url: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
    title: Clang documentation, UndefinedBehaviorSanitizer
    locator: Introduction; Available checks; Issue suppression and the -fno-sanitize-recover option; Minimal runtime
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: Clang documentation, AddressSanitizer
    locator: Introduction (the list of bugs and the typical slowdown); Security Considerations
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 7.7 [expr.const], paragraph 5.8
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 7.20 Checked Integer Arithmetic <stdckdint.h>, 7.20.1 paragraphs 2 and 5
  - url: https://doc.rust-lang.org/reference/behavior-considered-undefined.html
    title: The Rust Reference, Behavior considered undefined
    locator: "[undefined.intro] and the list that follows"
  - url: https://doc.rust-lang.org/reference/behavior-not-considered-unsafe.html
    title: The Rust Reference, Behavior not considered unsafe
    locator: Integer overflow
  - url: https://doc.rust-lang.org/reference/unsafety.html
    title: The Rust Reference, Unsafety
    locator: "[safety.unsafe-ops]"
  - url: https://doc.rust-lang.org/nomicon/meet-safe-and-unsafe.html
    title: The Rustonomicon, Meet Safe and Unsafe
    locator: the paragraph that begins "Safe Rust is the true Rust programming language"
  - url: https://go.dev/ref/spec#Integer_overflow
    title: The Go Programming Language Specification
    locator: Integer overflow; Package unsafe
  - url: https://go.dev/ref/mem
    title: The Go Memory Model
    locator: Introduction (the paragraph on implementation constraints); Implementation Restrictions for Programs Containing Data Races
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.18.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.18.2 Additive Operators (+ and -) for Numeric Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-17.html#jls-17.4.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 17.4.5 Happens-before Order (the paragraph on operations not affected by data races)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/unsafe-code
    title: C# language specification, Unsafe code
    locator: 24.6.2 Pointer indirection; 24.6.4 Pointer element access
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked
    title: C# language reference, The checked and unchecked statements
    locator: the first paragraph
  - url: https://tc39.es/ecma262/2026/#sec-sortindexedproperties
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 23.1.3.30.1 SortIndexedProperties, the paragraph on the sort order
  - url: https://docs.python.org/3.14/reference/datamodel.html#numbers-integral
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.2.4.1 numbers.Integral, Integers (int)
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 7.21.1 The unreachable macro; M.2 Fifth Edition
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 9.12.3 [dcl.attr.assume]
  - url: https://port70.net/~nsz/c/c89/c89-draft.html
    title: Draft of ANSI X3.159-1989, Programming Language C
    locator: 1.6 Definitions of terms, and its Examples
  - url: https://gcc.gnu.org/gcc-4.8/changes.html
    title: GCC 4.8 Release Series, Changes, New Features, and Fixes
    locator: General Optimizer Improvements, AddressSanitizer
  - url: https://gcc.gnu.org/gcc-4.9/changes.html
    title: GCC 4.9 Release Series, Changes, New Features, and Fixes
    locator: General Optimizer Improvements, UndefinedBehaviorSanitizer
  - url: https://gcc.gnu.org/releases.html
    title: GCC Releases
    locator: the release dates of GCC 4.8.0 and GCC 4.9.0
  - url: https://docs.python.org/3.14/library/ctypes.html
    title: The Python Standard Library, ctypes, Python 3.14
    locator: the introduction (the warning on incorrect use); Calling functions
---

## Idea

A language standard says what each program means. For some programs, it chooses to say nothing. The C standard defines undefined behavior as "behavior, upon use of a nonportable or erroneous program construct or of erroneous data, for which this document imposes no requirements".[^1] The C++ standard uses almost the same words: "behavior for which this document imposes no requirements".[^2] Signed integer overflow, the dereference of a null pointer, a read past the end of an array and a read of freed memory are all undefined in both languages.

"No requirements" is meant literally. The implementation may ignore the situation and give an unpredictable result. It may document a behavior. It may also stop the translation or the execution with a diagnostic.[^1] A program with undefined behavior can print the right answer in a debug build, a wrong answer in an optimized build, and crash with a different compiler. None of these results is a bug in the compiler.

The C example below adds 1 to `INT_MAX`. It is built with UndefinedBehaviorSanitizer, which stops the program at the overflow. Without the sanitizer, the program has no defined output, so the page shows none.

```c fails expect=sanitizer:undefined timeout=30 id=idea
#include <limits.h>
#include <stdio.h>

int main(void) {
    int balance = INT_MAX;
    int deposit = 1;
    printf("%d\n", balance + deposit);
}
```

Undefined behavior exists for two reasons. Some errors are expensive or impossible to detect on every machine, such as a pointer to freed memory. And when the standard does not fix a result, each compiler can use the fastest instruction of each machine. The C Rationale says that undefined behavior "gives the implementor license not to catch certain program errors that are difficult to diagnose", and that it marks places where an implementation may extend the language.[^6]

## How it works

### Three kinds of freedom

The C and C++ standards give an implementation three different kinds of freedom. The difference matters, because only one of them makes a program wrong.

| Kind | What the standard says | Example |
|---|---|---|
| Implementation-defined | two or more behaviors are allowed; each implementation documents its choice | the result of a right shift of a negative signed integer; `sizeof(int)` |
| Unspecified | two or more behaviors are allowed; the implementation need not document or repeat its choice | the order in which the arguments of a function call are evaluated |
| Undefined | no requirements | signed overflow; the dereference of a null pointer |

C defines implementation-defined behavior as "unspecified behavior where each implementation documents how the choice is made". Its example is the propagation of the high-order bit when a signed integer is shifted right. Unspecified behavior allows two or more possibilities with no further requirement on which one is chosen, and its example is the order of evaluation of the arguments.[^3] C++ uses the same three terms. Its examples are `sizeof(int)` for an implementation-defined parameter, the order of evaluation of the arguments for an unspecified aspect, and a change to a `const` object for an undefined operation.[^4]

A program that depends on implementation-defined behavior is correct but not portable. A program that depends on unspecified behavior is correct only if every allowed result is acceptable. A program with undefined behavior is not correct. The C standard says that a strictly conforming program produces no output that depends on any of the three kinds.[^5]

C marks undefined behavior in three ways: the words "undefined behavior", a "shall" outside a constraint that the program breaks, and the omission of any definition. The standard says that there is "no difference in emphasis" among the three.[^5] Annex J lists the cases: J.1 for unspecified behavior, J.2 for undefined behavior and J.3 for implementation-defined behavior.[^3]

### Some cases from the C standard

- **Arithmetic.** "If an exceptional condition occurs during the evaluation of an expression (that is, if the result is not mathematically defined or not in the range of representable values for its type), the behavior is undefined."[^7] Signed overflow and division by zero are such conditions. Unsigned arithmetic does not overflow. It wraps modulo 2ⁿ.
- **Shifts.** A shift by a negative amount, or by an amount equal to or greater than the width of the promoted left operand, is undefined. A left shift of a signed value is undefined if the value is negative or if the result does not fit. A right shift of a negative signed value is implementation-defined.[^7] Machines differ here: x86 masks a 32-bit shift amount to 5 bits, and PowerPC masks it to 6 bits.[^8]
- **Pointers.** The unary `*` operator on an invalid pointer is undefined. The invalid values include a null pointer, an address that is not aligned for the type, and the address of an object after the end of its lifetime.[^7] [[null-pointer-dereference]], [[use-after-free]] and [[out-of-bounds-write]] are weaknesses that come from these rules.
- **Order.** Two side effects on the same object that are not sequenced relative to each other, such as `i++ + i++`, are undefined.[^7]

### A whole execution has no meaning

The C++ standard describes the semantics with an abstract machine. An implementation needs to emulate only the observable behavior of this machine, which is the "as-if" rule. Then it says: if an execution contains an undefined operation, the standard "places no requirement on the implementation executing that program with that input (not even with regard to operations preceding the first undefined operation)".[^4] The undefined operation does not only make its own result unknown. It removes every requirement on the execution, before and after it.

A note in the C standard limits this for C. Observable behavior that happens before an operation with undefined behavior appears as specified.[^1] Output that the program has already written stays written. But the compiler can still move or delete code that has no observable effect.

### Why compilers assume that UB never happens

An optimizer must keep the meaning of every program. When an operation is undefined for some inputs, the optimizer needs to keep the meaning only for the other inputs. In practice, it assumes that the program never executes undefined behavior, and it uses that assumption as a fact.

- **Signed overflow.** Because `INT_MAX + 1` is undefined, the compiler may fold `x + 1 > x` to `true` and `x * 2 / 2` to `x`. In a loop `for (i = 0; i <= n; ++i)` with a signed `i`, it may assume that the loop runs exactly `n + 1` times. If overflow wrapped, the loop could be infinite when `n` is `INT_MAX`, and the compiler would have to give up the loop optimizations.[^8] Lattner names a cost of defined overflow on 64-bit targets: many sign extensions inside loops that use `int` as an index.[^8]
- **A null check after a dereference.** If a function dereferences `p` and then tests `p == NULL`, the compiler may reason that `p` cannot be null at the test, because the dereference would have been undefined. GCC documents this as `-fdelete-null-pointer-checks`, enabled by default on most targets: "if a pointer is checked after it has already been dereferenced, it cannot be null".[^9] Lattner shows that the result depends on the order of passes. If dead code elimination runs first, the unused load goes away and the check stays. If redundant null check elimination runs first, the check goes away.[^11]
- **Type-based aliasing.** C forbids access to an `int` object through a `float` lvalue. The compiler may then assume that a store through a `float *` does not change an `int` variable. This allows Clang to turn a loop that zeros an array of `float` into one call of `memset`.[^8] GCC turns on `-fstrict-aliasing` at `-O2`, `-O3` and `-Os`.[^9]
- **Loops with no effect.** C++ lets the implementation assume that every thread eventually terminates, calls a library I/O function, accesses a volatile object or performs a synchronization or atomic operation. A note says this is intended to allow the removal of empty loops, even when termination cannot be proven.[^13]

Each of these steps is valid alone. Together, they can turn a small error into a large change. The optimizer does not detect the undefined behavior and act on it. It uses the absence of undefined behavior as a premise, and the program breaks the premise.

### A real case: the TUN driver

In 2009, Brad Spengler published an exploit for the TUN driver of Linux 2.6.30. The function `tun_chr_poll` read `tun->sk` and then tested `if (!tun)`. GCC removed the test, by default, because the pointer had already been dereferenced. An attacker who mapped the page at address zero could then get into the body of the function with a null `tun` pointer.[^12] GCC keeps such checks when the option `-fno-delete-null-pointer-checks` is given.[^9]

The second C example shows the same pattern, caught by the sanitizer before any optimizer can act on it.

```c fails expect=sanitizer:undefined timeout=30 id=late-null-check
#include <stddef.h>
#include <stdio.h>

struct device {
    int flags;
};

int poll_flags(struct device *dev) {
    int flags = dev->flags; // dereference
    if (dev == NULL) {      // too late: the compiler may delete this
        return -1;
    }
    return flags;
}

int main(void) {
    struct device d = {3};
    printf("%d\n", poll_flags(&d));
    printf("%d\n", poll_flags(NULL));
}
```

### Detection: sanitizers

A sanitizer is a compiler mode that adds checks to the generated code. The program runs as usual, and the run-time library reports the first error that a check finds.

- **UndefinedBehaviorSanitizer** (`-fsanitize=undefined`) checks, among others, array subscripts with a static bound, shifts out of range, misaligned and null pointer dereferences, signed integer overflow and float-to-integer conversions that overflow. For most checks, it prints a report and continues. With `-fno-sanitize-recover`, it prints the report and exits.[^14] Its reports start with `runtime error:`, for example "signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'".[^14] A minimal runtime exists for use in production.[^14]
- **AddressSanitizer** (`-fsanitize=address`) detects out-of-bounds accesses to the heap, the stack and globals, use after free, use after return, double free and memory leaks. Its typical slowdown is 2x. Its runtime is not meant for production executables, because it was not built with security in mind.[^15]

A sanitizer only checks the paths that a test executes. A clean run shows that this input had no undefined behavior that the checks know. It does not prove that the program has none. Data races need a third mode, ThreadSanitizer (`-fsanitize=thread`).[^33]

### Detection: constant evaluation

C++ requires the compiler to reject an operation with undefined behavior in a constant expression.[^16][^2] A `constexpr` function that overflows compiles to unchecked code for a run-time call. The same function, called in a `static_assert`, is a compile error. See the C++ example file.

## In languages

The example files show the same errors in each language: a signed overflow, an index past the end, and the explicit way to check. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| C | native | the term of the standard; Annex J.2 lists the cases |
| C++ | native | the term of the standard; constant evaluation rejects it |
| Rust | native | only in `unsafe` code; safe code panics or wraps |
| C# | native | only in `unsafe` code; safe code wraps or throws |
| Go | absent | overflow wraps and indexes are checked; `unsafe` and multiword races are the exceptions |
| Java | absent | every operation has a result or throws; races are limited by the memory model |
| Python | absent | unlimited integers; errors raise exceptions |
| JavaScript | absent | every operation has a result; some results are implementation-defined |
| TypeScript | absent | as JavaScript |
| Kotlin | absent | the rules of the JVM |
| PowerShell | absent | managed .NET code |
| Bash | absent | no pointers and no raw memory |
| SQL | absent | no pointers and no raw memory |
| Regex | n/a | a pattern has no arithmetic |
| HTML/CSS | n/a | no arithmetic and no memory access |

**Rust** keeps undefined behavior, but only `unsafe` code can cause it. The Reference lists the behaviors that are undefined, among them data races, access through a dangling or misaligned pointer, a break of the aliasing rules and the production of an invalid value. It says: "`unsafe` only means that avoiding undefined behavior is on the programmer".[^18] Integer overflow is not on that list. It panics when debug assertions are on, and otherwise it may panic or wrap.[^19] The operations that only `unsafe` code may use are few: the dereference of a raw pointer, access to a mutable static, access to a union field, a call to an unsafe function, the implementation of an unsafe trait, and a few more.[^20] The Rustonomicon states the result: if all your code is safe Rust, you "will never endure a dangling pointer, a use-after-free, or any other kind of Undefined Behavior".[^21] The Reference puts the duty on the author of the `unsafe` code: no safe code that uses it may be able to trigger undefined behavior. Code that meets this duty is sound, and code that does not is unsound.[^18]

**C#** has the same split. Safe code wraps on overflow by default, and `checked` turns overflow into `System.OverflowException`.[^27] In unsafe code, the specification uses the words of C: if a pointer has an invalid value, the behavior of `*` is undefined, and an out-of-bounds pointer element access is undefined.[^26]

**Go** defines signed overflow. The specification says that "a compiler may not optimize code under the assumption that overflow does not occur. For instance, it may not assume that x < x + 1 is always true."[^22] Package `unsafe` allows operations that violate the type system.[^22] The memory model limits data races: a read of a single machine word must see a value that some write stored. This makes Go "less like C and C++, where the meaning of any program with a race is entirely undefined". A race on a value of several words, such as a slice, a string or an interface, can still lead to arbitrary memory corruption.[^23]

**Java** gives integer overflow a result: the low-order bits of the true sum in two's complement.[^24] A data race can give surprising results, but it cannot make the semantics of other operations wrong, for example the length of an array.[^25]

**JavaScript** has no undefined behavior in this sense, but it has implementation-defined results. The sort order of `Array.prototype.sort` is implementation-defined if the comparator is not consistent.[^28] **Python** integers have an unlimited range, subject only to memory.[^29] The `ctypes` module is the exception: it reaches the memory of the process directly, and its documentation warns that incorrect use can corrupt data or crash the process.[^36]

## Variations and related ideas

- **Defined errors.** Some languages define a bad operation as an error with a known effect, such as a panic or an exception. Safe Rust, Go, Java and C# do this for an index out of range. The cost is a range check on each access.[^8]
- **Wrapping as a choice.** Rust, C# and Go make wrapping a defined result. GCC and Clang offer `-fwrapv`, which makes signed overflow wrap in two's complement for addition, subtraction and multiplication.[^10] The flag removes this one source of undefined behavior and the optimizations that depend on it.
- **Checked arithmetic in the library.** C23 adds `ckd_add`, `ckd_sub` and `ckd_mul`. They compute the result as if the type had infinite range, store the wrapped value, and return `true` if the stored value is not the true result.[^17] Rust has `checked_add`, `wrapping_add`, `saturating_add` and `overflowing_add`. Java has `Math.addExact`.
- **Undefined behavior on purpose.** C23 `unreachable()` and the C++23 attribute `[[assume(expr)]]` give the compiler a fact that it can use. If the fact is false at run time, the behavior is undefined.[^30][^31] These tools make the optimizer's assumption explicit and local.
- **Weaknesses.** [[integer-overflow]], [[null-pointer-dereference]], [[use-after-free]] and [[out-of-bounds-write]] describe the security side of the same rules. [[data-races]] describes the concurrency side.

## Pitfalls

### Checking for overflow after the overflow

A test such as `if (a + b < a)` for signed `a` and `b` reads a value that the program had no right to compute. The compiler may fold it to `false`. The C++ example shows the same error in the form `x + 1 > x`. Test the operands before the operation, or use `ckd_add` in C23. Unsigned types wrap by definition, so the test `a + b < a` is correct for them.

### Trusting the output of a debug build

A program with undefined behavior often gives the expected result at `-O0` and a different one at `-O2`. Both builds are correct translations. The C standard requires nothing, and C++ removes every requirement on the whole execution.[^4] A passing test at one optimization level does not show that the code is correct. Run the tests under `-fsanitize=undefined` and `-fsanitize=address` as well.

### Shift amounts from data

A shift amount that comes from input can be equal to the width of the type. The result is undefined, also for unsigned types.[^7] Check the amount first, as the defined C example does: `n < 32 ? 1u << n : 0u`.

```c fails expect=sanitizer:undefined timeout=30 id=shift-width
#include <stdio.h>

unsigned int mask(unsigned int bits) {
    return (1u << bits) - 1; // undefined when bits is 32
}

int main(void) {
    printf("%u\n", mask(8));
    printf("%u\n", mask(32));
}
```

### Treating implementation-defined as undefined, or the reverse

`-7 >> 1` is implementation-defined in C, so a program may use it on the compilers that document an arithmetic shift.[^3][^7] `-7 << 1` looks like the same kind of operation, but it is undefined, because C defines a left shift of a signed value only when the value is not negative and the result fits.[^7] Read the clause, or the Annex J entry, before you decide which one you have.

### Believing that unsafe code is local

In Rust and C#, undefined behavior in an `unsafe` block does not stay in the block. If the block breaks an invariant, such as the length of a vector, safe code that runs later can read out of bounds. In Rust, the `unsafe` code must be correct for every use that safe code can make of it.[^18]

## Cost

- **Of undefined behavior.** It allows faster code. Lattner gives the examples: no range check on each array access, no extra `and` for each variable shift, loops that the compiler can analyze, and type-based alias analysis.[^8] The price is that a small bug can have effects far from its source, and that the effects can change with the compiler version.
- **Of defined behavior.** A bounds check adds a compare and a branch to each access. Lattner adds that range checks in C would also need a change of the ABI, so that range information travels with each pointer. Defined wrapping costs the loop optimizations that need the assumption of no overflow.[^8]
- **Of detection.** AddressSanitizer slows a program by about 2x.[^15] UndefinedBehaviorSanitizer adds a check at each instrumented operation. Both are for test builds. The minimal UBSan runtime is the documented exception for production.[^14]

## History

- **1989.** The draft of the ANSI C standard (X3.159-1989) defined undefined behavior, unspecified behavior and implementation-defined behavior. Its example of undefined behavior was integer overflow.[^32]
- **2003.** The C99 Rationale explained the three terms: undefined behavior lets an implementation skip errors that are difficult to diagnose, and it marks places where an implementation may extend the language.[^6]
- **2009.** An exploit of the Linux TUN driver used a null check that GCC had removed after a dereference.[^12]
- **2011.** Chris Lattner published a series on the LLVM blog that explained which optimizations depend on undefined behavior and why a small error can have large effects.[^8][^11]
- **2013 and 2014.** GCC 4.8.0 (March 22, 2013) added AddressSanitizer, and GCC 4.9.0 (April 22, 2014) added UndefinedBehaviorSanitizer.[^33][^34][^35]
- **2024.** C23, the fifth edition of the C standard, added the `unreachable` macro, whose invocation is undefined if the program reaches it. The same edition has the checked integer macros of `<stdckdint.h>`.[^30][^17]

## References

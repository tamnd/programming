---
tag: PQW5JT
kind: weakness
slug: integer-overflow
name: Integer overflow or wraparound
aliases: [integer overflow, integer wraparound, wraparound, cwe-190]
area: security/memory-safety-weaknesses
level: core
summary: >-
  An integer overflow happens when a calculation produces a value that does not fit in its integer type, so the program continues with a wrapped, truncated or undefined result that it treats as correct.
cwe: 190
abstraction: base
languages_affected: [c, cpp, csharp, go, java, kotlin, rust, javascript]
history:
  year: 2002
  people: [blexim]
  note: The Phrack 60 article "Basic Integer Overflows" described integer overflows and signedness bugs in C as two classes of bugs that change the values of variables, not memory, and showed that they often lead to buffer overflows.
same_as:
  wikidata: Q1423448
  wikipedia: Integer_overflow
sources:
  - url: https://cwe.mitre.org/data/definitions/190.html
    title: "CWE-190: Integer Overflow or Wraparound"
    locator: Description; Extended Description; Alternate Terms; Common Consequences; Potential Mitigations; Relationships; Observed Examples
  - url: https://cwe.mitre.org/top25/archive/2025/2025_onthecusp_list.html
    title: 2025 CWE Top 25, Weaknesses On the Cusp
    locator: the ranked list, rank 30, CVEs in KEV, Rank Last Year
  - url: https://owasp.org/Top10/2025/X01_2025-Next_Steps/
    title: "OWASP Top 10:2025, Next Steps, X02:2025 Memory Management Failures"
    locator: X02:2025, Description; How to prevent; List of Mapped CWEs
  - url: http://phrack.org/archives/issues/60/10.txt
    title: "blexim, Basic Integer Overflows, Phrack Magazine 11(60), article 10, December 2002"
    locator: 1.2 What is an integer overflow?; 1.3 Why can they be dangerous?
  - url: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
    title: Clang documentation, UndefinedBehaviorSanitizer
    locator: Available checks, -fsanitize=signed-integer-overflow and -fsanitize=unsigned-integer-overflow
  - url: https://en.cppreference.com/w/c/numeric/ckd_add
    title: cppreference, ckd_add
    locator: the description and the return value
  - url: https://doc.rust-lang.org/book/ch03-02-data-types.html
    title: The Rust Programming Language, Data Types
    locator: Integer Overflow
  - url: https://doc.rust-lang.org/reference/behavior-not-considered-unsafe.html
    title: The Rust Reference, Behavior not considered unsafe
    locator: Integer overflow
  - url: https://go.dev/ref/spec#Integer_overflow
    title: The Go Programming Language Specification
    locator: Integer overflow
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
    title: Java SE 25 API, java.lang.Math
    locator: addExact; multiplyExact
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked
    title: C# language reference, The checked and unchecked statements
    locator: the opening section
  - url: https://docs.python.org/3/library/stdtypes.html#numeric-types-int-float-complex
    title: The Python Standard Library, Built-in Types
    locator: Numeric Types, int, float, complex
  - url: https://docs.python.org/3/library/ctypes.html#fundamental-data-types
    title: The Python Standard Library, ctypes
    locator: Fundamental data types, c_byte and the other integer types
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MAX_SAFE_INTEGER
    title: MDN Web Docs, Number.MAX_SAFE_INTEGER
    locator: Description
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-uncontrolled-arithmetic/
    title: CodeQL query help, Uncontrolled data in arithmetic expression (cpp/uncontrolled-arithmetic)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-integer-multiplication-cast-to-long/
    title: CodeQL query help, Multiplication result converted to larger type (cpp/integer-multiplication-cast-to-long)
    locator: the query description
  - url: https://www.cve.org/CVERecord?id=CVE-2002-0639
    title: CVE-2002-0639, OpenSSH
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-30860
    title: CVE-2021-30860, Apple CoreGraphics
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2025-27363
    title: CVE-2025-27363, FreeType
    locator: the description and the publication date
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.17.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.17.1 Multiplication Operator *
  - url: https://kotlinlang.org/docs/numbers.html
    title: Kotlin documentation, Numbers
    locator: Data overflow, Overflow in operations
---

## Description

A machine integer has a fixed number of bits. A 32-bit `int` holds values from −2,147,483,648 to 2,147,483,647, and nothing more. Arithmetic in a program looks like arithmetic in mathematics, but the result must fit in the type. An integer overflow happens when it does not fit. The result wraps around to a small or negative value, or it is truncated, and the program goes on as if the value were correct.[^1] An underflow is the same error in the other direction, below the minimum. CWE files it as CWE-191.[^1]

The root cause is the distance between the integers of mathematics and the integers of the machine. The code says `price * quantity`, and the developer thinks of the product. The machine computes the product modulo 2³² or 2⁶⁴. The two agree for every value that a test is likely to use, so the bug stays hidden until an input is large. blexim wrote in 2002 that an overflow cannot be detected after it happens, so the program has no way to tell if a result is correct.[^4] The check must come before the operation, and a check written after it, such as `if (total < 0)`, is too late and often wrong.

Each language defines the result differently. In C and C++, unsigned arithmetic wraps modulo 2ⁿ. Signed overflow is undefined behavior, so a conforming compiler may do anything, from ignoring the overflow to aborting the program.[^4] Go, Java, Kotlin and C# in its default context wrap signed values.[^9][^20][^21][^11] Rust panics in a debug build and wraps in a release build.[^7] Python integers have unlimited precision.[^12] JavaScript numbers are doubles, which lose exact integers above 2⁵³ − 1.[^14]

The wrapped value is dangerous because of what the program does with it next. CWE-190 says that the weakness becomes a security problem when the result controls a loop, a security decision such as a quota or a limit, or the size of an allocation or a copy.[^1] A size that wraps to a small value makes a small buffer, and the copy that follows writes past its end. CWE-680 names this chain, and the page [[out-of-bounds-write]] shows the write. A time value that wraps can make an expiry check fail. CWE-190 lists the Year 2038 problem as an alternate term: some 32-bit time values on Unix-related systems overflow on January 19, 2038.[^1]

CWE-190 is a base weakness under CWE-682, incorrect calculation. It lists C as a language where it is often prevalent.[^1] It is not in the 2025 CWE Top 25. It is rank 30 on the list of weaknesses on the cusp, with 2 CVEs in the CISA catalogue of known exploited vulnerabilities, down from rank 23 the year before.[^2] The OWASP Top 10:2025 maps it to X02:2025 Memory Management Failures, a category in its Next Steps section and not in the ten.[^3]

### History

In December 2002, Phrack 60 published "Basic Integer Overflows" by blexim.[^4] It described integer overflows and signedness bugs in C as classes of bugs that put unexpected values in variables, not bugs that overwrite memory directly. It said that most of them are not exploitable, that they can lead to other bugs, frequently buffer overflows, and that they are hard to spot, so even well audited code can contain them.[^4] In the same year, CVE-2002-0639 recorded an integer overflow in the challenge response authentication of OpenSSH that let remote attackers run code.[^17]

## Vulnerable code

`order_fits` checks that an order is inside a budget. The second call uses the classic demonstration input, a quantity so large that the product does not fit in an `int`. The block builds with UndefinedBehaviorSanitizer, which reports the overflow.[^5]

```c fails expect=sanitizer:undefined timeout=30 id=multiply
#include <stdio.h>

// Returns 1 if the order costs no more than the budget.
int order_fits(int price, int quantity, int budget) {
    int total = price * quantity; // can overflow
    return total <= budget;
}

int main(void) {
    printf("3 items: %d\n", order_fits(100, 3, 1000));
    printf("42949673 items: %d\n", order_fits(100, 42949673, 1000));
}
```

UndefinedBehaviorSanitizer reports `signed integer overflow: 100 * 42949673 cannot be represented in type 'int'`. The true product is 4,294,967,300. If the multiplication wraps modulo 2³², the result is 4, and the order of forty million items fits in a budget of 1,000. The behavior is undefined, so the standard does not promise even that. Nothing in the output says that the result was wrong.

## Fixed code

The fix checks the operands before the multiplication. A product of two positive values fits only if `price` is not greater than `INT_MAX / quantity`. The block builds with AddressSanitizer and UndefinedBehaviorSanitizer, and both stay silent.

```c run flags="-fsanitize=address,undefined" id=check-first
#include <limits.h>
#include <stdio.h>

// Returns 1 if the order costs no more than the budget.
// Price and quantity must not be negative.
int order_fits(int price, int quantity, int budget) {
    if (price < 0 || quantity < 0) {
        return 0;
    }
    if (quantity != 0 && price > INT_MAX / quantity) {
        return 0; // the total does not fit in an int
    }
    int total = price * quantity;
    return total <= budget;
}

int main(void) {
    printf("3 items: %d\n", order_fits(100, 3, 1000));
    printf("42949673 items: %d\n", order_fits(100, 42949673, 1000));
}
```

```output for=check-first
3 items: 1
42949673 items: 0
```

The check rejects negative values first, as CWE-190 asks: a range check must test the minimum and the maximum.[^1] The header `<stdckdint.h>` of C has `ckd_add`, `ckd_sub` and `ckd_mul`. Each one computes the result as if the type had infinite range, stores the wrapped value and returns `true` when the value does not fit.[^6] The C example file shows `ckd_mul`.

### Checked arithmetic in other languages

Every language with fixed-width integer types has the same limit. The languages differ in what happens by default and in how much work the check takes.

Rust checks for overflow in a debug build. The program below panics with `attempt to multiply with overflow`.

```rust fails expect=panic id=debug-panic
fn order_fits(price: i32, quantity: i32, budget: i32) -> bool {
    let total = price * quantity;
    total <= budget
}

fn main() {
    println!("3 items: {}", order_fits(100, 3, 1000));
    println!("42949673 items: {}", order_fits(100, 42949673, 1000));
}
```

The Rust book says that a release build does not include these checks and wraps the value in two's complement, and that relying on the wrap is an error.[^7] The Rust reference calls overflow a programmer error, not undefined behavior.[^8] The `checked_*` methods return `None` on overflow in every build, and the Rust example file uses `checked_mul` for the fix.[^7]

In Java, `Math.multiplyExact` and `Math.addExact` throw `ArithmeticException` when the result overflows.[^10] In C#, a `checked` block or expression throws `System.OverflowException`. The default context is unchecked, and it discards the high-order bits.[^11] The Go specification says that signed overflow does not cause a run-time panic and that a compiler may not assume that `x < x + 1` is always true, so Go code must check the operands itself.[^9] Each example file shows the silent wrap and the check.

## How to detect it

- **Sanitizers.** Build the tests with `-fsanitize=undefined`. Its check `signed-integer-overflow` reports signed overflow, including `INT_MIN / -1`. The check `unsigned-integer-overflow` reports unsigned wraps. They are not undefined behavior, but they are often unintentional, so this check is not in the default set.[^5]
- **Fuzzing.** CWE-190 rates fuzzing as moderately effective.[^1] A fuzzer finds the large values that hand-written tests skip, and the sanitizer turns each overflow into a failure.
- **Static analysis.** CWE-190 rates automated static analysis as highly effective.[^1] CodeQL query `cpp/uncontrolled-arithmetic` finds arithmetic on data from outside the program with no guard.[^15] Query `cpp/integer-multiplication-cast-to-long` finds a product that is converted to a larger type after the multiplication, when the overflow has already happened.[^16]
- **Code review.** For each calculation of a size, a count, an index, an amount of money or a time, find the largest input and work out the result. Look for `malloc(n * size)`, `len + 1`, subtraction of unsigned values, and casts between signed and unsigned types.

## How to prevent it

- **Choose types and languages that check.** CWE-190 lists language selection first. Python integers do not overflow.[^12] Rust checks in debug builds, and checked APIs exist in Java, C# and C.[^7][^10][^11][^6]
- **Use checked operations for values from input.** Use `ckd_mul`, `checked_mul`, `Math.multiplyExact` or a `checked` block, and handle the failure. CWE-190 also names safe integer libraries such as SafeInt for C++.[^1]
- **Validate the range of input.** Check the minimum and the maximum of each numeric input before any arithmetic, against limits that come from the problem, not from the type.[^1]
- **Know the representation.** Learn the width of each type, the rules for signed and unsigned conversion, and the behavior on overflow of the language. CWE-190 links this to CWE-681, incorrect conversion between numeric types.[^1]
- **Treat compiler warnings as errors.** CWE-190 says to examine compiler warnings closely, such as a signed and unsigned mismatch in a memory operation.[^1]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2002-0639 | published 2003-04-02 | An integer overflow in `sshd` in OpenSSH 2.9.9 through 3.3 let remote attackers run code during challenge response authentication with SKEY or BSD_AUTH.[^17] |
| CVE-2021-30860 | published 2021-08-24 | An integer overflow in Apple operating systems let a crafted PDF run code. Apple was aware of a report that it may have been actively exploited.[^18][^1] |
| CVE-2025-27363 | published 2025-03-11 | FreeType 2.13.0 and earlier assigned a signed short value to an unsigned long and added a constant, so the size wrapped around and the heap buffer was too small. The code then wrote out of bounds. It may have been exploited in the wild.[^19][^1] |

## References

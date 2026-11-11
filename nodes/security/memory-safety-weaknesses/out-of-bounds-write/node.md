---
tag: P5VTR9
kind: weakness
slug: out-of-bounds-write
name: Out-of-bounds write
aliases: [buffer overflow, buffer overrun, memory corruption, cwe-787]
area: security/memory-safety-weaknesses
level: core
summary: >-
  An out-of-bounds write happens when a program writes data past the end, or before the start, of a buffer, and so changes memory that belongs to other data or to the control flow of the program.
cwe: 787
abstraction: base
languages_affected: [c, cpp, assembly, unsafe-code-in-any-language]
cwe_top25_2025: 5
history:
  year: 1988
  people: [Robert T. Morris]
  note: The Internet Worm of 2 November 1988 sent a string of 536 bytes to the fingerd daemon, which read it with gets into a buffer of 512 bytes on the stack; on VAX systems the overflow changed the return address.
sources:
  - url: https://cwe.mitre.org/data/definitions/787.html
    title: "CWE-787: Out-of-bounds Write"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Relationships; Applicable Platforms; Observed Examples
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 5, CVEs in KEV
  - url: https://owasp.org/Top10/2025/X01_2025-Next_Steps/
    title: "OWASP Top 10:2025, Next Steps, X02:2025 Memory Management Failures"
    locator: X02:2025, Description; How to prevent; List of Mapped CWEs
  - url: https://www.acsac.org/2003/papers/classic-spafford.pdf
    title: "Eugene H. Spafford, A Failure to Learn from the Past, ACSAC 2003"
    locator: 1. Introduction; 3.1 fingerd and gets; the description of the attack via the finger daemon
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: Clang documentation, AddressSanitizer
    locator: Introduction; How to build
  - url: https://eel.is/c++draft/sequence.reqmts
    title: "Working Draft, Programming Languages: C++, [sequence.reqmts]"
    locator: a[n]; a.at(n)
  - url: https://eel.is/c++draft/structure.specifications
    title: "Working Draft, Programming Languages: C++, [structure.specifications]"
    locator: Hardened preconditions
  - url: https://doc.rust-lang.org/reference/expressions/array-expr.html
    title: The Rust Reference, Array and array index expressions
    locator: Array and slice indexing expressions
  - url: https://go.dev/ref/spec#Index_expressions
    title: The Go Programming Language Specification
    locator: Index expressions
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
    title: "The Java Language Specification, Java SE 25 Edition, Chapter 10: Arrays"
    locator: 10.4 Array Access
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.indexoutofrangeexception
    title: .NET API, IndexOutOfRangeException Class
    locator: Remarks
  - url: https://docs.python.org/3.14/library/exceptions.html#IndexError
    title: The Python Standard Library, Built-in Exceptions, Python 3.14
    locator: IndexError
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray
    title: MDN Web Docs, TypedArray
    locator: Description; Examples, Property access
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-overrunning-write/
    title: CodeQL query help, Potentially overrunning write (cpp/overrunning-write)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-unbounded-write/
    title: CodeQL query help, Unbounded write (cpp/unbounded-write)
    locator: the query description
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-badly-bounded-write/
    title: CodeQL query help, Badly bounded write (cpp/badly-bounded-write)
    locator: the query description
  - url: https://www.chromium.org/Home/chromium-security/memory-safety/
    title: The Chromium Projects, Memory safety
    locator: the opening statistics
  - url: https://www.cve.org/CVERecord?id=CVE-2021-3156
    title: CVE-2021-3156, Sudo
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-0022
    title: CVE-2020-0022, Android Bluetooth
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2023-4863
    title: CVE-2023-4863, libwebp
    locator: the description and the publication date
---

## Description

A buffer is a run of memory with a fixed size: an array, a string, a block from `malloc`. In C and C++, a write through an index or a pointer goes to the address that the program computed. Nothing checks that the address is inside the buffer. An out-of-bounds write happens when the program writes past the end of the buffer, or before its start.[^1] The bytes land in the memory next to it, which can hold other variables, the bookkeeping of the allocator or a return address. The program meant to store a name. It changed something else.

The root cause is that the size of a buffer and the code that writes to it are separate. The size is fixed when the buffer is made. The amount of data comes later, often from input. Every copy, every loop and every index must repeat the size check, and one missed check is enough. Common forms are a copy with no size limit, a size that does not count the final `'\0'` of a string, a loop that runs one step too far, and a size computed with an integer that wraps around.[^1]

The consequences range from a crash to code execution. CWE-787 says that an attacker can sometimes change control data such as return addresses and run unexpected code.[^1] A write that changes only a nearby variable can still change the decisions of the program. When the bug does not crash, it is silent: the program goes on with corrupt memory.

CWE-787 names this weakness. Its alternate term is memory corruption. It is a base weakness under CWE-119, improper restriction of operations within the bounds of a memory buffer. Its children include the classic buffer overflow (CWE-120), the stack-based buffer overflow (CWE-121), the heap-based buffer overflow (CWE-122) and the buffer underwrite (CWE-124).[^1] CWE-787 lists C and C++ as languages where it is often prevalent.[^1] It is rank 5 of the 2025 CWE Top 25, with 12 CVEs in the CISA catalogue of known exploited vulnerabilities.[^2] The OWASP Top 10:2025 maps it to X02:2025 Memory Management Failures, a category in its Next Steps section and not in the ten.[^3]

### History

On the evening of 2 November 1988, a worm program spread through the Internet to thousands of machines that ran BSD-derived versions of UNIX.[^4] One of its attacks used the `fingerd` daemon. `fingerd` read one line of input with the C library function `gets`, which does no bounds check. The worm sent a string of 536 bytes into a buffer of 512 bytes, and on VAX computers the overflow changed the return address of `main` so that it pointed into the buffer.[^4] Spafford's paper, written fifteen years later, says that a whole family of routines in the C library can overrun buffers unless the programmer gives limits, and that hundreds of buffer overflow vulnerabilities had been found since.[^4] The paper names the author of the worm as Robert T. Morris.[^4]

## Vulnerable code

The function below copies a name to the heap. `malloc(strlen(name))` leaves no room for the final `'\0'`, and `strcpy` writes it one byte past the end. The block builds with AddressSanitizer, which reports the write.[^5]

```c fails expect=sanitizer:address timeout=30 id=heap-copy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of name on the heap.
char *copy_name(const char *name) {
    char *copy = malloc(strlen(name)); // no room for the final '\0'
    strcpy(copy, name);
    return copy;
}

int main(void) {
    char *name = copy_name("alice");
    printf("%s\n", name);
    free(name);
}
```

AddressSanitizer stops the program with a `heap-buffer-overflow` report for a `WRITE of size 6` in `strcpy`. Without the sanitizer, the same program can print `alice` and exit with status 0. The bug is still there, and a longer name from input makes it larger. The C example file also shows a stack buffer that `strcat` overflows with the classic demonstration input, a long run of `A`.

## Fixed code

The fix counts the `'\0'` in the size, and it copies exactly that size. The block builds with AddressSanitizer and UndefinedBehaviorSanitizer, and both stay silent.

```c run flags="-fsanitize=address,undefined" id=bounded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of name on the heap, or NULL.
char *copy_name(const char *name) {
    size_t size = strlen(name) + 1; // room for the final '\0'
    char *copy = malloc(size);
    if (copy != NULL) {
        memcpy(copy, name, size);
    }
    return copy;
}

int main(void) {
    char *name = copy_name("alice");
    if (name == NULL) {
        return 1;
    }
    printf("%s\n", name);
    free(name);
}
```

```output for=bounded
alice
```

In C, each write needs a size that comes from the destination. `snprintf` takes the size of the destination and tells the program when the text did not fit. The C example file shows it. CWE-787 warns that `strncpy` may leave a string without the final `'\0'`, so it is not a full replacement for `strcpy`.[^1]

### Languages that check the index

In most languages, an array or a string knows its length, and each index is checked at run time. The check turns the write into an error before any memory changes.

```rust fails expect=panic id=rust-index
fn set_score(scores: &mut [i32], player: usize, score: i32) {
    scores[player] = score;
}

fn main() {
    let mut scores = vec![0; 4];
    set_score(&mut scores, 2, 10);
    println!("player 2: {}", scores[2]);
    set_score(&mut scores, 4, 99);
}
```

The Rust reference says that an index that fails the check puts the thread in a panicked state.[^8] Go panics at run time.[^9] Java throws `ArrayIndexOutOfBoundsException`, because all array accesses are checked.[^10] C# throws `IndexOutOfRangeException`.[^11] Python raises `IndexError`.[^12] A JavaScript typed array reads and writes only its `ArrayBuffer`, and the JavaScript example shows that a write past the end is dropped.[^13] Each example file shows the error and a fixed version that checks the index first.

In C++, `std::vector::at` throws `std::out_of_range` when the index is not less than `size()`.[^6] `operator[]` has no such guarantee. The working draft of the standard lists `n < a.size()` as a hardened precondition of `a[n]`: a hardened implementation checks it and terminates, and in an implementation that is not hardened, a violation is undefined behavior.[^6][^7]

A panic or an exception is still a denial of service. It is not memory corruption, and it does not let an attacker change other data. The protection ends at `unsafe` code, raw pointers and native libraries. CWE-787 warns that the interface of a language to native code may still be subject to overflows.[^1]

## How to detect it

- **Sanitizers.** Build the tests with `-fsanitize=address`. AddressSanitizer detects out-of-bounds accesses to the heap, the stack and globals, with a typical slowdown of two times.[^5] It finds a bug only when a test runs the line with a bad size.
- **Fuzzing.** A fuzzer gives a sanitized build many generated inputs, so the tests reach sizes that a person did not think of. OWASP lists fuzzing of every input as a defence.[^3]
- **Static analysis.** CodeQL queries `cpp/overrunning-write`, `cpp/unbounded-write` and `cpp/badly-bounded-write` find copies and writes with no limit or with a wrong limit.[^14][^15][^16] OWASP says to fix all compiler errors and warnings.[^3]
- **Code review.** Search for `strcpy`, `strcat`, `sprintf`, `gets`, `memcpy` with a computed size, `malloc(strlen(...))` without `+ 1`, and loops with `<=`. For each write, find where the size of the destination comes from.

## How to prevent it

- **Use a memory-safe language.** OWASP puts a memory-safe language first among its defences against memory management failures, and CWE-787 lists language selection as its first mitigation.[^3][^1] The Chromium project finds that around 70% of its high severity security bugs are memory unsafety problems.[^17]
- **Use types that carry their size.** In C++, use `std::vector`, `std::string`, `std::array` and `std::span`, and `at` where the index comes from input. OWASP says to use managed buffers and strings rather than raw arrays and pointers.[^3]
- **Pass the size with every write.** In C, use `snprintf` and functions that take the size of the destination, check each index against the length, and count the final `'\0'`.[^1][^14]
- **Check size arithmetic.** A size that wraps around makes a small buffer, and every later write overflows it. CWE-787 lists this chain in its examples.[^1]
- **Harden the build.** Compiler options such as `FORTIFY_SOURCE` and stack canaries, and the address space layout randomization and non-executable memory of the operating system, make an overflow harder to exploit.[^1] CWE-787 says that none of these is a complete solution.[^1]

## Real cases

| Case | Date | What happened |
|---|---|---|
| Internet Worm | 1988-11-02 | The worm overflowed the 512-byte input buffer of `fingerd` with a 536-byte string, read by `gets`, and changed the return address on VAX systems.[^4] |
| CVE-2021-3156 | published 2021-01-26 | An off-by-one error in Sudo led to a heap-based buffer overflow and let a user become root through `sudoedit -s` and an argument that ends with a single backslash.[^18] |
| CVE-2020-0022 | published 2020-02-13 | An incorrect bounds calculation in `reassemble_and_dispatch` of the Android Bluetooth stack allowed an out-of-bounds write and remote code execution over Bluetooth with no user interaction.[^19][^1] |
| CVE-2023-4863 | published 2023-09-12 | A heap buffer overflow in libwebp let a crafted HTML page write out of bounds in Google Chrome.[^20] |

## References

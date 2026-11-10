---
tag: NEW-c
kind: language
slug: c
name: C
area: languages
level: intro
summary: >-
  C is a general-purpose, compiled language for system programming, with static, weak typing, pointers and manual memory management. The standard ISO/IEC 9899 defines it.
class: programming
tier: 1
paradigms: [imperative, procedural]
typing: {discipline: static, strength: weak, checking: [static], inference: local}
memory: manual
execution: [compiled-native]
appeared: 1972
designers: [dennis-ritchie]
influenced_by: [b, bcpl, algol-68, pl-i]
standard: iso-iec-9899
reference: {url: "https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf", licence: iso-copyright}
licence_of_reference_impl: null
file_extensions: [c, h]
hello_world: examples/c.md#hello
same_as: {wikidata: Q15777, wikipedia: C_(programming_language), pldb: c, linguist: C, rosetta: null, hopl: null}
sources:
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3280.htm#Introduction
    title: The C Standard charter, WG14 N3280
    locator: Introduction
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/
    title: ISO/IEC JTC1/SC22/WG14, C
    locator: home page
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 4 Conformance
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf
    title: Rationale for International Standard, Programming Languages, C, Revision 5.10
    locator: 0. Introduction, Keep the spirit of C
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 3.5.3 undefined behavior; J.2 Undefined behavior
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 5.1.2.3.2 Program startup
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.3.2.1 Lvalues, arrays, and function designators
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 7.24.3 Memory management functions; 7.24.3.3 The free function; 7.24.3.6 The malloc function
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.4.1 Keywords
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 7.21.2 The nullptr_t type
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.7.2 Storage-class specifiers
    accessed: 2026-11-10
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: GitHub Linguist, languages.yml
    locator: C, extensions
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.2.5 Types
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.3.1.8 Usual arithmetic conversions
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.7.10 Type inference
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 7.22 Integer types <stdint.h>
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.3.1.3 Signed and unsigned integers
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.5.1 General, paragraph 5
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 7.20 Checked Integer Arithmetic <stdckdint.h>
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: AddressSanitizer, Clang documentation
    locator: Introduction
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
    title: UndefinedBehaviorSanitizer, Clang documentation
    locator: Introduction
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 6.2.4 Storage durations of objects
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 7.1.1 Definitions of terms
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 5 Environment; 5.1.1.1 Program structure
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: Annex M (informative) Change History, M.2 to M.6
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 7.5 Errors <errno.h>
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 5.1.2.5 Multi-threaded executions and data races
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/projects
    title: Project status and milestones, WG14
    locator: ISO/IEC 9899, Revision of the C standard; Milestones
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/82075.html
    title: ISO/IEC 9899:2024
    locator: General information; Life cycle
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/74528.html
    title: ISO/IEC 9899:2018
    locator: General information; Life cycle
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html
    title: Using the GNU Compiler Collection (GCC), Options Controlling C Dialect
    locator: -std
    accessed: 2026-11-10
  - url: https://clang.llvm.org/c_status.html
    title: C Support in Clang
    locator: the table of standards; C2y implementation status; C23 implementation status
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/57853.html
    title: ISO/IEC 9899:2011
    locator: General information; Life cycle
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/29237.html
    title: ISO/IEC 9899:1999
    locator: General information; Life cycle
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/23909.html
    title: ISO/IEC 9899:1990/Amd 1:1995
    locator: General information; Life cycle
    accessed: 2026-11-10
  - url: https://www.iso.org/standard/17782.html
    title: ISO/IEC 9899:1990
    locator: General information; Life cycle
    accessed: 2026-11-10
  - url: https://nvlpubs.nist.gov/nistpubs/Legacy/FIPS/fipspub160.pdf
    title: ANSI X3.159-1989, Programming Language C (NIST FIPS PUB 160)
    locator: title page; Foreword
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: cover page; Foreword
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/wg14_document_log.htm
    title: WG14 document log
    locator: N3096; N3149; N3219; N3220; N3998
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/projects/c-status.html
    title: C Standards Support in GCC
    locator: C2y Support in GCC; C23 Support in GCC
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/releases.html
    title: GCC Releases
    locator: GCC Timeline
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/onlinedocs/gcc/Copying.html
    title: Using the GNU Compiler Collection (GCC)
    locator: GNU General Public License, Version 3
    accessed: 2026-11-10
  - url: https://github.com/llvm/llvm-project/releases/tag/llvmorg-23.1.5
    title: LLVM 23.1.5 release, llvm/llvm-project
    locator: the release
    accessed: 2026-11-10
  - url: https://github.com/llvm/llvm-project/blob/main/LICENSE.TXT
    title: LLVM Project, LICENSE.TXT
    locator: the first line
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance?view=msvc-170
    title: Microsoft C/C++ language conformance
    locator: C23 Core language features; Optional C11 Core Language features
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/visualstudio/releases/2026/release-notes
    title: Visual Studio 2026 release notes
    locator: Version 18.11.3; May Update 18.6.0, MSVC Build Tools v14.51
    accessed: 2026-11-10
  - url: https://visualstudio.microsoft.com/license-terms/
    title: License Terms, Visual Studio
    locator: Visual Studio 2026
    accessed: 2026-11-10
  - url: https://cmake.org/cmake/help/latest/manual/cmake.1.html
    title: cmake(1), CMake documentation
    locator: Description
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/vcpkg/get_started/overview
    title: What is vcpkg?, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://docs.conan.io/2/introduction.html
    title: Introduction, Conan 2 documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024
    locator: 7.2 Diagnostics <assert.h>
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/ClangFormat.html
    title: ClangFormat, Clang documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://clang.llvm.org/extra/clang-tidy/
    title: Clang-Tidy, Extra Clang Tools documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/chist.html
    title: Dennis M. Ritchie, The Development of the C Language
    locator: "Introduction; Origins: the languages; Embryonic C; Neonatal C; Acknowledgments"
    accessed: 2026-11-10
  - url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/chist.html
    title: Dennis M. Ritchie, The Development of the C Language
    locator: Portability; Standardization; Critique
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3280.htm#Principles
    title: The C Standard charter, WG14 N3280
    locator: Principles
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#ancestors
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: What are Go's ancestors?
    accessed: 2026-11-10
---

## Overview

C is a general-purpose programming language. The charter of its standards committee calls it "a general-purpose high-level programming language suitable for low-level programming", in other words a system programming language. The charter lists its uses, among them compiler targets, interpreters, operating systems and embedded programming.[^1]

The standard is ISO/IEC 9899. The working group ISO/IEC JTC1/SC22/WG14 maintains it, and ISO and IEC adopted the edition C23 in 2024.[^2] The standard defines two forms of conforming implementation. A hosted implementation supplies the full library. A freestanding implementation, for example for a target with no operating system, must supply only a small set of headers, such as `<stddef.h>` and `<stdint.h>`.[^3]

The C99 Rationale describes the "spirit of C" in a list of short phrases. The first is "Trust the programmer." The last is "Make it fast, even if it is not guaranteed to be portable."[^4] C gives direct access to memory through pointers, and it does few checks at run time. Many errors are therefore undefined behavior: the standard "imposes no requirements" on what the program then does.[^5]

## A first program

```c run id=hello
#include <stdio.h>

int main(void) {
    puts("Hello, world!");
    return 0;
}
```

```output for=hello
Hello, world!
```

A hosted program starts at a function named `main` that returns `int`. The form `main(void)` takes no parameters.[^6] `puts` writes the string and a newline to the standard output.

The next program copies an array of structs to the heap, moves the copies and prints them.

```c run id=points
#include <stdio.h>
#include <stdlib.h>

struct point {
    int x;
    int y;
};

// Copy n points to the heap and move each copy by (dx, dy).
static struct point *moved(const struct point *src, size_t n, int dx, int dy) {
    struct point *dst = malloc(n * sizeof *dst);
    if (dst == nullptr) {
        return nullptr;
    }
    for (size_t i = 0; i < n; i++) {
        dst[i] = (struct point){src[i].x + dx, src[i].y + dy};
    }
    return dst;
}

int main(void) {
    struct point path[] = {{0, 0}, {3, 4}, {6, 8}};
    constexpr size_t n = sizeof path / sizeof path[0];

    struct point *copy = moved(path, n, 10, -1);
    if (copy == nullptr) {
        fputs("out of memory\n", stderr);
        return EXIT_FAILURE;
    }
    for (const struct point *p = copy; p != copy + n; p++) {
        printf("(%d, %d)\n", p->x, p->y);
    }
    bool unchanged = path[1].x == 3;
    printf("%zu points, original unchanged: %s\n", n, unchanged ? "yes" : "no");
    free(copy);
    copy = nullptr;
    return EXIT_SUCCESS;
}
```

```output for=points
(10, -1)
(13, 3)
(16, 7)
3 points, original unchanged: yes
```

`path` is an array of three structs. In most expressions an array converts to a pointer to its first element, so `moved` receives a `const struct point *` and needs the length as a separate argument.[^7] `sizeof path / sizeof path[0]` counts the elements, but only where the array type is still visible. `malloc` returns a null pointer when it cannot allocate the space, so the program checks the result before it uses it. `free` releases the space. A second `free` of the same pointer is undefined behavior.[^8] C23 made `bool`, `constexpr` and `nullptr` keywords.[^9] `nullptr` is the null pointer constant, and its type is `nullptr_t`.[^10] An object declared `constexpr` must have an initializer, and the initializer must be a constant expression.[^11]

## The language at a glance

A source file has the extension `.c`, and a header file has the extension `.h`. Linguist also lists `.cats`, `.h.in` and `.idc` for C.[^12]

**Types.** Each object and each expression has a type that the compiler knows.[^13] The typing is weak: the compiler converts between the arithmetic types with no cast, and the usual arithmetic conversions choose the common type of two operands.[^14] From C23, a definition with `auto` and an initializer takes its type from the initializer.[^15] See [[type-conversion]].

**Integers.** The widths of `int` and `long` differ between platforms. `<stdint.h>` declares types of exact width, such as `int32_t` and `uint8_t`.[^16] Unsigned arithmetic "can never produce an overflow", because it is done modulo 2^N.[^13] A conversion of an out-of-range value to an unsigned type also reduces the value modulo the range.[^17] Signed arithmetic is different. A result that is not in the range of its type is undefined behavior.[^18] C23 adds `<stdckdint.h>`. Its macros `ckd_add`, `ckd_sub` and `ckd_mul` compute a result and tell whether it fits in the target type.[^19] See [[integer-overflow]].

```c run id=ints
#include <inttypes.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint8_t u = 250;
    u += 10; // 260 is reduced modulo 256 when it is stored back in u
    printf("u = %" PRIu8 "\n", u);

    int32_t big = INT32_MAX;
    int32_t sum;
    if (ckd_add(&sum, big, 1)) {
        printf("%" PRId32 " + 1 does not fit in int32_t\n", big);
    }
    int64_t wide = (int64_t)big + 1; // widen first, then add
    printf("wide = %" PRId64 "\n", wide);
    return 0;
}
```

```output for=ints
u = 4
2147483647 + 1 does not fit in int32_t
wide = 2147483648
```

`u + 10` is 260, and the store back into `uint8_t` gives 4. `ckd_add` returns true because the sum does not fit in `int32_t`. The cast to `int64_t` makes the addition happen in the wider type.

**Undefined behavior.** The definition of undefined behavior says that it is behavior "for which this document imposes no requirements". The result can range from "ignoring the situation completely with unpredictable results" to a stop with a diagnostic message. Annex J.2 lists the cases.[^5] They include signed overflow, a use of an object after its lifetime and a data race. Tools find some of them at run time. AddressSanitizer detects out-of-bounds accesses and use after free.[^20] UndefinedBehaviorSanitizer catches other kinds, such as an array subscript out of bounds.[^21] See [[undefined-behavior]].

**Memory.** C has no garbage collector. An object has one of four storage durations: static, thread, automatic or allocated. An automatic object lives until its block ends. An allocated object comes from the memory management functions of `<stdlib.h>`.[^22] It lives until the program calls `free`.[^8] A reference to an object outside its lifetime is undefined behavior. So is the use of a pointer value after the object that it points to reaches the end of its lifetime.[^22] These rules are the source of [[use-after-free]], [[out-of-bounds-write]] and [[null-pointer-dereference]] bugs. See [[pointers-and-references]], [[stack-and-heap]], [[explicit-memory-management]] and [[memory-leaks]].

A string is not a type. It is "a contiguous sequence of characters terminated by and including the first null character". Its length is the number of bytes before the null character.[^23]

**Execution.** The standard describes a translation environment, which translates the source files, and an execution environment, which runs the program. The parts of a program can be translated separately.[^24] GCC, Clang and MSVC compile ahead of time to native code.

**Paradigms.** C is an imperative, procedural language. It has no classes, no exceptions and no generics. `_Generic` selects an expression by the type of its operand.[^25] Errors are reported through return values and the macros of `<errno.h>`.[^26] C11 added threads and atomics in `<threads.h>` and `<stdatomic.h>`.[^25] A hosted implementation can leave threads out and define `__STDC_NO_THREADS__`. A data race "results in undefined behavior".[^27] See [[threads]] and [[data-races]].

## The guide

The guide to C has the 25 parts of the language template. Each row says what the part covers for C. Only this page exists so far, so the table has no links yet.

| Part | What it covers in C |
|---|---|
| Overview and history | this page: the standard, the editions, the implementations, the history |
| Getting started | installing GCC or Clang, compiling with `cc`, the `-std=` option, warnings |
| Lexical structure and syntax | translation phases, the preprocessor, keywords, literals, operator precedence |
| Values and types | the integer and floating types, `<stdint.h>`, `bool`, `_BitInt`, qualifiers, conversions |
| Variables, binding and scope | scopes, linkage, storage durations, `static` and `extern`, `constexpr`, `auto` |
| Control flow | `if`, `switch`, `for`, `while`, `do`, `goto`, `break` and `continue` |
| Functions | prototypes, parameters passed by value, function pointers, variadic functions, `inline` |
| Built-in collections | arrays, variable length arrays, the conversion of arrays to pointers, flexible array members |
| Strings and text | null-terminated strings, `<string.h>`, UTF-8 literals, wide characters |
| User-defined types | `struct`, `union`, `enum` with a fixed underlying type, `typedef`, `typeof` |
| Errors and exceptions | return codes, `errno`, `setjmp` and `longjmp`, `assert` |
| Memory and resources | storage durations, `malloc` and `free`, lifetimes, alignment, cleanup on every path |
| Modules, packages and builds | translation units, headers and `#include`, linkage, CMake, libraries |
| Concurrency and parallelism | `<threads.h>`, `<stdatomic.h>`, the memory model, data races |
| Metaprogramming | the preprocessor, macros, `_Generic`, `#embed`, attributes |
| The standard library | a map of the headers; the api cards hang here |
| I/O, files and the OS | `<stdio.h>`, streams and files, `<time.h>`, `<signal.h>`, POSIX |
| Testing | `assert`, test frameworks, sanitizers, fuzzing |
| Tooling | compiler warnings, clang-format, clang-tidy, debuggers, AddressSanitizer and UBSan |
| Ecosystem | WG14 and its papers, package managers, libraries by need |
| Performance | optimization levels, the cost model, `restrict`, inlining, profiling |
| Interoperability | the C ABI of a platform, calling C from other languages, foreign function interfaces |
| Idioms and style | `const` correctness, ownership rules in comments, error paths with `goto`, naming |
| Pitfalls | undefined behavior, signed overflow, buffer overflows, use after free, `sizeof` on a parameter |
| Versions and migration | the editions from C89 to C23, `__STDC_VERSION__`, the `-std=` options, C2y |

## Versions and support

WG14 writes each revision, and ISO publishes it. The table lists the editions and their values of `__STDC_VERSION__`.[^28][^25] ISO withdraws an edition when the next one replaces it. For example, ISO withdrew ISO/IEC 9899:2018 on 2024-10-31, the day it published ISO/IEC 9899:2024.[^29][^30] A standard has no end of life, so this guide gives the ISO status instead. Compilers keep modes for the older editions, for example `-std=c99` and `-std=c17`.[^31][^32]

| Edition | Document | `__STDC_VERSION__` | Published | Status | Public draft |
|---|---|---|---|---|---|
| C2y | not published | none yet | none yet | in development | N3998 (2026-10-25) |
| C23 | ISO/IEC 9899:2024 | `202311L` | 2024-10-31 | current | N3220 (2024-02-22) |
| C17 | ISO/IEC 9899:2018 | `201710L` | 2018-07-05 | withdrawn | N2310 (2018-11-11) |
| C11 | ISO/IEC 9899:2011 | `201112L` | 2011-12-08 | withdrawn | N1570 (2011-04-04) |
| C99 | ISO/IEC 9899:1999 | `199901L` | 1999-12-16 | withdrawn | N1256 (2007-09-07) |
| C95 | ISO/IEC 9899:1990/Amd 1:1995 | `199409L` | 1995-03-30 | withdrawn | none |
| C90 | ISO/IEC 9899:1990 | none | 1990-12-20 | withdrawn | none |
| C89 | ANSI X3.159-1989 | none | 1989-12-14 | superseded | the NIST copy |

The dates of the ISO editions come from the life cycle on each ISO page.[^29][^30][^33][^34][^35][^36] ANSI approved X3.159-1989 on 1989-12-14.[^37] WG14 lists a public draft that is similar to each edition, and calls N3220 an early C2y draft.[^28] This guide cites N3220 for section numbers. Its header names ISO/IEC 9899:2024. Its cover says that work on C2y began after the C23 cycle, and it lists only one editorial change, to a footnote in Annex K.[^38] N3096 (2023-04-02) is an earlier C23 working draft, from before the June 2023 meeting and the drafts N3149 and N3219. It is not the final text.[^39]

C2y is the next edition. WG14 gives no publication date, and its newest working draft is N3998 of 2026-10-25.[^28] GCC has "experimental and incomplete support" for C2y with `-std=c2y`.[^40] Clang has partial support with `-std=c2y` from Clang 19.[^32] A page in this guide that uses a C2y feature says so.

## Implementations

The standard has no reference implementation. The compat tables have a column for the standard and columns for GCC, Clang and MSVC. The table gives the C23 support that each compiler's own status page states, and the newest release on 2026-11-10.

| Implementation | C23 support | Newest release | Licence |
|---|---|---|---|
| GCC | each C23 feature has a GCC version; `-std=gnu23` is the default from GCC 15 | 16.2 (2026-08-07) | GPL-3.0 |
| Clang | Partial; `-std=c23` from Clang 18, `constexpr` objects and `#embed` from Clang 19 | 23.1.5 (2026-11-03) | Apache-2.0 WITH LLVM-exception |
| MSVC | Partial; no `auto`, `constexpr` objects, `nullptr`, `#embed` or `_BitInt` | Visual Studio 2026 18.11.3 (2026-10-29) | proprietary |

Sources: GCC[^40][^31][^41][^42], Clang[^32][^43][^44] and MSVC[^45][^46][^47]. The MSVC conformance page also marks variable length arrays as not supported, and lists them as an optional C11 feature.[^45] The examples on this page compile with no warnings under `cc -std=c23 -Wall`, with Apple clang 17 and with GCC 16.2, and they give the same output.

## Ecosystem

The standard defines no build tool and no package manager. Projects choose their own tools.

| Need | Tool |
|---|---|
| compile | GCC, Clang, MSVC |
| generate build files | CMake[^48] |
| manage dependencies | vcpkg[^49], Conan[^50] |
| check conditions in tests | the `assert` macro of `<assert.h>`[^51] |
| format | clang-format[^52] |
| lint | clang-tidy[^53] |
| find memory errors | AddressSanitizer[^20] |
| find undefined behavior | UndefinedBehaviorSanitizer[^21] |

vcpkg and Conan manage C and C++ libraries.[^49][^50] The guide's Ecosystem part lists the main libraries by need, as library cards.

## History and influence

Dennis Ritchie created C at Bell Labs. In his paper for the HOPL-II conference, he says that C came into being in the years 1969 to 1973, and that the most creative period was 1972.[^54] Ken Thompson created the language B in 1969 to 1970, and B came directly from BCPL, which Martin Richards designed in the mid-1960s. Ritchie says that B "can be thought of as C without types". He turned B into C during 1971 to 1973. He kept most of the syntax of B, added types and wrote the first compiler.[^54] The type structure of C owes much to Algol 68, and so does the name "cast" for a type conversion. Early C followed the example of PL/I and did not tie structure pointers firmly to their structures.[^54]

By early 1973 the essentials of the language were complete, and in the summer of that year the Unix kernel for the PDP-11 was rewritten in C. In 1978 Brian Kernighan and Ritchie wrote The C Programming Language, the book that was the definition of the language for several years. In 1983, at the urging of M. D. McIlroy, ANSI set up the committee X3J11 to write a standard. X3J11 finished at the end of 1989, and ISO then accepted the standard as ISO/IEC 9899.[^55] Ritchie names the two ideas that are most characteristic of C: "the relationship between arrays and pointers, and the way in which declaration syntax mimics expression syntax."[^55]

| Date | Event |
|---|---|
| 1969 to 1973 | Ritchie develops C from B at Bell Labs[^54] |
| 1973 | the Unix kernel for the PDP-11 is rewritten in C[^55] |
| 1978 | The C Programming Language, by Kernighan and Ritchie[^55] |
| 1989-12-14 | ANSI X3.159-1989 (C89)[^37] |
| 1990-12-20 | ISO/IEC 9899:1990 (C90)[^36] |
| 1995-03-30 | Amendment 1 (C95): digraphs, `<iso646.h>`, the wide character library[^35][^25] |
| 1999-12-16 | C99: `long long`, `<stdint.h>`, `<stdbool.h>`, `//` comments, variable length arrays, designated initializers, `restrict`[^34][^25] |
| 2011-12-08 | C11: threads, atomics, `_Generic`, static assertions, the removal of `gets`[^33][^25] |
| 2018-07-05 | C17: technical corrections and clarifications only[^30][^25] |
| 2024-10-31 | C23: `bool`, `nullptr`, `constexpr` objects, `auto`, `typeof`, `#embed`, `_BitInt(N)`, and the removal of old-style function definitions[^29][^25] |

The WG14 charter states its principles. Unless a new feature addresses a significant deficiency, "no new inventions should be entertained". Other principles are to avoid quiet changes to the meaning of working code, and to enable secure programming.[^56] The charter calls C "a lingua franca to translate between various systems and languages".[^1]

The influence goes both ways. Ritchie says that X3J11 took the syntax of function prototypes from C++.[^55] The Go FAQ says that Go is "mostly in the C family" for its basic syntax.[^57]

## Coming from another language

- From [[cpp]]: C has no classes, templates, references, exceptions or destructors, so cleanup is explicit on every path. C23 shares `nullptr`, `auto`, `constexpr` and `bool` with C++, but `constexpr` in C is a storage-class specifier for objects, and there are no `constexpr` functions.[^11] From C23, an empty parameter list means no parameters, as in C++.[^25]
- From [[rust]]: there is no borrow checker. The compiler accepts a use of a pointer after `free` and an index past the end of an array. The program then has undefined behavior, not a compile error.[^22][^5] See [[use-after-free]] and [[out-of-bounds-write]].
- From [[java]]: there is no garbage collector and there are no exceptions. An index out of bounds does not throw; it is undefined behavior.[^5] See [[garbage-collection]] and [[exception-handling]].
- From [[go]]: an array does not carry its length. When an array is passed to a function, the function receives a pointer to the first element, so the length must travel as a separate argument.[^7] A string is an array of characters that ends with a null character.[^23]
- From [[python]]: integers have a fixed width. Unsigned arithmetic wraps, and signed overflow is undefined behavior. Use `<stdckdint.h>` to check.[^13][^18][^19] See [[integer-overflow]].

## References

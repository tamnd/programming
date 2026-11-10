---
tag: NEW-use-after-free
kind: weakness
slug: use-after-free
name: Use after free
aliases: [UAF, dangling pointer, cwe-416]
area: security/memory-safety-weaknesses
level: core
summary: >-
  A use after free happens when a program uses a pointer to memory that it has already freed, so it reads or writes memory that may now belong to a different object.
cwe: 416
abstraction: variant
languages_affected: [c, cpp, unsafe-code-in-any-language]
cwe_top25_2025: 7
history:
  year: 2007
  people: [Jonathan Afek, Adi Sharabani]
  note: The Watchfire whitepaper "Dangling Pointer, Smashing the Pointer for Fun and Profit" said that many developers saw the dangling pointer as a quality problem, and showed an exploit of one in Microsoft IIS 5.1.
sources:
  - url: https://cwe.mitre.org/data/definitions/416.html
    title: "CWE-416: Use After Free"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Relationships; Weakness Ordinalities; Observed Examples
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 7, CVEs in KEV
  - url: https://owasp.org/Top10/2025/X01_2025-Next_Steps/
    title: "OWASP Top 10:2025, Next Steps, X02:2025 Memory Management Failures"
    locator: X02:2025, Description; How to prevent; Example attack scenarios; List of Mapped CWEs
  - url: https://www.blackhat.com/presentations/bh-usa-07/Afek/Whitepaper/bh-usa-07-afek-WP.pdf
    title: "Jonathan Afek and Adi Sharabani, Dangling Pointer: Smashing the Pointer for Fun and Profit, Watchfire, 2007"
    locator: 1. Abstract; 2. Understanding the Dangling Pointer Bug
  - url: https://www.cve.org/CVERecord?id=CVE-2010-0249
    title: CVE-2010-0249, Microsoft Internet Explorer
    locator: the description and the publication date
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: Clang documentation, AddressSanitizer
    locator: Introduction; Usage
  - url: https://www.chromium.org/Home/chromium-security/memory-safety/
    title: The Chromium Projects, Memory safety
    locator: the opening statistics
  - url: https://eel.is/c++draft/vector.modifiers
    title: "Working Draft, Programming Languages: C++, [vector.modifiers]"
    locator: Remarks on insert, emplace_back and push_back
  - url: https://doc.rust-lang.org/book/ch04-02-references-and-borrowing.html
    title: The Rust Programming Language, References and Borrowing
    locator: Dangling References
  - url: https://go.dev/doc/faq#stack_or_heap
    title: Go FAQ
    locator: How do I know whether a variable is allocated on the heap or the stack?
  - url: https://docs.python.org/3.14/library/io.html#io.IOBase.close
    title: The Python Standard Library, io, Python 3.14
    locator: IOBase.close
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/Reader.html#close()
    title: Java SE 25 API, java.io.Reader
    locator: close
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.objectdisposedexception
    title: .NET API, ObjectDisposedException Class
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/using
    title: C# language reference, using statement
    locator: the using statement; the using declaration
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-use-after-free/
    title: CodeQL query help, Potential use after free (cpp/use-after-free)
    locator: the query description; Recommendation
  - url: https://www.cve.org/CVERecord?id=CVE-2014-1776
    title: CVE-2014-1776, Microsoft Internet Explorer
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-6819
    title: CVE-2020-6819, Mozilla Firefox
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-0920
    title: CVE-2021-0920, Linux kernel in Android
    locator: the description and the publication date
---

## Description

In C and C++, a program asks the allocator for a block of memory, uses it through pointers, and gives it back with `free` or `delete`. The free returns the block to the allocator. It does not change the pointers that still hold its address. A use after free happens when the program later reads or writes through one of those pointers.[^1] The pointer is now dangling. The allocator can give the same block to a new object, and the old pointer then points into that new object.[^1] The program meant to read a session. It reads whatever lives there now.

The root cause is that the lifetime of an object is not part of its pointer. Nothing in the type of a pointer says if the memory is still valid. The rule "who frees this, and when" lives in comments and in the heads of developers. It breaks when two parts of a program keep pointers to the same object, when an error path frees early, when a callback runs after the owner is gone, and when a container moves its elements. CWE-416 says that a use after free is resultant: some other error came first, such as a race condition, a badly handled error, confusion over which part of the program frees the memory, or a free that is too early.[^1]

The effects depend on what the block holds when the old pointer is used. A read can leak data of another object, and a write can corrupt it. If the new object holds function pointers, as C++ objects with virtual functions do, a write through the old pointer can lead to code execution.[^1] Afek and Sharabani showed in 2007 that an attacker who controls the new contents of the block can change the behavior of the program.[^4] OWASP describes the same pattern in a browser: script makes the browser free an object while a dangling pointer to it remains, and then makes a new object in the same memory.[^3]

CWE-416 names this weakness. Its alternate terms are dangling pointer, UAF and use-after-free. It is a variant weakness under CWE-825, expired pointer dereference, and a peer of the double free, CWE-415.[^1] CWE-416 lists C and C++ as languages where it is often prevalent. It is rank 7 of the 2025 CWE Top 25, with 14 CVEs in the CISA catalogue of known exploited vulnerabilities. Only OS command injection has more on that list.[^2] The OWASP Top 10:2025 maps it to X02:2025 Memory Management Failures, a category in its Next Steps section and not in the ten.[^3] The Chromium project finds that around 70% of its high severity security bugs are memory unsafety problems, and that half of those are use-after-free bugs.[^7]

### History

In 2007, Jonathan Afek and Adi Sharabani of Watchfire published the whitepaper "Dangling Pointer: Smashing the Pointer for Fun and Profit".[^4] It said that many developers called the dangling pointer a quality problem, and that security experts did not consider it a severe issue. It said that no exploit of this type of bug was publicly known, and it presented one against Microsoft IIS 5.1.[^4] In January 2010, CVE-2010-0249 recorded a use after free in Internet Explorer that had been exploited in the wild in December 2009 and January 2010, during Operation Aurora.[^5]

## Vulnerable code

`close_session` frees the session, and `main` then reads it through the old pointer. The block builds with AddressSanitizer, which reports the read.[^6]

```c fails expect=sanitizer:address timeout=30 id=free-then-use
#include <stdio.h>
#include <stdlib.h>

struct session {
    char user[16];
    int logged_in;
};

struct session *open_session(const char *user) {
    struct session *s = calloc(1, sizeof *s);
    if (s != NULL) {
        snprintf(s->user, sizeof s->user, "%s", user);
        s->logged_in = 1;
    }
    return s;
}

void close_session(struct session *s) {
    free(s);
}

int main(void) {
    struct session *s = open_session("alice");
    if (s == NULL) {
        return 1;
    }
    close_session(s);
    printf("%s logged in: %d\n", s->user, s->logged_in); // s points to freed memory
}
```

AddressSanitizer stops the program with a `heap-use-after-free` report. Without the sanitizer, the behavior is undefined. The program may print the old values, other data or garbage, and still exit with status 0. Nothing in the output says that the read was wrong.

A use after free does not need a visible `free`. In C++, `push_back` on a full vector allocates a new block, moves the elements and frees the old block. The standard says that the reallocation invalidates all references, pointers and iterators to the elements.[^8] A reference taken before the call then dangles. The C++ example file shows AddressSanitizer catching it.

## Fixed code

The program uses the session before it closes it, and `close_session` clears the pointer of the caller. The block builds with AddressSanitizer and UndefinedBehaviorSanitizer, and both stay silent.

```c run flags="-fsanitize=address,undefined" id=null-after-free
#include <stdio.h>
#include <stdlib.h>

struct session {
    char user[16];
    int logged_in;
};

struct session *open_session(const char *user) {
    struct session *s = calloc(1, sizeof *s);
    if (s != NULL) {
        snprintf(s->user, sizeof s->user, "%s", user);
        s->logged_in = 1;
    }
    return s;
}

// Frees the session and clears the pointer of the caller.
void close_session(struct session **s) {
    free(*s);
    *s = NULL;
}

int main(void) {
    struct session *s = open_session("alice");
    if (s == NULL) {
        return 1;
    }
    printf("%s logged in: %d\n", s->user, s->logged_in);
    close_session(&s);
    if (s == NULL) {
        printf("the session is closed\n");
    }
}
```

```output for=null-after-free
alice logged in: 1
the session is closed
```

The order of the calls is the fix. Setting the pointer to `NULL` is defence in depth. CWE-416 says that a later use then becomes a null pointer dereference, which can still crash but reduces or removes the risk of code execution. It also warns that complex data structures make this measure less useful, because other copies of the pointer keep the old address.[^1]

### Ownership checked by the compiler

Rust makes each value have one owner and checks every reference against the lifetime of its value. The program below frees the string with `drop` while a reference to it is still in use.

```rust fails expect=error:E0505 id=drop-while-borrowed
fn main() {
    let session = String::from("alice");
    let user = &session;
    drop(session);
    println!("{user}");
}
```

```output for=drop-while-borrowed stream=stderr
error[E0505]: cannot move out of `session` because it is borrowed
 --> main.rs:4:10
  |
2 |     let session = String::from("alice");
  |         ------- binding `session` declared here
3 |     let user = &session;
  |                -------- borrow of `session` occurs here
4 |     drop(session);
  |          ^^^^^^^ move out of `session` occurs here
5 |     println!("{user}");
  |                ---- borrow later used here
  |
help: consider cloning the value if the performance cost is acceptable
  |
3 |     let user = &session.clone();
  |                        ++++++++

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0505`.
```

The compiler rejects the program. The Rust book says that the compiler guarantees that references will never be dangling: the data will not go out of scope before the reference to it does.[^9] The Rust example file also shows the vector case from C++, which fails with error E0502.

### Garbage collection

Go, Java, C#, Python and JavaScript free an object only when no reference to it is left, so code in these languages cannot reach freed memory. OWASP lists them as memory-safe.[^3] The Go FAQ says that each variable exists as long as there are references to it, and that the compiler moves a local variable to the heap when it cannot prove that the variable is not used after the function returns.[^10]

The same mistake still happens one level up, with resources that the program closes by hand. Here it is safe and visible. Python raises `ValueError` for an operation on a closed file.[^11] Java throws `IOException` for a read from a closed reader.[^12] .NET throws `ObjectDisposedException` for a member of a disposed object.[^13] The C# documentation says to declare a disposable variable in the `using` statement, because an instance that stays in scope after the dispose can still be used.[^14] The example files show each case.

## How to detect it

- **Sanitizers.** Build the tests with `-fsanitize=address`. AddressSanitizer detects use after free, use after return, use after scope and double free.[^6] It finds the bug only when a test runs the path, so combine it with fuzzing.[^3]
- **Static analysis.** CodeQL query `cpp/use-after-free` finds accesses through a pointer after the memory is freed. Its documentation says that the check is an approximation.[^15] OWASP lists code reviews and static analysis.[^3]
- **Code review.** For each `free`, `delete` and `close`, find every other pointer to the object and check that no code uses it after that point. Look at error paths, callbacks, destructors and code that runs on two threads. Two of the real cases below start with a race condition.[^17][^18] For each reference into a container, check that no call between its creation and its use can change the container.

## How to prevent it

- **Use a memory-safe language.** CWE-416 lists a language with automatic memory management as its first mitigation. OWASP puts a memory-safe language first among its defences against memory management failures.[^1][^3]
- **Give each object one owner.** Decide which part of the program frees each object, and let no other part free it. CWE-416 names confusion over this as one cause of the weakness.[^1] In Rust, the compiler checks ownership.[^9]
- **Do not keep references across a change.** Keep an index or a copy across a call that can reallocate or remove elements of a container.[^8]
- **Clear the pointer after the free.** It turns a later use into a null pointer dereference. It is defence in depth, not a fix.[^1]
- **Limit the scope of resources.** Use `with` in Python, `try`-with-resources in Java and `using` in C#, so that the name of a closed resource is not in scope after the close.[^14]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2010-0249 | published 2010-01-15 | A use after free in Internet Explorer 6, 7 and 8 let remote attackers run code through a pointer to a deleted object. It was exploited in the wild during Operation Aurora.[^5] |
| CVE-2014-1776 | published 2014-04-27 | A use after free in Internet Explorer 6 through 11, related to `CMarkup::IsConnectedToPrimaryMarkup`, let remote attackers run code. It was exploited in the wild in April 2014.[^16] |
| CVE-2020-6819 | published 2020-04-24 | A race condition in the `nsDocShell` destructor of Firefox could cause a use after free. Mozilla was aware of targeted attacks in the wild.[^17] |
| CVE-2021-0920 | published 2021-12-15 | A race condition in `unix_scm_to_skb` of `af_unix.c` in the kernel used by Android caused a use after free and could lead to local escalation of privilege.[^18][^1] |

## References

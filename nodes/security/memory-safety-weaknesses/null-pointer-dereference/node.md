---
tag: NEW-null-pointer-dereference
kind: weakness
slug: null-pointer-dereference
name: NULL pointer dereference
aliases: [null dereference, NPD, null deref, NullPointerException, NPE, nil pointer dereference, cwe-476]
area: security/memory-safety-weaknesses
level: core
summary: >-
  A NULL pointer dereference happens when a program uses a pointer or a reference that is NULL as if it were valid, so the program crashes or, in rare cases, uses memory at address zero.
cwe: 476
abstraction: base
languages_affected: [c, cpp, java, csharp, go, python, javascript]
owasp_top10_2025: A10
cwe_top25_2025: 13
history:
  year: 2009
  people: [Tavis Ormandy, Julien Tinnes]
  note: Ormandy and Tinnes found CVE-2009-2692, a NULL pointer dereference in the socket code of the Linux kernel that affected 2.4 and 2.6 kernels since 2001 and let a local user gain privileges.
sources:
  - url: https://cwe.mitre.org/data/definitions/476.html
    title: "CWE-476: NULL Pointer Dereference"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Weakness Ordinalities; Detection Methods; Demonstrative Examples, Example 1; Observed Examples
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 13, CVEs in KEV, Rank Last Year
  - url: https://owasp.org/Top10/2025/A10_2025-Mishandling_of_Exceptional_Conditions/
    title: "OWASP Top 10:2025, A10:2025 Mishandling of Exceptional Conditions"
    locator: Background; List of Mapped CWEs
  - url: https://blog.cr0.org/2009/08/linux-null-pointer-dereference-due-to.html
    title: "Julien Tinnes, Linux NULL pointer dereference due to incorrect proto_ops initializations (CVE-2009-2692), August 13, 2009"
    locator: the opening paragraphs
  - url: https://www.cve.org/CVERecord?id=CVE-2009-2692
    title: CVE-2009-2692, Linux kernel
    locator: the description and the publication date
  - url: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
    title: Clang documentation, UndefinedBehaviorSanitizer
    locator: Available checks, -fsanitize=null
  - url: https://doc.rust-lang.org/book/ch06-01-defining-an-enum.html
    title: The Rust Programming Language, Defining an Enum
    locator: The Option Enum and Its Advantages Over Null Values
  - url: https://go.dev/ref/spec#Address_operators
    title: The Go Programming Language Specification
    locator: Address operators
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/NullPointerException.html
    title: Java SE 25 API, java.lang.NullPointerException
    locator: the class description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Optional.html
    title: Java SE 25 API, java.util.Optional
    locator: the class description, API Note
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.nullreferenceexception
    title: .NET API reference, NullReferenceException
    locator: the class description; Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/null-safety/nullable-reference-types
    title: C# fundamentals, Nullable reference types
    locator: the opening section
  - url: https://kotlinlang.org/docs/null-safety.html
    title: Kotlin documentation, Null safety
    locator: the opening section, the only possible causes of an NPE
  - url: https://www.typescriptlang.org/tsconfig/strictNullChecks.html
    title: TypeScript TSConfig reference, strictNullChecks
    locator: the option description
  - url: https://docs.python.org/3/library/exceptions.html#AttributeError
    title: The Python Standard Library, Built-in Exceptions
    locator: AttributeError
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Optional_chaining
    title: MDN Web Docs, Optional chaining (?.)
    locator: Description
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-inconsistent-null-check/
    title: CodeQL query help, Inconsistent null check (cpp/inconsistent-null-check)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/java/java-dereferenced-value-may-be-null/
    title: CodeQL query help, Dereferenced variable may be null (java/dereferenced-value-may-be-null)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/csharp/cs-dereferenced-value-may-be-null/
    title: CodeQL query help, Dereferenced variable may be null (cs/dereferenced-value-may-be-null)
    locator: the query description; Recommendation
  - url: https://www.cve.org/CVERecord?id=CVE-2004-0079
    title: CVE-2004-0079, OpenSSL
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-29652
    title: CVE-2020-29652, golang.org/x/crypto/ssh
    locator: the description and the publication date
---

## Description

A pointer is an address. A program uses the special value NULL to say that a pointer points to nothing: a lookup found no record, an allocation failed, or a field is not set yet. NULL is not an object. A NULL pointer dereference happens when the program reads or writes through the pointer as if it pointed to a valid object.[^1] Languages with references have the same weakness under other names. Java calls it a `NullPointerException`, C# a `NullReferenceException`, and Go a nil pointer dereference.[^1]

The root cause is that one type carries two meanings. In C, the type `struct user *` means "a user" and also "maybe a user". The compiler accepts `u->quota` for both meanings, so it cannot tell the developer that a check is missing. The Rust book says that this property is pervasive, so the use of a null value as a value that is not null is an extremely common error.[^7] The design history of the null reference is on the page [[null-references]].

The missing check is usually on a rare path. CWE-476 says that the weakness is frequently resultant from rarely encountered error conditions and race conditions.[^1] A test calls the lookup with a name that exists, and the error path never runs. Its Example 1 calls `gethostbyaddr` for an address that has no host name, so the call returns NULL, and the code copies `hp->h_name` without a check. CWE files the missing check as CWE-252, an unchecked return value.[^1] Other causes are a variable that is not initialized, a structure that is used before it is set up, and a pointer that is cleared after a free. The page [[use-after-free]] shows that clearing a pointer turns a later use into a NULL dereference, which is a smaller risk.

The usual result is a crash, so the main impact is on availability. A crash of a server is a denial of service. CWE-476 says that even with exception handling, it can be hard to return the program to a safe state. In rare cases, when NULL is the address 0x0 and privileged code can access that address, a dereference can read or write memory and can lead to code execution.[^1]

CWE-476 is a base weakness. It is rank 13 in the 2025 CWE Top 25, with no CVEs in the CISA catalogue of known exploited vulnerabilities, up from rank 21 the year before.[^2] The OWASP Top 10:2025 lists it as a notable weakness of A10:2025 Mishandling of Exceptional Conditions, a category that the 2025 edition added.[^3]

### History

In August 2009, Tavis Ormandy and Julien Tinnes published CVE-2009-2692.[^4] Tinnes wrote that it affected all 2.4 and 2.6 Linux kernels since 2001 on all architectures. Functions such as `sock_sendpage` did not check for NULL pointers before they called operations in the `proto_ops` structure. The kernel relied on correct initialization of those structures with stubs, and some structures were not initialized.[^4] The CVE record says that a local user could gain privileges, because a process could map page zero, and the kernel then ran code from that page.[^5] The case showed that a NULL dereference in privileged code is not only a crash.

## Vulnerable code

`find_user` returns NULL when no user has the name. The caller does not check, and it reads `u->quota`. The second name is the classic demonstration input, a name that does not exist. The block builds with UndefinedBehaviorSanitizer, which has a check for the use of a null pointer.[^6]

```c fails expect=sanitizer:undefined timeout=30 id=no-check
#include <stdio.h>
#include <string.h>

struct user {
    const char *name;
    int quota;
};

static struct user users[] = {{"alice", 10}, {"bob", 20}};

// Returns the user with this name, or NULL if there is none.
struct user *find_user(const char *name) {
    for (size_t i = 0; i < sizeof users / sizeof users[0]; i++) {
        if (strcmp(users[i].name, name) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

int main(void) {
    const char *names[] = {"alice", "nobody"};
    for (int i = 0; i < 2; i++) {
        struct user *u = find_user(names[i]);
        printf("%s: quota %d\n", names[i], u->quota); // u can be NULL
    }
}
```

UndefinedBehaviorSanitizer reports a `member access within null pointer of type 'struct user'`. Without the sanitizer, the process usually stops with a segmentation fault. The function and the call look correct. Only the comment on `find_user` says that NULL is a possible result.

## Fixed code

The fix checks the result before each use, and it handles the missing user as a normal case. The block builds with AddressSanitizer and UndefinedBehaviorSanitizer, and both stay silent.

```c run flags="-fsanitize=address,undefined" id=check-result
#include <stdio.h>
#include <string.h>

struct user {
    const char *name;
    int quota;
};

static struct user users[] = {{"alice", 10}, {"bob", 20}};

// Returns the user with this name, or NULL if there is none.
struct user *find_user(const char *name) {
    for (size_t i = 0; i < sizeof users / sizeof users[0]; i++) {
        if (strcmp(users[i].name, name) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

int main(void) {
    const char *names[] = {"alice", "nobody"};
    for (int i = 0; i < 2; i++) {
        struct user *u = find_user(names[i]);
        if (u == NULL) {
            printf("%s: no such user\n", names[i]);
            continue;
        }
        printf("%s: quota %d\n", names[i], u->quota);
    }
}
```

```output for=check-result
alice: quota 10
nobody: no such user
```

CWE-476 rates a check of every return value as moderately effective. A check is not enough when another thread can change the pointer between the check and the use, so the check and the use must be under the same lock.[^1]

### Types that say "maybe"

A check that the developer must remember is the weakest fix. A stronger fix puts "maybe absent" in the type, so the compiler refuses the use without a check. Rust has no null. Its `Option<T>` has the variants `Some(T)` and `None`, and an `Option<T>` is not a `T`.[^7] The program below does not compile, because it uses the result of a lookup as the value itself.

```rust fails expect=compile id=option-as-value
fn find_quota(name: &str) -> Option<i32> {
    match name {
        "alice" => Some(10),
        "bob" => Some(20),
        _ => None,
    }
}

fn main() {
    for name in ["alice", "nobody"] {
        let quota: i32 = find_quota(name);
        println!("{name}: quota {quota}");
    }
}
```

The compiler reports `mismatched types`: it expected `i32` and found `Option<i32>`. The fix is a `match` or an `if let` that handles `None`, and the Rust example file shows it.

Kotlin types do not include null unless the type has a `?`. The Kotlin documentation says that the only possible causes of a `NullPointerException` are an explicit throw, the `!!` operator, problems during initialization and Java interoperation.[^13] TypeScript with `strictNullChecks` gives `null` and `undefined` their own types and reports a type error where a value is expected.[^14] C# nullable reference types are a compile-time feature. The compiler warns when the code can dereference a variable that may be null, and the runtime behavior does not change.[^12] Java, Python and JavaScript find the error only at run time. Java has `Optional` for a method that can return "no result".[^10]

## How to detect it

- **Sanitizers.** Build the tests with `-fsanitize=null`, which is part of `-fsanitize=undefined`. It reports the use of a null pointer at the line of the use.[^6]
- **Fuzzing and error injection.** CWE-476 rates automated dynamic analysis such as fuzzing as moderately effective. It also suggests manual tests that force rare error conditions, such as low memory.[^1] Make the lookup fail, the allocation fail and the file open fail, and see what the caller does.
- **Static analysis.** CodeQL query `cpp/inconsistent-null-check` finds a call whose result is not checked for NULL when most other calls to the same function check it.[^17] Queries `java/dereferenced-value-may-be-null` and `cs/dereferenced-value-may-be-null` find a variable that may be null on some path to a dereference.[^18][^19] In C#, the nullable warnings of the compiler do the same work.[^12]
- **Code review.** For each function that can return NULL, find every caller and check that it handles NULL. Look for an error path that sets a pointer to NULL and then falls through, for structures with function pointers that are not all set, and for a check that comes after the dereference.

## How to prevent it

- **Use types that cannot be null.** CWE-476 lists language selection first.[^1] Use `Option` in Rust, types without `?` in Kotlin, `strictNullChecks` in TypeScript and nullable reference types with warnings as errors in C#.[^7][^13][^14][^12]
- **Return an explicit absence.** In Java, return `Optional` from a lookup instead of null.[^10] In Go and C, return a second value or an error code with the pointer, and check it.
- **Check every result that can be NULL.** Check the pointer before the use, and use a lock in concurrent code so that the pointer does not change between the check and the use.[^1]
- **Initialize everything.** CWE-476 asks to initialize all variables explicitly, at the declaration or immediately before the first use. Fill every function pointer in a table with a valid stub.[^1][^4]
- **Fail closed.** When a value is absent, refuse the operation with a clear error. OWASP asks to handle each exceptional condition where it occurs, and to roll back every part of a transaction that fails part way.[^3]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2004-0079 | published 2004-03-18 | A crafted SSL/TLS handshake triggered a null dereference in `do_change_cipher_spec` in OpenSSL 0.9.6c to 0.9.6k and 0.9.7a to 0.9.7c, and crashed the server.[^20][^1] |
| CVE-2009-2692 | published 2009-08-14 | The Linux kernel did not initialize all function pointers in `proto_ops` structures. A local user could trigger a NULL pointer dereference and gain privileges.[^5][^4] |
| CVE-2020-29652 | published 2020-12-17 | A nil pointer dereference in `golang.org/x/crypto/ssh` let remote attackers cause a denial of service against SSH servers.[^21][^1] |

## References

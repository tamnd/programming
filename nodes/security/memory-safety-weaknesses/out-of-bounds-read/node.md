---
tag: 4SWV9E
kind: weakness
slug: out-of-bounds-read
name: Out-of-bounds read
aliases: [OOB read, buffer over-read, buffer under-read, cwe-125]
area: security/memory-safety-weaknesses
level: core
summary: >-
  An out-of-bounds read happens when a program reads memory past the end, or before the start, of a buffer, so it returns or uses bytes that belong to other data.
cwe: 125
abstraction: base
languages_affected: [c, cpp, assembly, unsafe-code-in-any-language]
cwe_top25_2025: 8
history:
  year: 2014
  people: [Neel Mehta]
  note: Heartbleed, an out-of-bounds read in the TLS heartbeat code of OpenSSL, was found independently by Neel Mehta of Google Security and a team at Codenomicon, and let anyone on the Internet read memory of affected servers.
same_as:
  wikidata: Q16848739
  wikipedia: Out-of-bounds_read
sources:
  - url: https://cwe.mitre.org/data/definitions/125.html
    title: "CWE-125: Out-of-bounds Read"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Relationships; Demonstrative Examples, Example 1; Detection Methods; Observed Examples
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 8, CVEs in KEV, Rank Last Year
  - url: https://owasp.org/Top10/2025/X01_2025-Next_Steps/
    title: "OWASP Top 10:2025, Next Steps, X02:2025 Memory Management Failures"
    locator: X02:2025, Description; How to prevent; List of Mapped CWEs
  - url: https://www.rfc-editor.org/rfc/rfc6520#section-4
    title: "RFC 6520, Transport Layer Security (TLS) and Datagram Transport Layer Security (DTLS) Heartbeat Extension, February 2012"
    locator: 4. Heartbeat Request and Response Messages
  - url: https://heartbleed.com/
    title: The Heartbleed Bug, Codenomicon
    locator: the opening section; Who found the Heartbleed Bug?
  - url: https://www.cve.org/CVERecord?id=CVE-2014-0160
    title: CVE-2014-0160, OpenSSL (Heartbleed)
    locator: the description and the publication date
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: Clang documentation, AddressSanitizer
    locator: Introduction; Usage
  - url: https://go.dev/ref/spec#Slice_expressions
    title: The Go Programming Language Specification
    locator: Slice expressions, Simple slice expressions
  - url: https://doc.rust-lang.org/std/primitive.slice.html#method.get
    title: Rust standard library, slice
    locator: get
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
    title: Java SE 25 API, java.util.Arrays
    locator: copyOfRange
  - url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
    title: The Python Standard Library, Built-in Types
    locator: Common Sequence Operations, note 4
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray/subarray
    title: MDN Web Docs, TypedArray.prototype.subarray()
    locator: Description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.memoryextensions.asspan
    title: .NET API, MemoryExtensions.AsSpan Method
    locator: the overloads with start and length, Exceptions
  - url: https://nodejs.org/api/buffer.html#static-method-bufferallocunsafesize
    title: Node.js documentation, Buffer
    locator: Static method Buffer.allocUnsafe(size)
  - url: https://codeql.github.com/codeql-query-help/cpp/cpp-overflow-buffer/
    title: CodeQL query help, Call to memory access function may overflow buffer (cpp/overflow-buffer)
    locator: the query description; Recommendation
  - url: https://www.cve.org/CVERecord?id=CVE-2020-11899
    title: CVE-2020-11899, Treck TCP/IP stack
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2023-1018
    title: CVE-2023-1018, TPM 2.0 reference library
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2018-10887
    title: CVE-2018-10887, libgit2
    locator: the description and the publication date
---

## Description

A buffer in C and C++ is a run of bytes at an address. A read through an index or a pointer returns whatever is at the computed address, and nothing checks that the address is inside the buffer. An out-of-bounds read happens when the program reads past the end of the buffer, or before its start.[^1] The read does not change memory and it does not crash in most cases. The program gets bytes that belong to something else, and it uses them, prints them or sends them back.

The root cause is a length or an index that the program does not compare with the real size of the buffer. Three forms are common. The first is a length that comes from input: a message says that its payload is 64 bytes long, and the code copies 64 bytes from a payload of 5. CWE-125 describes Heartbleed in these terms, an inconsistent length parameter that enables a buffer over-read.[^1] The second is a check that tests only one side of the range: `if (index < len)` with a signed index lets `-1` through and reads before the buffer.[^1] The third is a sentinel that is not there: a string with no final `'\0'` makes `strlen` and `printf` read on until they find a zero byte somewhere in memory.[^1]

This weakness is quick to make and hard to see. A write past the end often corrupts something and fails later. A read past the end usually returns plausible bytes and the program goes on. The tests send well-formed input, so the length and the buffer agree. The bug shows only when someone sends a length that does not match, and then the output contains data that the program never meant to send.

CWE-125 lists the consequences. An attacker can get secret values such as cryptographic keys and personal data. The bytes can hold memory addresses that help to defeat address space layout randomization when the attacker exploits a separate bug. A read that runs into unmapped memory crashes the program, and this is likely when the code relies on a sentinel.[^1]

CWE-125 names this weakness. Its alternate term is OOB read. It is a base weakness under CWE-119, and its children are the buffer over-read, CWE-126, and the buffer under-read, CWE-127. It is often resultant: an earlier error, such as an [[integer-overflow]] in a length, produces the bad index.[^1] It is rank 8 of the 2025 CWE Top 25, with 3 CVEs in the CISA catalogue of known exploited vulnerabilities, and it was rank 6 the year before.[^2] The OWASP Top 10:2025 maps it to X02:2025 Memory Management Failures, a category in its Next Steps section and not in the ten.[^3] The write in the other direction is [[out-of-bounds-write]].

### History

RFC 6520, published in February 2012, defined the TLS and DTLS heartbeat extension. A heartbeat request carries a payload and a 16-bit `payload_length`, and the peer sends the payload back. The RFC says that a message with a `payload_length` that is too large must be discarded silently.[^4] The OpenSSL implementation did not do this check. On 2014-04-07, CVE-2014-0160 recorded that crafted heartbeat packets made OpenSSL 1.0.1 before 1.0.1g read past its buffer and return process memory, including private keys.[^6] The bug was named Heartbleed. It was found independently by Neel Mehta of Google Security and by a team of engineers at Codenomicon, while they improved a feature of their security testing tools.[^5]

## Vulnerable code

`echo` copies the number of bytes that the sender claims into a reply, as a heartbeat handler does. The program sends the classic demonstration input: a 5-byte payload with a claimed length of 64. The block builds with AddressSanitizer, which reports the read.[^7]

```c fails expect=sanitizer:address timeout=30 id=claimed-length
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of the first claimed bytes of payload.
unsigned char *echo(const unsigned char *payload, size_t received, size_t claimed) {
    (void)received; // the real size is ignored
    unsigned char *reply = malloc(claimed);
    if (reply != NULL) {
        memcpy(reply, payload, claimed); // claimed comes from the sender
    }
    return reply;
}

int main(void) {
    size_t received = 5;
    unsigned char *payload = malloc(received);
    if (payload == NULL) {
        return 1;
    }
    memcpy(payload, "hello", received);
    unsigned char *reply = echo(payload, received, 64);
    if (reply != NULL) {
        printf("reply of 64 bytes starts with %.5s\n", (const char *)reply);
    }
    free(reply);
    free(payload);
}
```

AddressSanitizer stops the program with a `heap-buffer-overflow` report for a `READ of size 64` in `memcpy`. Without the sanitizer, the program copies 59 bytes of whatever lies after the payload on the heap, and it exits with status 0. In a server, those bytes go back to the sender.

## Fixed code

The fix compares the claimed length with the number of bytes that arrived, and it drops the message when they do not agree, as RFC 6520 requires.[^4] The block builds with AddressSanitizer and UndefinedBehaviorSanitizer, and both stay silent.

```c run flags="-fsanitize=address,undefined" id=check-length
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of the first claimed bytes of payload,
// or NULL when the payload is shorter than claimed.
unsigned char *echo(const unsigned char *payload, size_t received, size_t claimed) {
    if (claimed > received) {
        return NULL; // discard the message
    }
    unsigned char *reply = malloc(claimed > 0 ? claimed : 1);
    if (reply != NULL) {
        memcpy(reply, payload, claimed);
    }
    return reply;
}

int main(void) {
    size_t received = 5;
    unsigned char *payload = malloc(received);
    if (payload == NULL) {
        return 1;
    }
    memcpy(payload, "hello", received);
    size_t claims[] = {5, 64};
    for (size_t i = 0; i < 2; i++) {
        unsigned char *reply = echo(payload, received, claims[i]);
        if (reply == NULL) {
            printf("claimed %zu: discarded\n", claims[i]);
        } else {
            printf("claimed %zu: %.*s\n", claims[i], (int)claims[i], (const char *)reply);
            free(reply);
        }
    }
    free(payload);
}
```

```output for=check-length
claimed 5: hello
claimed 64: discarded
```

The length that controls the copy must come from the buffer, not from the message. CWE-125 says to validate every length argument, buffer size calculation and offset, and to be careful with sentinels in untrusted input.[^1] For a signed index, check both ends: `index >= 0 && index < len`.[^1] The C example file shows the missing `'\0'`.

### Languages that check reads

In Rust, Go, Java, C#, Python and JavaScript, a slice, an array or a string knows its length, and the runtime checks each index. The page [[out-of-bounds-write]] shows the check for single elements. A read of a range is different. Several languages do not report a range that is too long. They return less data or fill the gap, and the program goes on with a reply of the wrong size.

- Python clamps a slice. If an end is greater than `len(s)`, Python uses `len(s)`.[^11] `payload[:64]` returns 5 bytes.
- `subarray` of a JavaScript typed array clamps the range to the valid indices.[^12]
- `Arrays.copyOfRange` in Java accepts an end past the length of the array and fills the rest of the copy with zeros.[^10]
- A Go slice expression can go up to the capacity of the slice, not only to its length.[^8] When a program reuses a buffer, `buf[:n]` with `n` past the length returns old bytes from the earlier use. This is Heartbleed inside a memory-safe language.
- Rust `get` with a range returns `None` when the range is out of bounds.[^9] A C# span over a range that does not fit throws `ArgumentOutOfRangeException`.[^13]

Each example file shows the behavior and the explicit check. In every language, the fix is the same as in C: compare the claimed length with the real length, and reject the input when they differ.

## How to detect it

- **Sanitizers.** Build the tests with `-fsanitize=address`. AddressSanitizer detects out-of-bounds accesses to the heap, the stack and globals.[^7] CWE-125 rates it as moderately effective, because a test must reach the code with a bad length.[^1]
- **Fuzzing.** CWE-125 rates fuzzing as highly effective.[^1] A fuzzer sends lengths that do not match the data, and a sanitized build turns each over-read into a failure. Codenomicon found Heartbleed with its security testing tools.[^5]
- **Static analysis.** CWE-125 rates automated static analysis as highly effective.[^1] CodeQL query `cpp/overflow-buffer` finds calls such as `memcpy` whose size can be larger than the buffer that they read or write.[^15]
- **Code review.** For each copy and each loop over a buffer, find where the length comes from. A length from a header, a field of a message or a file is untrusted. Check signed indices at both ends. Check that each string from outside has a final `'\0'` before a string function reads it.

## How to prevent it

- **Use a memory-safe language.** OWASP puts a memory-safe language first among its defences against memory management failures, and CWE-125 lists language selection as a mitigation.[^3][^1]
- **Take the length from the buffer.** Copy `min(claimed, received)` bytes, or better, reject the input when they differ. Use types that carry their size, such as `std::span` in C++ and slices in Rust and Go.
- **Validate every length, offset and index.** Check the minimum and the maximum, and check the arithmetic that computes a length for overflow.[^1]
- **Do not trust sentinels in input.** CWE-125 warns against relying on a sentinel such as NUL in untrusted input.[^1] On data from outside, use functions that take a maximum size, such as `strnlen` and `memchr`.
- **Clear memory that holds secrets, and do not hand out uninitialized memory.** A read past the end returns what is next to the buffer. The Node.js documentation warns that `Buffer.allocUnsafe` returns memory that is not initialized and may contain sensitive data.[^14]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2014-0160 | published 2014-04-07 | Heartbleed. The TLS and DTLS heartbeat code of OpenSSL 1.0.1 before 1.0.1g read past its buffer and sent process memory, including private keys, to remote attackers.[^6][^5] |
| CVE-2020-11899 | published 2020-06-17 | The Treck TCP/IP stack before 6.0.1.66 had an out-of-bounds read in its IPv6 code. CWE-125 lists it as exploited in the wild.[^16][^1] |
| CVE-2023-1018 | published 2023-02-28 | The TPM 2.0 module library read 2 bytes past the end of a command in `CryptParameterDecryption`, which could expose data stored in the TPM.[^17] |
| CVE-2018-10887 | published 2018-07-10 | An unexpected sign extension in `git_delta_apply` of libgit2 led to an integer overflow and then to a read before the base object, which could leak memory addresses.[^18] |

## References

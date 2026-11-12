---
tag: VE8WQW
kind: weakness
slug: uncontrolled-resource-consumption
name: Uncontrolled resource consumption
aliases: [resource exhaustion, denial of service by resource exhaustion, unbounded allocation, cwe-400]
area: security/resource-exhaustion
level: core
summary: >-
  Uncontrolled resource consumption happens when input decides how much memory, time, stack or other resource a program uses, with no limit, so that one request can starve the others.
cwe: 400
abstraction: class
languages_affected: [any]
history:
  year: 2003
  people: [Scott A. Crosby, Dan S. Wallach]
  note: In "Denial of Service via Algorithmic Complexity Attacks", Crosby and Wallach showed that chosen input can push hash tables and binary trees to their worst case. Low-bandwidth traffic made a Bro intrusion detection server drop as much as 71% of its traffic.
sources:
  - url: https://cwe.mitre.org/data/definitions/400.html
    title: "CWE-400: Uncontrolled Resource Consumption"
    locator: Description; Alternate Terms; Relationships; Modes Of Introduction; Common Consequences; Potential Mitigations; Detection Methods; Demonstrative Examples, the Go example with io.ReadAll; Observed Examples; Vulnerability Mapping Notes; Content History
  - url: https://www.usenix.org/conference/12th-usenix-security-symposium/denial-service-algorithmic-complexity-attacks
    title: "Scott A. Crosby and Dan S. Wallach, Denial of Service via Algorithmic Complexity Attacks, 12th USENIX Security Symposium, August 2003"
    locator: the abstract
  - url: https://cwe.mitre.org/top25/archive/2024/2024_top25_list.html
    title: 2024 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 24
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 25, Rank Last Year
  - url: https://cwe.mitre.org/data/definitions/770.html
    title: "CWE-770: Allocation of Resources Without Limits or Throttling"
    locator: Description; Relationships; Potential Mitigations; Detection Methods
  - url: https://cwe.mitre.org/data/definitions/674.html
    title: "CWE-674: Uncontrolled Recursion"
    locator: Description; Alternate Terms; Potential Mitigations
  - url: https://cwe.mitre.org/data/definitions/1333.html
    title: "CWE-1333: Inefficient Regular Expression Complexity"
    locator: Description; Alternate Terms; Potential Mitigations; Observed Examples
  - url: https://cwe.mitre.org/data/definitions/409.html
    title: "CWE-409: Improper Handling of Highly Compressed Data (Data Amplification)"
    locator: Description; Observed Examples
  - url: https://cwe.mitre.org/data/definitions/776.html
    title: "CWE-776: Improper Restriction of Recursive Entity References in DTDs ('XML Entity Expansion')"
    locator: Description; Alternate Terms; Relationships
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Denial_of_Service_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Denial of Service Cheat Sheet
    locator: Application attacks, the list of mitigations; Rate limiting
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Input_Validation_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Input Validation Cheat Sheet
    locator: Parse Safely, Then Validate
  - url: https://top10.owasp.org/2025/A10_2025-Mishandling_of_Exceptional_Conditions/
    title: "OWASP Top 10:2025, A10:2025 Mishandling of Exceptional Conditions"
    locator: How to prevent
  - url: https://owasp.org/www-community/attacks/Regular_expression_Denial_of_Service_-_ReDoS
    title: OWASP Community, Regular expression Denial of Service - ReDoS
    locator: Description; Evil Regex
  - url: https://blog.cloudflare.com/details-of-the-cloudflare-outage-on-july-2-2019/
    title: "Cloudflare, Details of the Cloudflare outage on July 2, 2019"
    locator: the opening section; the list of causes; What's happened since last Tuesday; Appendix, About Regular Expression Backtracking
  - url: https://docs.python.org/3/library/zipfile.html
    title: The Python Standard Library, zipfile
    locator: Decompression pitfalls, Resources limitations
  - url: https://docs.python.org/3/library/zlib.html
    title: The Python Standard Library, zlib
    locator: Decompress.decompress, the max_length parameter; Decompress.unconsumed_tail
  - url: https://docs.python.org/3/library/stdtypes.html
    title: The Python Standard Library, Built-in Types
    locator: Integer string conversion length limitation
  - url: https://docs.python.org/3/library/sys.html
    title: The Python Standard Library, sys
    locator: sys.setrecursionlimit
  - url: https://docs.python.org/3/library/xml.html
    title: The Python Standard Library, XML Processing Modules
    locator: XML security
  - url: https://docs.python.org/3.14/library/re.html
    title: The Python Standard Library, re, Python 3.14
    locator: Regular Expression Syntax, the possessive quantifiers and (?>...)
  - url: https://pkg.go.dev/net/http
    title: Go standard library, net/http
    locator: func MaxBytesReader; type MaxBytesError; type Server, the fields ReadTimeout, ReadHeaderTimeout, WriteTimeout, IdleTimeout and MaxHeaderBytes; const DefaultMaxHeaderBytes
  - url: https://pkg.go.dev/regexp
    title: Go standard library, regexp
    locator: Overview, the paragraph on linear time
  - url: https://nodejs.org/api/http.html
    title: Node.js documentation, HTTP
    locator: server.headersTimeout; server.requestTimeout
  - url: https://www.cve.org/CVERecord?id=CVE-2003-1564
    title: CVE-2003-1564, libxml2
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2006-1173
    title: CVE-2006-1173, Sendmail
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2009-1955
    title: CVE-2009-1955, Apache APR-util
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2019-16215
    title: CVE-2019-16215, Zulip
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-7218
    title: CVE-2020-7218, HashiCorp Nomad
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-21317
    title: CVE-2021-21317, uap-core
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-10735
    title: CVE-2020-10735, Python int and str conversion
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2023-44487
    title: CVE-2023-44487, HTTP/2 rapid reset
    locator: the description and the publication date
---

## Description

Uncontrolled resource consumption happens when the product does not properly control the allocation and maintenance of a limited resource.[^1] The resource can be memory, CPU time, the call stack, disk space, file descriptors, connections, threads or processes. The input decides how much of the resource one request uses, and nothing sets a ceiling. One large or cleverly shaped request then uses what many normal requests need, and the program slows down, stops answering or crashes. CWE also calls this resource exhaustion.[^1]

The weakness is easy to make for a few reasons:

- **Tests use normal input.** A developer tests a form with a name of ten letters and a list of five items. The code works, and it is fast. Nobody tests a body of one gigabyte or a list nested a million levels deep.
- **The worst case hides behind the average.** Crosby and Wallach showed that hash tables and binary trees, which are fast on average, can degenerate to linked lists with carefully chosen input.[^2] A regular expression can be the same: fast on most text and exponential on a few strings.
- **Small input can mean large work.** A few kilobytes of compressed data can expand to gigabytes. A short XML document can define entities that refer to other entities. A length field of four bytes can ask the program to allocate four gigabytes.
- **Many defaults have no limit.** A function that reads "all" of a stream reads until the end, however far that is. A server field with the zero value often means "no timeout". The limit has to be a decision that someone makes.
- **Design leaves it out.** CWE notes that the designer might not consider how to handle and throttle excessive resource requests, and that a product can run in an environment with lower resource limits than expected.[^1]

The usual result is denial of service. CWE lists crashes, exits and restarts, and the consumption of CPU, memory and other resources. Valid users cannot reach the product, and the effect can spread to other parts of the system.[^1] When the program crashes on memory or stack exhaustion, an attacker can repeat the request and keep it down.

CWE-400 is a class weakness.[^1] CWE discourages its use for mapping a vulnerability. CWE-400 is often used for the impact of a bug and not for the mistake, so CWE asks analysts to map to a child or descendant that names the specific mistake.[^1] The main children are these:

- CWE-770, Allocation of Resources Without Limits or Throttling, a child of CWE-400.[^5]
- CWE-674, Uncontrolled Recursion, also called stack exhaustion.[^6]
- CWE-1333, Inefficient Regular Expression Complexity, also called ReDoS.[^7]
- CWE-409, Improper Handling of Highly Compressed Data, also called data amplification.[^8]
- CWE-776, XML Entity Expansion, also called the billion laughs attack.[^9]

CWE-400 was rank 24 on the 2024 CWE Top 25.[^3] It is not on the 2025 list. Its child CWE-770 is rank 25 on the 2025 list, up from rank 26 the year before.[^4] The OWASP Top 10:2025 does not map CWE-400. Its category A10:2025, Mishandling of Exceptional Conditions, says: "Nothing in information technology should be limitless, as this leads to a lack of application resilience, denial of service, successful brute force attacks, and extraordinary cloud bills."[^12]

This page is about limits in the code. A missing check of the form or the range of a value is [[improper-input-validation]]. A check of the size of an uploaded file also belongs to [[unrestricted-file-upload]]. In C and C++, an unchecked size can lead to [[integer-overflow]] and then to an [[out-of-bounds-write]] before it leads to exhaustion.

### History

CWE-400 was submitted to CWE on 2006-07-19 from CLASP, and it first appeared in Draft 3.[^1] An early and clear statement of the algorithmic form came in 2003. Scott A. Crosby and Dan S. Wallach presented "Denial of Service via Algorithmic Complexity Attacks" at the 12th USENIX Security Symposium. They attacked the hash tables of two versions of Perl, the Squid web proxy and the Bro intrusion detection system. With less bandwidth than a typical dialup modem, they made a dedicated Bro server drop as much as 71% of its traffic after six minutes, with all of its CPU in use.[^2] They also showed that universal hashing gives performance comparable to common hash functions and is provably secure against these attacks.[^2]

The same idea, small input and large work, appears again and again. XML entity expansion in libxml2 is recorded as CVE-2003-1564, the "billion laughs attack".[^24] Regular expression backtracking took down the Cloudflare network in 2019.[^14] In 2023, the HTTP/2 "rapid reset" attack, CVE-2023-44487, used request cancellation to reset many streams quickly, and it was exploited in the wild from August to October 2023.[^31]

### Regular expression backtracking

Most regular expression engines in common use match by backtracking. When one path fails, the engine goes back and tries another. A pattern with a quantifier inside a repeated group, such as `(a+)+$`, has many ways to match the same letters. The OWASP ReDoS page uses it as the classic example. For the input `aaaaX` there are 16 paths. With sixteen letters there are 65536 paths, and each extra letter doubles the number.[^13] OWASP calls such a pattern an "evil regex", and lists the input of many `a` letters followed by `!`.[^13]

```python fails expect=timeout timeout=2 id=evil-regex
import re

# A nested quantifier: the classic example from the OWASP ReDoS page.
EVIL = re.compile(r"(a+)+$")

# 30 letters and one character that cannot match. Each extra "a" doubles the work.
print(EVIL.match("a" * 30 + "!"))
```

The program does not finish in two seconds. On a fast machine it needs much longer, and with ten more letters it needs about a thousand times longer.

The Cloudflare outage of 2 July 2019 is the well-known case. A new rule in the web application firewall had a regular expression that backtracked enormously. CPU use rose to nearly 100% on the servers that handled HTTP and HTTPS traffic, and the core proxy, CDN and WAF were down for 27 minutes. The critical part of the pattern was `.*(?:.*=.*)`.[^14] The firewall used PCRE through Lua, and a protection against excessive CPU use by a regular expression had been removed by mistake weeks before.[^14] The report points to the linear-time method that Ken Thompson described in 1968, and it says that Cloudflare would move to the re2 or Rust regex engine, which both have run-time guarantees.[^14]

The defences, from the strongest:

1. **Use an engine that does not backtrack** for patterns that see untrusted input. The Go `regexp` package is guaranteed to run in time linear in the size of the input.[^22] CWE rates this as highly effective.[^7]
2. **Remove nested quantifiers and overlapping alternatives.** `a+` matches the same strings as `(a+)+`. Python supports possessive quantifiers and atomic groups, which do not give back what they matched.[^20]
3. **Limit the length of the input** before the match.[^7]
4. **Set a time limit** on the match or on the process, where the engine or the platform allows it.[^7]

```python run id=one-quantifier
import re

MAX_LENGTH = 100

# One quantifier, no nesting. The engine has only one way to match each letter.
PLAIN = re.compile(r"a+")


def check(text):
    if len(text) > MAX_LENGTH:
        return "rejected: too long"
    return "match" if PLAIN.fullmatch(text) else "no match"


print(check("a" * 30 + "!"))
print(check("a" * 30))
print(check("a" * 10_000 + "!"))
```

```output for=one-quantifier
no match
match
rejected: too long
```

### Zip bombs and data amplification

A compressed file can hold far more data than its size suggests. A file of a few kilobytes can expand to gigabytes, because long runs of the same byte compress very well. A program that unpacks an upload with no limit on the output fills its memory or its disk. The Python documentation of `zipfile` lists decompression bombs, also known as ZIP bombs, under resource limitations, because they can exhaust the disk volume.[^15] CWE calls this data amplification and files it as CWE-409.[^8]

This page does not include a bomb file. The `inflate` handler in the examples below shows the ratio with 2 MiB of zeros, which compresses to about 2 KiB. The defence is the same for every format: decide the maximum output size, decompress in steps, and stop when the output reaches the limit. In Python, `zlib.Decompress.decompress` takes a `max_length`, and the input that it did not use stays in `unconsumed_tail`.[^16] For an archive, also limit the number of entries and the total size of all of them, and do not trust the sizes that the archive header claims.

XML entity expansion is the same idea in a different format. A small document defines an entity that expands to many copies of another entity, level after level. CWE-776 lists "Billion Laughs Attack" and "XML Bomb" as alternate terms.[^9] The Python documentation warns that Expat versions lower than 2.7.2 may be vulnerable to the "billion laughs", "quadratic blowup" and "large tokens" vulnerabilities.[^19]

## Vulnerable code

The three handlers below have no limits. Each function stands for a request handler, and the inputs are small on purpose: 3 MiB for a large body, 50,000 levels of nesting and 2 MiB of zeros. A real attacker sends more.

```python wrong id=no-limits
import io
import zlib

# Each function stands for a request handler. The inputs are small on purpose.


# 1. Read the whole body into memory, however large it is.
def upload(stream):
    body = stream.read()
    return 200, f"stored {len(body)} bytes"


# 2. Parse nested lists with one call for each level.
def parse(text, i=0):
    items = []
    i += 1  # skip "["
    while text[i] != "]":
        child, i = parse(text, i)
        items.append(child)
    return items, i + 1


def nested(text):
    value, _ = parse(text)
    return 200, f"parsed {len(text)} characters"


# 3. Decompress the whole body in one call.
def inflate(data):
    out = zlib.decompress(data)
    return 200, f"{len(data) // 1024} KiB in, {len(out) // 1024} KiB out"


def serve(handler, arg):
    try:
        status, text = handler(arg)
    except Exception as e:
        status, text = 500, type(e).__name__
    print(handler.__name__, status, text)


serve(upload, io.BytesIO(b"x" * (3 * 1024 * 1024)))
serve(nested, "[" * 50_000 + "]" * 50_000)
serve(inflate, zlib.compress(bytes(2 * 1024 * 1024)))
```

```output for=no-limits
upload 200 stored 3145728 bytes
nested 500 RecursionError
inflate 200 2 KiB in, 2048 KiB out
```

1. `upload` reads the whole stream into memory. The body here is 3 MiB, but the code would read 3 GiB the same way. This is the mistake of the Go example in CWE-400, which reads the body with `io.ReadAll`.[^1]
2. `parse` calls itself once for each level of nesting. Python stops it at its recursion limit with `RecursionError`, which protects the C stack of the interpreter.[^18] The handler still fails, and in C the same code crashes the process, as the C example shows.
3. `inflate` turns about 2 KiB into 2 MiB, a ratio of about 1000 to 1. The output size is under the control of whoever made the compressed data.

## Fixed code

The fixed handlers keep the same shape. Each one sets a fixed limit before it uses the resource, and it rejects the request when the input goes past the limit.

```python run id=limits
import io
import zlib

# The same handlers with a fixed limit on each resource.
MAX_BODY = 1024 * 1024  # bytes
MAX_DEPTH = 32  # levels of nesting
MAX_OUTPUT = 1024 * 1024  # bytes after decompression


class TooLarge(Exception):
    pass


class TooDeep(Exception):
    pass


# 1. Read at most one byte more than the limit, then decide.
def upload(stream):
    body = stream.read(MAX_BODY + 1)
    if len(body) > MAX_BODY:
        raise TooLarge("body")
    return 200, f"stored {len(body)} bytes"


# 2. Count the depth, and stop at the limit.
def parse(text, i=0, depth=1):
    if depth > MAX_DEPTH:
        raise TooDeep(f"more than {MAX_DEPTH} levels")
    items = []
    i += 1  # skip "["
    while text[i] != "]":
        child, i = parse(text, i, depth + 1)
        items.append(child)
    return items, i + 1


def nested(text):
    value, _ = parse(text)
    return 200, f"parsed {len(text)} characters"


# 3. Ask for at most one byte more than the limit of output.
def inflate(data):
    d = zlib.decompressobj()
    out = d.decompress(data, MAX_OUTPUT + 1)
    if len(out) > MAX_OUTPUT:
        raise TooLarge("decompressed body")
    return 200, f"{len(data)} bytes in, {len(out)} bytes out"


def serve(handler, arg):
    try:
        status, text = handler(arg)
    except TooLarge as e:
        status, text = 413, f"{e} too large"
    except TooDeep as e:
        status, text = 400, f"nesting too deep: {e}"
    print(handler.__name__, status, text)


serve(upload, io.BytesIO(b"x" * (3 * 1024 * 1024)))
serve(upload, io.BytesIO(b"x" * 1000))
serve(nested, "[" * 50_000 + "]" * 50_000)
serve(nested, "[[][[]]]")
serve(inflate, zlib.compress(bytes(2 * 1024 * 1024)))
serve(inflate, zlib.compress(b"hello"))
```

```output for=limits
upload 413 body too large
upload 200 stored 1000 bytes
nested 400 nesting too deep: more than 32 levels
nested 200 parsed 8 characters
inflate 413 decompressed body too large
inflate 200 13 bytes in, 5 bytes out
```

1. **Read at most the limit, plus one byte.** `stream.read(MAX_BODY + 1)` never holds more than 1 MiB and one byte. If it gets the extra byte, the body is too large. The OWASP Denial of Service Cheat Sheet lists limits on the size of file uploads and on the total request size.[^10]
2. **Count the depth.** `parse` takes the depth as an argument and stops at 32 levels. The limit is far below the recursion limit, so the handler controls the error, not the runtime. CWE-674 asks for an end condition that checks the depth of recursion and exits with an error if the recursion goes too deep.[^6] The OWASP Input Validation Cheat Sheet asks to apply request size limits before buffering or parsing input, and to configure parser limits such as maximum nesting depth.[^11]
3. **Limit the output, not the input.** `decompressobj().decompress(data, MAX_OUTPUT + 1)` stops after the limit.[^16] The small compressed input is not the problem. Its output is.
4. **Reject with a clear status.** The handler answers 413 for a body that is too large and 400 for nesting that is too deep. CWE asks that every failure in resource allocation put the system into a safe posture.[^1]

The examples for other languages show the same limits with their standard libraries: `http.MaxBytesReader` and server timeouts in Go, a byte counter in Node.js, a depth limit in C under AddressSanitizer, and `StackOverflowError` in Java.

### Limits that the platform gives

Some limits already exist, and it helps to know them:

- **Python integers.** CPython limits the conversion between `int` and decimal `str` to 4300 digits by default, and raises `ValueError` above it. The documentation says that this limit offers a practical way to avoid CVE-2020-10735.[^17] That CVE describes conversions with quadratic time: a string of 100,000 digits took 50 ms and a string of 1,000,000 digits took 5 s.[^30]
- **Python recursion.** `sys.setrecursionlimit` sets the maximum depth of the interpreter stack. A limit that is too high can lead to a crash.[^18]
- **Go HTTP headers.** `http.DefaultMaxHeaderBytes` is 1 MB, and `Server.MaxHeaderBytes` overrides it. The `Server` type also has `ReadTimeout`, `ReadHeaderTimeout`, `WriteTimeout` and `IdleTimeout`.[^21]
- **Node.js timeouts.** `server.requestTimeout` and `server.headersTimeout` limit the time to receive a request and its headers. The documentation says that each must be set to a non-zero value to protect against potential denial of service attacks when the server has no reverse proxy in front.[^23]

A limit in the platform is a floor, not a design. It does not know the size of a normal request for your application.

## How to detect it

- **Find every place where input decides a size.** Look for reads that read "all", allocations whose size comes from a header or a length field, loops whose count comes from the input, recursion over input structure, and caches or maps that grow with each request. Each one needs a limit that a reader can find.
- **Look at the server configuration.** Check the request size, the header size, the timeouts and the number of connections. A zero or a missing value often means "no limit". CWE notes that automated analysis of configuration files may be able to detect settings that do not specify a maximum value.[^1]
- **Test with large and deep input.** Send a body larger than the limit, nesting deeper than the limit, a compressed body with a high ratio, and a string that makes a regular expression backtrack. The program must answer with an error, quickly, and keep serving other requests. CWE rates automated dynamic analysis, such as many requests in a short time, as moderately effective.[^1]
- **Run the C and C++ tests under AddressSanitizer.** It reports a stack overflow from deep recursion with a stack trace, where a plain build crashes with a signal.
- **Fuzz.** CWE rates fuzzing as opportunistic for this weakness. A fuzzer that does not restart the target between test cases can find exhaustion, and a crash that does not repeat can be a sign of it.[^1]
- **Check regular expressions.** Look for a quantifier inside a repeated group, and for alternatives that can match the same text. CWE rates automated static analysis as highly effective for CWE-1333.[^7]
- **Know the limits of tools.** CWE rates automated static analysis as limited for CWE-400. It finds problems with program-independent resources, such as files, sockets and processes, but not with resources that the application defines.[^1]

## How to prevent it

- **Put a limit on every resource that input controls.** Decide the limits in the requirements: the largest body, the deepest nesting, the longest string, the most items and the longest time. CWE-770 asks to specify the minimum and maximum expectations, and the behaviour when allocation reaches a limit.[^5]
- **Apply the limit before the work.** Check the size before you buffer, the depth before you recurse, and the output size while you decompress.[^11] A check after the work is too late.
- **Prefer algorithms with a known worst case.** Use a regular expression engine that runs in linear time,[^22] universal hashing for tables keyed by input,[^2] and an iterative parser with an explicit stack.
- **Set time limits.** Give each request a deadline, and set the read, write and idle timeouts of the server. A request that has used its time stops, and its resources go back.[^21][^23]
- **Throttle and set quotas.** Limit the rate of requests and the resources for each user or each client. The OWASP Denial of Service Cheat Sheet lists rate limiting, and CWE-770 asks for limits per user.[^10][^5] Authentication helps, because it makes the cost of an attack visible and it names the user. CWE-400 says: "The best protection is to limit the amount of resources that an unauthorized user can cause to be expended."[^1]
- **Use limits from the operating system as a second layer.** CWE-770 names `setrlimit` on POSIX systems.[^5] Container memory limits and process limits do the same job. They stop one process from taking the machine, but the process still fails.
- **Fail safely.** When a limit is hit, release what the request holds, answer with an error, and keep serving.[^1]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2003-1564 | published 2008-09-02 | libxml2, possibly before 2.5.0, did not detect recursion during entity expansion. A crafted XML document with many nested entity references caused memory and CPU consumption, the "billion laughs attack".[^24] |
| CVE-2006-1173 | published 2006-06-07 | Sendmail before 8.13.7 allowed deeply nested, malformed multipart MIME messages to exhaust the stack in the recursive `mime8to7` function. Queued messages were not delivered, and core dump files might consume the disk.[^25] |
| CVE-2009-1955 | published 2009-06-06 | The XML parser in Apache APR-util before 1.3.7, as used in `mod_dav` and `mod_dav_svn`, allowed memory consumption through a document with many nested entity references, for example in a PROPFIND request.[^26] |
| CVE-2019-16215 | published 2019-09-18 | The Markdown parser in Zulip server before 2.0.5 used a regular expression with exponential backtracking. A logged-in user could make the server spend an effectively arbitrary amount of CPU time and stall the processing of later messages.[^27] |
| CVE-2020-7218 | published 2020-01-31 | The HTTP and RPC services of HashiCorp Nomad up to 0.10.2 allowed unbounded resource usage and were open to denial of service without authentication.[^28] |
| CVE-2021-21317 | published 2021-02-16 | Some regular expressions in uap-core, a user agent parser, before 0.11.0 had overlapping capture groups. A long crafted `User-Agent` header could overload a server.[^29] |
| CVE-2020-10735 | published 2022-09-09 | Conversion between `int` and decimal `str` in Python had quadratic time. Python now limits the number of digits by default.[^30][^17] |
| CVE-2023-44487 | published 2023-10-10 | The HTTP/2 protocol allowed server resource consumption because request cancellation can reset many streams quickly. It was exploited in the wild from August to October 2023.[^31] |

## References

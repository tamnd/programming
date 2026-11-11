---
tag: FXQ4B4
kind: weakness
slug: sensitive-information-exposure
name: Exposure of sensitive information
aliases: [exposure of sensitive information to an unauthorized actor, information exposure, information disclosure, information leak, verbose error messages, secrets in logs, cwe-200]
area: security/secrets-handling
level: core
summary: >-
  Sensitive information exposure happens when a program shows data to someone who may not see it, such as a stack trace to a user, a secret in a log, or a timing difference.
cwe: 200
abstraction: class
languages_affected: [any]
owasp_top10_2025: A01
cwe_top25_2025: 20
history:
  year: 1973
  people: [Butler W. Lampson]
  note: A Note on the Confinement Problem asked how to stop a program from passing information to anyone except its caller, and named covert channels, such as the effect of a program on the system load.
sources:
  - url: https://cwe.mitre.org/data/definitions/200.html
    title: "CWE-200: Exposure of Sensitive Information to an Unauthorized Actor"
    locator: Description; Extended Description; Alternate Terms; Relationships; Potential Mitigations; Demonstrative Examples; Mapping Notes; Content History
  - url: https://bwlampson.site/11-Confinement/Acrobat.pdf
    title: "Butler W. Lampson, A Note on the Confinement Problem, Communications of the ACM 16(10), October 1973, pp. 613-615"
    locator: the abstract; the list of the three kinds of channels
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 20, Rank Last Year
  - url: https://top10.owasp.org/2025/A01_2025-Broken_Access_Control/
    title: "OWASP Top 10:2025, A01:2025 Broken Access Control"
    locator: Overview, the notable CWEs; List of Mapped CWEs
  - url: https://cwe.mitre.org/data/definitions/209.html
    title: "CWE-209: Generation of Error Message Containing Sensitive Information"
    locator: Description; Potential Mitigations
  - url: https://cwe.mitre.org/data/definitions/532.html
    title: "CWE-532: Insertion of Sensitive Information into Log File"
    locator: Description; Potential Mitigations; Observed Examples
  - url: https://cwe.mitre.org/data/definitions/208.html
    title: "CWE-208: Observable Timing Discrepancy"
    locator: Description; Observed Examples
  - url: https://cwe.mitre.org/data/definitions/203.html
    title: "CWE-203: Observable Discrepancy"
    locator: Description
  - url: https://top10.owasp.org/2025/A02_2025-Security_Misconfiguration/
    title: "OWASP Top 10:2025, A02:2025 Security Misconfiguration"
    locator: Description, the list of misconfigurations; How to prevent; Example attack scenarios, Scenario 3
  - url: https://top10.owasp.org/2025/A10_2025-Mishandling_of_Exceptional_Conditions/
    title: "OWASP Top 10:2025, A10:2025 Mishandling of Exceptional Conditions"
    locator: Overview; List of Mapped CWEs
  - url: https://top10.owasp.org/2025/A09_2025-Security_Logging_and_Alerting_Failures/
    title: "OWASP Top 10:2025, A09:2025 Security Logging and Alerting Failures"
    locator: Overview; List of Mapped CWEs
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Error_Handling_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Error Handling Cheat Sheet
    locator: Introduction; Context, the Struts and Tomcat example; Objective; Proposition
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Logging_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Logging Cheat Sheet
    locator: Data to exclude
  - url: https://paulkocher.com/doc/TimingAttacks.pdf
    title: "Paul C. Kocher, Timing Attacks on Implementations of Diffie-Hellman, RSA, DSS, and Other Systems, Advances in Cryptology, CRYPTO '96, Lecture Notes in Computer Science, pp. 104-113, 1996"
    locator: the abstract; section 1, Introduction
  - url: https://docs.python.org/3.14/library/hmac.html
    title: The Python Standard Library, hmac, Python 3.14
    locator: hmac.compare_digest
  - url: https://pkg.go.dev/log/slog
    title: Go standard library, log/slog
    locator: Customizing a type's logging behavior; type LogValuer, the example Secret
  - url: https://doc.rust-lang.org/std/fmt/trait.Debug.html
    title: Rust standard library, std::fmt::Debug
    locator: the paragraph on derive
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Record.html
    title: Java SE 25 API, java.lang.Record
    locator: toString, Implementation Requirements
  - url: https://www.cve.org/CVERecord?id=CVE-2003-0190
    title: CVE-2003-0190, OpenSSH-portable
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2007-5172
    title: CVE-2007-5172, Quicksilver Forums
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2017-9615
    title: CVE-2017-9615, Cognito Software Moneyworks
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2018-1999036
    title: CVE-2018-1999036, Jenkins SSH Agent Plugin
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2019-10071
    title: CVE-2019-10071, Apache Tapestry
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2022-31162
    title: CVE-2022-31162, slack-morphism-rust
    locator: the description, the CWEs and the publication date
---

## Description

A program holds information that not everyone may see: passwords, tokens, keys, personal data, the paths and versions of its own parts, and the state of its own checks. CWE-200 covers a product that exposes sensitive information to an actor that is not explicitly authorized to have access to that information.[^1] CWE lists the kinds of information: private personal data, the status of the system and its environment, business secrets, network configuration, the product's own code or internal state, metadata such as connection logs, and indirect information, such as a difference between two operations that an outsider can observe.[^1]

CWE names three ways that the exposure happens.[^1] The code explicitly inserts the information into a message or a resource that others can read. A different mistake indirectly inserts it, as when a script error reveals the full path of the program. Or the information sits in a resource that is meant to hold it, and the resource is unintentionally made accessible.

This page covers the first two ways in ordinary application code:

- **Error messages.** CWE-209 covers an error message that includes sensitive information about the environment, the users or the data.[^5] A stack trace sent to the user is the common case. It shows file paths, the names of functions and libraries, and often the values that caused the error.
- **Logs and debug output.** CWE-532 covers a product that writes sensitive information to a log file.[^6] Logs go to more people and more systems than the data that they describe. A token in a log is readable by everyone who can read the log.
- **Responses with too many fields.** An API turns a whole record into JSON, and the record has a password hash or a reset token. The JavaScript example file shows this case.
- **Timing and other differences.** CWE-203 covers a product that behaves differently in a way that an unauthorized actor can observe.[^8] CWE-208 is the case where the difference is in time.[^7] A short section below explains it.

The weakness is easy to make, because the code that exposes the data is helpful code. A full stack trace helps the developer find a bug. A log line with the whole request helps the operator. A derived `toString` or `Debug` saves typing. Each one was right on the developer's machine, and each one becomes a problem when the output reaches someone else.

CWE-200 is a class weakness.[^1] It is rank 20 on the 2025 CWE Top 25, down from rank 17 the year before.[^3] The OWASP Top 10:2025 maps it to A01:2025 Broken Access Control.[^4] It maps CWE-209 to A10:2025 Mishandling of Exceptional Conditions,[^10] and CWE-532 to A09:2025 Security Logging and Alerting Failures.[^11] CWE discourages the use of CWE-200 to map a vulnerability. Loss of confidentiality is a technical impact, and many weaknesses cause it. If an error or mistake causes the disclosure, CWE asks for the CWE ID of that error.[^1] An out-of-bounds read that returns memory, for example, is [[out-of-bounds-read]]. CWE also avoids the word "leak", because it has other meanings in security, such as a memory leak.[^1]

A secret in the source code is [[hard-coded-credentials]]. A secret in a log or an error message is this weakness, even when the secret came from a secret store.

### History

Butler Lampson stated the problem in 1973. "A Note on the Confinement Problem" asks how to confine a program during its execution so that it cannot transmit information to any other program except its caller.[^2] Lampson sorted the ways out into three kinds of channels: storage that the service writes and another program reads, legitimate channels such as the bill, and covert channels, those not intended for information transfer at all, such as the service program's effect on the system load.[^2] Error messages and logs are the storage and the legitimate channels of today. Timing is a covert channel.

In 1996 Paul Kocher showed that the covert channel of time was practical against cryptography. By carefully measuring the time of private key operations, attackers may be able to find fixed Diffie-Hellman exponents, factor RSA keys and break other cryptosystems.[^14]

CWE-200 entered CWE in July 2006, from the PLOVER list.[^1]

## Vulnerable code

The handler below fails because the database is down. It logs every request with all of its headers, and when an error happens it sends the traceback to the client. The connection string holds a placeholder password. The demonstration is the most common trigger of all: an error that nobody planned for.

```python wrong id=traceback-to-client
import logging
import os
import sys
import traceback

logging.basicConfig(stream=sys.stdout, level=logging.INFO, format="LOG %(message)s")
log = logging.getLogger("shop")

# A placeholder. A real service reads this from a secret store.
DATABASE_URL = "postgresql://shop:PLACEHOLDER-PASSWORD@db.internal:5432/shop"


def connect(url):
    raise ConnectionError(f"could not connect to {url}")


def get_order(order_id):
    connect(DATABASE_URL)


def handle(headers, order_id):
    log.info("GET /orders/%s headers=%s", order_id, headers)
    try:
        return 200, get_order(order_id)
    except Exception:
        return 500, traceback.format_exc()  # the client gets the traceback


status, body = handle({"Authorization": "Bearer PLACEHOLDER-TOKEN"}, 7)
print(status)
# The traceback shows the full path of this file. The demo shortens it, so
# that the output is the same on every machine.
print(body.replace(os.path.dirname(os.path.abspath(__file__)), "<app>"))
```

```output for=traceback-to-client
LOG GET /orders/7 headers={'Authorization': 'Bearer PLACEHOLDER-TOKEN'}
500
Traceback (most recent call last):
  File "<app>/main.py", line 24, in handle
    return 200, get_order(order_id)
                ~~~~~~~~~^^^^^^^^^^
  File "<app>/main.py", line 18, in get_order
    connect(DATABASE_URL)
    ~~~~~~~^^^^^^^^^^^^^^
  File "<app>/main.py", line 14, in connect
    raise ConnectionError(f"could not connect to {url}")
ConnectionError: could not connect to postgresql://shop:PLACEHOLDER-PASSWORD@db.internal:5432/shop
```

Two things reach people who should not see them:

1. **The log line has the bearer token.** Everyone who can read the logs can use the token.
2. **The client gets the traceback.** It shows the file names, the function names, the source lines and the connection string with the password. The OWASP Error Handling Cheat Sheet gives the same kind of page from Struts and Tomcat as its example, and notes that it reveals the versions of the stack.[^12] CWE-209 lists a forum product whose connection errors showed the database password.[^5][^20]

## Fixed code

The fixed handler catches the error, logs it on the server with an incident id, and sends the client a generic message with only that id. The log line lists the fields that it needs, so a header that holds a token never gets in. The password is a `Secret`, whose `repr` and `str` never show the value, so an error message that includes it shows only `***`.

```python run id=generic-error
import io
import itertools
import json
import logging
import os


class Secret:
    """Holds a secret. Its repr and str never show the value."""

    def __init__(self, value):
        self._value = value

    def reveal(self):
        return self._value

    def __repr__(self):
        return "Secret('***')"

    __str__ = __repr__


# The server log. In the demo it is a buffer, so the program can print it.
server_log = io.StringIO()
handler = logging.StreamHandler(server_log)
handler.setFormatter(logging.Formatter("LOG %(levelname)s %(message)s"))
log = logging.getLogger("shop")
log.addHandler(handler)
log.setLevel(logging.INFO)
log.propagate = False

# Placeholders. A real service reads these from a secret store.
DB_HOST = "db.internal:5432"
DB_PASSWORD = Secret("PLACEHOLDER-PASSWORD")
INCIDENTS = itertools.count(1001)  # a real service uses a random id


def connect(host, password):
    raise ConnectionError(f"could not connect to {host} with password {password}")


def get_order(order_id):
    connect(DB_HOST, DB_PASSWORD)


def handle(headers, order_id):
    # Log an allow list of fields, never the whole set of headers.
    log.info("GET /orders/%s auth=%s", order_id, "present" if "Authorization" in headers else "absent")
    try:
        return 200, get_order(order_id)
    except Exception:
        incident = f"E{next(INCIDENTS)}"
        log.exception("incident %s", incident)  # the details stay on the server
        return 500, json.dumps({"error": "internal error", "incident": incident})


status, body = handle({"Authorization": "Bearer PLACEHOLDER-TOKEN"}, 7)
print("client gets:", status, body)
print("server log:")
print(server_log.getvalue().replace(os.path.dirname(os.path.abspath(__file__)), "<app>"), end="")
```

```output for=generic-error
client gets: 500 {"error": "internal error", "incident": "E1001"}
server log:
LOG INFO GET /orders/7 auth=present
LOG ERROR incident E1001
Traceback (most recent call last):
  File "<app>/main.py", line 50, in handle
    return 200, get_order(order_id)
                ~~~~~~~~~^^^^^^^^^^
  File "<app>/main.py", line 43, in get_order
    connect(DB_HOST, DB_PASSWORD)
    ~~~~~~~^^^^^^^^^^^^^^^^^^^^^^
  File "<app>/main.py", line 39, in connect
    raise ConnectionError(f"could not connect to {host} with password {password}")
ConnectionError: could not connect to db.internal:5432 with password Secret('***')
```

The client gets a status code, a short message and an id. A support person can find the full story in the server log with that id. The OWASP Error Handling Cheat Sheet describes this design: a generic response for the user, and the details logged on the server for investigation.[^12] The OWASP Top 10:2025 also asks for a central configuration to intercept excessive error messages, as a backup.[^9]

The fix has three layers, and each one catches a mistake that the other two miss:

- **The boundary.** One handler turns every unexpected error into the generic response. A new error type cannot reach the client.
- **The log.** The log line names its fields. It does not dump a whole object, so a new field with a secret stays out.
- **The type.** A secret is wrapped in a type that prints as `***`. Code that needs the value calls `reveal()`, which is easy to find in a review.

### Timing differences

Some exposure has no message at all. CWE-208 describes two operations that take different amounts of time in a way that reveals something about the state of the product, such as whether an operation succeeded.[^7] A comparison that stops at the first wrong character takes longer when more characters are right. The program below counts the steps of such a comparison, instead of measuring time, so that its output is the same on every run.

```python run id=steps
import hmac

EXPECTED = "7f3a9c"  # a placeholder token


def early_exit_equal(a, b):
    """Compares like a plain loop: it stops at the first difference."""
    steps = 0
    if len(a) != len(b):
        return False, steps
    for x, y in zip(a, b):
        steps += 1
        if x != y:
            return False, steps
    return True, steps


for guess in ["000000", "700000", "7f0000", "7f3000", "7f3a9c"]:
    equal, steps = early_exit_equal(guess, EXPECTED)
    print(guess, equal, "steps:", steps)

print(hmac.compare_digest(b"7f0000", EXPECTED.encode()))
print(hmac.compare_digest(b"7f3a9c", EXPECTED.encode()))
```

```output for=steps
000000 False steps: 1
700000 False steps: 2
7f0000 False steps: 3
7f3000 False steps: 4
7f3a9c True steps: 6
False
True
```

Each correct character at the start adds a step. An attacker who can measure the time of many requests can try one position at a time. CWE lists a router whose password check stopped at the first incorrect character, and a web framework that compared HMAC signatures with `String.equals()`.[^7] `hmac.compare_digest` avoids content-based short circuiting, so its time does not depend on where the values differ.[^15] It can still show the length of the values.[^15]

Time is not the only difference. A login that answers "no such user" for one name and "wrong password" for another tells the caller which names exist. So does a login that returns at once for an unknown user, as in the OpenSSH case below. The fix is the same answer, and about the same work, for every failure.

## How to detect it

- **Make the program fail and read the output.** Send malformed input, stop the database and call each endpoint. Look at the body, the headers and the status of each error response. Then look at the logs that the same requests wrote.
- **Search the logs for secrets.** Plant known placeholder values, such as a test password and a test token, then run the test suite and search every log and every response for them.
- **Review every place that converts an object to text.** Look for `repr`, `toString`, `Debug`, `JSON.stringify`, `%v` and logging calls that receive a whole request, a whole configuration or a whole user.
- **Check the production settings.** Look for debug modes, verbose error pages and settings such as `display_errors` in PHP, which CWE names.[^5] The OWASP Top 10:2025 lists error handling that reveals stack traces as a misconfiguration.[^9]
- **Check comparisons of secrets.** Any `==` on a password, a token, a MAC or a signature is a finding. Use the constant-time function of the language.
- **Use static analysis for the common patterns.** A taint analysis can follow a value from a source that holds secrets to a log call or a response. It needs to know which values are secret, so name them with a type or a naming convention, as CWE-209 suggests.[^5]

## How to prevent it

- **Return generic errors.** Handle exceptions in one place, and send the user a short message and an incident id. Log the details on the server.[^5][^12]
- **Turn off debug output in production.** Debug information should not make its way into a production release.[^5]
- **Log an allow list of fields.** Do not log passwords, session ids, access tokens, connection strings or keys. The OWASP Logging Cheat Sheet lists these as data to remove, mask, hash or encrypt.[^13] CWE adds: protect log files against unauthorized reads, and remove debug log files before release.[^6]
- **Give secrets their own type.** Wrap them in a type that does not print its value: a `LogValuer` in Go,[^16] a hand-written `Debug` in Rust, and an overridden `toString` in Java. A derived `Debug` in Rust prints the name and value of each field,[^17] and the implicit `toString` of a Java record includes the names and values of its components.[^18] The example files show both.
- **Build responses from an allow list.** Copy the fields that the client may see into a new object. Do not serialize database records directly.
- **Compare secrets in constant time.** Use `hmac.compare_digest` in Python,[^15] `crypto/subtle.ConstantTimeCompare` in Go, `MessageDigest.isEqual` in Java and `crypto.timingSafeEqual` in Node.js.
- **Give the same answer for every failure.** A login, a password reset and a sign-up give the same message whether the account exists or not.

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2003-0190 | published 2003-05-02 | OpenSSH-portable 3.6.1p1 and earlier, with PAM support, sent an error message at once when a user did not exist. Remote attackers could find valid user names through a timing attack.[^19] |
| CVE-2007-5172 | published 2007-10-01 | In Quicksilver Forums before 1.4.1, connection errors revealed the database password in the error message.[^20] |
| CVE-2017-9615 | published 2017-06-26 | In Cognito Software Moneyworks 8.0.3 and earlier, verbose logging wrote the administrator password to a world-readable file.[^21] |
| CVE-2018-1999036 | published 2018-08-01 | The Jenkins SSH Agent Plugin 1.15 and earlier exposed the password of the SSH private key to users with permission to read the build log.[^22] |
| CVE-2019-10071 | published 2019-09-16 | Apache Tapestry compared the HMAC of form submissions with `String.equals()`, which made a timing side channel.[^23] |
| CVE-2022-31162 | published 2022-07-21 | Before 0.41.0, slack-morphism-rust could print Slack OAuth client information in application debug logs. Version 0.41.0 added stricter debug formatting for OAuth secret types.[^24] |

## References

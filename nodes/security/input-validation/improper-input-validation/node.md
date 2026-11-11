---
tag: PAFC4G
kind: weakness
slug: improper-input-validation
name: Improper input validation
aliases: [insufficient input validation, missing input validation, cwe-20]
area: security/input-validation
level: core
summary: >-
  Improper input validation happens when a program uses input without a correct check of the properties it needs, such as type, range and format, so values it cannot handle get through.
cwe: 20
abstraction: class
languages_affected: [any]
owasp_top10_2025: A05
cwe_top25_2025: 18
history:
  year: 2005
  people: [Katrina Tsipenyuk, Brian Chess, Gary McGraw]
  note: The Seven Pernicious Kingdoms taxonomy put Input Validation and Representation first of its seven kingdoms, and said that its security problems result from trusting input. CWE-20 came from this taxonomy in 2006.
sources:
  - url: https://cwe.mitre.org/data/definitions/20.html
    title: "CWE-20: Improper Input Validation"
    locator: Description; Extended Description; Relationships; Modes Of Introduction; Potential Mitigations; Detection Methods; Demonstrative Examples, Example 1; Observed Examples; Notes; Mapping Notes; Content History
  - url: https://cwe.mitre.org/documents/sources/SevenPerniciousKingdoms.pdf
    title: "Katrina Tsipenyuk, Brian Chess and Gary McGraw, Seven Pernicious Kingdoms: A Taxonomy of Software Security Errors, NIST Workshop on Software Security Assurance Tools, Techniques and Metrics, November 2005"
    locator: the list of the seven kingdoms; section 1, Input Validation and Representation
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 18, Rank Last Year
  - url: https://top10.owasp.org/2025/A05_2025-Injection/
    title: "OWASP Top 10:2025, A05:2025 Injection"
    locator: List of Mapped CWEs
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Input_Validation_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Input Validation Cheat Sheet
    locator: Introduction; Input Validation Strategies; Allowlist vs Denylist; Parse Safely, Then Validate; Validating Free-form Unicode Text; Regular Expressions (Regex); Common Pitfalls
  - url: https://lexi-lambda.github.io/blog/2019/11/05/parse-don-t-validate/
    title: "Alexis King, Parse, don't validate, 5 November 2019"
    locator: the introduction; the section on the power of parsing, with the quotation on shotgun parsing; the list of practical advice
  - url: https://cwe.mitre.org/data/definitions/184.html
    title: "CWE-184: Incomplete List of Disallowed Inputs"
    locator: Description; Alternate Terms
  - url: https://cwe.mitre.org/data/definitions/179.html
    title: "CWE-179: Incorrect Behavior Order: Early Validation"
    locator: Description
  - url: https://docs.python.org/3.14/library/re.html
    title: The Python Standard Library, re, Python 3.14
    locator: Regular Expression Syntax, the entries for $ and \d; Module Contents, re.fullmatch
  - url: https://docs.python.org/3.14/library/unicodedata.html
    title: The Python Standard Library, unicodedata, Python 3.14
    locator: unicodedata.normalize
  - url: https://go.dev/doc/security/fuzz/
    title: Go documentation, Go Fuzzing
    locator: Overview
  - url: https://www.cve.org/CVERecord?id=CVE-2008-2223
    title: CVE-2008-2223, vShare YouTube Clone
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-3452
    title: CVE-2020-3452, Cisco ASA and FTD Software
    locator: the description, the CWE and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-22205
    title: CVE-2021-22205, GitLab CE/EE
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2024-37032
    title: CVE-2024-37032, Ollama
    locator: the description and the publication date
---

## Description

A program reads data from outside: a form field, a header, a file, a message from another service. Each part of the program assumes that the data has certain properties. A quantity is a positive number. An id has exactly 64 hexadecimal digits. An end date comes after a start date. Improper input validation happens when the program receives input but does not validate, or incorrectly validates, that the input has the properties that it needs to process the data safely and correctly.[^1] CWE lists the properties to check: quantities, indexes, well-formedness, type, consistency between fields, business rules, equivalence and authenticity.[^1]

The weakness is easy to make for three reasons. The test data that a developer types is always well formed, so the missing check never fails. The checks that exist often look for bad characters, and the real problem is a value that is well formed but wrong, such as a negative quantity. And the checks are often in the wrong place: in the browser, which the attacker controls, or deep in the code, after part of the input was already used.

CWE-20 is a class weakness. It is a child of CWE-707, Improper Neutralization, and it can precede path traversal, injection, out-of-bounds access and allocation without limits.[^1] CWE discourages its use as the mapping of a real vulnerability, because reports often use it when a more precise weakness fits.[^1] CWE also separates validation, which checks whether an input is already safe, from techniques that transform input into something safe, such as escaping.[^1] CWE-20 is rank 18 on the 2025 CWE Top 25, down from rank 12 the year before.[^3] The OWASP Top 10:2025 maps it to A05:2025 Injection.[^4]

This page is about the check itself. The weaknesses that follow a missing check have their own pages: [[sql-injection]], [[os-command-injection]], [[path-traversal]], [[integer-overflow]] and [[uncontrolled-resource-consumption]].

### History

The Seven Pernicious Kingdoms taxonomy of 2005 put Input Validation and Representation first of its seven kingdoms. It said that these problems are caused by metacharacters, alternate encodings and numeric representations, and that the security problems result from trusting input.[^2] CWE-20 came from this taxonomy in July 2006, under the name Insufficient Input Validation. CWE gave it its present name in January 2009.[^1]

### Allow lists and deny lists

A deny list names the inputs that are not allowed. CWE-184 describes its usual failure: the list is incomplete.[^7] There are many encodings and spellings of a bad value, and the list cannot know about the ones that do not exist yet. A deny list also checks the wrong property. It looks for characters that are dangerous in one context, such as a quote, and it does not check what the field means.

An allow list states what each field accepts, and it rejects everything else. The OWASP Input Validation Cheat Sheet says to define what the application accepts, and not to try to recognize every malicious string.[^5] It notes that blocking apostrophes rejects legitimate names and still does not make a database query safe.[^5] CWE rates an accept-known-good strategy as highly effective, and gives an example: if the field is a colour, the value "boat" is syntactically valid, but it is not a colour.[^1]

### Parse, do not validate

Alexis King gave this slogan in 2019.[^6] A validator checks a value and then returns the same value, with the same loose type. The rest of the program cannot tell whether the check ran. A parser turns the loose input into a value of a precise type, and it fails when it cannot. After the parse, the type itself carries the result of the check. King advises that the program gets the data into the most precise representation it needs as early as it can, at the boundary of the system and before it acts on any of the data.[^6] The same post quotes the language-theoretic security term "shotgun parsing": checks mixed with the processing code and spread across it, so that the program may act on part of an input before a later check rejects it.[^6]

### Validation at the trust boundary

A trust boundary is the point where data enters from a less trusted place. The server is the place that enforces the check. A check in the browser gives the user fast feedback, but CWE notes that attackers can bypass client-side checks, so the server must duplicate them.[^1] Data from internal services, queues, partner feeds and the database also crosses a boundary. The cheat sheet lists trust in internal sources as a common pitfall.[^5]

## Vulnerable code

The function below takes the fields of an order form as text. It has a deny list of characters that look dangerous, and it converts the quantity with `int`. The second call sends the quantity `-3`. This is the case of the first demonstrative example of CWE-20, where the code never checks that the quantity is positive.[^1]

```python wrong id=denylist
PRICES = {"pen": 150, "book": 1200}  # in cents
BLOCKED = ("<", ">", "'", '"', ";", "--")


def order_total(product, quantity):
    for bad in BLOCKED:
        if bad in product or bad in quantity:
            raise ValueError("blocked character")
    return PRICES[product] * int(quantity)


print(order_total("book", "2"))
print(order_total("book", "-3"))
print(order_total("book", " 2\n"))
print(order_total("book", "٣"))  # ARABIC-INDIC DIGIT THREE
```

```output for=denylist
2400
-3600
2400
3600
```

The deny list passes every call, because no call has a blocked character. The total of the second order is negative, so the shop would pay the customer. The last two calls show that `int` is more lenient than the developer thought. It strips white space, and it accepts any Unicode decimal digit. Neither one is an attack, but the program accepts inputs that its author did not plan for.

## Fixed code

The fixed code parses the form into an `Order` at the boundary. The parser checks each field against an allow list: the product must be a key of the price table, and the quantity must be one to three ASCII digits with a value from 1 to 100. It rejects the input and does not try to repair it. The function that computes the total takes an `Order`, so it cannot get the raw text.

```python run id=parse
import re
from dataclasses import dataclass

PRICES = {"pen": 150, "book": 1200}  # in cents
QUANTITY = re.compile(r"[0-9]{1,3}")


@dataclass(frozen=True)
class Order:
    product: str
    quantity: int


def parse_order(form):
    product = form.get("product", "")
    if product not in PRICES:
        raise ValueError("unknown product")
    text = form.get("quantity", "")
    if not QUANTITY.fullmatch(text):
        raise ValueError("quantity must be 1 to 3 ASCII digits")
    quantity = int(text)
    if not 1 <= quantity <= 100:
        raise ValueError("quantity must be from 1 to 100")
    return Order(product, quantity)


def order_total(order: Order) -> int:
    return PRICES[order.product] * order.quantity


forms = [
    {"product": "book", "quantity": "2"},
    {"product": "book", "quantity": "-3"},
    {"product": "book", "quantity": " 2\n"},
    {"product": "book", "quantity": "٣"},
    {"product": "book", "quantity": "0"},
    {"product": "boat", "quantity": "1"},
]
for form in forms:
    try:
        print(order_total(parse_order(form)))
    except ValueError as error:
        print("rejected:", error)
```

```output for=parse
2400
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be from 1 to 100
rejected: unknown product
```

Three details of the pattern matter. It uses `fullmatch`, because `$` in Python also matches before a newline at the end of the string.[^9] It uses `[0-9]` and not `\d`, because `\d` in a `str` pattern matches any Unicode decimal digit.[^9] And the range check comes after the conversion, because a value with the right type can still be the wrong quantity.[^5] The example files show the same parse in other languages. In Rust and Java, the type system then keeps code outside the parser from making an `Order` that was not checked.

### Canonical form first

A check is true only for the exact value that it checked. If the program changes the value later, by decoding, normalizing or trimming, the check may no longer hold. CWE-179 names this order of steps: the product validates input before it applies the mechanisms that modify it.[^8] Decode or normalize once, then validate that result, and then use that same result.[^5] The Python example file shows the effect with Unicode normalization: the compatibility form NFKC changes the ligature `ﬃ` into the three letters `ffi`,[^10] so a length check made before the normalization does not hold after it.

## How to detect it

- **Code review at each entry point.** List where data enters: request handlers, message consumers, file readers and the results of calls to other services. For each field, find the code that turns it into a precise value, and check that no code reads the raw value first.
- **Look for lenient conversions.** `int()` in Python, `Number()` in JavaScript, `atoi` in C and `Integer.parseInt` in Java each accept forms that a reader may not expect. The example files show which ones.
- **Fuzzing.** CWE lists fuzzing as a detection method for this weakness.[^1] Go has fuzzing in its standard toolchain.[^11] A fuzz target that calls the parser and then checks the properties of the result finds inputs that pass the parser but break the rules.
- **Boundary tests.** Test zero, negative values, the largest value and one more, empty strings, white space, a trailing newline and non-ASCII digits. The cheat sheet also asks for tests of missing fields, invalid nested items and oversized arrays.[^5]
- **Static analysis.** CWE notes that automated static analysis might not recognize when proper validation is performed, which leads to false positives. Custom checks such as business rules need manual analysis.[^1] A taint tool finds the routes from input to a dangerous function, but it cannot know the rule that a quantity is positive.

## How to prevent it

- **Parse into a precise type at the boundary.** Code inside the boundary takes the parsed type and never the raw text.[^6]
- **Allow-list each field.** State the type, the range, the length, the format and the set of values. Reject anything else, and do not try to repair it.[^1][^5]
- **Check the meaning, not only the syntax.** A booking needs an end date after the start date. A quantity needs to be positive.[^5]
- **Decode and normalize once, then validate.** Validate the form that the program uses, and do not decode it again later.[^1][^5]
- **Check on the server at every trust boundary.** Client-side checks give feedback to the user. They are not a control.[^1][^5]
- **Keep the other defences.** Validation reduces the risk, but it does not replace parameterized queries or output encoding.[^5] A valid account id also does not mean that the caller may access the account, which is a question of [[missing-authorization]].[^5]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2008-2223 | published 2008-05-14 | SQL injection through the `tid` parameter of vShare YouTube Clone 2.6.[^12] CWE lists it as an id that was supposed to be numeric and was not checked.[^1] |
| CVE-2020-3452 | published 2020-07-22 | Cisco ASA and FTD software did not properly validate URLs in HTTP requests. An unauthenticated attacker could use directory traversal to read sensitive files.[^13] |
| CVE-2021-22205 | published 2021-04-23 | GitLab CE/EE from version 11.9 did not properly validate image files that it passed to a file parser, which resulted in remote command execution.[^14] |
| CVE-2024-37032 | published 2024-05-31 | Ollama before 0.1.34 did not validate the format of a digest, which must be sha256 with 64 hexadecimal digits, when it built the path of a model. Inputs with fewer or more digits, or with a leading `../`, were not rejected.[^15] |

## References

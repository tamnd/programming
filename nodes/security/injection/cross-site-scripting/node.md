---
tag: NEW-cross-site-scripting
kind: weakness
slug: cross-site-scripting
name: Cross-site scripting
aliases: [XSS, cwe-79]
area: security/injection
level: core
summary: >-
  Cross-site scripting happens when a web application puts outside input into a page without the right encoding, so the browser reads part of the input as markup or script of the site.
cwe: 79
abstraction: base
languages_affected: [any-that-generates-html]
owasp_top10_2025: A05
cwe_top25_2025: 1
history:
  year: 2000
  people: []
  note: CERT/CC advisory CA-2000-02 of 2 February 2000 described malicious tags and script embedded in client web requests, and said that the problem had also been described as cross-site scripting.
same_as:
  wikidata: Q371199
  wikipedia: Cross-site_scripting
sources:
  - url: https://cwe.mitre.org/data/definitions/79.html
    title: "CWE-79: Improper Neutralization of Input During Web Page Generation ('Cross-site Scripting')"
    locator: Description; Alternate Terms; Relationships; Potential Mitigations; Notes
  - url: https://top10.owasp.org/2025/A05_2025-Injection/
    title: "OWASP Top 10:2025, A05:2025 Injection"
    locator: List of Mapped CWEs
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 1, CVEs in KEV
  - url: https://www.sei.cmu.edu/documents/507/2000_019_001_496188.pdf
    title: "Software Engineering Institute, 2000 CERT Advisories, CA-2000-02: Malicious HTML Tags Embedded in Client Web Requests"
    locator: chapter 2, Overview and I. Description
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Cross_Site_Scripting_Prevention_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Cross Site Scripting Prevention Cheat Sheet
    locator: XSS Defense Philosophy; Output Encoding; Dangerous Contexts; HTML Sanitization; Safe Sinks; Other Controls
  - url: https://docs.python.org/3.14/library/html.html#html.escape
    title: The Python Standard Library, html, Python 3.14
    locator: html.escape
  - url: https://pkg.go.dev/html/template
    title: Go standard library, package html/template
    locator: Overview, Introduction, Contexts and Security Model
  - url: https://learn.microsoft.com/en-us/aspnet/core/security/cross-site-scripting
    title: ASP.NET Core documentation, Prevent Cross-Site Scripting (XSS) in ASP.NET Core
    locator: HTML Encoding using Razor; Accessing encoders in code
  - url: https://codeql.github.com/codeql-query-help/python/py-reflective-xss/
    title: CodeQL query help, Reflected server-side cross-site scripting (py/reflective-xss)
    locator: the query description
  - url: https://docs.astral.sh/ruff/rules/jinja2-autoescape-false/
    title: Ruff rules, jinja2-autoescape-false (S701)
    locator: What it does; Why is this bad?
  - url: https://docs.astral.sh/ruff/rules/suspicious-mark-safe-usage/
    title: Ruff rules, suspicious-mark-safe-usage (S308)
    locator: What it does; Why is this bad?
  - url: https://github.com/securego/gosec/blob/master/RULES.md
    title: gosec rules
    locator: G203 and G705
  - url: https://www.cve.org/CVERecord?id=CVE-2021-1879
    title: CVE-2021-1879, Apple WebKit
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-3580
    title: CVE-2020-3580, Cisco ASA and FTD web services interface
    locator: the description and the publication date
---

## Description

A web page is text that the browser parses. The parse decides which parts are tags, attributes and script, and which parts are text to show. Cross-site scripting (XSS) happens when a server or a script builds the page by joining fixed markup with input from outside. When the input holds a `<`, a quote or a URL scheme, the browser can read it as a new tag, a new attribute or a new script. The program meant to show a comment. The browser runs code.

The root cause is the same as in SQL injection: code and data share one channel. The difference is that a page has many languages in it. Text between tags, a quoted attribute, a URL, a `<script>` block and a style sheet each have their own syntax, so each needs its own encoding.[^5] An escape that is correct in one place is wrong in another.

The script runs in the origin of the site. It can read the page, read cookies that are not `HttpOnly`, send requests as the user, and change what the user sees.[^1][^4] The attacker does not attack the server. The server carries the attack to its own users.

CWE-79 names this weakness. It is a base weakness under CWE-74, injection, and its variants cover narrower cases, such as script-related tags (CWE-80) and script in attributes (CWE-83).[^1] It is rank 1 of the 2025 CWE Top 25, and seven of its CVEs are in the CISA catalogue of known exploited vulnerabilities.[^3] In the OWASP Top 10:2025 it is part of A05:2025 Injection.[^2]

### Three kinds

CWE-79 describes three kinds.[^1]

- **Reflected (type 1).** The input comes in the request, often in a URL parameter, and the server writes it into the response. An attacker sends a crafted link to a victim.
- **Stored (type 2).** The application stores the input, for example as a comment, and later writes it into pages for other users. One stored value reaches every reader. CWE-79 gives the Samy worm on MySpace as an example: script in a profile changed the profile of each user who viewed it.[^1]
- **DOM-based (type 0).** Script of the site reads input in the browser, for example from the URL, and writes it into the page with a sink that parses markup. The server may never see the input.

### History

The CERT Coordination Center published advisory CA-2000-02, "Malicious HTML Tags Embedded in Client Web Requests", on 2 February 2000, together with DoD-CERT, JTF-CND, FedCIRC and NIPC.[^4] It described both a message board that shows one client's tags to another client and a link that makes a client send script to a site, which the site then reflects back. It said that because one source injects code into pages sent by another source, the problem "has also been described as "cross-site" scripting".[^4] Its advice to developers was to check the values that build a page against the characters that are explicitly allowed, at the time the page is made, and to set the character set of each page. It warned that removing dangerous characters from the input leaves risks.[^4]

## Vulnerable code

The function below builds HTML with an f-string. The second call uses the classic demonstration input `<script>alert(1)</script>`. In a browser, it opens a dialog box, which proves that the input runs as script.

```python wrong id=fstring
def comment_html(author, text):
    return f'<p class="comment"><b>{author}</b>: {text}</p>'


print(comment_html("alice", "Nice post."))
print(comment_html("mallory", "<script>alert(1)</script>"))
```

```output for=fstring
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: <script>alert(1)</script></p>
```

The same weakness appears with every way to build a string: `+`, `format`, template literals, `Sprintf`, `String.format` and string interpolation. In the browser, it appears when script assigns input to `innerHTML`.[^5] The example files show it in each language.

## Fixed code

`html.escape` replaces the characters that have a meaning in HTML text with character references.[^6] The browser then shows them as characters and does not parse them as markup.

```python run id=escape
from html import escape


def comment_html(author, text):
    return f'<p class="comment"><b>{escape(author)}</b>: {escape(text)}</p>'


print(comment_html("alice", "Nice post."))
print(comment_html("mallory", "<script>alert(1)</script>"))
```

```output for=escape
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: &lt;script&gt;alert(1)&lt;/script&gt;</p>
```

The comment is now text that reads `<script>alert(1)</script>` on the page. A user who writes `I <3 cats` also sees the right text. This is why an escape is better than a filter that removes `<`.

### Encode for the context

The escape must match the place where the value goes.[^5]

| Context | Example | Protection |
|---|---|---|
| HTML text | `<p>VALUE</p>` | HTML entity encoding of `&`, `<`, `>`, `"` and `'` |
| Quoted attribute | `<input value="VALUE">` | attribute encoding, and the value always in quotes |
| URL | `<a href="VALUE">` | an allow-list of schemes such as `https`, then attribute encoding |
| Data in a script | `<script>const m = "VALUE"</script>` | an encoder made for JavaScript strings in HTML |
| CSS | `style="color: VALUE"` | an allow-list of values for the property |

CWE-79 says that HTML entity encoding is only appropriate for the HTML body.[^1] In a URL attribute, the escape keeps the value inside the quotes, but a `javascript:` URL is still a script. The Python example file shows this case and the fix. The OWASP cheat sheet calls some places dangerous contexts, where no encoding is safe: directly in a script, in an HTML comment, directly in CSS, in a tag name or an attribute name, and in the code of an event handler such as `onclick`.[^5] Do not put input there.

### Templates that know the context

A template engine with automatic escaping removes the need to remember each call. Go's `html/template` goes further: it parses the template, finds the context of each action, and picks the escape for it. It replaces a URL with an unsafe scheme by `#ZgotmplZ`.[^7] Razor in ASP.NET Core encodes each value that a view writes with `@`.[^8] Each engine also has a way to turn the escape off, such as `template.HTML` in Go, `HtmlString` in ASP.NET Core and `mark_safe` in Django.[^7][^8][^11] Such a value must never come from input.

### When users write HTML

Some applications let users write rich text. Encoding would show the tags as text. Here the defence is a sanitizer, a library that parses the HTML and keeps only an allow-list of tags and attributes. OWASP names DOMPurify, and says to keep the sanitizer up to date and not to change its output after the sanitize step.[^5]

## How to detect it

- **Static analysis with taint tracking.** CodeQL follows input from a request to a write into the page. Its queries are `py/reflective-xss`, `js/xss`, `js/reflected-xss`, `java/xss`, `go/reflected-xss` and `cs/web/xss`.[^9]
- **Pattern linters.** Ruff rule S701 reports Jinja2 environments with `autoescape=False`, and S308 reports `mark_safe`.[^10][^11] gosec rule G203 reports data converted to the safe types of `html/template`, and G705 follows input to the response.[^12]
- **Code review.** Search for HTML built with `+`, `format`, f-strings or template literals, for `innerHTML`, and for each way to turn the escape of the template engine off. For each value, ask which context it lands in.
- **Tests.** Put a value with `<`, `"` and `'` through each input that reaches a page. The page must show it as text. A value that becomes part of the markup shows the weakness.

## How to prevent it

- **Use a template engine that escapes by default.** Coverage matters: one missed value in a page is enough.[^5] A context-aware engine such as `html/template` also picks the right escape.
- **Encode each value for its context.** When there is no engine, call the encoder for the context: HTML text, attribute, JavaScript string or URL component.[^5] Validate URLs against an allow-list of schemes.
- **Use text sinks in the browser.** Assign text to `textContent` and form values to `value`, not to `innerHTML`.[^5]
- **Sanitize HTML that users write.** Use a maintained sanitizer with an allow-list.[^5]
- **Add defence in depth.** A Content Security Policy is an allow-list that limits the content that a page can load. `HttpOnly` stops script from reading the session cookie.[^1] Trusted Types make DOM sinks reject plain strings.[^5] None of these fixes the code, and the OWASP cheat sheet warns against a CSP as the only defence.[^5]
- **Validate input as a second layer.** A check that a field holds a number rejects bad input early. CWE-79 says that validation is not enough for free text, which can contain `<` for valid reasons.[^1]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CA-2000-02 | 2000-02-02 | CERT/CC warned that web sites that build pages from unvalidated input let one client run script in another client's browser, in the security context of the site.[^4] |
| CVE-2020-3580 | published 2020-10-21 | XSS in the web services interface of Cisco ASA and FTD software. An unauthenticated attacker had to persuade a user of the interface to click a crafted link.[^14] |
| CVE-2021-1879 | published 2021-04-02 | Crafted web content could lead to universal cross-site scripting in Apple WebKit. The fix improved the management of object lifetimes, and Apple said that the issue may have been actively exploited.[^13] |

## References

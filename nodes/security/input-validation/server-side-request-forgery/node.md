---
tag: NEW-server-side-request-forgery
kind: weakness
slug: server-side-request-forgery
name: Server-side request forgery
aliases: [SSRF, XSPA, cross site port attack, cwe-918]
area: security/input-validation
level: core
summary: >-
  Server-side request forgery happens when a server fetches a URL from outside input and does not check the destination, so an attacker can reach hosts that only the server can reach.
cwe: 918
abstraction: base
languages_affected: [any-that-makes-network-requests]
owasp_top10_2025: A01
cwe_top25_2025: 22
history:
  year: 2008
  people: [Deral Heiland]
  note: At ShmooCon 2008, Heiland showed in "Web Portals, Gateway To Information Or A Hole In Our Perimeter Defenses" that web portlets which load files from other HTTP sources can be used to attack the internal network through a URL parameter.
same_as:
  wikidata: Q56064107
  wikipedia: Server-side_request_forgery
sources:
  - url: https://cwe.mitre.org/data/definitions/918.html
    title: "CWE-918: Server-Side Request Forgery"
    locator: Description; Alternate Terms; Relationships; Common Consequences; Potential Mitigations; Detection Methods; Observed Examples; References; Content History
  - url: https://top10.owasp.org/2025/A01_2025-Broken_Access_Control/
    title: "OWASP Top 10:2025, A01:2025 Broken Access Control"
    locator: Background; List of Mapped CWEs
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 22, Rank Last Year
  - url: https://media.blackhat.com/bh-us-12/Briefings/Polyakov/BH_US_12_Polyakov_SSRF_Business_Slides.pdf
    title: "Alexander Polyakov and Dmitry Chastukhin, SSRF vs. Business-critical applications: XXE tunneling in SAP, Black Hat USA 2012, slides"
    locator: the slides "SSRF History, the beginning" and "SSRF history, World research"
  - url: https://top10.owasp.org/2021/A10_2021-Server-Side_Request_Forgery_(SSRF)
    title: "OWASP Top 10:2021, A10:2021 Server-Side Request Forgery (SSRF)"
    locator: Overview
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Server_Side_Request_Forgery_Prevention_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Server-Side Request Forgery Prevention Cheat Sheet
    locator: Case 1, Application can send request only to identified and trusted applications; Case 2, Application can send requests to any external IP address or domain name; Network layer
  - url: https://docs.python.org/3/library/urllib.request.html
    title: The Python Standard Library, urllib.request, Python 3.15
    locator: HTTPRedirectHandler.redirect_request; urllib.request Restrictions
  - url: https://pkg.go.dev/net#Dialer
    title: Go standard library, package net, type Dialer
    locator: the Control field
  - url: https://pkg.go.dev/net/http#Client
    title: Go standard library, package net/http, type Client
    locator: the CheckRedirect field; ErrUseLastResponse
  - url: https://developer.mozilla.org/en-US/docs/Web/API/RequestInit
    title: MDN Web Docs, RequestInit
    locator: redirect
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.net.http/java/net/http/HttpClient.Builder.html
    title: Java SE 25 API, HttpClient.Builder
    locator: followRedirects; NO_PROXY
  - url: https://codeql.github.com/codeql-query-help/python/py-full-ssrf/
    title: CodeQL query help, Full server-side request forgery (py/full-ssrf)
    locator: the query description
  - url: https://docs.astral.sh/ruff/rules/suspicious-url-open-usage/
    title: Ruff rules, suspicious-url-open-usage (S310)
    locator: What it does; Why is this bad?
  - url: https://github.com/securego/gosec/blob/master/RULES.md
    title: gosec, rules
    locator: G107; G704
  - url: https://www.cve.org/CVERecord?id=CVE-2021-26855
    title: CVE-2021-26855, Microsoft Exchange Server
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-21973
    title: CVE-2021-21973, VMware vCenter Server
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2024-3095
    title: CVE-2024-3095, langchain Web Research Retriever
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2009-0037
    title: CVE-2009-0037, curl and libcurl
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2002-1484
    title: CVE-2002-1484, DB4Web
    locator: the description and the publication date
---

## Description

Many server features fetch a URL for the user. A link preview reads a page to show its title. An import reads a file from a URL. A webhook sends an event to an address that the user gave. In each case, the server makes a network request, and part of the destination comes from outside input. Server-side request forgery (SSRF) happens when the server does not make sure that the request goes to an expected destination.[^1]

The weakness is about where the request comes from. The request leaves from the server, not from the attacker. The server can often reach hosts that the attacker cannot reach. Examples are services on the loopback address, hosts on the internal network, and administration ports behind a firewall. The server becomes a proxy for the attacker, and its requests can pass a firewall that stops the attacker.[^1]

An attacker can use SSRF to read internal pages and data, and to scan internal hosts for open ports. The requests can also bypass a firewall. When the client accepts other URL schemes, such as `file:`, the same input can read local files.[^1] What the attacker gets depends on the services that the server can reach.

CWE-918 names this weakness. It is a base weakness and a child of CWE-441, unintended proxy or intermediary. Its alternate terms are SSRF and XSPA, cross site port attack.[^1] It is rank 22 of the 2025 CWE Top 25, down from rank 19 in 2024.[^3] The OWASP Top 10:2021 had SSRF as its own category, A10:2021, which came from the community survey.[^5] In the OWASP Top 10:2025, it is part of A01:2025 Broken Access Control.[^2]

### History

Alexander Polyakov and Dmitry Chastukhin gave a talk on SSRF at Black Hat USA 2012. Their slides say that SSRF was discussed in 2008, with very little information about theory and practical examples. The first item on their list of earlier research was Deral Heiland's talk at ShmooCon 2008. That talk showed that web portlets, which load files from other HTTP sources, can attack the internal network through a URL parameter.[^4] CWE added CWE-918 in version 2.4 in February 2013, and its references include both talks.[^1]

### Related weaknesses

An XML parser that resolves external entities can make a request to a URL in the document. This is XML external entity injection, CWE-611, and CWE-918 lists it as a related weakness.[^1] A file name that reaches a file open call is [[path-traversal]], not SSRF. A server that runs a shell command such as `curl` with the URL in the command text can also have [[os-command-injection]].

## Vulnerable code

The function below shows a preview of any URL that the user sends. The program starts a small HTTP server on 127.0.0.1 in a thread. It stands for an internal service that only the local network can reach. The classic demonstration input is a URL on the loopback address, such as `http://127.0.0.1/admin`.

```python wrong id=fetch-any-url
import http.server
import pathlib
import tempfile
import threading
import urllib.request


class InternalAdmin(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"internal admin page: 3 users, debug mode on")

    def log_message(self, *args):
        pass


# A service that only the local network should reach.
internal = http.server.HTTPServer(("127.0.0.1", 0), InternalAdmin)
threading.Thread(target=internal.serve_forever, daemon=True).start()
port = internal.server_address[1]

# No proxy, so every request in this example stays on this machine.
opener = urllib.request.build_opener(urllib.request.ProxyHandler({}))


def preview(url):
    # The feature: show a preview of a page that the user names.
    with opener.open(url, timeout=2) as response:
        return response.read(200).decode()


print(preview(f"http://127.0.0.1:{port}/admin"))

with tempfile.TemporaryDirectory() as tmp:
    settings = pathlib.Path(tmp) / "settings.ini"
    settings.write_text("api_key = placeholder-not-a-real-key\n")
    print(preview(settings.as_uri()))
```

```output for=fetch-any-url
internal admin page: 3 users, debug mode on
api_key = placeholder-not-a-real-key
```

The first call returns the internal page. The second call reads a local file, because `urllib.request` also opens `file:` URLs.[^7] The user saw data that the feature was never meant to show.

## Fixed code

The fix decides the destination on the server. The OWASP cheat sheet separates two cases.[^6] In the first case, the application talks only to known hosts, such as one media host or one partner API. Then the server keeps an allow-list of destinations and refuses all others. In the second case, the application must reach any public address, as a link preview does. Then the server must refuse internal addresses, and it must check the address that it really connects to.

### An allow-list of destinations

The code below parses the URL and compares the scheme, the host and the port with an allow-list. It does not follow redirects, because a redirect from an allowed host can point to any other host.

```python run id=allow-list
import http.server
import threading
import urllib.error
import urllib.parse
import urllib.request


class Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/cat.txt":
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b"a picture of a cat")
        elif self.path == "/moved":
            self.send_response(302)
            self.send_header("Location", f"http://127.0.0.1:{admin_port}/admin")
            self.end_headers()
        else:
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b"internal admin page: 3 users, debug mode on")

    def log_message(self, *args):
        pass


def start():
    server = http.server.HTTPServer(("127.0.0.1", 0), Handler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server.server_address[1]


media_port = start()  # stands for a public media host
admin_port = start()  # stands for an internal service

# The operator chooses the destinations. Here both servers run on 127.0.0.1,
# so the port tells them apart. In production, the host names do.
ALLOWED = {("http", "127.0.0.1", media_port)}


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        raise urllib.error.HTTPError(req.full_url, code, "redirect refused", headers, fp)


opener = urllib.request.build_opener(urllib.request.ProxyHandler({}), NoRedirect)


def preview(url):
    parts = urllib.parse.urlsplit(url)
    if (parts.scheme, parts.hostname, parts.port) not in ALLOWED:
        return "refused: destination not allowed"
    try:
        with opener.open(url, timeout=2) as response:
            return response.read(200).decode()
    except urllib.error.HTTPError as error:
        return f"refused: upstream answered {error.code}"


print(preview(f"http://127.0.0.1:{media_port}/cat.txt"))
print(preview(f"http://127.0.0.1:{admin_port}/admin"))
print(preview("file:///tmp/settings.ini"))
print(preview(f"http://127.0.0.1:{media_port}/moved"))
```

```output for=allow-list
a picture of a cat
refused: destination not allowed
refused: destination not allowed
refused: upstream answered 302
```

The allowed URL works. The internal URL and the `file:` URL fail the check before any request. The redirect from the allowed host stops at the first response. A method of `HTTPRedirectHandler` that raises `HTTPError` stops the redirect.[^7]

### Compare parsed values, not strings

The check compares the parts of a parsed URL. A check with `startswith` or with a regular expression on the URL text is weak, because one URL has many spellings. The cheat sheet says to validate addresses and domain names with a vetted parser, and to compare its output.[^6] When possible, do not accept a full URL at all. Take an identifier, and build the URL on the server with a fixed scheme, host, port and path.[^6]

### Check the address at connect time

When the destination can be any public host, an allow-list of hosts is not possible. The server must then refuse internal addresses. A check of the host name before the request is not enough. The name can resolve to an internal address. Also, a second DNS lookup between the check and the connection can return a different address.[^6] The check must run on the address that the socket connects to. In Go, the `Control` function of a `net.Dialer` runs after the program creates the connection and before it dials, with the resolved address.[^8] The Go example file uses it. A denylist of address ranges is a weaker defence than an allow-list, and CWE rates it Moderate.[^1]

### Redirects and schemes

HTTP clients follow redirects by default in many languages. In Go, the default policy of `http.Client` stops after 10 requests, and `CheckRedirect` can return `ErrUseLastResponse` to stop at the first response.[^9] The `fetch` function takes `redirect: "error"`, and the default is `follow`.[^10] The Java `HttpClient` uses the policy `NEVER` when the program does not choose another.[^11] Allow only the schemes that the feature needs, usually `https`.

## How to detect it

- **Static analysis with taint tracking.** CodeQL follows input from a request to the URL of an outgoing request. Its queries include `py/full-ssrf`, `py/partial-ssrf`, `js/request-forgery`, `go/request-forgery` and `java/ssrf`.[^12] CWE rates static analysis as highly effective for this weakness.[^1]
- **Pattern linters.** Ruff rule S310 flags `urlopen` calls that can open schemes such as `file:`.[^13] gosec rule G107 flags a URL for an HTTP request that comes from tainted input, and rule G704 finds SSRF with taint analysis.[^14]
- **Code review.** Find every place where the server makes a request: HTTP clients, XML parsers, image and PDF converters, and webhook senders. For each one, ask where the destination comes from.
- **Tests.** Send a URL on the loopback address, such as `http://127.0.0.1/`, to each input that takes a URL. Also send a URL to a test server that answers with a redirect. The feature must refuse both.
- **Network logs.** Outgoing requests from an application server to internal addresses, or to ports that the feature never uses, show a possible SSRF.

## How to prevent it

- **Do not take a URL when an identifier is enough.** Map the identifier to a destination on the server.[^6]
- **Allow-list destinations.** Compare the scheme, host and port of the parsed URL with a fixed list.[^6]
- **Check the connected address.** When any public host is allowed, refuse loopback, private and link-local addresses at connect time, after DNS resolution.[^6]
- **Turn off redirects,** or check each step of a redirect with the same rules.
- **Allow only the schemes you need.** Most features need only `https`.
- **Segregate the network.** Put the services that fetch URLs in a network segment that cannot reach internal hosts. The cheat sheet calls network segregation highly recommended, and it works even when the code check has a bug.[^6]
- **Do not return the raw response.** Show the user only what the feature needs, such as a title or an image. Keep upstream error messages out of the response.
- **Require authentication on internal services.** A service that trusts every request from the internal network turns each SSRF into a full compromise.

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2002-1484 | published 2003-03-18 | The DB4Web server, with verbose debug messages, was a proxy for TCP connections to other systems. Remote attackers could use it for a port scan.[^19] |
| CVE-2009-0037 | published 2009-03-05 | The redirect code of curl and libcurl 5.11 to 7.19.3 accepted any `Location` value when `CURLOPT_FOLLOWLOCATION` was on. A server could redirect the client to intranet hosts, or to `file:` and `scp:` URLs.[^18] |
| CVE-2021-21973 | published 2021-02-24 | An SSRF in a vCenter Server plugin of the vSphere Client came from improper validation of URLs.[^16] |
| CVE-2021-26855 | published 2021-03-02 | CWE-918 lists this Microsoft Exchange Server vulnerability as an SSRF in a mail server that was exploited in the wild. The CVE record names it a remote code execution vulnerability.[^1][^15] |
| CVE-2024-3095 | published 2024-06-06 | The Web Research Retriever of langchain 0.1.5 did not restrict its requests to remote internet addresses. It could reach local addresses.[^17] |

## References

---
tag: RGR5K5
kind: feature
owner: python
slug: http-in-the-stdlib
name: HTTP in the stdlib
aliases: [urllib.request.Request, http.client, http.server, urllib.parse, HTTPStatus, BaseHTTPRequestHandler, Python HTTP client]
area: languages/python/io-and-os
level: core
summary: >-
  urllib.request sends HTTP requests, http.client is the protocol layer under it, http.server builds simple servers and urllib.parse splits and builds URLs.
grammar: "urllib.request.Request(url, data=None, headers={}, origin_req_host=None, unverifiable=False, method=None)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/urllib.request.html", locator: "urllib.request module"}
see_also: [python/internet-access, python/sockets, python/saving-structured-data-with-json, python/text-and-binary-io]
sources:
  - url: https://docs.python.org/3.15/library/urllib.request.html
    title: urllib.request, Python 3.15 documentation
    locator: module introduction; urlopen(); Request; HTTPDefaultErrorHandler
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/http.client.html
    title: http.client, Python 3.15 documentation
    locator: module introduction; HTTPConnection; HTTPConnection.request(); getresponse(); HTTPResponse
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/http.server.html
    title: http.server, Python 3.15 documentation
    locator: module introduction; HTTPServer; ThreadingHTTPServer; BaseHTTPRequestHandler
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/urllib.parse.html
    title: urllib.parse, Python 3.15 documentation
    locator: module introduction; urlsplit(); parse_qs(); urljoin(); quote(); urlencode()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/http.html
    title: http, Python 3.15 documentation
    locator: http.HTTPStatus
    accessed: 2026-11-16
  - url: https://pkg.go.dev/net/http
    title: Package http, Go standard library
    locator: Overview; func Get
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import json
import threading
import urllib.parse
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

class API(BaseHTTPRequestHandler):
    def do_GET(self):
        query = urllib.parse.parse_qs(urllib.parse.urlsplit(self.path).query)
        self.reply({'hello': query.get('name', ['world'])[0]})

    def do_POST(self):
        size = int(self.headers['Content-Length'])
        item = json.loads(self.rfile.read(size))
        self.reply({'created': item['title']}, status=201)

    def reply(self, obj, status=200):
        body = json.dumps(obj).encode()
        self.send_response(status)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Content-Length', str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format, *args):
        pass

server = ThreadingHTTPServer(('127.0.0.1', 0), API)
threading.Thread(target=server.serve_forever, daemon=True).start()
base = f'http://127.0.0.1:{server.server_port}'

query = urllib.parse.urlencode({'name': 'Ada Lovelace'})
with urllib.request.urlopen(f'{base}/greet?{query}') as resp:
    print(resp.status, json.load(resp))

req = urllib.request.Request(
    f'{base}/items', method='POST',
    data=json.dumps({'title': 'buy milk'}).encode(),
    headers={'Content-Type': 'application/json'})
with urllib.request.urlopen(req) as resp:
    print(resp.status, resp.headers['Content-Type'], json.load(resp))
server.shutdown()
```

```output for=synopsis
200 {'hello': 'Ada Lovelace'}
201 application/json {'created': 'buy milk'}
```

The program runs a small JSON server on a free local port and calls it twice with urllib.request: a GET with a query string and a POST with a JSON body.

## Semantics

**r1.** urlopen() "always returns an object which can work as a context manager and has the properties url, headers, and status."[^1] To set the method, headers or body, pass a `Request`, which "is an abstraction of a URL request."[^1] Its method "is 'GET' if data is None or 'POST' otherwise."[^1]

**r2.** The request body is bytes or a stream of bytes: "The supported object types include bytes, file-like objects, and iterables of bytes-like objects."[^1] Example: [`str-data`](#str-data).

**r3.** urlopen() turns error statuses into exceptions. `HTTPDefaultErrorHandler` is "a default handler for HTTP error responses; all responses are turned into HTTPError exceptions."[^1]

**r4.** http.client defines "classes that implement the client side of the HTTP and HTTPS protocols. It is normally not used directly — the module urllib.request uses it to handle URLs that use HTTP and HTTPS."[^2] "An HTTPConnection instance represents one transaction with an HTTP server."[^2] `request()` sends the request, and `getresponse()` "Should be called after a request is sent to get the response from the server."[^2] Example: [`client`](#client).

**r5.** http.server "defines classes for implementing HTTP servers."[^3] A `BaseHTTPRequestHandler` subclass handles each method: "for the request method SPAM, the do_SPAM() method will be called with no arguments."[^3] It reads the body from `rfile` and writes the reply to `wfile`. After `send_header()` calls, "end_headers() MUST BE called in order to complete the operation."[^3]

**r6.** urllib.parse breaks URL strings "up in components (addressing scheme, network location, path etc.), to combine the components back into a URL string, and to convert a “relative URL” to an absolute URL given a “base URL.”"[^4] `parse_qs()` returns a dictionary whose "values are lists of values for each name."[^4] `urlencode()` converts a mapping "to a percent-encoded ASCII text string."[^4] Example: [`parse`](#parse).

**r7.** `http.HTTPStatus` is "A subclass of enum.IntEnum that defines a set of HTTP status codes, reason phrases and long descriptions written in English."[^5] Example: [`status`](#status).

**r8.** The stdlib clients are low level. Both the urllib.request and the http.client pages say: "The Requests package is recommended for a higher-level HTTP client interface."[^1][^2]

## Examples

```python run id=client
import http.client
import threading
from http.server import BaseHTTPRequestHandler, HTTPServer

class Missing(BaseHTTPRequestHandler):
    def do_GET(self):
        body = b'nothing here\n'
        self.send_response(404)
        self.send_header('Content-Length', str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format, *args):
        pass

server = HTTPServer(('127.0.0.1', 0), Missing)
threading.Thread(target=server.serve_forever, daemon=True).start()

conn = http.client.HTTPConnection('127.0.0.1', server.server_port, timeout=5)
conn.request('GET', '/report')
resp = conn.getresponse()
print(resp.status, resp.reason)
print(resp.getheader('Content-Length'), resp.read())
conn.close()
server.shutdown()
```

```output for=client
404 Not Found
13 b'nothing here\n'
```

http.client hands back the 404 as a normal response, where urlopen() would raise HTTPError (rule r3). `status` is the "Status code returned by server." and `reason` is the "Reason phrase returned by server."[^2]

```python run id=parse
from urllib.parse import parse_qs, quote, urlencode, urljoin, urlsplit

parts = urlsplit('https://example.com:8443/docs/page?tag=io&tag=os#top')
print(parts.scheme, parts.hostname, parts.port, parts.path)
print(parse_qs(parts.query), parts.fragment)
print(urlencode({'q': 'a&b c', 'page': 2}))
print(quote('/files/my report.pdf'))
print(urljoin('https://example.com/docs/page', '../img/logo.png'))
```

```output for=parse
https example.com 8443 /docs/page
{'tag': ['io', 'os']} top
q=a%26b+c&page=2
/files/my%20report.pdf
https://example.com/img/logo.png
```

`urlsplit()` follows "the general structure of a URL: scheme://netloc/path?query#fragment."[^4] `quote()` replaces "special characters in string using the %xx escape", and by default it "is intended for quoting the path section of a URL."[^4] Build query strings with `urlencode()`, not by joining strings, so `&` and spaces in values stay inside their value.

```python run id=status
from http import HTTPStatus

print(HTTPStatus.NOT_FOUND == 404, HTTPStatus(404).phrase)
print(HTTPStatus.CREATED.value, HTTPStatus(201).is_success)
```

```output for=status
True Not Found
201 True
```

`is_success` means "200 <= status <= 299".[^5]

## Pitfalls

The body must be bytes. A `str` body raises TypeError:

```python fails id=str-data expect=exception:TypeError
import urllib.request

urllib.request.urlopen('http://127.0.0.1:9/', data='name=ada')
```

Encode the text first, for example with `urlencode(...).encode('ascii')` or `json.dumps(...).encode()` (rule r2).

urlopen() raises HTTPError for a 404 or a 500, so wrap calls in `try` when an error status is a normal answer (rule r3). http.client does not raise for these.

Do not put http.server on the open internet. "http.server is not recommended for production. It only implements basic security checks."[^3]

`BaseHTTPRequestHandler` logs each request to standard error. `log_message()` "Logs an arbitrary message to sys.stderr. This is typically overridden to create custom error logging mechanisms."[^3] The examples override it to keep the output clean.

## Comparison

"Package http provides HTTP client and server implementations."[^6] Go puts the client and the server in one package, where Python splits them over urllib.request, http.client and http.server. Go's `http.Get` does not raise for an error status: "A non-2xx response doesn't cause an error."[^6] Python's urlopen() raises HTTPError instead (rule r3).

---
tag: 03FFWB
kind: feature
owner: python
slug: http-and-networking-libraries
name: HTTP and networking libraries
aliases: [urllib.request, http.client, http.server, socket, Requests, urllib3, HTTPX, aiohttp, HTTP client]
area: languages/python/ecosystem
level: core
summary: >-
  The standard library has socket, http.client, http.server and urllib.request. Requests, urllib3, HTTPX and aiohttp are the common HTTP libraries on PyPI. Set a timeout on each request.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/library/urllib.request.html", locator: "urllib.request, Extensible library for opening URLs"}
see_also: [python/web-libraries, python/the-python-package-index]
sources:
  - url: https://docs.python.org/3/library/urllib.request.html
    title: urllib.request, Extensible library for opening URLs
    locator: introduction; urlopen
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/http.client.html
    title: http.client, HTTP protocol client
    locator: introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/http.server.html
    title: http.server, HTTP servers
    locator: introduction; HTTPServer
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/socket.html
    title: socket, Low-level networking interface
    locator: introduction; getdefaulttimeout
    accessed: 2026-11-16
  - url: https://requests.readthedocs.io/en/latest/
    title: "Requests: HTTP for Humans"
    locator: home page; Beloved Features
    accessed: 2026-11-16
  - url: https://urllib3.readthedocs.io/en/stable/
    title: urllib3
    locator: home page
    accessed: 2026-11-16
  - url: https://www.python-httpx.org/
    title: HTTPX
    locator: home page
    accessed: 2026-11-16
  - url: https://docs.aiohttp.org/en/stable/
    title: Welcome to AIOHTTP
    locator: Key Features
    accessed: 2026-11-16
  - url: https://lp.jetbrains.com/python-developers-survey-2024/
    title: Python Developers Survey 2024 Results
    locator: Frameworks and Libraries, Web frameworks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/net/http
    title: Package net/http
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=local-server
import http.server
import json
import threading
import urllib.request

class Hello(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        body = json.dumps({'path': self.path}).encode()
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Content-Length', str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format, *args):
        pass

server = http.server.HTTPServer(('127.0.0.1', 0), Hello)
threading.Thread(target=server.serve_forever, daemon=True).start()
url = f'http://127.0.0.1:{server.server_port}/greet?name=Ada'

with urllib.request.urlopen(url, timeout=5) as resp:
    print(resp.status, resp.headers['Content-Type'])
    print(json.load(resp))
server.shutdown()
```

```output for=local-server
200 application/json
{'path': '/greet?name=Ada'}
```

The standard library has an HTTP client (`urllib.request` and `http.client`) and a small HTTP server (`http.server`). The example starts the server on the loopback address in a thread and fetches a URL from it. For a larger program, the documentation recommends the Requests package, and HTTPX and aiohttp add async APIs.

## Semantics

**r1.** "The urllib.request module defines functions and classes which help in opening URLs (mostly HTTP) in a complex world".[^1] "The Requests package is recommended for a higher-level HTTP client interface."[^1] Example: [`local-server`](#local-server).

**r2.** `http.client` "defines classes that implement the client side of the HTTP and HTTPS protocols. It is normally not used directly".[^2] The module `urllib.request` uses it for the URLs that use HTTP and HTTPS.[^2] Example: [`http-client`](#http-client).

**r3.** "http.server is not recommended for production. It only implements basic security checks."[^3] Its class `HTTPServer` "builds on the TCPServer class by storing the server address as instance variables named server_name and server_port."[^3]

**r4.** "This module provides access to the BSD socket interface."[^4] HTTP/1 is text on a socket. Example: [`raw-socket`](#raw-socket).

**r5.** The timeout of `urlopen` is "a timeout in seconds for blocking operations like the connection attempt (if not specified, the global default timeout setting will be used)."[^1] For that default, "A value of None indicates that new socket objects have no timeout. When the socket module is first imported, the default is None."[^4] Example: [`timeout`](#timeout).

**r6.** "Requests is an elegant and simple HTTP library for Python, built for human beings."[^5] In Requests, "Keep-alive and HTTP connection pooling are 100% automatic, thanks to urllib3."[^5]

**r7.** "urllib3 is a powerful, user-friendly HTTP client for Python."[^6] It "brings many critical features that are missing from the Python standard libraries".[^6]

**r8.** "HTTPX is a fully featured HTTP client for Python 3, which provides sync and async APIs, and support for both HTTP/1.1 and HTTP/2."[^7]

**r9.** aiohttp is an "Asynchronous HTTP Client/Server for asyncio and Python."[^8] It "Supports both Client and HTTP Server."[^8]

**r10.** The table of web frameworks in the Python Developers Survey 2024 also lists HTTP libraries. Its 2024 column gives Requests 33%, httpx 15% and aiohttp 13%.[^9] "Percentages are calculated within each column."[^9]

## Examples

`http.client` gives the status, the reason and each header of a response (rule r2):

```python run id=http-client
import http.client
import http.server
import threading

class NotFound(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_error(404)

    def log_message(self, format, *args):
        pass

server = http.server.HTTPServer(('127.0.0.1', 0), NotFound)
threading.Thread(target=server.serve_forever, daemon=True).start()

conn = http.client.HTTPConnection('127.0.0.1', server.server_port, timeout=5)
conn.request('GET', '/missing')
resp = conn.getresponse()
print(resp.status, resp.reason)
print(resp.getheader('Content-Type'))
resp.read()
conn.close()
server.shutdown()
```

```output for=http-client
404 Not Found
text/html;charset=utf-8
```

An HTTP/1.0 request is text on a socket (rule r4):

```python run id=raw-socket
import http.server
import socket
import threading

class Hello(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'hello')

    def log_message(self, format, *args):
        pass

server = http.server.HTTPServer(('127.0.0.1', 0), Hello)
threading.Thread(target=server.serve_forever, daemon=True).start()

with socket.create_connection(('127.0.0.1', server.server_port), timeout=5) as s:
    s.sendall(b'GET / HTTP/1.0\r\nHost: localhost\r\n\r\n')
    data = b''
    while chunk := s.recv(4096):
        data += chunk
head, _, body = data.partition(b'\r\n\r\n')
print(head.split(b'\r\n')[0])
print(body)
server.shutdown()
```

```output for=raw-socket
b'HTTP/1.0 200 OK'
b'hello'
```

The default timeout is `None`, so a request without a timeout can wait forever (rule r5):

```python run id=timeout
import socket

print(socket.getdefaulttimeout())
```

```output for=timeout
None
```

## Pitfalls

Give a timeout to each request (rule r5). With the default timeout of `None`, a program can wait forever for a server that does not answer. Each example on this page gives `timeout=5`.

Do not use `http.server` in production (rule r3).

`urlopen` and `http.client` act differently on an error status such as 404. In `urllib.request`, `HTTPDefaultErrorHandler` is "A class which defines a default handler for HTTP error responses; all responses are turned into HTTPError exceptions."[^1] `http.client` returns the response with the status, as in the example [`http-client`](#http-client).

For async code, use a library with an async API, such as HTTPX or aiohttp (rules r8 and r9).

## Comparison

"Package http provides HTTP client and server implementations."[^10] "Get, Head, Post, and PostForm make HTTP (or HTTPS) requests".[^10] Go has one standard package for the client and the server. In Python, the client is in `urllib.request` and `http.client`, the server is in `http.server`, and the documentation recommends Requests for a higher-level client (rule r1).

[^1]: [urllib.request, Extensible library for opening URLs](https://docs.python.org/3/library/urllib.request.html)
[^2]: [http.client, HTTP protocol client](https://docs.python.org/3/library/http.client.html)
[^3]: [http.server, HTTP servers](https://docs.python.org/3/library/http.server.html)
[^4]: [socket, Low-level networking interface](https://docs.python.org/3/library/socket.html)
[^5]: [Requests: HTTP for Humans](https://requests.readthedocs.io/en/latest/)
[^6]: [urllib3](https://urllib3.readthedocs.io/en/stable/)
[^7]: [HTTPX](https://www.python-httpx.org/)
[^8]: [Welcome to AIOHTTP](https://docs.aiohttp.org/en/stable/)
[^9]: [Python Developers Survey 2024 Results](https://lp.jetbrains.com/python-developers-survey-2024/)
[^10]: [Package net/http](https://pkg.go.dev/net/http)

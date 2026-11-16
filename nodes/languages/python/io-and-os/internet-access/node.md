---
tag: E0H9TJ
kind: feature
owner: python
slug: internet-access
name: Internet Access
aliases: [urllib.request, urlopen, smtplib, send mail from Python, fetch a URL, HTTPError, URLError]
area: languages/python/io-and-os
level: core
summary: >-
  urllib.request.urlopen() fetches a URL and returns a response that reads as bytes. smtplib sends mail to an SMTP server. An HTTP error status raises HTTPError.
grammar: "urllib.request.urlopen(url, data=None, [timeout, ]*, context=None)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/urllib.request.html#urllib.request.urlopen", locator: "urllib.request.urlopen()"}
see_also: [python/http-in-the-stdlib, python/sockets, python/saving-structured-data-with-json, python/text-and-binary-io]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#internet-access
    title: The Python Tutorial, 10. Brief tour of the standard library
    locator: 10.7. Internet access
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/urllib.request.html
    title: urllib.request, Python 3.15 documentation
    locator: urllib.request.urlopen(); HTTPDefaultErrorHandler
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/urllib.error.html
    title: urllib.error, Python 3.15 documentation
    locator: URLError; HTTPError
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/smtplib.html
    title: smtplib, Python 3.15 documentation
    locator: module introduction; SMTP; SMTP.starttls(); SMTP.send_message()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/socket.html#socket.getdefaulttimeout
    title: socket, Python 3.15 documentation
    locator: socket.getdefaulttimeout()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/net/http
    title: Package http, Go standard library
    locator: Overview; func Get
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import threading
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.request import urlopen

class Page(BaseHTTPRequestHandler):
    def do_GET(self):
        body = b'line one\nupdated today\nline three\n'
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain; charset=utf-8')
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, *args):
        pass

server = HTTPServer(('127.0.0.1', 0), Page)
threading.Thread(target=server.serve_forever, daemon=True).start()
url = f'http://127.0.0.1:{server.server_port}/'

with urlopen(url) as response:
    print(response.status, response.headers['Content-Type'])
    for line in response:
        line = line.decode()
        if 'updated' in line:
            print(line.rstrip())
server.shutdown()
```

```output for=synopsis
200 text/plain; charset=utf-8
updated today
```

The first half starts a small web server on this machine, so the example needs no network. The `with urlopen(...)` block is the client: it reads the page line by line, as bytes, and prints the lines that match.

## Semantics

**r1.** "There are a number of modules for accessing the internet and processing internet protocols. Two of the simplest are urllib.request for retrieving data from URLs and smtplib for sending mail".[^1]

**r2.** `urlopen()` will "Open url, which can be either a string containing a valid, properly encoded URL, or a Request object."[^2] It "always returns an object which can work as a context manager and has the properties url, headers, and status."[^2] The body reads as bytes, so text needs `decode()`.[^1]

**r3.** "data must be an object specifying additional data to be sent to the server, or None if no such data is needed."[^2] "The optional timeout parameter specifies a timeout in seconds for blocking operations like the connection attempt".[^2]

**r4.** For an error status, the default handler applies: "all responses are turned into HTTPError exceptions."[^2] An `HTTPError` is "a subclass of URLError", and it "can also function as a non-exceptional file-like return value".[^3] `URLError` is "a subclass of OSError."[^3] `urlopen()` "Raises URLError on protocol errors."[^2] Example: [`http-error`](#http-error).

**r5.** "The smtplib module defines an SMTP client session object that can be used to send mail to any internet machine with an SMTP or ESMTP listener daemon."[^4] "The SMTP class supports the with statement. When used like this, the SMTP QUIT command is issued automatically when the with statement exits."[^4] Example: [`smtp`](#smtp).

**r6.** `send_message()` takes an `email.message.Message`. "If from_addr is None or to_addrs is None, send_message fills those arguments with addresses extracted from the headers of msg".[^4]

**r7.** `starttls()` will "Put the SMTP connection in TLS (Transport Layer Security) mode. All SMTP commands that follow will be encrypted. You should then call ehlo() again."[^4]

## Examples

```python run id=http-error
import threading
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.error import HTTPError, URLError
from urllib.request import urlopen

class Missing(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_error(404, 'No such page')

    def log_message(self, *args):
        pass

server = HTTPServer(('127.0.0.1', 0), Missing)
threading.Thread(target=server.serve_forever, daemon=True).start()
port = server.server_port

try:
    urlopen(f'http://127.0.0.1:{port}/nope', timeout=5)
except HTTPError as err:
    print(err.code, err.reason, isinstance(err, URLError))
server.shutdown()
server.server_close()

try:
    urlopen(f'http://127.0.0.1:{port}/', timeout=5)
except URLError as err:
    print(type(err.reason).__name__)
```

```output for=http-error
404 No such page True
ConnectionRefusedError
```

A 404 page raises `HTTPError` with the status code. When no server listens on the port, the error is a `URLError` whose `reason` is the `OSError` from the connection (rule r4).

```python run id=smtp
import smtplib
import socketserver
import threading
from email.message import EmailMessage

class FakeSMTP(socketserver.StreamRequestHandler):
    def reply(self, text):
        self.wfile.write(text.encode() + b'\r\n')

    def handle(self):
        self.reply('220 localhost ready')
        for raw in self.rfile:
            command = raw.decode().strip()
            verb = command.split()[0].upper()
            if verb == 'DATA':
                self.reply('354 end with a dot')
                lines = []
                for raw in self.rfile:
                    if raw == b'.\r\n':
                        break
                    lines.append(raw.decode().rstrip())
                print('server: message ends with', repr(lines[-1]))
                self.reply('250 queued')
            elif verb == 'QUIT':
                self.reply('221 bye')
                return
            else:
                print('server:', command)
                self.reply('250 ok')

server = socketserver.TCPServer(('127.0.0.1', 0), FakeSMTP)
threading.Thread(target=server.serve_forever, daemon=True).start()

msg = EmailMessage()
msg['From'] = 'soothsayer@example.org'
msg['To'] = 'jcaesar@example.org'
msg['Subject'] = 'A warning'
msg.set_content('Beware the Ides of March.')

with smtplib.SMTP('127.0.0.1', server.server_address[1],
                  local_hostname='client.example') as smtp:
    smtp.send_message(msg)
server.shutdown()
```

```output for=smtp
server: ehlo client.example
server: mail FROM:<soothsayer@example.org>
server: rcpt TO:<jcaesar@example.org>
server: message ends with 'Beware the Ides of March.'
```

The fake server prints each command that `smtplib` sends. `send_message()` takes the sender and the recipient from the `From` and `To` headers (rule r6), and the `with` block sends `QUIT` at the end (rule r5). A real program connects to its mail server instead.

## Pitfalls

An error status is an exception, not a return value:

```python fails id=no-handler expect=exception:HTTPError
import threading
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.request import urlopen

class Down(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_error(503)

    def log_message(self, *args):
        pass

server = HTTPServer(('127.0.0.1', 0), Down)
threading.Thread(target=server.serve_forever, daemon=True).start()
with urlopen(f'http://127.0.0.1:{server.server_port}/') as response:
    print(response.status)
```

Catch `HTTPError` before `URLError`, since it is a subclass (rule r4).

Without `timeout`, "the global default timeout setting will be used".[^2] For sockets, "A value of None indicates that new socket objects have no timeout. When the socket module is first imported, the default is None."[^5] So a server that stops answering can stall the call. Pass `timeout=` to every `urlopen()` in a long-running program (rule r3).

The body is bytes (rule r2). Decode it with the charset from the `Content-Type` header, not a guess.

The tutorial's mail example "needs a mailserver running on localhost."[^1] Real servers often require TLS and a login, so call `starttls()` before sending (rule r7).

## Comparison

Go's "Package http provides HTTP client and server implementations."[^6] `http.Get` differs on errors: "A non-2xx response doesn't cause an error."[^6] Python's `urlopen()` raises `HTTPError` for the same response (rule r4). In Go, "The caller must close the response body when finished with it",[^6] which a `with` block does in Python (rule r2).

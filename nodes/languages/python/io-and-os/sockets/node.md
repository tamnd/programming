---
tag: J8QB9X
kind: feature
owner: python
slug: sockets
name: Sockets
aliases: [socket module, TCP socket, UDP socket, socketserver, selectors, ssl.wrap_socket, create_connection, create_server]
area: languages/python/io-and-os
level: advanced
summary: >-
  The socket module is the BSD socket interface: TCP and UDP sockets that send and receive bytes. socketserver builds servers on it, selectors waits on many sockets and ssl adds TLS.
grammar: "socket.socket(family=AF_INET, type=SOCK_STREAM, proto=0, fileno=None)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/socket.html", locator: "socket module"}
see_also: [python/internet-access, python/http-in-the-stdlib, python/binary-data-record-layouts, python/subprocess, python/signals]
sources:
  - url: https://docs.python.org/3.15/library/socket.html
    title: socket, Python 3.15 documentation
    locator: module introduction; create_connection(); create_server(); socketpair(); socket objects; notes on socket timeouts; socket.timeout
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/sockets.html
    title: Socket Programming HOWTO, Python 3.15 documentation
    locator: Using a socket
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/socketserver.html
    title: socketserver, Python 3.15 documentation
    locator: module introduction; server creation notes; serve_forever(); shutdown(); StreamRequestHandler
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/selectors.html
    title: selectors, Python 3.15 documentation
    locator: Introduction; DefaultSelector; BaseSelector.register(); BaseSelector.select()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/ssl.html
    title: ssl, Python 3.15 documentation
    locator: module introduction; socket creation; create_default_context(); PROTOCOL_TLS_CLIENT; SSLContext.check_hostname
    accessed: 2026-11-16
  - url: https://pkg.go.dev/net
    title: Package net, Go standard library
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import socket
import threading

server = socket.create_server(('127.0.0.1', 0))
address = server.getsockname()

def serve():
    conn, peer = server.accept()
    with conn:
        while data := conn.recv(1024):
            conn.sendall(data.upper())

worker = threading.Thread(target=serve)
worker.start()
with socket.create_connection(address) as client:
    client.sendall(b'hello, socket')
    client.shutdown(socket.SHUT_WR)
    reply = b''
    while chunk := client.recv(1024):
        reply += chunk
print(reply)
worker.join()
server.close()
```

```output for=synopsis
b'HELLO, SOCKET'
```

The server listens on a free port of the loopback address. The client sends its bytes, shuts down its sending side and reads until the server closes the connection.

## Semantics

**r1.** "This module provides access to the BSD socket interface."[^1] A socket object has methods that "correspond to Unix system calls applicable to sockets."[^1] `socket.socket()` takes an address family and a socket type. Of the types, "Only SOCK_STREAM and SOCK_DGRAM appear to be generally useful."[^1] The HOWTO talks about "STREAM (i.e. TCP) sockets".[^2] For UDP, the socketserver docs say the server "uses datagrams".[^3]

**r2.** `socket.create_server()` is a "Convenience function which creates a TCP socket bound to address (a 2-tuple (host, port)) and returns the socket object."[^1] `accept()` returns "a pair (conn, address) where conn is a new socket object usable to send and receive data on the connection".[^1] The server reads and writes on `conn`, not on the listening socket.[^1] Port 0 asks the system for "an arbitrary unused port".[^3]

**r3.** `socket.create_connection()` connects "to a TCP service listening on the internet address (a 2-tuple (host, port)), and return the socket object."[^1] For a host name, "it will try to resolve it for both AF_INET and AF_INET6, and then try to connect to all possible addresses in turn until a connection succeeds."[^1]

**r4.** Sockets carry bytes, not text. `recv(bufsize)` returns "a bytes object representing the data received."[^1] "A returned empty bytes object indicates that the client has disconnected."[^1] `send()` "Returns the number of bytes sent. Applications are responsible for checking that all data has been sent".[^1] `sendall()` "continues to send data from bytes until either all data has been sent or an error occurs."[^1]

**r5.** A stream has no message boundaries. The HOWTO says: "messages must either be fixed length (yuck), or be delimited (shrug), or indicate how long they are (much better), or end by shutting down the connection."[^2] Example: [`no-boundaries`](#no-boundaries).

**r6.** "A socket object can be in one of three modes: blocking, non-blocking, or timeout."[^1] With a non-zero `settimeout()` value, "subsequent socket operations will raise a timeout exception if the timeout period value has elapsed before the operation has completed."[^1] `socket.timeout` is "A deprecated alias of TimeoutError."[^1]

**r7.** Socket objects work in a `with` statement: "Exiting the context manager is equivalent to calling close()."[^1] `close()` "releases the resource associated with a connection but does not necessarily close the connection immediately. If you want to close the connection in a timely fashion, call shutdown() before close()."[^1]

**r8.** "The socketserver module simplifies the task of writing network servers."[^3] A server subclasses a request handler, overrides `handle()`, and passes the handler class to a server class.[^3] `StreamRequestHandler` provides "rfile and wfile attributes".[^3] `serve_forever()` handles "requests until an explicit shutdown() request."[^3] Example: [`socketserver`](#socketserver).

**r9.** The selectors module "allows high-level and efficient I/O multiplexing, built upon the select module primitives."[^4] `DefaultSelector` "is an alias to the most efficient implementation available on the current platform".[^4] `select()` returns "a list of (key, events) tuples, one for each ready file object."[^4] Example: [`selectors`](#selectors).

**r10.** The ssl module "provides access to Transport Layer Security (often known as “Secure Sockets Layer”) encryption and peer authentication facilities for network sockets".[^5] "Instances of SSLSocket must be created using the SSLContext.wrap_socket() method. The helper function create_default_context() returns a new context with secure default settings."[^5] Example: [`ssl`](#ssl).

## Examples

```python run id=udp
import socket

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as rx, \
     socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as tx:
    rx.bind(('127.0.0.1', 0))
    tx.bind(('127.0.0.1', 0))
    tx.sendto(b'ping', rx.getsockname())
    tx.sendto(b'pong', rx.getsockname())
    for _ in range(2):
        data, sender = rx.recvfrom(1024)
        print(data, sender == tx.getsockname())
```

```output for=udp
b'ping' True
b'pong' True
```

UDP needs no connection. `recvfrom()` returns "a pair (bytes, address)".[^1] Datagrams are "discrete packets of information that may arrive out of order or be lost while in transit."[^3]

```python run id=socketserver
import socket
import socketserver
import threading

class Upper(socketserver.StreamRequestHandler):
    def handle(self):
        for line in self.rfile:
            self.wfile.write(line.upper())

with socketserver.ThreadingTCPServer(('127.0.0.1', 0), Upper) as server:
    threading.Thread(target=server.serve_forever, daemon=True).start()
    with socket.create_connection(server.server_address) as s:
        f = s.makefile('rwb')
        for word in [b'one\n', b'two\n']:
            f.write(word)
            f.flush()
            print(f.readline())
        f.close()
    server.shutdown()
```

```output for=socketserver
b'ONE\n'
b'TWO\n'
```

The handler reads lines, so a newline marks the end of each message (rule r5). `makefile()` returns "a file object associated with the socket".[^1] The HOWTO warns that you "need to use flush on sockets", or "you may wait forever for the reply, because the request may still be in your output buffer."[^2] `shutdown()` "must be called while serve_forever() is running in a different thread otherwise it will deadlock."[^3]

```python run id=selectors
import selectors
import socket

sel = selectors.DefaultSelector()
a_out, a_in = socket.socketpair()
b_out, b_in = socket.socketpair()
sel.register(a_in, selectors.EVENT_READ, 'a')
sel.register(b_in, selectors.EVENT_READ, 'b')

b_out.sendall(b'data for b')
for key, events in sel.select(timeout=1):
    print(key.data, events == selectors.EVENT_READ, key.fileobj.recv(100))
print(sel.select(timeout=0))
```

```output for=selectors
b True b'data for b'
[]
```

`socketpair()` builds "a pair of connected socket objects".[^1] Only the `b` socket has data, so only its key comes back. "If timeout <= 0, the call won’t block, and will report the currently ready file objects."[^4]

```python run id=ssl
import ssl

ctx = ssl.create_default_context()
print(ctx.protocol == ssl.PROTOCOL_TLS_CLIENT)
print(ctx.verify_mode == ssl.CERT_REQUIRED, ctx.check_hostname)
```

```output for=ssl
True
True True
```

The default client context checks the server's certificate and host name. `PROTOCOL_TLS_CLIENT` "enables CERT_REQUIRED and check_hostname by default."[^5] A client wraps a connected socket with `ctx.wrap_socket(sock, server_hostname=host)`.[^5]

## Pitfalls

One `recv()` can return the bytes of two `sendall()` calls:

```python run id=no-boundaries
import socket

a, b = socket.socketpair()
a.sendall(b'first')
a.sendall(b'second')
print(b.recv(1024))
```

```output for=no-boundaries
b'firstsecond'
```

`send` and `recv` "do not necessarily handle all the bytes you hand them (or expect from them)".[^2] Give each message a delimiter or a length prefix (rule r5).

A `recv()` with no data waits forever unless the socket has a timeout. "Sockets are by default always created in blocking mode".[^1] With a timeout, the wait ends in TimeoutError:

```python fails id=timeout expect=exception:TimeoutError
import socket

a, b = socket.socketpair()
b.settimeout(0.2)
b.recv(1024)
```

The value of the exception "is currently always “timed out”."[^1]

`send()` can send less than the whole buffer (rule r4). Use `sendall()` unless the program checks the count.

The default context checks host names, so `wrap_socket()` needs the host name. For `check_hostname`, "you must pass server_hostname to wrap_socket() in order to match the hostname."[^5]

```python fails id=no-hostname expect=exception:ValueError
import socket
import ssl

ctx = ssl.create_default_context()
with socket.socket() as sock:
    ctx.wrap_socket(sock)
```

## Comparison

"Package net provides a portable interface for network I/O, including TCP/IP, UDP, domain name resolution, and Unix domain sockets."[^6] Go uses `net.Dial` and `net.Listen` where Python uses `create_connection()` and `create_server()`: "most clients will need only the basic interface provided by the Dial, Listen, and Accept functions and the associated Conn and Listener interfaces."[^6] Go's TLS sits in a separate package: "The crypto/tls package uses the same interfaces and similar Dial and Listen functions."[^6]

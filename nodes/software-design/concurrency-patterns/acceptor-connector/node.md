---
tag: 42HRJH
kind: pattern
slug: acceptor-connector
name: Acceptor-Connector
aliases: [acceptor connector, acceptor-connector pattern, acceptor and connector]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  Acceptor-Connector puts the code that sets up a connection in an acceptor or a connector, and the code that uses the connection in a service handler.
family: concurrency
polarity: positive
intent: Decouple the passive and the active set-up of connections, and the start of the service on them, from the processing that the service does after the connection is set up.
also_known_as: []
history:
  year: 1997
  people: [Douglas C. Schmidt]
  note: The paper appeared in Pattern Languages of Program Design 3 (1997). POSA2 (2000) put the pattern in its event handling patterns.
see_also: [reactor, proactor, asynchronous-completion-token, factory-method-pattern, template-method-pattern, strategy-pattern, event-driven-programming, threads]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/Acc-Con.pdf
    title: Douglas C. Schmidt, Acceptor-Connector, An Object Creational Pattern for Connecting and Initializing Communication Services
    locator: Intent; Structure; Known Uses; Consequences; See Also
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 3 Event Handling Patterns
  - url: https://docs.python.org/3/library/socketserver.html
    title: socketserver, A framework for network servers, Python documentation
    locator: Server Creation Notes
---

## Intent

The Acceptor-Connector design pattern decouples connection establishment and service initialization in a distributed system from the processing performed once a service is initialized.[^1] This decoupling is achieved with three components: acceptors, connectors, and service handlers.[^1]

## Problem

A program in a network both waits for connections and starts them. The code that listens, connects and builds the objects for a new connection is much the same in each program. The code that runs on the connection is different for each service. When both are in one function, a new service needs a copy of the set-up code, and a change to the set-up code touches each service.

## Solution

There are three roles.[^1]

- **Acceptor.** An Acceptor is a factory that implements the strategy for passively establishing a connection and initializing its associated Service Handler.[^1]
- **Connector.** A Connector is a factory that implements the strategy for actively establishing a connection and initializing its associated Service Handler.[^1] It provides another method that finishes activating Service Handlers whose connections were initiated either synchronously or asynchronously.[^1]
- **Service Handler.** A Service Handler implements an application service, typically playing the client role, server role, or both roles.[^1] It provides a hook method that is called by an Acceptor or Connector to activate the application service when the connection is established.[^1]

```text
 passive side                         active side
 Acceptor(addr, make_handler)         Connector.connect(addr, make_handler)
   listen(addr)  <------- connect --------+
   accept(): h = make_handler()           | complete(): h = make_handler()
             h.open(conn)                 |             h.open(conn)
                    \                    /
                 Service Handler <--> Service Handler
                      (application protocol only)
```

A dispatcher, such as a Reactor, tells the acceptor when a connection arrives and the connector when a connection completes.[^1]

## Example

The example files show one program in each language. A table of listeners stands in for the network. The acceptor listens on an address and makes a status handler for each connection. The connector connects to that address and makes a command handler. After the set-up, only the two handlers talk. The program prints:

```text
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

## Consequences

Benefits:[^1]

- **Reuse, portability and extension.** The application-independent mechanisms in the Acceptor and Connector are reusable components that know how to establish connections and initialize the associated Service Handler.[^1]
- **Local variation.** By parameterizing the Acceptor and Connector with a Service Handler, the impact of this variation is localized to a small number of components in the software.[^1]
- **Independent strategies.** The initialization strategy and the service handling strategy are decoupled, so each strategy can evolve independently.[^1]

Drawbacks:[^1]

- **Additional indirection.** The pattern can incur additional indirection compared with using the underlying network programming interfaces directly.[^1]
- **Additional complexity.** The pattern may add unnecessary complexity for simple client applications that connect with a single server and perform a single service.[^1]

The connector must activate its handler only when the connection is complete. A common mistake is to start the service when the connect call starts. With an asynchronous connect, the first message then goes nowhere:

```python wrong id=too-early
class Connection:
    def __init__(self):
        self.established = False
        self.delivered = []

    def send(self, message):
        if self.established:
            self.delivered.append(message)
        # before the connection is up, the message is lost


def open_handler(connection):
    connection.send("hello")


connection = Connection()
open_handler(connection)  # activated when the connect starts
connection.established = True  # the asynchronous connect completes later
print("delivered:", connection.delivered)
```

```output for=too-early
delivered: []
```

The fix is to call the hook method of the handler from the method that completes the connection.[^1]

## When not to use it

- The program is a small client that connects to one server for one service.[^1]
- A framework already does the set-up, for example a web framework that calls a request handler. The program then writes only the service handler.

## Variants and related patterns

- **Template Method and Factory Method.** The Acceptor's accept and the Connector's connect and complete functions are Template Methods.[^1] The Factory Method pattern is used to decouple the creation of a Service Handler from its subsequent use.[^1]
- **Reactor and Proactor.** They tell the acceptor and the connector when a connection is ready or done. POSA2 puts the four patterns in its event handling patterns.[^2]
- **Server frameworks.** In Python, `socketserver` asks for a request handler class with a `handle()` method, and a server class that is passed the server's address and the request handler class.[^3] The server is the acceptor and the handler class is the service handler. Here the client side is a plain connect:

```python run id=socketserver
import socket
import socketserver
import threading


class StatusHandler(socketserver.BaseRequestHandler):  # the service handler
    def handle(self):
        command = self.request.recv(100).decode()
        print("status: got", command)
        self.request.sendall(b"ok")


with socketserver.TCPServer(("127.0.0.1", 0), StatusHandler) as server:  # the acceptor
    worker = threading.Thread(target=server.handle_request)
    worker.start()
    with socket.create_connection(server.server_address) as client:  # the connector
        client.sendall(b"reset")
        print("command: reply", client.recv(100).decode())
    worker.join()
```

```output for=socketserver
status: got reset
command: reply ok
```

## Known uses

- **UNIX superservers.** inetd and similar daemons utilize a master Acceptor process that listens for connections on a set of communication ports.[^1] Each port is associated with a service such as ftp, telnet, daytime or echo.[^1]
- **ACE.** Implementations of the Service Handler, Connector, and Acceptor classes are provided as reusable components in the ACE object-oriented network programming framework.[^1]
- **CORBA ORBs.** The ORB Core layer in many implementations of CORBA uses the pattern to passively initialize server object implementations.[^1]
- **Web browsers.** Netscape and Internet Explorer use the asynchronous version of the connector component to establish connections with servers associated with images embedded in HTML pages.[^1]
- **Python socketserver.** The server class does the passive set-up, and the request handler class holds the service.[^3]

## References

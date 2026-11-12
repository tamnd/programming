```python run id=acceptor
class Channel:
    def __init__(self):
        self.messages = []


class StatusHandler:  # service handler on the passive side
    def open(self, channel):
        self.channel = channel
        print("accept -> status handler open")

    def poll(self):
        print("status: got", self.channel.messages.pop(0))


class CommandHandler:  # service handler on the active side
    def open(self, channel):
        self.channel = channel
        print("connect done -> command handler open")

    def send(self, text):
        print("command: send", text)
        self.channel.messages.append(text)


listeners = {}  # stands in for the network


class Acceptor:
    def __init__(self, address, make_handler):
        self.make_handler = make_handler
        self.handlers = []
        listeners[address] = self
        print("listen", address)

    def accept(self, channel):
        handler = self.make_handler()
        handler.open(channel)
        self.handlers.append(handler)


class Connector:
    def connect(self, address, make_handler):
        print("connect", address)
        channel = Channel()
        listeners[address].accept(channel)
        return self.complete(channel, make_handler)

    def complete(self, channel, make_handler):
        handler = make_handler()
        handler.open(channel)  # activate only when the connection is up
        return handler


acceptor = Acceptor("gateway:9000", StatusHandler)
command = Connector().connect("gateway:9000", CommandHandler)
command.send("reset")
acceptor.handlers[0].poll()
```

```output
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

- Verdict: idiom. A class is a factory, so the acceptor and the connector take the handler class itself and call it to make each handler.

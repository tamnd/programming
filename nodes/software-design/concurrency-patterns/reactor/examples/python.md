```python run id=reactor
class Reactor:
    def __init__(self):
        self.handlers = {}

    def register(self, handle, handler):
        self.handlers[handle] = handler

    def remove(self, handle):
        del self.handlers[handle]

    def run(self, ready):
        # ready stands in for select: (handle, data) pairs in arrival order
        for handle, data in ready:
            self.handlers[handle](handle, data)


reactor = Reactor()


def echo(handle, data):
    if data:
        print(f"{handle}: {data}")
    else:
        print("close", handle)
        reactor.remove(handle)


def acceptor(handle, new_handle):
    print("accept", new_handle)
    reactor.register(new_handle, echo)


reactor.register("listener", acceptor)
reactor.run([
    ("listener", "conn-1"), ("listener", "conn-2"),
    ("conn-1", "hello"), ("conn-2", "hi"),
    ("conn-1", ""), ("conn-2", ""),
])
```

```output
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

- Verdict: native. A handler is a function, so the table of handlers is a dictionary from handle to function. The `selectors` module gives the real demultiplexer.

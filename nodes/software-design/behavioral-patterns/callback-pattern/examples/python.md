```python run id=callbacks
def for_each(items, callback):
    for item in items:
        callback(item)  # a blocking callback: called before for_each returns


class Queue:
    def __init__(self):
        self.pending = []

    def later(self, callback):
        self.pending.append(callback)  # stored, to be called later

    def run(self):
        while self.pending:
            self.pending.pop(0)()


def shout(word):
    print(word.upper())


for_each(["hello", "world"], shout)

queue = Queue()
queue.later(lambda: print("saved"))  # a deferred callback
print("request sent")
queue.run()
```

```output for=callbacks
HELLO
WORLD
request sent
saved
```

- Verdict: native. Functions are values, and a lambda can be passed directly.

```python run id=survive-crash
import os
import tempfile


class DurableQueue:
    def __init__(self, path):
        self.path = path
        self.pending = {}
        if os.path.exists(path):
            with open(path) as f:
                for line in f:
                    op, mid, *body = line.rstrip("\n").split(" ", 2)
                    if op == "send":
                        self.pending[mid] = body[0]
                    else:
                        self.pending.pop(mid, None)

    def _log(self, line):
        with open(self.path, "a") as f:
            f.write(line + "\n")
            f.flush()
            os.fsync(f.fileno())

    def send(self, mid, body):
        self._log(f"send {mid} {body}")
        self.pending[mid] = body

    def ack(self, mid):
        self._log(f"ack {mid}")
        del self.pending[mid]


path = os.path.join(tempfile.mkdtemp(), "orders.log")
memory = {}
durable = DurableQueue(path)
for i in (1, 2, 3):
    memory[f"m{i}"] = f"order-{i}"
    durable.send(f"m{i}", f"order-{i}")
print(f"before crash: memory {len(memory)}, durable {len(durable.pending)}")

memory = {}  # a crash loses what was only in memory
durable = DurableQueue(path)
print(f"after crash: memory {len(memory)}, durable {len(durable.pending)}")

mid = next(iter(durable.pending))
print(f"receive {mid} {durable.pending[mid]}, ack")
durable.ack(mid)

durable = DurableQueue(path)
print("after second crash: durable", len(durable.pending), " ".join(durable.pending))
os.remove(path)
```

```output
before crash: memory 3, durable 3
after crash: memory 0, durable 3
receive m1 order-1, ack
after second crash: durable 2 m2 m3
```

- Verdict: idiom. Python has no persistent queue. A file that the program appends to, with `os.fsync` after each line, is the store; the queue reads it again when it starts.
- `send` returns only after `fsync`, so a caller that sees `send` return knows that the line is on disk. This is the confirm of the pattern.
- The log grows for ever. A real store removes the acked records from time to time, for example by writing the pending ones to a new file and renaming it over the old one.

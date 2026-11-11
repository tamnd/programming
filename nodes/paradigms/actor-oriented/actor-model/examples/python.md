```python run id=supervisor
import queue
import threading


def spawn(behavior, state, supervisor):
    mailbox = queue.Queue()

    def loop():
        s = state
        while (msg := mailbox.get()) is not None:
            if msg[0] == "get":
                msg[1].put(s)
                continue
            try:
                s = behavior(s, msg[1])
            except Exception as e:
                supervisor.put(("crashed", repr(e)))
                return

    threading.Thread(target=loop, daemon=True).start()
    return mailbox


def add(total, text):
    return total + int(text)


events = queue.Queue()
worker = spawn(add, 0, events)
for text in ["1", "2", "x"]:
    worker.put(("num", text))

kind, reason = events.get()  # the supervisor waits for the crash
print(kind, reason)
worker = spawn(add, 0, events)  # restart with fresh state
print("restarted with fresh state")
worker.put(("num", "3"))
reply = queue.Queue()
worker.put(("get", reply))
print("total:", reply.get())
```

```output for=supervisor
crashed ValueError("invalid literal for int() with base 10: 'x'")
restarted with fresh state
total: 3
```

```python wrong id=shared-message
import queue
import threading

inbox = queue.Queue()
seen = []


def actor():
    seen.append(list(inbox.get()))


order = ["tea"]
inbox.put(order)  # sends a reference, not a copy
order.append("cake")  # the sender changes the message after it was sent
t = threading.Thread(target=actor)
t.start()
t.join()
print(seen)
```

```output for=shared-message
[['tea', 'cake']]
```

- Verdict: idiom. Python has no actors in the standard library, but a thread with a `queue.Queue` as its mailbox is an actor. The supervisor in the first block does not try to handle the bad input. It lets the worker crash, and starts a new one with clean state, which is the Erlang way.
- In the wrong block, the message is a list, and the queue passes a reference to it. The sender changes the list after the send, and the actor sees the change. Actors must not share mutable data. Send immutable values, such as tuples and frozen data classes, or send a copy.

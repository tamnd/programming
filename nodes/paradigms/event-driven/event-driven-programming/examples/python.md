```python run id=selectors
import selectors
import socket

sel = selectors.DefaultSelector()
client, server = socket.socketpair()


def on_server_readable(conn):
    msg = conn.recv(100).decode()
    print("server got", msg)
    conn.send(msg.replace("ping", "pong").encode())


def on_client_readable(conn):
    msg = conn.recv(100).decode()
    print("client got", msg)
    n = int(msg.split()[1])
    if n < 3:
        conn.send(f"ping {n + 1}".encode())
    else:
        sel.unregister(client)
        sel.unregister(server)


sel.register(server, selectors.EVENT_READ, on_server_readable)
sel.register(client, selectors.EVENT_READ, on_client_readable)
client.send(b"ping 1")

while sel.get_map():
    for key, _ in sel.select(timeout=1):
        key.data(key.fileobj)
```

```output for=selectors
server got ping 1
client got pong 1
server got ping 2
client got pong 2
server got ping 3
client got pong 3
```

```python wrong id=blocking-handler
import asyncio
import time


async def main():
    loop = asyncio.get_running_loop()
    start = loop.time()
    late = []
    loop.call_later(0.05, lambda: late.append(loop.time() - start - 0.05))
    time.sleep(0.3)  # blocks the whole loop
    await asyncio.sleep(0.1)
    print("timer ran late by more than 0.2 s:", late[0] > 0.2)


asyncio.run(main())
```

```output for=blocking-handler
timer ran late by more than 0.2 s: True
```

- Verdict: stdlib. The `selectors` module waits on many sockets at once and is the base of `asyncio`. The Python documentation says it "allows high-level and efficient I/O multiplexing, built upon the select module primitives." `asyncio` adds an event loop with timers, callbacks and coroutines, and `tkinter` has an event loop for user interfaces.
- In the first block, each handler runs when its socket has data, does a little work and returns to the loop. The data in `key.data` is the handler that `register` stored.
- In the wrong block, `time.sleep` blocks the thread that runs the loop, so the timer cannot fire on time. Use `await asyncio.sleep`, or move blocking work to a thread with `loop.run_in_executor` or `asyncio.to_thread`.

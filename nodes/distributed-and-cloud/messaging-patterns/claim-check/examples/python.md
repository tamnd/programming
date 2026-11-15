```python run id=claim-report
import hashlib

THRESHOLD = 1024  # payloads larger than this go to the store
store = {}        # stands for a blob store or a database
channel = []


def send(kind, payload):
    if len(payload) <= THRESHOLD:
        message = f"{kind};inline;{payload.decode()}"
    else:
        key = "sha256:" + hashlib.sha256(payload).hexdigest()
        store[key] = payload  # store first, then publish the claim check
        message = f"{kind};claim;{key};{len(payload)}"
    channel.append(message)
    print(f"sent {kind}: {len(message)} bytes on the channel"
          + (f" for {len(payload)} bytes in the store" if len(payload) > THRESHOLD else ""))


def receive(message):
    kind, mode, rest = message.split(";", 2)
    if mode == "inline":
        return f"received {kind}: {rest}"
    key, size = rest.split(";")
    payload = store.get(key)
    if payload is None:
        return f"received {kind}: claim {key[:19]}... not found, to the dead letter channel"
    if "sha256:" + hashlib.sha256(payload).hexdigest() != key or len(payload) != int(size):
        return f"received {kind}: payload does not match the claim check"
    rows = payload.count(b"\n")
    return f"received {kind}: {rows} rows, hash {key[7:19]} ok"


report = b"".join(b"row %05d;widget;12.50\n" % i for i in range(12000))
send("invoice", b"INV-17 total 12.50")
send("report", report)
for m in channel:
    print(receive(m))

# The lifecycle rule of the store removes the payload; then the broker delivers the claim again.
store.clear()
print(receive(channel[1]))
```

```output
sent invoice: 33 bytes on the channel
sent report: 91 bytes on the channel for 276000 bytes in the store
received invoice: INV-17 total 12.50
received report: 12000 rows, hash 63b98860796d ok
received report: claim sha256:63b98860796d... not found, to the dead letter channel
```

- Verdict: idiom. `hashlib.sha256(payload).hexdigest()` gives a key that depends only on the content, and a dict stands for the blob store.
- The sender writes to the store before it puts the claim check on the channel, so a receiver never gets a key with no data behind it at that time.
- The receiver checks the hash and the size; a missing payload gives a message for the dead letter channel, not an exception.

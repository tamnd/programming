```python run id=sessions-on-two-servers
class SessionStore:
    """One server's sessions, kept in its memory as serialized text, with the time of their last use."""

    TIMEOUT = 30  # minutes with no request, then the session ends

    def __init__(self, name):
        self.name = name
        self.sessions = {}  # session ID -> [serialized state, minute of last use]
        self.count = 0

    def new_id(self):
        # A real server makes a random ID with a secure generator, such as secrets.token_urlsafe(32).
        self.count += 1
        return f"{self.name}{self.count}"

    def load(self, sid, now):
        entry = self.sessions.get(sid)
        if entry is None or now - entry[1] > self.TIMEOUT:
            return None
        return deserialize(entry[0])

    def save(self, sid, cart, now):
        self.sessions[sid] = [serialize(cart), now]

    def sweep(self, now):
        idle = sorted(s for s, (_, last) in self.sessions.items() if now - last > self.TIMEOUT)
        for sid in idle:
            del self.sessions[sid]
        return idle


def serialize(cart):
    return ",".join(f"{item}:{qty}" for item, qty in cart.items())


def deserialize(text):
    cart = {}
    for part in filter(None, text.split(",")):
        item, qty = part.split(":")
        cart[item] = int(qty)
    return cart


def show(cart):
    return ", ".join(f"{item} {qty}" for item, qty in cart.items())


def add(server, sid, who, item, now):
    """One request. The client sends only its session ID; the state is on the server."""
    cart = server.load(sid, now) if sid else None
    if cart is None:
        status = f"session {sid} is unknown here, " if sid else ""
        sid, cart = server.new_id(), {}
        status += f"new session {sid}"
    else:
        status = f"session {sid}"
    cart[item] = cart.get(item, 0) + 1
    server.save(sid, cart, now)
    print(f"10:{now:02d} {who} adds {item} on server {server.name}: {status}, cart {show(cart)}")
    return sid


a, b = SessionStore("a"), SessionStore("b")
ann = add(a, None, "ann", "pen", 0)
bob = add(a, None, "bob", "ink", 1)
add(a, ann, "ann", "pen", 2)
add(b, ann, "ann", "ink", 3)  # a load balancer sends this request to the other server
add(a, ann, "ann", "ink", 30)
print("10:40 server a removes idle sessions:", ", ".join(a.sweep(40)))
for sid, (text, last) in sorted(a.sessions.items()):
    print(f"server a keeps {sid} = '{text}', last used at 10:{last:02d}")
```

```output
10:00 ann adds pen on server a: new session a1, cart pen 1
10:01 bob adds ink on server a: new session a2, cart ink 1
10:02 ann adds pen on server a: session a1, cart pen 2
10:03 ann adds ink on server b: session a1 is unknown here, new session b1, cart ink 1
10:30 ann adds ink on server a: session a1, cart pen 2, ink 1
10:40 server a removes idle sessions: a2
server a keeps a1 = 'pen:2,ink:1', last used at 10:30
```

- Verdict: idiom. `SessionStore` is a dictionary from the session ID to the serialized cart and the minute of its last use. `load` treats a session that is idle for more than `TIMEOUT` minutes as unknown, and `sweep` removes such sessions.
- The IDs `a1` and `b1` keep the output stable. A real server makes them with `secrets.token_urlsafe`, so that no one can guess the ID of another session.

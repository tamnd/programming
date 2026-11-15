```python run id=cookie-cart
import hashlib
import hmac

SECRET = b"server secret, never sent to the client"


def sign(data):
    return hmac.new(SECRET, data.encode(), hashlib.sha256).hexdigest()


def encode(cart):
    """Serializes the session state into the value of a cookie: the data, a bar and the signature."""
    data = ",".join(f"{item}:{qty}" for item, qty in cart.items())
    return data + "|" + sign(data)


def decode(cookie):
    """Checks the signature and gives back the state. A changed cookie gives an error."""
    data, _, tag = cookie.rpartition("|")
    if not hmac.compare_digest(tag, sign(data)):
        raise ValueError("bad signature")
    cart = {}
    for part in filter(None, data.split(",")):
        item, qty = part.split(":")
        cart[item] = int(qty)
    return cart


def handle_add(cookie, item):
    """One request. The server keeps nothing between requests: the state comes in and goes out in the cookie."""
    cart = decode(cookie) if cookie else {}
    cart[item] = cart.get(item, 0) + 1
    return encode(cart)


def show(cookie):
    data, _, tag = cookie.rpartition("|")
    return f"{data}|{tag[:8]}... ({len(cookie)} bytes)"


cookie = None
for item in ["pen", "pen", "ink"]:
    cookie = handle_add(cookie, item)
    print(f"add {item}: cookie {show(cookie)}")

tampered = cookie.replace("ink:1", "ink:9")
try:
    handle_add(tampered, "pen")
except ValueError as e:
    print("a client changes ink:1 to ink:9:", e)

print("cart:", ", ".join(f"{item} {qty}" for item, qty in decode(cookie).items()))
```

```output
add pen: cookie pen:1|2c06beca... (70 bytes)
add pen: cookie pen:2|bdba40d9... (70 bytes)
add ink: cookie pen:2,ink:1|8bdcc52b... (76 bytes)
a client changes ink:1 to ink:9: bad signature
cart: pen 2, ink 1
```

- Verdict: stdlib. `hmac.new` with `hashlib.sha256` signs the serialized cart, and `hmac.compare_digest` checks the signature in a time that does not depend on where the first difference is.
- The data part of the cookie is plain text, so the client can read it. A cookie that must be secret needs encryption, not only a signature.

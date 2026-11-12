```python run id=act
def service(requests):
    # replies in the reverse order; the token comes back unchanged
    return [(token, f"{name} loaded") for token, name in reversed(requests)]


pending = {}  # token -> state that the completion needs
requests = []
for token, name in enumerate(["image-7", "image-9"], start=1):
    pending[token] = name
    requests.append((token, name))
    print(f"send {token}: load {name}")

for token, result in service(requests):
    name = pending.pop(token)  # the token finds the state at once
    print(f"done {token}: {result}")
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. The token is an integer key into a dictionary of pending operations. A future from `concurrent.futures` can take the place of the integer.

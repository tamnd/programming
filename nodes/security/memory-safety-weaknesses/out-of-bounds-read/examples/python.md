```python wrong id=clamp
def echo(payload, claimed):
    return payload[:claimed]


payload = b"hello"
reply = echo(payload, 64)
print("claimed 64, reply has", len(reply), "bytes:", reply)
```

```output for=clamp
claimed 64, reply has 5 bytes: b'hello'
```

A slice end past the length is clamped to the length. No memory leaks, but the program does not notice that the message lied about its size.

```python run id=check
def echo(payload, claimed):
    if not 0 <= claimed <= len(payload):
        raise ValueError(f"claimed {claimed} bytes, received {len(payload)}")
    return payload[:claimed]


payload = b"hello"
for claimed in [5, 64]:
    try:
        print(f"claimed {claimed}:", echo(payload, claimed))
    except ValueError as error:
        print("discarded:", error)
```

```output for=check
claimed 5: b'hello'
discarded: claimed 64 bytes, received 5
```

- Verdict: native. Indexing raises `IndexError` out of range, and slicing clamps the bounds. The interpreter never reads outside an object.
- A negative bound counts from the end, so a negative length from input selects other bytes. Check the range explicitly.
- C extensions, `ctypes` and `memoryview` over foreign memory have no such check.

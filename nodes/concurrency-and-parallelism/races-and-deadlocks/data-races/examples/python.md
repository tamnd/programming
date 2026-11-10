```python run id=lock
import threading

count = 0
lock = threading.Lock()


def work():
    global count
    for _ in range(100_000):
        with lock:
            count += 1


workers = [threading.Thread(target=work) for _ in range(2)]
for worker in workers:
    worker.start()
for worker in workers:
    worker.join()
print(count)
```

```output for=lock
200000
```

- Verdict: stdlib. The `threading` module has `Lock`, and `with lock:` holds it for the whole read, add and store of `count += 1`.
- The interpreter switches threads only between bytecode instructions, and `count += 1` is three of them, so the GIL does not make it atomic.
- Python has no atomic integer type in the standard library. Single operations on built-in types, such as `list.append`, are atomic, but compound updates are not.
- The free-threaded build has no GIL. Its built-in types use internal locks, and the documentation still recommends `threading.Lock` for shared state.

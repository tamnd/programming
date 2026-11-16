```python run id=bind-before-read
def count_words(words):
    try:
        print(count)
    except UnboundLocalError as e:
        print(type(e).__name__)
    count = 0
    for _ in words:
        count += 1
    return count


print(count_words(["a", "b", "c"]))
try:
    print(missing)
except NameError as e:
    print(type(e).__name__)
```

```output for=bind-before-read
UnboundLocalError
3
NameError
```

- Verdict: native. Python has no declarations: the first binding of a name, such as `count = 0`, is the initialization. A function that binds `count` anywhere treats `count` as a local name in the whole function, so a read before the binding raises `UnboundLocalError`. A name with no binding at all raises `NameError`.

```python run id=countdown
class Countdown:
    def __init__(self, start):
        self.current = start

    def __iter__(self):
        return self

    def __next__(self):
        if self.current <= 0:
            raise StopIteration
        value = self.current
        self.current -= 1
        return value


for n in Countdown(3):
    print(n)

it = iter(["a", "b"])
print(next(it), next(it))
print(next(it, "end"))
```

```output for=countdown
3
2
1
a b
end
```

- Verdict: native. An iterator is an object with `__next__`, and an iterable is an object whose `__iter__` returns an iterator. The `for` statement calls `iter()` once and then `next()` until `StopIteration`.
- An iterator's own `__iter__` returns `self`, so an iterator can go where an iterable is expected.
- `next(it, default)` returns the default instead of raising `StopIteration` when the iterator is exhausted.
- A generator function (a `def` with `yield`) makes an iterator with less code. The class above shows the protocol that the generator implements for you.

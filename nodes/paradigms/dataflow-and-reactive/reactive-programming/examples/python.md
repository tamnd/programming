```python run id=observable
class Observable:
    def __init__(self, subscribe):
        self.subscribe = subscribe

    def map(self, f):
        return Observable(lambda emit: self.subscribe(lambda x: emit(f(x))))

    def filter(self, keep):
        return Observable(lambda emit: self.subscribe(lambda x: keep(x) and emit(x)))

    def scan(self, f, acc):
        def subscribe(emit):
            state = [acc]

            def step(x):
                state[0] = f(state[0], x)
                emit(state[0])

            self.subscribe(step)

        return Observable(subscribe)


def readings(emit):
    for t in [18, 22, 19, 25, 30]:
        emit(t)


hot = Observable(readings).filter(lambda t: t > 20)
hot.map(lambda t: f"hot {t}").subscribe(print)
hot.scan(lambda total, t: total + t, 0).subscribe(lambda s: print("running sum", s))
```

```output for=observable
hot 22
hot 25
hot 30
running sum 22
running sum 47
running sum 77
```

```python wrong id=glitch
class Cell:
    def __init__(self, value):
        self.value, self.observers = value, []

    def set(self, value):
        self.value = value
        for notify in self.observers:
            notify()


a = Cell(1)
b = Cell(a.value * 2)
c = Cell(a.value + 1)
d = Cell(b.value + c.value)
a.observers += [lambda: b.set(a.value * 2), lambda: c.set(a.value + 1)]
b.observers.append(lambda: d.set(b.value + c.value))
c.observers.append(lambda: d.set(b.value + c.value))
d.observers.append(lambda: print("d =", d.value))

a.set(10)
```

```output for=glitch
d = 22
d = 31
```

- Verdict: idiom. Python has no observables or signals in the standard library. A few lines of closures give an observable with `map`, `filter` and `scan`, and the first block of this page gives signals with automatic dependency tracking. Libraries such as RxPY give the full ReactiveX operators.
- Each call to `subscribe` runs the source again, so the readings are produced twice, once for each subscriber. This is a cold observable. A hot observable shares one run of the source among all subscribers.
- The wrong block pushes each change at once. When `a` changes, `b` updates and pushes to `d` before `c` has updated, so `d` shows 22, a value that matches no consistent state: the right value is 31. This is a glitch. Engines avoid it by updating in topological order, or by marking values as dirty and computing them when read, as the first block of the page does.

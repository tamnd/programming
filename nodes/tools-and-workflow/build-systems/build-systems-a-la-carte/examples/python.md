```python run id=models
# Two build systems from the same parts: a scheduler and a rebuilder.
# The tasks are a small spreadsheet. B3 has a dynamic dependency.

TASKS = {
    "B1": lambda fetch: fetch("A1") + fetch("A2"),
    "B2": lambda fetch: fetch("B1") * 2,
    "B3": lambda fetch: fetch("B2") if fetch("C1") == 1 else fetch("A2"),
}
# A topological scheduler must know the dependencies before the build.
# For B3 it must list both branches.
STATIC_DEPS = {"B1": ["A1", "A2"], "B2": ["B1"], "B3": ["C1", "B2", "A2"]}
TARGETS = ["B2", "B3"]


class MakeLike:
    """Topological scheduler with a dirty bit."""

    def __init__(self, inputs):
        self.store = dict(inputs)
        self.dirty = set(inputs)

    def set_input(self, key, value):
        self.store[key] = value
        self.dirty.add(key)  # writing a file sets its time, even with the same content

    def build(self):
        order, seen = [], set()

        def visit(k):
            if k in seen or k not in TASKS:
                return
            seen.add(k)
            for d in STATIC_DEPS[k]:
                visit(d)
            order.append(k)

        for t in TARGETS:
            visit(t)
        ran = []
        for k in order:
            if k not in self.store or any(d in self.dirty for d in STATIC_DEPS[k]):
                self.store[k] = TASKS[k](lambda d: self.store[d])
                self.dirty.add(k)
                ran.append(k)
        self.dirty.clear()
        return ran


class ShakeLike:
    """Suspending scheduler with verifying traces."""

    def __init__(self, inputs):
        self.store = dict(inputs)
        self.traces = {}  # key -> (list of (dependency, hash), hash of the result)

    def set_input(self, key, value):
        self.store[key] = value

    def build(self):
        done, ran = set(), []

        def fetch(k):
            if k not in TASKS or k in done:
                return self.store[k]
            trace = self.traces.get(k)
            if trace is not None and str(self.store[k]) == trace[1]:
                if all(str(fetch(d)) == h for d, h in trace[0]):
                    done.add(k)
                    return self.store[k]
            deps = []

            def tracked(d):
                # The task stops here until d is up to date: this is the suspending scheduler.
                v = fetch(d)
                deps.append((d, str(v)))
                return v

            v = TASKS[k](tracked)
            self.store[k] = v
            self.traces[k] = (deps, str(v))
            done.add(k)
            ran.append(k)
            return v

        for t in TARGETS:
            fetch(t)
        return ran


inputs = {"A1": 10, "A2": 20, "C1": 1}
make, shake = MakeLike(inputs), ShakeLike(inputs)
steps = [
    ("clean build", {}),
    ("A2 = 20 again", {"A2": 20}),
    ("A1 = 11, A2 = 19", {"A1": 11, "A2": 19}),
    ("C1 = 2", {"C1": 2}),
    ("A1 = 12", {"A1": 12}),
]
print(f"{'step':<18}{'make-like':<14}shake-like")
for name, changes in steps:
    for k, v in changes.items():
        make.set_input(k, v)
        shake.set_input(k, v)
    m = " ".join(make.build()) or "-"
    s = " ".join(shake.build()) or "-"
    print(f"{name:<18}{m:<14}{s}")
print("same values:", all(make.store[k] == shake.store[k] for k in TASKS))
print("B1, B2, B3 =", shake.store["B1"], shake.store["B2"], shake.store["B3"])
```

```output
step              make-like     shake-like
clean build       B1 B2 B3      B1 B2 B3
A2 = 20 again     B1 B2 B3      -
A1 = 11, A2 = 19  B1 B2 B3      B1
C1 = 2            B3            B3
A1 = 12           B1 B2 B3      B1 B2
same values: True
B1, B2, B3 = 31 62 19
```

- Verdict: idiom. A dict maps each key to a task, and a task is a lambda that takes `fetch`. The dict of static dependencies is used only by the Make-like system.
- `ShakeLike.build` defines `fetch` inside the build, so the set `done` belongs to one run. `tracked` wraps `fetch` and records each key that the task reads, in order, with its hash.
- `all` stops at the first dependency whose hash changed. So when `C1` changes, the old trace of `B3` does not make the build fetch `B2`.

```python run id=finalize-as-safety-net
import gc
import weakref


class Conn:
    def __init__(self, name):
        self.name = name
        # A safety net: runs if the program forgets to close, at most once.
        self._finalizer = weakref.finalize(self, print, "safety net closes", name)

    def close(self):
        if self._finalizer.detach():  # cancel the safety net
            print("closed", self.name)

    def __enter__(self):
        return self

    def __exit__(self, *exc):
        self.close()


with Conn("a") as a:
    print("using", a.name)
a.close()  # a second close does nothing

b = Conn("b")
del b  # forgotten: CPython frees b now, so the safety net runs now

c = Conn("c")
c.me = c  # forgotten and in a cycle
del c
print("c is unreachable")
gc.collect()
print("end")
```

```output for=finalize-as-safety-net
using a
closed a
safety net closes b
c is unreachable
safety net closes c
end
```

```python wrong id=resurrection
saved = []


class Conn:
    def __init__(self, name):
        self.name = name

    def __del__(self):
        print("finalize", self.name)
        saved.append(self)  # stores a new reference: the object comes back


c = Conn("db")
del c
print(len(saved), saved[0].name)  # the finalized object is still in use
saved.clear()  # dropped again, but CPython does not finalize it a second time
print("end")
```

```output for=resurrection
finalize db
1 db
end
```

- Verdict: native. CPython frees most objects the moment their reference count reaches zero, so `__del__` and `weakref.finalize` often run at a predictable point, as they do for `b`. An object in a cycle waits for the cycle collector, as `c` does. Other implementations, such as PyPy, use a tracing collector and run finalizers later. `weakref.finalize` is the safer tool: it runs at most once, `detach` cancels it after an explicit close, and it holds no reference to the object, so it cannot bring the object back.
- The wrong block stores `self` from inside `__del__`. The object comes back to life after its finalizer ran, and CPython does not run `__del__` again when the object dies the second time, so any cleanup in it happens only once, for an object that is still in use.

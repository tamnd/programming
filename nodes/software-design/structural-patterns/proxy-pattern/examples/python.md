```python run id=report
class Report:
    def __init__(self):
        print("load: read the report from disk")
        self.orders = 42

    def render(self):
        return f"report: {self.orders} orders"


class LazyReport:
    """A virtual proxy: it makes the real report on first use."""

    def __init__(self, factory):
        self._factory = factory
        self._real = None

    def __getattr__(self, name):
        # Python calls __getattr__ only for names that the proxy itself does not have.
        if self._real is None:
            self._real = self._factory()
        return getattr(self._real, name)


class GuardedReport:
    """A protection proxy: it checks the role and then forwards."""

    def __init__(self, report, role):
        self._report = report
        self._role = role

    def render(self):
        if self._role != "admin":
            return "access denied"
        return self._report.render()


lazy = LazyReport(Report)
print("proxy made")
print(lazy.render())
print(lazy.render())
for role in ["guest", "admin"]:
    print(f"{role}: {GuardedReport(lazy, role).render()}")
```

```output
proxy made
load: read the report from disk
report: 42 orders
report: 42 orders
guest: access denied
admin: report: 42 orders
```

- Verdict: idiom. `__getattr__` runs only when the normal lookup fails, so one method forwards every attribute that the proxy does not define. The proxy needs no copy of the real class's interface.
- `__getattr__` does not catch special methods such as `__len__`. Python looks them up on the type, not on the instance, so `len(lazy)` fails. The node page shows this error.
- The guard is written out by hand. A protection proxy names the methods it checks, so a forwarding method for each of them is clearer than `__getattr__`.
- `functools.cached_property` gives a lazy attribute without a proxy object, when the class that owns the attribute can change.

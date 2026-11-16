```python run id=init-subclass-registry
class Plugin:
    registry = {}

    def __init_subclass__(cls, /, name=None, **kwargs):  # runs for each subclass
        super().__init_subclass__(**kwargs)
        if not hasattr(cls, "run"):
            raise TypeError(f"{cls.__name__} must define run()")
        Plugin.registry[name or cls.__name__.lower()] = cls


class Upper(Plugin):
    def run(self, text):
        return text.upper()


class Reverse(Plugin, name="rev"):
    def run(self, text):
        return text[::-1]


print(type(Upper).__name__, sorted(Plugin.registry))
print(Plugin.registry["rev"]().run("abc"))
try:

    class Broken(Plugin):
        pass

except TypeError as e:
    print(e)
print(sorted(Plugin.registry))
```

```output for=init-subclass-registry
type ['rev', 'upper']
cba
Broken must define run()
['rev', 'upper']
```

- Verdict: native. A subclass of `type` is a metaclass, and `class Plugin(metaclass=PluginMeta)` sets it. This file shows `__init_subclass__`, which does the most common job of a metaclass with less code.
- `__init_subclass__` runs on the base class each time a subclass is defined, after the subclass exists. Keyword arguments in the class statement, such as `name="rev"`, go to it. The type of `Upper` is still `type`, so this class has no metaclass to conflict with the metaclass of another library.

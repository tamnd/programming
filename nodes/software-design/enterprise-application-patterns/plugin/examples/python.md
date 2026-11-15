```python run id=key-generator-plugin
import importlib


class CounterKeyGenerator:  # for unit tests: a counter in memory
    def __init__(self):
        self.last = 0

    def next_key(self):
        self.last += 1
        return self.last


class SequenceKeyGenerator:  # for production: it stands for a database sequence
    def __init__(self):
        self.last = 1000

    def next_key(self):
        self.last += 1
        return self.last


# One configuration for each deployment. Only the configuration names the classes.
CONFIGS = {
    "test": {"KeyGenerator": "__main__:CounterKeyGenerator"},
    "production": {"KeyGenerator": "__main__:SequenceKeyGenerator"},
    "broken": {"KeyGenerator": "__main__:FileKeyGenerator"},
}


class PluginFactory:
    def __init__(self, config):
        self.config = config

    def get(self, interface):
        module_name, _, class_name = self.config[interface].partition(":")
        cls = getattr(importlib.import_module(module_name), class_name, None)
        if cls is None:
            raise LookupError(f"no plugin named {class_name}")
        return cls()


for name, config in CONFIGS.items():
    try:
        gen = PluginFactory(config).get("KeyGenerator")
    except LookupError as e:
        print(f"{name}: error: {e}")
        continue
    keys = ", ".join(str(gen.next_key()) for _ in range(3))
    print(f"{name}: keys {keys} from {type(gen).__name__}")
```

```output
test: keys 1, 2, 3 from CounterKeyGenerator
production: keys 1001, 1002, 1003 from SequenceKeyGenerator
broken: error: no plugin named FileKeyGenerator
```

- Verdict: stdlib. `importlib.import_module` and `getattr` load a class from a reference of the form `module:attribute`, which is the form of an entry point. Installed packages can publish such references as entry points, and `importlib.metadata.entry_points` finds them.
- The configuration names the module `__main__` here because the example is one file. In a real program, each plugin is in its own module.

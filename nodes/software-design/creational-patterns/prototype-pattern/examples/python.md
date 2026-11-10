```python run id=spawner
import copy


class Monster:
    def __init__(self, name, health, items):
        self.name = name
        self.health = health
        self.items = items

    def clone(self):
        # A deep copy: the clone gets its own list of items.
        return copy.deepcopy(self)

    def describe(self):
        return f"{self.name}: health {self.health}, items {', '.join(self.items)}"


class Trap:
    def __init__(self, name, damage):
        self.name = name
        self.damage = damage

    def clone(self):
        # Every field is immutable, so a shallow copy is enough.
        return copy.copy(self)

    def describe(self):
        return f"{self.name}: damage {self.damage}"


class Registry:
    def __init__(self):
        self._prototypes = {}

    def add(self, key, prototype):
        self._prototypes[key] = prototype

    def get(self, key):
        return self._prototypes[key]

    def spawn(self, key):
        return self._prototypes[key].clone()


registry = Registry()
registry.add("goblin", Monster("goblin", 30, ["dagger"]))
registry.add("spikes", Trap("spikes", 5))

first = registry.spawn("goblin")
first.items.append("torch")
second = registry.spawn("goblin")
trap = registry.spawn("spikes")
for thing in [first, second, trap]:
    print(thing.describe())
print("prototype", registry.get("goblin").describe())
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
```

- Verdict: stdlib. `copy.copy` makes a shallow copy and `copy.deepcopy` makes a deep copy of any object whose class does not forbid it. A class can change the result with `__copy__` and `__deepcopy__`.
- The registry calls `clone` and does not know the class of the object that it copies. Each class decides how deep its own copy goes.
- `deepcopy` keeps a memo of the objects that it has already copied, so a cycle of references or a shared object appears once in the copy, as in the original.
- A frozen `dataclass` with `dataclasses.replace` makes a copy with some fields changed, which covers many uses of a prototype with no clone method.

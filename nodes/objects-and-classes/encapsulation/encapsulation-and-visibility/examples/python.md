```python run id=properties-and-mangling
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius  # goes through the setter, so the check runs here too

    @property
    def celsius(self):
        return self._celsius

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError(f"{value} is below absolute zero")
        self._celsius = value


t = Temperature(20)
t.celsius = 25  # looks like a field, runs a method
print(t.celsius)
try:
    t.celsius = -300
except ValueError as e:
    print(e)


class Base:
    def __init__(self):
        self.__id = "base"  # name mangling: stored as _Base__id

    def base_id(self):
        return self.__id


class Child(Base):
    def __init__(self):
        super().__init__()
        self.__id = "child"  # stored as _Child__id: no clash with Base


c = Child()
print(c.base_id(), sorted(vars(c)))
print(c._Base__id)  # mangling avoids clashes; it does not enforce privacy
```

```output for=properties-and-mangling
25
-300 is below absolute zero
base ['_Base__id', '_Child__id']
base
```

```python wrong id=leaked-list
class Team:
    def __init__(self, members):
        self._members = list(members)  # a private copy of the input

    def members(self):
        return self._members  # returns the internal list itself

    def size(self):
        return len(self._members)


team = Team(["ana", "bo"])
names = team.members()
names.append("mallory")  # the caller changes the team's state
print(team.size(), team.members())
```

```output for=leaked-list
3 ['ana', 'bo', 'mallory']
```

- Verdict: idiom. Python has no access modifiers. A leading underscore marks a name as internal by convention, and tools and readers respect it, but the language does not. A property turns an attribute into a pair of methods, so a class can check every write, as `Temperature` does, without changing the code of its callers. Two leading underscores trigger name mangling: `self.__id` in `Base` is stored as `_Base__id`, so a subclass that uses the same name does not overwrite it. The name stays reachable from outside under its mangled form.
- The wrong block returns the internal list from a method. The caller gets the list itself, not a copy, and appending to it changes the state of the team without going through any method. Return a copy, a tuple, or an iterator instead.

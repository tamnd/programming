```python run id=attribute-lookup
import types


class Point:  # the class object is the parent that instances delegate to
    x = 0
    y = 0

    def describe(self):
        return f"({self.x}, {self.y})"


p = Point()
p.x = 3  # an own attribute in p.__dict__ shadows Point.x
print(p.describe(), p.__dict__, Point().describe())

Point.describe = lambda self: f"<{self.x} {self.y}>"  # change the parent at run time
print(p.describe())

p.describe = types.MethodType(lambda self: "special", p)  # a method of one object
print(p.describe(), Point().describe())


class Labeled:
    def describe(self):
        return f"label {self.x}"


p.__class__ = Labeled  # switch the parent of one object
del p.describe
print(p.describe())
```

```output for=attribute-lookup
(3, 0) {'x': 3} (0, 0)
<3 0>
special <0 0>
label 3
```

```python wrong id=shallow-clone
import copy

prototype = {"name": "form", "fields": ["id"]}

login = copy.copy(prototype)  # clone the prototype
login["name"] = "login"
login["fields"].append("password")

print(login)
print(prototype)
```

```output for=shallow-clone
{'name': 'login', 'fields': ['id', 'password']}
{'name': 'form', 'fields': ['id', 'password']}
```

- Verdict: idiom. Python is class-based, but its attribute lookup works like delegation: a read looks in the instance dictionary, then in the class, then in the base classes, and a write goes to the instance. Classes are objects that a program can change at run time, so a new method on a class reaches every instance at once, a single object can get its own method, and assigning `__class__` changes the object that an instance delegates to. A prototype system with clones and parent links, as the Idea section builds, takes a few lines.
- The wrong block clones a prototype with `copy.copy`, which copies the dictionary but not the list inside it. The clone and the prototype share `fields`, so adding a field to `login` also changes the prototype and every later clone. Use `copy.deepcopy`, or build the mutable parts afresh in each clone.

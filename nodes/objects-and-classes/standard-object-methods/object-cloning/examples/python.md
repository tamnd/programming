```python run id=shallow-and-deep-copy
import copy


class Team:
    def __init__(self, name, members):
        self.name = name
        self.members = members

    def __deepcopy__(self, memo):
        # The name is an immutable string, so only the list needs a copy.
        return Team(self.name, copy.deepcopy(self.members, memo))


a = Team("red", ["Ann", "Bo"])
shallow = copy.copy(a)
deep = copy.deepcopy(a)

a.members.append("Cy")
shallow.name = "blue"
print(a.name, a.members)
print(shallow.name, shallow.members)
print(deep.name, deep.members)
print(shallow.members is a.members, deep.members is a.members)
```

```output for=shallow-and-deep-copy
red ['Ann', 'Bo', 'Cy']
blue ['Ann', 'Bo', 'Cy']
red ['Ann', 'Bo']
True False
```

- Verdict: stdlib. `copy.copy` makes a shallow copy and `copy.deepcopy` makes a deep copy of any object, without code in the class.
- The shallow copy has its own `name`, so the new name does not change `a`, but it shares the `members` list with `a`.
- `__deepcopy__` is optional; here it shows how a class controls its deep copy. It passes `memo` on, so shared objects and cycles are copied once.

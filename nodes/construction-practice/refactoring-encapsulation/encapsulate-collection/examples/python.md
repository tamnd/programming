```python run id=owner-decides
from types import MappingProxyType

class Team:
    MAX = 3

    def __init__(self):
        self._members = []
        self._roles = {}

    @property
    def members(self):
        return tuple(self._members)  # a copy that cannot be changed

    @property
    def roles(self):
        return MappingProxyType(self._roles)  # a live, read-only view

    def add_member(self, name, role="dev"):
        if name in self._members:
            return
        if len(self._members) >= self.MAX:
            raise ValueError("team is full")
        self._members.append(name)
        self._roles[name] = role

    def remove_member(self, name):
        self._members.remove(name)
        del self._roles[name]

team = Team()
for n in ("ana", "bo", "cy"):
    team.add_member(n)
roles = team.roles  # taken before the change below
team.remove_member("bo")
team.add_member("dee", role="lead")
assert team.members == ("ana", "cy", "dee")
assert roles["dee"] == "lead"  # the view shows the change
def set_role():
    roles["x"] = "y"

for change in (lambda: team.members.append("x"), set_role):
    try:
        change()
        raise AssertionError("expected an error")
    except (AttributeError, TypeError) as e:
        print(type(e).__name__)
print("encapsulate collection: tests pass")
```

```output
AttributeError
TypeError
encapsulate collection: tests pass
```

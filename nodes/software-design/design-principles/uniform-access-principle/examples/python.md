```python run id=rect-area
class StoredRect:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.area = width * height  # stored once


class ComputedRect:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    @property
    def area(self):
        return self.width * self.height  # computed on each read


def report(name, rect):
    print(f"{name}: {rect.area}")


a = StoredRect(2, 3)
b = ComputedRect(2, 3)
report("stored", a)
report("computed", b)

a.width = 4
b.width = 4
report("stored", a)
report("computed", b)
```

```output
stored: 6
computed: 6
stored: 6
computed: 12
```

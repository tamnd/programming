```python run id=inline-the-layers
# Before: a class with one method, and a function that only forwards to it.
class NameFormatter:
    def format(self, person):
        return f"{person['first']} {person['last']}"


formatter = NameFormatter()


def full_name_before(person):
    return formatter.format(person)


def greeting_before(person):
    return f"Hello, {full_name_before(person)}"


# After: one function, with the body inline.
def full_name(person):
    return f"{person['first']} {person['last']}"


def greeting(person):
    return f"Hello, {full_name(person)}"


ada = {"first": "Ada", "last": "Lovelace"}
assert greeting_before(ada) == "Hello, Ada Lovelace"
assert greeting(ada) == greeting_before(ada)
assert full_name(ada) == full_name_before(ada)
print("greeting: tests pass")
```

```output
greeting: tests pass
```

- Python functions are values and can live at the top level of a module, so a class is not needed to group one function.
- If a caller elsewhere still imports `NameFormatter`, inline the class at each import first, and delete it last.

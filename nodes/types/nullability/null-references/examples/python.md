```python run id=none
def find_user(users: list[dict], name: str) -> dict | None:
    for user in users:
        if user["name"] == name:
            return user
    return None


def log(message: str) -> None:
    print(message)


users = [{"name": "ada", "age": 36}]
for name in ["ada", "bob"]:
    user = find_user(users, name)
    if user is None:
        print(name, "not found")
    else:
        print(name, user["age"])

result = log("no return statement")
print(result, type(result).__name__, bool(result))
print(bool(0), bool(""), bool([]))
```

```output
ada 36
bob not found
no return statement
None NoneType False
False False False
```

- Verdict: native. `None` is the single value of the type `NoneType`, and it marks the absence of a value.
- Any name, element or attribute can hold `None`, whatever its annotation says. The interpreter does not check annotations. `dict | None` and `Optional[dict]` are hints for an external type checker.
- A function that ends without a `return` statement returns `None`, as `log` shows. A bare `return` also returns `None`.
- Test for absence with `is None`. `None` is false in a Boolean context, but so are `0`, `""` and `[]`. A test `if not user` does not separate a missing value from an empty one.

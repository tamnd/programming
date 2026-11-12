```python run id=mangling-and-soft-keywords
import keyword


class Account:
    __balance = 0  # a name that starts with two underscores is mangled
    _owner = "ann"  # one underscore is only a convention for "internal"


print([n for n in vars(Account) if "balance" in n or "owner" in n])

# A namespace is a dict of strings, so code can store a key that is not a valid name.
obj = type("Obj", (), {})()
setattr(obj, "my-var", 1)
print(getattr(obj, "my-var"), "my-var".isidentifier())

# The keyword module lists the hard keywords and the soft keywords of this version.
print(len(keyword.kwlist), keyword.softkwlist)

# The soft keyword match is still a valid name outside a match statement.
match = [1, 2]
match match:
    case [a, b]:
        print("matched", a + b)
```

```output for=mangling-and-soft-keywords
['_Account__balance', '_owner']
1 False
35 ['_', 'case', 'match', 'type']
matched 3
```

- Verdict: native. A name with two leading underscores in a class body is mangled to `_Account__balance`, and one underscore is only a convention. A namespace is a dictionary of strings, so `setattr` can store a key such as `my-var` that is not a valid name. The `keyword` module lists the hard keywords and the soft keywords, and the soft keyword `match` still works as a variable name.

```python run id=alias
def first(items: list[int]) -> int:
    return items[0]


alias = list[int]
print(alias.__origin__, alias.__args__)
items = alias(["not", "ints"])
print(type(items), first(items))
print(first.__annotations__)
try:
    isinstance(items, list[int])
except TypeError as error:
    print("TypeError:", error)
```

```output for=alias
<class 'list'> (<class 'int'>,)
<class 'list'> not
{'items': list[int], 'return': <class 'int'>}
TypeError: isinstance() argument 2 cannot be a parameterized generic
```

- Verdict: native. The interpreter keeps annotations as data, but it does not check them.
- `list[int]` is a generic alias. It keeps its type argument in `__args__`.
- An object made from `list[int]` is a plain `list`. It has no record of `int`, and its elements are not checked.
- `isinstance` refuses a parameterized generic and raises `TypeError`.

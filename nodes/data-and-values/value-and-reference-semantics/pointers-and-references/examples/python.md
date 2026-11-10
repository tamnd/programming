```python run id=change
def rebind(items):
    items = [0, 0, 0]


def append(items):
    items.append(4)


numbers = [1, 2, 3]
rebind(numbers)
print("after rebind:", numbers)
append(numbers)
print("after append:", numbers)

alias = numbers
alias[0] = 100
print("numbers:", numbers)
print("alias is numbers:", alias is numbers)

copy = list(numbers)
print("copy is numbers:", copy is numbers)
print("copy == numbers:", copy == numbers)
```

```output for=change
after rebind: [1, 2, 3]
after append: [1, 2, 3, 4]
numbers: [100, 2, 3, 4]
alias is numbers: True
copy is numbers: False
copy == numbers: True
```

- Verdict: native. Every Python variable holds a reference to an object. The language has no pointer type and no address operator.
- Arguments are passed by assignment. The parameter `items` is a second name for the list of the caller. An assignment to `items` changes only the parameter, so `rebind` has no effect.
- A method call on the parameter, such as `append`, changes the shared list, and the caller sees the change.
- `alias = numbers` copies the reference, not the list. `is` compares identity, and `==` compares values.
- Python has no call by reference. A function that must give back several values returns a tuple, and the caller assigns the parts.

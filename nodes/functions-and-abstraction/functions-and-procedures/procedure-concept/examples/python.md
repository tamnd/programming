```python run id=procedure-python
def greet(name):
    print("Hello, " + name + "!")


greet("Ada")
result = greet("Alan")
print(result)
```

```output for=procedure-python
Hello, Ada!
Hello, Alan!
None
```

- Verdict: native. Python has no separate procedure keyword. A function with no `return` statement is a procedure, and its call gives back `None`.

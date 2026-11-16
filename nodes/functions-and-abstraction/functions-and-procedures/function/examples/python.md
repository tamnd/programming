```python run id=function-python
def rectangle_area(width, height):
    return width * height


def greeting(name):
    return "Hello, " + name + "!"


print(rectangle_area(3, 4))
print(greeting("Ada"))
```

```output for=function-python
12
Hello, Ada!
```

- Verdict: native. `def` defines a function, and `return` gives back its result. The parameters have no types unless the program adds annotations.

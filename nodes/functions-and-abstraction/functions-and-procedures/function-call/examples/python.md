```python run id=function-call-python
def rectangle_area(width, height):
    print("computing", width, "by", height)
    return width * height


area = rectangle_area(3, 4)
print("area", area)
```

```output for=function-call-python
computing 3 by 4
area 12
```

- Verdict: native. A call writes the arguments in parentheses after the name. Python evaluates the arguments, binds them to the parameters and runs the body.

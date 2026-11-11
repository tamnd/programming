```python run id=conditional-expression-and-walrus
def label_statement(score):
    if score >= 90:  # if is a statement: each branch assigns
        label = "A"
    elif score >= 50:
        label = "pass"
    else:
        label = "fail"
    return label


def label_expression(score):  # a conditional expression has a value
    return "A" if score >= 90 else "pass" if score >= 50 else "fail"


scores = [95, 70, 30]
print([label_statement(s) for s in scores])
print([label_expression(s) for s in scores])

data = iter([3, 1, 4, 0, 5])
total = 0
while (n := next(data)) != 0:  # assign and test in one expression
    total += n
print(total)
```

```output for=conditional-expression-and-walrus
['A', 'pass', 'fail']
['A', 'pass', 'fail']
8
```

- Verdict: native. Python separates statements and expressions strictly: `if`, `while`, `for`, `def` and `=` are statements, and an expression cannot contain them. The language adds expression forms for the common cases: the conditional expression `a if c else b`, comprehensions in place of loops, `lambda` in place of `def`, and since Python 3.8 the assignment expression `:=`, which assigns and gives the value in one step.

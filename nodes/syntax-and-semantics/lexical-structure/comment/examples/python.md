```python run id=docstrings-and-getcomments
import inspect


# Return the area of a circle.
def area(r):
    """Return the area of a circle of radius r.

    A docstring is a string literal, so it stays in the program at run time.
    """
    return 3.14159 * r * r  # pi to 5 decimal places


print(area.__doc__.splitlines()[0])
print(repr(inspect.getcomments(area)))

# A comment is gone after parsing; the compiled code has no trace of it.
print("pi to 5" in str(area.__code__.co_consts))

# A string as the first statement is a docstring and is kept.
# A string on its own line later is an expression statement, and the compiler drops it.
for text in ['"first"\nx = 1\n', 'x = 1\n"later"\n']:
    code = compile(text, "<demo>", "exec")
    print([c for c in code.co_consts if isinstance(c, str)])
```

```output for=docstrings-and-getcomments
Return the area of a circle of radius r.
'# Return the area of a circle.\n'
False
['first']
[]
```

- Verdict: native. `#` starts a comment that ends at the end of the line, and Python has no block comment. `inspect.getcomments` reads the comment lines above a function from its source file. A docstring is a string literal, so `__doc__` keeps it at run time, but the compiled code has no trace of the comment. A string statement that is not the first statement is not a docstring, and the compiler drops it.

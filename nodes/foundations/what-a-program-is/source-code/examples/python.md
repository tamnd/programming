```python run id=source-of-a-function
import ast
import inspect


def area(w, h):
    # The comment is part of the source text.
    return w * h


print(inspect.getsource(area), end="")
print("first line in the file:", area.__code__.co_firstlineno)
print("tree to text:", ast.unparse(ast.parse(inspect.getsource(area))).splitlines())

# Two different texts, one tree: the parser ignores spaces and brackets around a name.
print(ast.dump(ast.parse("x=(a)+1")) == ast.dump(ast.parse("x = a + 1")))
```

```output for=source-of-a-function
def area(w, h):
    # The comment is part of the source text.
    return w * h
first line in the file: 5
tree to text: ['def area(w, h):', '    return w * h']
True
```

- Verdict: native. CPython keeps the file name and the first line of each function in its code object, and `inspect.getsource` reads the lines from the file again, so the comment is still there. The tree from `ast` has no comments, so `ast.unparse` gives the code without it. Two texts with different spacing and brackets parse to the same tree.

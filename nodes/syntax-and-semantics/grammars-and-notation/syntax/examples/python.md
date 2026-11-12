```python run id=ast-unparse-and-sugar
import ast

# The abstract syntax tree drops the layout and the parentheses.
print(ast.dump(ast.parse("f(a, b=2)", mode="eval").body))

# ast.unparse prints the tree as text in a standard form.
print(ast.unparse(ast.parse("x=(1+2)*  3")))

# Syntactic sugar: a decorator is a short form of a call and an assignment.
def shout(f):
    return lambda: f().upper()


@shout
def hi():
    return "hi"


def hey():
    return "hey"


hey = shout(hey)  # the same thing without the sugar
print(hi(), hey())

# a[i] is sugar for a call of __getitem__.
xs = [10, 20, 30]
print(xs[1], type(xs).__getitem__(xs, 1))

# A syntax error has a position.
try:
    ast.parse("if x\n    pass")
except SyntaxError as e:
    print("SyntaxError:", e.msg, "line", e.lineno, "offset", e.offset)
```

```output for=ast-unparse-and-sugar
Call(func=Name(id='f', ctx=Load()), args=[Name(id='a', ctx=Load())], keywords=[keyword(arg='b', value=Constant(value=2))])
x = (1 + 2) * 3
HI HEY
20 20
SyntaxError: expected ':' line 1 offset 5
```

- Verdict: native. The `ast` module gives the abstract syntax tree, where a keyword argument is a `keyword` node. `ast.unparse` prints the tree in a standard form, so the extra parentheses and spaces go away. A decorator is sugar for a call and an assignment, and `xs[1]` is sugar for `__getitem__`. A syntax error has a line and an offset.

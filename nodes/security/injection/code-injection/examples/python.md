```python wrong id=eval
def calculate(expression):
    # The feature: a calculator for the formulas that users type.
    return eval(expression)


print(calculate("6 * 7"))
print(calculate("__import__('platform').python_implementation()"))
```

```output for=eval
42
CPython
```

The second call loads the module `platform` and calls one of its functions. The calculator never offered either.

```python run id=ast-walk
import ast
import operator

OPERATORS = {
    ast.Add: operator.add,
    ast.Sub: operator.sub,
    ast.Mult: operator.mul,
    ast.Div: operator.truediv,
    ast.USub: operator.neg,
}


def evaluate(node):
    if isinstance(node, ast.Constant) and type(node.value) in (int, float):
        return node.value
    if isinstance(node, ast.BinOp) and type(node.op) in OPERATORS:
        return OPERATORS[type(node.op)](evaluate(node.left), evaluate(node.right))
    if isinstance(node, ast.UnaryOp) and type(node.op) in OPERATORS:
        return OPERATORS[type(node.op)](evaluate(node.operand))
    raise ValueError(f"not allowed: {type(getattr(node, 'op', node)).__name__}")


def calculate(expression):
    if len(expression) > 100:
        raise ValueError("expression too long")
    tree = ast.parse(expression, mode="eval")
    return evaluate(tree.body)


for text in ["6 * 7", "(1 + 2) / 4", "__import__('platform').python_implementation()", "2 ** 10"]:
    try:
        print(text, "=", calculate(text))
    except (ValueError, SyntaxError, ZeroDivisionError) as error:
        print(text, "->", error)
```

```output for=ast-walk
6 * 7 = 42
(1 + 2) / 4 = 0.75
__import__('platform').python_implementation() -> not allowed: Call
2 ** 10 -> not allowed: Pow
```

The walk accepts only the node types in the allow-list. A call, an attribute, a name or a power fails before any evaluation.

```python wrong id=format-string
class User:
    def __init__(self, name, password_hash):
        self.name = name
        self.password_hash = password_hash


user = User("alice", "placeholder-hash-not-real")


def greeting(template):
    # The feature: each user writes the text of a greeting.
    return template.format(user=user)


print(greeting("Hello, {user.name}!"))
print(greeting("Hello, {user.password_hash}!"))
```

```output for=format-string
Hello, alice!
Hello, placeholder-hash-not-real!
```

`str.format` reads attributes with `.name` and items with `[index]`. A user who writes the format string can read any attribute of the objects that the program passes in.

```python run id=template-string
import string


class User:
    def __init__(self, name, password_hash):
        self.name = name
        self.password_hash = password_hash


user = User("alice", "placeholder-hash-not-real")


def greeting(template):
    # Only the values in this dictionary can appear in the text.
    values = {"name": user.name}
    return string.Template(template).safe_substitute(values)


print(greeting("Hello, $name!"))
print(greeting("Hello, $password_hash!"))
print(greeting("Hello, {user.password_hash}!"))
```

```output for=template-string
Hello, alice!
Hello, $password_hash!
Hello, {user.password_hash}!
```

`string.Template` replaces `$name` with a value from the dictionary and has no attribute access. A placeholder with no value stays in the text, and braces are plain text.

- Verdict: stdlib. Python has no safe form of `eval`. The `ast` module parses an expression into a tree that the program can check and evaluate itself.
- The documentation of `eval` says that overriding `__builtins__` is not a security mechanism. Do not pass input to `eval` or `exec` with a reduced set of globals and call it safe.
- `ast.literal_eval` accepts only literals, but the documentation does not recommend it for untrusted data, because a small input can exhaust memory or the C stack. Use `json.loads` for data.
- `compile`, `exec`, `eval`, `pickle.loads` and `importlib.import_module` with a module name from input all run code. The last two belong to [[deserialization-of-untrusted-data]] and to unsafe reflection.
- Template engines from PyPI, such as Jinja2, can run more than `str.format` can. Keep their template text in the program, and pass input as context values.
- Ruff rules S307 (`eval`) and S102 (`exec`) come from flake8-bandit. CodeQL has `py/code-injection` and `py/template-injection`.

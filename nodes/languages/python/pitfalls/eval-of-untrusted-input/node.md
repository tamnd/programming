---
tag: NEW-python-eval-of-untrusted-input
kind: feature
owner: python
slug: eval-of-untrusted-input
name: eval of untrusted input
aliases: [eval injection, exec of user input, eval security, ast.literal_eval, safe eval, CWE-95, S307, W0123, eval-used]
area: languages/python/pitfalls
level: core
summary: >-
  eval and exec run any Python code in the string, and an empty __builtins__ does not stop that. For data, use ast.literal_eval or json. For a small formula language, walk the ast with an allow list.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/functions.html#eval", locator: "eval(); exec()"}
proposals: []
see_also: [python/eval-and-exec, python/the-ast-module, python/pickle-of-untrusted-data, python/shell-true-injection, python/saving-structured-data-with-json, python/type-conversions, code-injection]
sources:
  - url: https://docs.python.org/3.15/library/functions.html#eval
    title: Built-in Functions, Python 3.15 documentation
    locator: eval(); exec()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/ast.html#ast.literal_eval
    title: ast, Abstract syntax trees, Python 3.15 documentation
    locator: ast.literal_eval()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/suspicious-eval-usage/
    title: suspicious-eval-usage (S307), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/warning/eval-used.html
    title: eval-used / W0123, Pylint documentation
    locator: Message emitted
    accessed: 2026-11-18
  - url: https://cwe.mitre.org/data/definitions/95.html
    title: CWE-95, Improper Neutralization of Directives in Dynamically Evaluated Code
    locator: Description
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=calculator
def calculate(text):
    return eval(text)


print(calculate("2 + 3 * 4"))
print(calculate("__import__('sys').version_info >= (3,)"))
```

```output for=calculator
14
True
```

The "calculator" also imported a module and read from it. Any text that a user types runs as Python code with the rights of the program.

## Semantics

**r1.** The documentation of `eval` and `exec` says: "This function executes arbitrary code. Calling it with untrusted user-supplied input will lead to security vulnerabilities."[^1] Example: [`calculator`](#calculator).

**r2.** This is CWE-95. "The product receives input from an upstream component, but it does not neutralize or incorrectly neutralizes code syntax before using the input in a dynamic evaluation call (e.g. "eval")."[^5] For the general class of flaw, see [[code-injection]].

**r3.** A restricted namespace is not a fix. "Overriding __builtins__ can be used to restrict or change the available names, but this is not a security mechanism: the executed code can still access all builtins."[^1] Code can reach classes and functions through the attributes of any object, such as `().__class__`. Example: [`empty-builtins`](#empty-builtins).

**r4.** For Python literals, use `ast.literal_eval`. It accepts "a string containing only a Python literal or container display", made of "strings, bytes, numbers, tuples, lists, dicts, sets, booleans, None and Ellipsis."[^2] "This is specifically designed not to execute Python code, unlike the more general eval(). There is no namespace, no name lookups, or ability to call out."[^2] Example: [`literal-eval`](#literal-eval).

**r5.** `ast.literal_eval` is still not free of risk: "A relatively small input can lead to memory exhaustion or to C stack exhaustion, crashing the process."[^2] Limit the length of the input. For data that crosses a network, a format such as JSON with its own parser is often the better choice.

**r6.** For a small expression language, parse with `ast.parse(text, mode="eval")` and walk the tree. Accept only the node types and operators on an allow list, and compute the result yourself. Nothing in the text runs. Example: [`allow-list`](#allow-list).

**r7.** The linters:

- Ruff S307 "Checks for uses of the builtin eval() function."[^3] Its reason: "The eval() function is insecure as it enables arbitrary code execution."[^3]
- Pylint W0123 `eval-used` has the message "Use of eval".[^4]

## Examples

```python run id=literal-eval
import ast

print(ast.literal_eval("[1, 2.5, {'a': (3, None)}]"))
print(ast.literal_eval("-7"))
```

```output for=literal-eval
[1, 2.5, {'a': (3, None)}]
-7
```

`ast.literal_eval` builds the value without running code (rule r4).

```python run id=allow-list
import ast
import operator

OPS = {ast.Add: operator.add, ast.Sub: operator.sub, ast.Mult: operator.mul, ast.Div: operator.truediv}


def calculate(text):
    def walk(node):
        if isinstance(node, ast.Constant) and type(node.value) in (int, float):
            return node.value
        if isinstance(node, ast.BinOp) and type(node.op) in OPS:
            return OPS[type(node.op)](walk(node.left), walk(node.right))
        raise ValueError(f"not allowed: {type(node).__name__}")

    if len(text) > 200:
        raise ValueError("too long")
    return walk(ast.parse(text, mode="eval").body)


print(calculate("2 + 3 * 4"))
try:
    calculate("__import__('sys')")
except ValueError as e:
    print(e)
```

```output for=allow-list
14
not allowed: Call
```

The function accepts numbers and four operators. A call is a node that is not on the list, so it is rejected before anything runs (rule r6).

## Versions

The risk is the same in all Python versions. It follows from rule r1.

## Pitfalls

```python wrong id=empty-builtins
text = "().__class__.__base__.__subclasses__()"
found = eval(text, {"__builtins__": {}})
print(type(found).__name__, len(found) > 10)
```

```output for=empty-builtins
list True
```

With no builtins, the text still reached `object` and the list of all its subclasses. From there, code can find classes that open files or run commands (rule r3).

```python fails id=literal-eval-rejects expect=exception:ValueError
import ast

ast.literal_eval("__import__('os').system('echo hi')")
```

`ast.literal_eval` raises `ValueError` for a call, so the command does not run (rule r4). It can also raise `TypeError`, `SyntaxError`, `MemoryError` and `RecursionError` for bad input, so catch these too.

## Comparison

In Python, `eval` runs the full language, and the language can reach almost any object through attributes (rule r3). So no simple filter on the text or on the namespace makes it safe. Parse the data with a parser that can only build values (rules r4 and r6). For `eval` and `exec` in general, see [[python/eval-and-exec]]. For the ast module, see [[python/the-ast-module]].

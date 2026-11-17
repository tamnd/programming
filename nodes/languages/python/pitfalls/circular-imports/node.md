---
tag: J7K6W0
kind: feature
owner: python
slug: circular-imports
name: Circular imports
aliases: [circular import, cyclic import, import cycle, partially initialized module, cannot import name, R0401, cyclic-import]
area: languages/python/pitfalls
level: advanced
summary: >-
  When two modules import each other, one of them sees the other before it has finished running. A from import or an attribute read at the top level then fails. Restructure, or move the import.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html#loading", locator: "5.4 Loading"}
proposals: []
see_also: [python/the-import-statement, python/modules, python/import-conventions, python/lazy-imports, python/the-module-search-path, python/intra-package-references, python/annotations-at-runtime, python/startup-time]
sources:
  - url: https://docs.python.org/3.15/reference/import.html#loading
    title: The Python Language Reference, Python 3.15 documentation
    locator: 5.3.1 The module cache; 5.4 Loading
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/faq/programming.html#what-are-the-best-practices-for-using-import-in-a-module
    title: Programming FAQ, Python 3.15 documentation
    locator: What are the best practices for using import in a module?
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/refactor/cyclic-import.html
    title: cyclic-import / R0401, Pylint documentation
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.15.html
    title: What’s New In Python 3.15
    locator: "PEP 810: Explicit lazy imports"
    accessed: 2026-11-18
---

## Synopsis

```python fails id=from-import-cycle expect=exception:ImportError
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "orders.py").write_text(
        "from customers import Customer\n"
        "class Order:\n"
        "    pass\n"
    )
    pathlib.Path(folder, "customers.py").write_text(
        "from orders import Order\n"
        "class Customer:\n"
        "    pass\n"
    )
    sys.path.insert(0, folder)
    import orders
```

`orders` imports `customers`, and `customers` asks `orders` for the name `Order` before `orders` has defined it.

## Semantics

**r1.** An import first looks in a cache. "The first place checked during import search is sys.modules. This mapping serves as a cache of all modules that have been previously imported".[^1]

**r2.** A module goes into the cache before its code runs. "The module will exist in sys.modules before the loader executes the module code. This is crucial because the module code may (directly or indirectly) import itself; adding it to sys.modules beforehand prevents unbounded recursion in the worst case and multiple loading in the best."[^1] So a cycle does not loop forever. The second import gets the module object that is still running.

**r3.** That module object is partly filled. It has only the names that its code has bound so far. The FAQ states the result: "Circular imports are fine where both modules use the “import <module>” form of import. They fail when the 2nd module wants to grab a name out of the first (“from module import name”) and the import is at the top level. That’s because names in the 1st are not yet available, because the first module is busy importing the 2nd."[^2] Example: [`from-import-cycle`](#from-import-cycle).

**r4.** The form `import module` binds only the module object, so it works in a cycle. A read of an attribute at the top level, such as `orders.Order` in a class statement, still fails with `AttributeError`, because the name is not there yet. Examples: [`module-form`](#module-form), [`top-level-attribute`](#top-level-attribute).

**r5.** Fix 1, restructure. Move the code that both modules need into a third module that imports neither of them. This removes the cycle.

**r6.** Fix 2, a late import. "if the second module is only used in one function, then the import can easily be moved into that function. By the time the import is called, the first module will have finished initializing, and the second module can do its import."[^2] Example: [`late-import`](#late-import).

**r7.** Pylint reports R0401 `cyclic-import`, "Used when a cyclic import between two or more modules is detected."[^3]

## Examples

```python run id=module-form
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "orders.py").write_text(
        "import customers\n"
        "class Order:\n"
        "    def owner(self):\n"
        "        return customers.Customer()\n"
    )
    pathlib.Path(folder, "customers.py").write_text(
        "import orders\n"
        "class Customer:\n"
        "    def first_order(self):\n"
        "        return orders.Order()\n"
    )
    sys.path.insert(0, folder)
    import orders

    print(type(orders.Order().owner()).__name__)
    print(type(orders.customers.Customer().first_order()).__name__)
```

```output for=module-form
Customer
Order
```

Both modules use `import module`, and they read the attributes only inside functions, so the cycle works (rule r4).

```python run id=late-import
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "orders.py").write_text(
        "from customers import Customer\n"
        "class Order:\n"
        "    customer = Customer\n"
    )
    pathlib.Path(folder, "customers.py").write_text(
        "class Customer:\n"
        "    def new_order(self):\n"
        "        from orders import Order\n"
        "        return Order()\n"
    )
    sys.path.insert(0, folder)
    import orders

    print(type(orders.Customer().new_order()).__name__)
```

```output for=late-import
Order
```

`customers` imports `orders` only when `new_order` runs. By then, `orders` has finished (rule r6).

```python run id=partial-module
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "first.py").write_text(
        "A = 1\n"
        "import second\n"
        "B = 2\n"
    )
    pathlib.Path(folder, "second.py").write_text(
        "import first\n"
        "print(sorted(n for n in vars(first) if n.isupper()))\n"
    )
    sys.path.insert(0, folder)
    import first

    print(sorted(n for n in vars(first) if n.isupper()))
```

```output for=partial-module
['A']
['A', 'B']
```

While `second` runs, the module `first` has only `A` (rules r2 and r3). After the import ends, it also has `B`.

## Versions

> [!since v="3.15"]
> The new soft keyword `lazy` changes when a module loads. "When you mark an import as lazy, Python defers the actual module loading until the imported name is first used."[^4] "The lazy keyword works with both import and from".[^4] So `lazy from customers import Customer` does not run `customers` at the import line. "Lazy imports are only permitted at module scope".[^4] For details, see [[python/lazy-imports]].

## Pitfalls

```python fails id=top-level-attribute expect=exception:AttributeError
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "orders.py").write_text(
        "import customers\n"
        "class Order:\n"
        "    pass\n"
    )
    pathlib.Path(folder, "customers.py").write_text(
        "import orders\n"
        "class Customer(orders.Order):\n"
        "    pass\n"
    )
    sys.path.insert(0, folder)
    import orders
```

The form `import orders` works, but the class statement reads `orders.Order` at the top level of `customers`, before `orders` has defined it (rule r4). Move `Order` to a module that does not import `customers`.

```python fails id=line-order expect=exception:ImportError
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "app.py").write_text(
        "import helpers\n"
        "NAME = 'app'\n"
    )
    pathlib.Path(folder, "helpers.py").write_text(
        "from app import NAME\n"
        "def greet():\n"
        "    return 'hi ' + NAME\n"
    )
    sys.path.insert(0, folder)
    import app
```

In `app`, the constant `NAME` comes after `import helpers`, so it does not exist yet when `helpers` asks for it. The order of the lines in a module matters in a cycle (rule r3).

## Comparison

In Python, a module is code that runs from the top, and an import runs that code. So a cycle is a problem of order at run time: the result depends on which module starts first and on the order of the lines in each module. The best fix is a structure with no cycle (rule r5). For the import system, see [[python/the-import-statement]]. For the order of imports in a file, see [[python/import-conventions]].

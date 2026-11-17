---
tag: 3GSAV3
kind: feature
owner: python
slug: module-objects
name: Module objects
aliases: [module object, types.ModuleType, module __dict__, __spec__, __loader__, __file__, module __getattr__, module __dir__, PEP 562, importlib.util.module_from_spec]
area: languages/python/modules-and-packages
level: advanced
summary: >-
  A module is an object with a namespace dictionary. The import system sets __name__, __spec__, __loader__, __file__ and __path__ from the spec, and a module can define __getattr__ and __dir__.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#modules", locator: "3.2.9. Modules"}
see_also: [python/modules, python/packages, python/namespace-packages, python/the-import-system, python/the-main-module]
sources:
- url: https://docs.python.org/3.15/reference/datamodel.html#modules
  title: The Python Language Reference, 3. Data model
  locator: 3.2.9. Modules; 3.2.9.1. Import-related attributes on module objects; 3.2.9.3. Module dictionaries; 3.3.2.1. Customizing module attribute access
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/types.html#types.ModuleType
  title: types — Dynamic type creation and names for built-in types
  locator: types.ModuleType
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/importlib.html#importlib.util.module_from_spec
  title: importlib — The implementation of import
  locator: importlib.util.module_from_spec
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/import
  title: import, JavaScript reference, MDN
  locator: Namespace import
  accessed: 2026-11-18
---

## Synopsis

```text
import shop.cart as m

m.x                       # the same as m.__dict__["x"]
m.x = 1                   # the same as m.__dict__["x"] = 1
m.__name__                # "shop.cart", or "__main__" for the program
m.__spec__                # the ModuleSpec: name, parent, origin, loader, submodule_search_locations
m.__file__, m.__loader__  # copies of spec fields; __path__ only on packages

# in a module: hooks for missing names and for dir()
def __getattr__(name): ...
def __dir__(): ...
```

## Semantics

**r1.** "A module object has a namespace implemented by a dictionary object (this is the dictionary referenced by the __globals__ attribute of functions defined in the module). Attribute references are translated to lookups in this dictionary, e.g., m.x is equivalent to m.__dict__["x"]."[^1] "Attribute assignment updates the module’s namespace dictionary".[^1] Example: [`namespace`](#namespace).

**r2.** `__dict__` "cannot be accessed as a global variable from within a module; it can only be accessed as an attribute on module objects."[^1] Code in the module uses `globals()` for the same dictionary. Example: [`namespace`](#namespace).

**r3.** The import system sets the import-related attributes from the spec: "these attributes are filled in based on the module’s spec, before the loader executes and loads the module."[^1] The reference advises: "With the exception of __name__, it is strongly recommended that you rely on __spec__ and its attributes instead of any of the other individual attributes listed in this subsection."[^1] Example: [`spec`](#spec).

**r4.** `__name__` is "The name used to uniquely identify the module in the import system."[^1] For a module that runs as the program, the value is `'__main__'`. `__path__` is for packages only: "Non-package modules should not have a __path__ attribute."[^1] `__file__` "might be missing for certain types of modules, such as C modules that are statically linked into the interpreter".[^1] Example: [`spec`](#spec).

**r5.** `types.ModuleType` is "The type of modules."[^2] "Modules created using the ModuleType constructor are created with many of their special attributes unset or set to default values."[^2] `importlib.util.module_from_spec()` "is preferred over using types.ModuleType to create a new module as spec is used to set as many import-controlled attributes on the module as possible."[^3] Example: [`create`](#create).

**r6.** A module can compute missing attributes. "If an attribute is not found on a module object through the normal lookup, i.e. object.__getattribute__(), then __getattr__ is searched in the module __dict__ before raising an AttributeError. If found, it is called with the attribute name and the result is returned."[^1] A module `__dir__` "should accept no arguments, and return an iterable of strings that represents the names accessible on module."[^1] Example: [`getattr-hook`](#getattr-hook).

**r7.** These hooks work only for attribute access: "Defining module __getattr__ and setting module __class__ only affect lookups made using the attribute access syntax – directly accessing the module globals (whether by code within the module, or via a reference to the module’s globals dictionary) is unaffected."[^1] Example: [`hook-not-global`](#hook-not-global).

## Examples

Module attributes are the entries of one dictionary (rules r1 and r2). A function of the module sees an assignment made from outside, because its `__globals__` is that dictionary.

```python norun reason=fragment file=counter.py group=namespace
count = 0


def bump():
    global count
    count += 1
    return count


print("__dict__" in globals(), "count" in globals())
```

```python run file=main.py group=namespace id=namespace
import counter

counter.bump()
print(counter.count, counter.__dict__["count"])

counter.count = 10
print(counter.bump())
print(counter.bump.__globals__ is counter.__dict__)
print(vars(counter) is counter.__dict__)
```

```output for=namespace
False True
1 1
11
True
True
```

The spec holds the import state, and the module attributes repeat parts of it (rules r3 and r4). The built-in module `sys` has no file.

```python norun reason=fragment file=shop/__init__.py group=spec
"""The shop package."""
```

```python norun reason=fragment file=shop/cart.py group=spec
ITEMS = []
```

```python run file=main.py group=spec id=spec
import os
import sys

import shop.cart

cart = shop.cart
spec = cart.__spec__
print(cart.__name__, spec.name, spec.parent)
print(os.path.relpath(spec.origin), os.path.relpath(cart.__file__))
print(type(spec.loader).__name__, cart.__loader__ is spec.loader)
print(spec.submodule_search_locations)
print([os.path.relpath(p) for p in shop.__spec__.submodule_search_locations])
print(hasattr(cart, "__path__"), hasattr(shop, "__path__"))
print(hasattr(sys, "__file__"), sys.__spec__.origin)
```

```output for=spec
shop.cart shop.cart shop
shop/cart.py shop/cart.py
SourceFileLoader True
None
['shop']
False True
False built-in
```

A module made with `types.ModuleType` has no spec and no loader. `module_from_spec` sets them from a spec (rule r5).

```python norun reason=fragment file=tools.py group=create
def double(x):
    return 2 * x
```

```python run file=main.py group=create id=create
import importlib.util
import os
import types

plain = types.ModuleType("plain", "A module made by hand.")
print(plain.__name__, plain.__doc__, plain.__spec__, plain.__loader__)
exec("def hello():\n    return 'hi from ' + __name__", plain.__dict__)
print(plain.hello())

spec = importlib.util.spec_from_file_location("tools", "tools.py")
tools = importlib.util.module_from_spec(spec)
spec.loader.exec_module(tools)
print(tools.__name__, os.path.relpath(tools.__file__), type(tools.__loader__).__name__)
print(tools.double(4))
```

```output for=create
plain A module made by hand. None None
hi from plain
tools tools.py SourceFileLoader
8
```

A module `__getattr__` keeps an old name working with a warning, and `__dir__` hides it from `dir()` (rule r6). The old name is not in the module dictionary.

```python norun reason=fragment file=units.py group=getattr-hook
import warnings

METRE = 1.0
_RENAMED = {"meter": "METRE"}


def __getattr__(name):
    if name in _RENAMED:
        new = _RENAMED[name]
        warnings.warn(f"{name} is deprecated; use {new}", DeprecationWarning, stacklevel=2)
        return globals()[new]
    raise AttributeError(f"module {__name__!r} has no attribute {name!r}")


def __dir__():
    return ["METRE"]
```

```python run file=main.py group=getattr-hook id=getattr-hook
import warnings

import units

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    print(units.meter)

print(caught[0].category.__name__, caught[0].message)
print(dir(units))
print("meter" in vars(units))
```

```output for=getattr-hook
1.0
DeprecationWarning meter is deprecated; use METRE
['METRE']
False
```

The examples give the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Versions

The reference says of `__cached__`: "Changed in version 3.15: The __cached__ attribute is no longer set on modules or taken into consideration by the import system or standard library."[^1] For `shop.cart` in [`spec`](#spec), `"__cached__" in vars(cart)` is `True` on CPython 3.12.3, 3.13.14 and 3.14.6, and `False` on CPython 3.15.0b2. Use `cart.__spec__.cached` instead.

## Pitfalls

### The module `__getattr__` is not used for globals

A module `__getattr__` runs for `module.name` only (rule r7). Code inside the module that uses the old name as a global gets a `NameError`.

```python norun reason=fragment file=units.py group=hook-not-global
METRE = 1.0


def __getattr__(name):
    if name == "meter":
        return METRE
    raise AttributeError(name)


def length(n):
    return n * meter
```

```python fails file=main.py group=hook-not-global id=hook-not-global expect=exception:NameError
import units

print(units.meter)
print(units.length(3))
```

Inside the module, use the real name.

### A changed spec does not change the module

The attributes are copied from the spec when the module is made (rule r3). The reference warns: "Note that updating an attribute on __spec__ will not update the corresponding attribute on the module itself".[^1]

```python run id=spec-copy
import json

json.__spec__.name = "other"
print(json.__name__, json.__spec__.name)

json.__name__ = "renamed"
print(json.__name__, json.__spec__.name)
```

```output for=spec-copy
json other
renamed other
```

Do not change these attributes on a module that other code uses.

## Comparison

**JavaScript** gives a module namespace object for `import * as myModule`. MDN says: "myModule is a sealed object with null prototype."[^4] Code cannot add a property to it. A Python module is an ordinary object, and `m.x = 1` from any module changes its namespace (rule r1).

## Implementation notes

"A module object does not contain the code object used to initialize the module (since it isn’t needed once the initialization is done)."[^1] The reference also notes a CPython detail of module dictionaries: "Because of the way CPython clears module dictionaries, the module dictionary will be cleared when the module falls out of scope even if the dictionary still has live references. To avoid this, copy the dictionary or keep the module around while using its dictionary directly."[^1]

## References

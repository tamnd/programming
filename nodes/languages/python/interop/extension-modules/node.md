---
tag: E9475P
kind: feature
owner: python
slug: extension-modules
name: Extension modules
aliases: [C extension, C extension module, PyInit, PyModExport, export hook, PyModuleDef, multi-phase initialization, single-phase initialization, Py_mod_exec, Py_mod_gil, meson-python, setuptools Extension, EXTENSION_SUFFIXES]
area: languages/python/interop
level: advanced
summary: >-
  An extension module is a shared library that CPython imports like a Python module. It exports an export hook or a PyInit function, Python makes the module in phases, and a build backend compiles it.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/c-api/extension-modules.html", locator: "Python/C API reference manual, Defining extension modules"}
see_also: [python/the-python-c-api, python/the-limited-api-and-stable-abi, python/native-acceleration, python/free-threaded-python, python/subinterpreters]
sources:
  - url: https://docs.python.org/3/c-api/extension-modules.html
    title: Defining extension modules
    locator: introduction; Extension export hook; Multi-phase initialization; Multiple module instances; PyInit function
    accessed: 2026-11-17
  - url: https://docs.python.org/3/extending/first-extension-module.html
    title: Your first C API extension module
    locator: Running your build tool; Module export hook; Appendix, Workaround for missing PyInit function
    accessed: 2026-11-17
  - url: https://docs.python.org/3/c-api/module.html
    title: Module Objects
    locator: Py_mod_gil; Py_mod_exec
    accessed: 2026-11-17
  - url: https://peps.python.org/pep-0489/
    title: "PEP 489: Multi-phase extension module initialization"
    locator: Abstract
    accessed: 2026-11-17
  - url: https://peps.python.org/pep-0793/
    title: "PEP 793: PyModExport: A new entry point for C extension modules"
    locator: Abstract
    accessed: 2026-11-17
  - url: https://setuptools.pypa.io/en/latest/userguide/ext_modules.html
    title: Building Extension Modules
    locator: introduction
    accessed: 2026-11-17
  - url: https://mesonbuild.com/meson-python/
    title: meson-python
    locator: introduction
    accessed: 2026-11-17
  - url: https://docs.python.org/3/extending/index.html
    title: Extending and Embedding the Python Interpreter
    locator: introduction, the note
    accessed: 2026-11-17
  - url: https://nodejs.org/api/n-api.html
    title: Node-API
    locator: introduction
    accessed: 2026-11-17
  - url: https://docs.python.org/3/c-api/stable.html
    title: C API and ABI Stability
    locator: introduction
    accessed: 2026-11-17
  - url: https://docs.python.org/3/whatsnew/3.15.html
    title: What’s new in Python 3.15
    locator: C API changes, New features
    accessed: 2026-11-17
---

## Synopsis

```c norun reason=platform
#include <Python.h>

static PyObject *
spam_twice(PyObject *module, PyObject *arg)
{
    return PyNumber_Add(arg, arg);   /* a new reference, or NULL */
}

static PyMethodDef spam_methods[] = {
    {"twice", spam_twice, METH_O, "Return arg + arg."},
    {NULL, NULL, 0, NULL}
};

static int
spam_exec(PyObject *module)
{
    return PyModule_AddIntConstant(module, "ANSWER", 42);
}

static PyModuleDef_Slot spam_module_slots[] = {
    {Py_mod_exec, spam_exec},
    {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
    {0, NULL}
};

static struct PyModuleDef spam_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "spam",
    .m_doc = "An example module.",
    .m_size = 0,
    .m_methods = spam_methods,
    .m_slots = spam_module_slots,
};

PyMODINIT_FUNC
PyInit_spam(void)
{
    return PyModuleDef_Init(&spam_module);
}
```

An extension module is a module that is written in C, C++ or another compiled language. CPython loads it from a shared library file, and Python code imports it with a normal `import` statement. The file above makes a module `spam` with a function `twice` and a constant `ANSWER`. After a build, `spam.twice(21)` gives `42` and `spam.twice('ab')` gives `'abab'`. The function `PyInit_spam` gives the module definition to Python, and Python makes the module in phases.

## Semantics

**r1.** An extension module is a shared library that "exports an export hook function (or an old-style initialization function)."[^1] To be found by the default importer, the file "must be named after the module name plus an extension listed in importlib.machinery.EXTENSION_SUFFIXES."[^1] The name of the hook comes from the name of the module: "For modules with ASCII-only names, the export hook must be named PyModExport_<name>, with <name> replaced by the module’s name."[^1] Examples: [`suffixes`](#suffixes), [`hook`](#hook), [`nohook`](#nohook).

**r2.** "The export hook returns an array of PySlot entries, terminated by an entry with a slot ID of 0. These slots describe how the module should be created and initialized."[^1] "This array must remain valid and constant until interpreter shutdown."[^1] PEP 793 added this hook. It removes "the most common obstacle to making one compiled library file usable with both regular and free-threaded builds of CPython."[^5] The C code is in [Examples](#examples).

**r3.** A module can also define the older function `PyInit_<name>`. For this function, the documentation says: "This functionality will not get new features, but there are no plans to remove it."[^1] "Normally, the initialization function (PyInit_modulename) returns a PyModuleDef instance with non-NULL m_slots. This allows Python to use multi-phase initialization."[^1] "If a module exports both PyInit_<name> and PyModExport_<name>, the PyInit_<name> function is ignored."[^1] The synopsis uses `PyInit_spam`, so it builds with older versions of CPython too.

**r4.** In multi-phase initialization, Python first gets the slots and checks the capabilities of the module. By default, "Python itself then creates the module object" and "sets initial module attributes like __package__ and __loader__, and inserts the module object into sys.modules."[^1] Last, the module is initialized, which is "the equivalent of __init__() when creating an object, or of executing top-level code in a Python-language module. The behavior is specified using the Py_mod_exec slot."[^1] PEP 489 brought this design, "with parallels to __new__ and __init__ of classes."[^4]

**r5.** "By default, extension modules are not singletons."[^1] "if the sys.modules entry is removed and the module is re-imported, a new module object is created and, typically, populated with fresh method and type objects."[^1] So "each instance of an extension module should be isolated".[^1] The legacy single-phase initialization makes the whole module in one function. It is "a legacy mechanism to initialize extension modules, with known drawbacks and design flaws."[^1] Example: [`instances`](#instances).

**r6.** The slot `Py_mod_gil` tells a free-threaded build if the module needs the GIL. `Py_MOD_GIL_NOT_USED` means: "The module is safe to run without an active GIL."[^3] "If Py_mod_gil is not specified, the import machinery defaults to Py_MOD_GIL_USED."[^3] On a free-threaded build, the slot "determines whether or not importing this module will cause the GIL to be automatically enabled."[^3] Also, "A module may also be limited to the main interpreter using the Py_mod_multiple_interpreters slot."[^1]

**r7.** "CPython itself does not come with a tool to build extension modules; it is recommended to use a third-party project for this."[^2] "Setuptools can build C/C++ extension modules."[^6] meson-python is "A Python package build backend leveraging the Meson build system."[^7] It "is thus best suited for Python packages building extension modules in compiled languages."[^7] It enables "Python build front-ends such as pip and build to build and install Python packages based on a Meson build definition."[^7]

**r8.** "The C extension interface is specific to CPython, and extension modules do not work on other Python implementations."[^8] A compiled module also matches one minor version of CPython: "code compiled for Python 3.10.0 will work on 3.10.8 and vice versa, but will need to be compiled separately for 3.9.x and 3.11.x."[^10] The stable ABI is the exception. The page [The limited API and the stable ABI](/python/the-limited-api-and-stable-abi/) tells more.

## Examples

The same module with the export hook of PEP 793 (rule r2). This form needs the headers of CPython 3.15 or a later version:[^11]

```c norun reason=fragment
PyABIInfo_VAR(abi_info);

static PySlot spam_slots[] = {
    PySlot_STATIC_DATA(Py_mod_abi, &abi_info),
    PySlot_STATIC_DATA(Py_mod_name, "spam"),
    PySlot_STATIC_DATA(Py_mod_doc, "An example module."),
    PySlot_STATIC_DATA(Py_mod_methods, spam_methods),
    PySlot_FUNC(Py_mod_exec, spam_exec),
    PySlot_DATA(Py_mod_gil, Py_MOD_GIL_NOT_USED),
    PySlot_END
};

PyMODEXPORT_FUNC PyModExport_spam(void);

PyMODEXPORT_FUNC
PyModExport_spam(void)
{
    return spam_slots;
}
```

The `Py_mod_abi` slot and the `PyABIInfo_VAR` macro "are a bit of boilerplate that helps prevent extensions compiled for a different version of Python from crashing the interpreter."[^2]

To build the module with meson-python, put a `pyproject.toml` file and a `meson.build` file next to the C file. This is the `pyproject.toml` file:

```toml
[build-system]
build-backend = 'mesonpy'
requires = ['meson-python']

[project]
name = 'spam'
version = '0.1'
```

This is the `meson.build` file:

```text
project('spam', 'c')
py = import('python').find_installation(pure: false)
py.extension_module('spam', 'spammodule.c', install: true)
```

Then `python -m pip install .` compiles the module and installs it. With setuptools, the `pyproject.toml` file has a table `[tool.setuptools]` with the list `ext-modules`, and each item has a `name` and a list of `sources`.[^6]

The importer finds an extension module by its file name. The suffixes of the current interpreter are in `importlib.machinery.EXTENSION_SUFFIXES`. The first suffix names one CPython version, `.abi3.so` is the suffix of the stable ABI, and `.so` is the plain suffix:

```python run id=suffixes
import importlib.machinery

suffixes = importlib.machinery.EXTENSION_SUFFIXES
print(suffixes[0].startswith('.cpython-'))
print('.abi3.so' in suffixes)
print(suffixes[-1] == '.so')
```

```output for=suffixes
True
True
True
```

The documentation gives this function for the name of the export hook (rule r1). A name that is not ASCII uses Punycode and the prefix `PyModExportU_`:

```python run id=hook
def hook_name(name):
    try:
        suffix = b'_' + name.encode('ascii')
    except UnicodeEncodeError:
        suffix = b'U_' + name.encode('punycode').replace(b'-', b'_')
    return b'PyModExport' + suffix

for name in ['spam', 'café']:
    print(name, hook_name(name).decode())
```

```output for=hook
spam PyModExport_spam
café PyModExportU_caf_dma
```

A shared library that has no hook is not an extension module. This example copies the C math library `libm` to a file `spam.so` and imports it. The import finds the file, but it does not find the function. The text of the error is not the same in each version of CPython, so the example prints only a test:

```python run id=nohook
import os
import shutil
import sys
import tempfile

with open('/proc/self/maps') as maps:
    libm = next(line.split()[-1] for line in maps if '/libm.so' in line)
folder = tempfile.mkdtemp()
shutil.copy(libm, os.path.join(folder, 'spam.so'))
sys.path.insert(0, folder)
try:
    import spam
except ImportError as error:
    print('ImportError')
    print('PyInit_spam' in str(error))
```

```output for=nohook
ImportError
True
```

The `array` module of the standard library uses multi-phase initialization. A new import makes a new module object with new type objects (rule r5):

```python run id=instances
import sys

import array as first
del sys.modules['array']
import array as second

print('new module object:', first is not second)
print('new type object:', first.array is not second.array)
```

```output for=instances
new module object: True
new type object: True
```

## Pitfalls

Do not keep Python objects in C global variables. A second module object, in a subinterpreter or after a new import, then shares the objects with the first one. Keep the state in the module object, or block a second initialization (rule r5).

Set `Py_mod_gil` only when the code is safe without the GIL. The value `Py_MOD_GIL_NOT_USED` is a promise that the module does its own locking (rule r6).

Put the `#include <Python.h>` line before the other includes. The tutorial says: "Be sure to put stdlib.h, and any other standard library includes, after Python.h."[^2]

Make each C function `static`, except the hook. "CPython only needs to access the export hook; all global variables and all other functions should generally be static, so that they don’t clash with other extensions."[^2]

A build tool can expect the function `PyInit_<name>` even when the module has the new hook. The tutorial gives a small function `PyInit_spam` that returns `NULL` as a workaround, because "some build tools may still assume that all extension modules need to define it."[^2]

Do not write an extension module only to call a C library. The `ctypes` module or the cffi library can call it. The documentation says that these modules "are more portable between implementations of Python than writing and compiling a C extension module."[^8] See [The ctypes module](/python/ctypes/).

## Comparison

Node.js has native addons, and Node-API is the C interface for them. "This API will be Application Binary Interface (ABI) stable across versions of Node.js."[^9] It allows "modules compiled for one major version to run on later major versions of Node.js without recompilation."[^9] The full Python C API does not give this promise: an extension module is built for one CPython version, unless it uses only the limited API (rule r8).

## References

[^1]: [Defining extension modules](https://docs.python.org/3/c-api/extension-modules.html)
[^2]: [Your first C API extension module](https://docs.python.org/3/extending/first-extension-module.html)
[^3]: [Module Objects](https://docs.python.org/3/c-api/module.html)
[^4]: [PEP 489: Multi-phase extension module initialization](https://peps.python.org/pep-0489/)
[^5]: [PEP 793: PyModExport: A new entry point for C extension modules](https://peps.python.org/pep-0793/)
[^6]: [Building Extension Modules](https://setuptools.pypa.io/en/latest/userguide/ext_modules.html)
[^7]: [meson-python](https://mesonbuild.com/meson-python/)
[^8]: [Extending and Embedding the Python Interpreter](https://docs.python.org/3/extending/index.html)
[^9]: [Node-API](https://nodejs.org/api/n-api.html)
[^10]: [C API and ABI Stability](https://docs.python.org/3/c-api/stable.html)
[^11]: [What’s new in Python 3.15](https://docs.python.org/3/whatsnew/3.15.html)

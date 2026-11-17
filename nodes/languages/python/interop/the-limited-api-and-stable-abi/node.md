---
tag: 2BQKRB
kind: feature
owner: python
slug: the-limited-api-and-stable-abi
name: The limited API and the stable ABI
aliases: [Limited API, Stable ABI, Limited C API, abi3, abi3t, Py_LIMITED_API, Py_TARGET_ABI3T, abi3 wheel, python3.dll, Py_PACK_VERSION, Py_mod_abi, PEP 384, PEP 652, PEP 803]
area: languages/python/interop
level: research
summary: >-
  The limited API is a subset of the C API. An extension that uses only this subset gets the stable ABI (abi3), so one binary loads on its target version of CPython and on all later 3.x versions.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/c-api/stable.html", locator: "Python/C API reference manual, C API and ABI Stability"}
see_also: [python/the-python-c-api, python/extension-modules, python/ctypes, python/free-threaded-python, python/native-acceleration]
sources:
  - url: https://docs.python.org/3/c-api/stable.html
    title: C API and ABI Stability
    locator: introduction; Stable Application Binary Interfaces; Compiling for Stable ABI; Stable ABI Scope and Performance; Stable ABI Caveats; ABI Checking
    accessed: 2026-11-17
  - url: https://peps.python.org/pep-0652/
    title: "PEP 652: Maintaining the Stable ABI"
    locator: header; Abstract; Stable ABI vs. Limited API
    accessed: 2026-11-17
  - url: https://peps.python.org/pep-0384/
    title: "PEP 384: Defining a Stable ABI"
    locator: header; Abstract; Rationale
    accessed: 2026-11-17
  - url: https://peps.python.org/pep-0803/
    title: "PEP 803: abi3t, Stable ABI for Free-Threaded Builds"
    locator: Abstract; Motivation; Opaque PyObject; the wheel tag section
    accessed: 2026-11-17
  - url: https://docs.python.org/3/whatsnew/3.15.html
    title: What's new in Python 3.15
    locator: "PEP 803: Stable ABI for free-threaded builds"
    accessed: 2026-11-17
  - url: https://packaging.python.org/en/latest/specifications/platform-compatibility-tags/
    title: Platform compatibility tags
    locator: ABI Tag; Use
    accessed: 2026-11-17
  - url: https://docs.python.org/3/c-api/apiabiversion.html
    title: API and ABI Versioning
    locator: Py_Version; Py_PACK_VERSION
    accessed: 2026-11-17
  - url: https://nodejs.org/api/n-api.html
    title: Node-API
    locator: introduction; Implications of ABI stability
    accessed: 2026-11-17
---

## Synopsis

```python run id=exported
import ctypes
import sys

def pack_version(major, minor):
    return (major << 24) | (minor << 16)

print(hex(pack_version(3, 13)))
version = ctypes.c_ulong.in_dll(ctypes.pythonapi, 'Py_Version').value
print(version == sys.hexversion, version >= pack_version(3, 11))
for name in ['PyList_GetItem', 'PyList_GET_ITEM']:
    print(name, hasattr(ctypes.pythonapi, name))
```

```output for=exported
0x30d0000
True True
PyList_GetItem True
PyList_GET_ITEM False
```

The limited API is a subset of the Python C API. The stable ABI is the promise that an extension that uses only this subset loads on later versions of CPython without a new build. The extension defines the macro `Py_LIMITED_API` before it includes `Python.h`, and the value of the macro is the lowest Python version that the extension supports. The build then makes one file, for example `spam.abi3.so`, in place of one file for each Python version. The example packs a version number in the format that the macro uses. It also reads the run-time version `Py_Version`, which is in the stable ABI, and shows that the function `PyList_GetItem` is exported from the Python library but the macro `PyList_GET_ITEM` is not.

## Semantics

**r1.** The two names are for two related things. "The Stable ABI is a promise that certain extensions compiled with CPython 3.x will be binary compatible with all subsequent versions of CPython 3.x."[^2] "The Limited API is a subset of CPython’s C API that produces such extensions."[^2] Without the stable ABI, the binary interface changes with each minor version: "code compiled for Python 3.10.0 will work on 3.10.8 and vice versa, but will need to be compiled separately for 3.9.x and 3.11.x."[^1]

**r2.** "There are two Stable ABIs:"[^1] abi3 is from Python 3.2, and abi3t is from Python 3.15.[^1] abi3 "is compatible with non-free-threaded builds of CPython."[^1] abi3t "is compatible with free-threaded builds of CPython. It has stricter API limitations than abi3."[^1] "It is possible for an extension to be compiled for both abi3 and abi3t at the same time; the result will be compatible with both free-threaded and non-free-threaded builds of Python."[^1]

**r3.** To use the limited API, define `Py_LIMITED_API` (for abi3) or `Py_TARGET_ABI3T` (for abi3t). Define it "to the lowest Python version your extension should support, in Py_PACK_VERSION format."[^1] "The macros must be defined before including Python.h."[^1] The value for Python 3.13 is `0x30d0000`, and the value for 3.15 is `0x30f0000`.[^1] "When one of the macros is defined, Python.h will only expose API that is compatible with the given Stable ABI".[^1] An older form is the value 3: it "works the same as 0x03020000 (Python 3.2, the version that introduced Stable ABI)."[^1] Example: [`exported`](#exported).

**r4.** "Each Stable ABI is versioned using the first two numbers of the Python version."[^1] "An extension compiled for Stable ABI 3.x is ABI-compatible with Python 3.x and above."[^1] The promise is for one major version: "It will not necessarily be compatible with CPython 4.0".[^2] The run-time version is in `Py_Version`, which is in the stable ABI from Python 3.11 on.[^7]

**r5.** The limited API leaves out the parts of the C API that depend on the layout of a structure. "For example, while PyList_GetItem() is available, its “unsafe” macro variant PyList_GET_ITEM() is not. The macro can be faster because it can rely on version-specific implementation details of the list object."[^1] "Compiling for Stable ABI disables this inlining, allowing stability as Python’s data structures are improved, but possibly reducing performance."[^1] "All functions in Stable ABI are present as functions in Python’s shared library, not solely as macros. This makes them usable in languages that don’t use the C preprocessor, including Python’s ctypes."[^1] Example: [`exported`](#exported).

**r6.** The file name and the wheel tag tell which ABI a binary uses. "On some platforms, Python will look for and load shared library files named with the abi3 or abi3t tag (for example, mymodule.abi3.so)."[^1] "On Windows, extensions that use a Stable ABI should be linked against python3.dll rather than a version-specific library such as python39.dll."[^1] For wheels, "The CPython stable ABI is abi3 as in the shared library suffix."[^6] The Python tag of an abi3 wheel gives the lowest version, so a wheel tagged `cp311-abi3` installs on 3.11 and later. Example: [`tags`](#tags).

**r7.** CPython does not fully check the ABI when it loads a module. "Python does not necessarily check that extensions it loads have compatible ABI."[^1] "the user (or their packaging tool) is ultimately responsible for ensuring that, for example, extensions built for Stable ABI 3.10 are not installed for lower versions of Python."[^1] Python 3.15 adds the slot `Py_mod_abi` and the function `PyABIInfo_Check`, so that "detectable incompatibilities raise exceptions rather than crash."[^1]

**r8.** The stable ABI keeps the binary interface, not the behavior. "Note that compiling for Stable ABI is not a complete guarantee that code will be compatible with the expected Python versions."[^1] "For these reasons, we recommend testing an extension with all minor Python versions it supports."[^1] Also, "the same source will not necessarily compile with Py_LIMITED_API set to 3.12", and "parts of the Limited API may be deprecated and removed, provided that Stable ABI stays stable."[^1]

**r9.** The free-threaded build had no stable ABI before 3.15. "Extensions built for GIL-enabled builds of CPython will fail to load (or crash) on free-threaded builds."[^4] PEP 803 adds abi3t: it "will be based on the existing Stable ABI (abi3), but make the PyObject structure opaque."[^4] With an opaque `PyObject`, "Expressions such as sizeof(PyObject) will no longer work."[^4] The source changes are "Switching to API introduced in PEP 697 (Python 3.12), such as negative basicsize and PyObject_GetTypeData(), rather than making PyObject part of the instance struct" and "Switching from a PyInit_ function to a new export hook, PyModExport_*".[^5] A wheel for both builds uses the compressed tag `abi3.abi3t`: "a wheel tagged cp315-abi3.abi3t will be compatible with 3.15, 3.16, and later versions".[^4]

**r10.** PEP 384 defined the stable ABI in 2009 for Python 3.2.[^3] Its reason was: "The primary source of ABI incompatibility are changes to the lay-out of in-memory structures."[^3] PEP 652, for Python 3.10, says that the limited API and the stable ABI "will be formalized in a single definitive file, tested, and documented."[^2]

## Examples

This C file is a module that defines `Py_LIMITED_API` as Python 3.13 (rule r3). It uses `PyList_GetItem`, which is in the limited API:

```c norun reason=platform
#define Py_LIMITED_API 0x030d0000
#include <Python.h>

static PyObject *
spam_first(PyObject *module, PyObject *list)
{
    PyObject *item = PyList_GetItem(list, 0);   /* in the limited API */
    return Py_XNewRef(item);
}

static PyMethodDef spam_methods[] = {
    {"first", spam_first, METH_O, "Return list[0]."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef_Slot spam_module_slots[] = {
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
    {0, NULL}
};

static struct PyModuleDef spam_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "spam",
    .m_methods = spam_methods,
    .m_slots = spam_module_slots,
};

PyMODINIT_FUNC
PyInit_spam(void)
{
    return PyModuleDef_Init(&spam_module);
}
```

Compile it one time with the headers of Python 3.13, and name the result `spam.abi3.so`:

```sh norun reason=platform
cc -shared -fPIC -O2 -I"$(python3.13 -c 'import sysconfig; print(sysconfig.get_path("include"))')" spam.c -o spam.abi3.so
```

On Linux, the same file then imports on CPython 3.13, 3.14 and 3.15, because `.abi3.so` is in the list `importlib.machinery.EXTENSION_SUFFIXES` of each version (rule r6). Python 3.15 also has `.abi3t.so` in the list. On Python 3.12, the import fails with `SystemError: module spam uses unknown slot ID 4`, because Python 3.13 added the slot `Py_mod_gil`. The file is for 3.13 and later only, and nothing stops a user from putting it on 3.12 (rule r7).

If you change the function to use the macro `PyList_GET_ITEM`, `Python.h` does not declare it, because the macro is not in the limited API (rule r5). GCC 13 gives only a warning about an implicit declaration of `PyList_GET_ITEM`, and the file still builds. Make this warning an error with `-Werror=implicit-function-declaration`, so that a call outside the limited API stops the build.

This model of the wheel tag rules tells if a wheel can install on a Python version (rules r6 and r9). A GIL-enabled build needs the tag `abi3`, and a free-threaded build needs the tag `abi3t`:

```python run id=tags
def fits(wheel, version, free_threaded=False):
    python_tag, abi_tag = wheel[:-len('.whl')].split('-')[2:4]
    lowest = (3, int(python_tag[len('cp3'):]))
    wanted = 'abi3t' if free_threaded else 'abi3'
    return wanted in abi_tag.split('.') and version >= lowest

wheels = ['spam-1.0-cp311-abi3-manylinux_2_28_x86_64.whl',
          'spam-1.0-cp315-abi3.abi3t-manylinux_2_28_x86_64.whl']
pythons = [((3, 10), False), ((3, 14), False), ((3, 15), False), ((3, 15), True)]
for wheel in wheels:
    tag = '-'.join(wheel.split('-')[2:4])
    for version, ft in pythons:
        name = '%d.%d%s' % (version[0], version[1], 't' if ft else '')
        print(tag, name, fits(wheel, version, ft))
```

```output for=tags
cp311-abi3 3.10 False
cp311-abi3 3.14 True
cp311-abi3 3.15 True
cp311-abi3 3.15t False
cp315-abi3.abi3t 3.10 False
cp315-abi3.abi3t 3.14 False
cp315-abi3.abi3t 3.15 True
cp315-abi3.abi3t 3.15t True
```

The model is simple. A real installer makes the full list of the tags that the interpreter supports, in order of preference, and picks the first wheel that matches.[^6]

## Pitfalls

Set `Py_LIMITED_API` to the lowest version that you support, not to the version of the headers that you build with. The documentation says: "Typically, you should choose a specific value rather than the version of the Python headers you are compiling against."[^1]

Do not trust a successful build as proof. Even with the macro defined, "a few private declarations are exposed for technical reasons (or even unintentionally, as bugs)."[^1] Check the documentation of each function that you use, and test the extension on each minor version that it supports (rule r8).

Do not install an abi3 file on a version lower than its target. Python can load it and then fail or crash (rule r7). Use the correct wheel tag, and in 3.15 add the `Py_mod_abi` slot.

Do not expect abi3 to load on the free-threaded build. Build for abi3t, or for both with the tag `abi3.abi3t`, or build a separate `cp315t` wheel (rule r9).

Expect a cost in speed. Measure the hot paths. You can ship a version-specific build next to the abi3 build: "A potentially faster version-specific extension can then be distributed alongside a version compiled for Stable ABI".[^1] The abi3 build is then the slower fallback that works on more versions.

## Comparison

Node.js has a similar layer for native add-ons. "This API will be Application Binary Interface (ABI) stable across versions of Node.js."[^8] It is intended to "allow modules compiled for one major version to run on later major versions of Node.js without recompilation."[^8] Like the stable ABI of Python, the promise covers only that API: "Although Node-API provides an ABI stability guarantee, other parts of Node.js do not".[^8] A difference is that Node-API is a separate API, while the limited API of Python is a subset of the full C API, so one source file can build for abi3 or for one version.

## References

[^1]: [C API and ABI Stability](https://docs.python.org/3/c-api/stable.html)
[^2]: [PEP 652: Maintaining the Stable ABI](https://peps.python.org/pep-0652/)
[^3]: [PEP 384: Defining a Stable ABI](https://peps.python.org/pep-0384/)
[^4]: [PEP 803: abi3t, Stable ABI for Free-Threaded Builds](https://peps.python.org/pep-0803/)
[^5]: [What's new in Python 3.15](https://docs.python.org/3/whatsnew/3.15.html)
[^6]: [Platform compatibility tags](https://packaging.python.org/en/latest/specifications/platform-compatibility-tags/)
[^7]: [API and ABI Versioning](https://docs.python.org/3/c-api/apiabiversion.html)
[^8]: [Node-API](https://nodejs.org/api/n-api.html)

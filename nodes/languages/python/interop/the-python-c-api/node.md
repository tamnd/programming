---
tag: GD5K89
kind: feature
owner: python
slug: the-python-c-api
name: The Python C API
aliases: [Python/C API, C API, Python.h, PyObject, new reference, borrowed reference, stolen reference, Py_INCREF, Py_DECREF, Py_BEGIN_ALLOW_THREADS, PyUnstable, C API Working Group, ctypes.pythonapi]
area: languages/python/interop
level: advanced
summary: >-
  The C API lets C code use Python objects through PyObject pointers. The code must own each reference that it keeps, check each result for an error, and hold the GIL when it touches an object.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/c-api/intro.html", locator: "Python/C API reference manual, Introduction"}
see_also: [python/reference-counting-in-cpython, python/the-global-interpreter-lock, python/free-threaded-python, python/extension-modules, python/the-limited-api-and-stable-abi, python/ctypes, reference-counting]
sources:
  - url: https://docs.python.org/3/c-api/intro.html
    title: Introduction
    locator: Objects, Types and Reference Counts; Reference Count Details; Exceptions
    accessed: 2026-11-17
  - url: https://docs.python.org/3/c-api/structures.html
    title: Common Object Structures
    locator: PyObject
    accessed: 2026-11-17
  - url: https://docs.python.org/3/c-api/list.html
    title: List Objects
    locator: PyList_GetItem; PyList_SetItem
    accessed: 2026-11-17
  - url: https://docs.python.org/3/c-api/threads.html
    title: Thread states and the global interpreter lock
    locator: introduction; Detaching the thread state from extension code
    accessed: 2026-11-17
  - url: https://docs.python.org/3/c-api/stable.html
    title: C API and ABI Stability
    locator: introduction; Unstable C API
    accessed: 2026-11-17
  - url: https://peps.python.org/pep-0731/
    title: "PEP 731: C API Working Group Charter"
    locator: Abstract
    accessed: 2026-11-17
  - url: https://peps.python.org/pep-0733/
    title: "PEP 733: An Evaluation of Python's Public C API"
    locator: Abstract; Introduction
    accessed: 2026-11-17
  - url: https://docs.python.org/3/library/ctypes.html
    title: ctypes, A foreign function library for Python
    locator: Loading shared libraries; PyDLL; pythonapi
    accessed: 2026-11-17
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions
    locator: id()
    accessed: 2026-11-17
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions
    locator: sys.getrefcount
    accessed: 2026-11-17
  - url: https://docs.python.org/3/extending/index.html
    title: Extending and Embedding the Python Interpreter
    locator: introduction, the note
    accessed: 2026-11-17
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/jni/design.html
    title: "Java Native Interface Specification: 2, Design Overview"
    locator: Referencing Java Objects; Global and Local References
    accessed: 2026-11-17
  - url: https://docs.python.org/3/extending/extending.html
    title: "Using the C API: Assorted topics"
    locator: Errors and Exceptions
    accessed: 2026-11-17
---

## Synopsis

```python run id=address
import ctypes

api = ctypes.pythonapi
api.PyList_GetItem.restype = ctypes.c_void_p
api.PyList_GetItem.argtypes = [ctypes.py_object, ctypes.c_ssize_t]

items = ['first', 'second']
address = api.PyList_GetItem(items, 0)
print(address == id(items[0]))
print(type(api).__name__)
```

```output for=address
True
PyDLL
```

The Python C API is the set of C functions, macros and types that CPython gives to C code. An extension module uses it to make Python objects, and a C program uses it to run Python. Each object is a `PyObject *` pointer. The C code must count the references that it keeps, check each call for an error, and hold the global interpreter lock (GIL) while it uses objects. The example calls the C function `PyList_GetItem` from Python through `ctypes.pythonapi`. The pointer that it gets is the same number as `id()` of the item.

## Semantics

**r1.** Most C API functions take and give `PyObject *`. "This type is a pointer to an opaque data type representing an arbitrary Python object."[^1] "All Python objects (even Python integers) have a type and a reference count."[^1] In a release build, the `PyObject` structure contains "only the object’s reference count and a pointer to the corresponding type object."[^2] "The members must not be accessed directly"; the C code uses macros such as `Py_REFCNT` and `Py_TYPE`.[^2] In CPython, `id()` gives the same pointer: "This is the address of the object in memory."[^9] Example: [`address`](#address).

**r2.** "Ownership pertains to references, never to objects (objects are not owned: they are always shared)."[^1] To own a reference "means being responsible for calling Py_DECREF on it when the reference is no longer needed."[^1] "When a function passes ownership of a reference on to its caller, the caller is said to receive a new reference. When no ownership is transferred, the caller is said to borrow the reference. Nothing needs to be done for a borrowed reference."[^1] The documentation of each function tells which kind of reference it returns. For `PyList_GetItem` it says "Return value: Borrowed reference."[^3]

**r3.** "Stealing a reference means that when you pass a reference to a function, that function assumes that it now owns that reference."[^1] "Few functions steal references; the two notable exceptions are PyList_SetItem() and PyTuple_SetItem()".[^1] `PyList_SetItem` steals the reference to the item "even on error."[^3] To keep an object that you give to such a function, take one more reference first. Example: [`steal`](#steal).

**r4.** A borrowed reference is safe only while another reference keeps the object alive. "However, a common pitfall is to extract an object from a list and hold on to it for a while without taking a new reference."[^1] "The real danger is that innocent-looking operations may invoke arbitrary Python code which could do this".[^1] The generic functions, whose names start with `PyObject_`, `PyNumber_`, `PySequence_` or `PyMapping_`, "always create a new strong reference (i.e. increment the reference count) of the object they return."[^1]

**r5.** "For C programmers, however, error checking always has to be explicit."[^1] "In general, when a function encounters an error, it sets an exception, discards any object references that it owns, and returns an error indicator. If not documented otherwise, this indicator is either NULL or -1, depending on the function’s return type."[^1] A function that gets an error value from a call returns an error value too, so the error goes up to the caller. "Once the error reaches the Python interpreter’s main loop, this aborts the currently executing Python code and tries to find an exception handler specified by the Python programmer."[^13] Example: [`errors`](#errors).

**r6.** On a build with the GIL, "only a thread that holds the GIL may operate on Python objects or invoke Python’s C API."[^4] The C code releases the lock around slow work that does not touch objects, with the macros `Py_BEGIN_ALLOW_THREADS` and `Py_END_ALLOW_THREADS`.[^4] On the free-threaded build there is no GIL, but "it is still required to have an attached thread state, as the interpreter needs to keep track of which threads may access Python objects."[^4] Example: [`gil`](#gil).

**r7.** The C API has levels of stability. "Unless documented otherwise, Python’s C API is covered by the Backwards Compatibility Policy, PEP 387."[^5] "There are two tiers of C API with different stability expectations:"[^5] the unstable API "may change in minor versions without a deprecation period. It is marked by the PyUnstable prefix in names."[^5] The limited API "is compatible across several minor releases. When Py_LIMITED_API is defined, only this subset is exposed from Python.h."[^5] "Names prefixed by an underscore, such as _Py_InternalState, are private API that can change without notice even in patch releases."[^5] The page [The limited API and the stable ABI](/python/the-limited-api-and-stable-abi/) tells more.

**r8.** The C API Working Group is "a small committee of Python core developers responsible for overseeing and coordinating the development and maintenance of the Python C API."[^6] "As delegated by the Steering Council it is the deciding body for changes to the C API".[^6] PEP 733 says: "Python’s C API was not designed for the different purposes it currently fulfills. It evolved from what was initially the internal API between the C code of the interpreter and the Python language and libraries."[^7] It lists the "problems of the C API categorized into nine areas of weakness".[^7]

**r9.** "The C extension interface is specific to CPython, and extension modules do not work on other Python implementations."[^11] The page [PyPy and alternative runtimes](/python/pypy-and-alternative-runtimes/) tells how PyPy runs such modules.

## Examples

This C function shows the rules r2, r4 and r5. It returns the first item of a list in upper case:

```c norun reason=fragment
static PyObject *
first_upper(PyObject *self, PyObject *list)
{
    PyObject *item = PyList_GetItem(list, 0);   /* borrowed */
    if (item == NULL) {
        return NULL;                            /* the exception is set */
    }
    Py_INCREF(item);                            /* now this code owns a reference */
    PyObject *result = PyObject_CallMethod(item, "upper", NULL);   /* new reference */
    Py_DECREF(item);
    return result;                              /* the caller owns the result */
}
```

The reference from `PyList_GetItem` is borrowed, so the function takes its own reference before it calls Python code. The result of `PyObject_CallMethod` is a new reference, and the function gives it to its caller. If a call fails, the function returns `NULL` and the exception stays set.

`ctypes.pythonapi` calls the same functions from Python. Its functions are "assumed to return C int, which is of course not always the truth, so you have to assign the correct restype attribute to use these functions."[^8] The type `ctypes.py_object` makes a Python object from the result and takes ownership of the reference, so use it only for a function that returns a new reference.

This example fills a list with `PyList_SetItem`. The function steals a reference, so the code calls `Py_IncRef` first (rule r3). The counts are differences, because the value of `sys.getrefcount` is not exact:

```python run id=steal
import ctypes
import sys

api = ctypes.pythonapi
api.PyList_New.restype = ctypes.py_object
api.PyList_New.argtypes = [ctypes.c_ssize_t]
api.PyList_SetItem.argtypes = [ctypes.py_object, ctypes.c_ssize_t, ctypes.py_object]
api.Py_IncRef.argtypes = [ctypes.py_object]

item = object()
before = sys.getrefcount(item)
new_list = api.PyList_New(1)
api.Py_IncRef(item)
api.PyList_SetItem(new_list, 0, item)
print('the list holds a reference:', sys.getrefcount(item) - before)
print(new_list[0] is item)
del new_list
print('after the list is freed:', sys.getrefcount(item) - before)
```

```output for=steal
the list holds a reference: 1
True
after the list is freed: 0
```

`PyLong_AsLong` returns `-1` and sets an exception when it cannot convert the object (rule r5). A `PyDLL` checks the error flag after each call: "If the error flag is set, a Python exception is raised."[^8]

```python run id=errors
import ctypes

api = ctypes.pythonapi
api.PyLong_AsLong.argtypes = [ctypes.py_object]
api.PyLong_AsLong.restype = ctypes.c_long

print(api.PyLong_AsLong(42))
for value in ['text', 2**80]:
    try:
        api.PyLong_AsLong(value)
    except (TypeError, OverflowError) as e:
        print(type(e).__name__)
```

```output for=errors
42
TypeError
OverflowError
```

The C code below releases the GIL while it waits for a file. It does not touch a Python object between the two macros (rule r6):

```c norun reason=fragment
ssize_t n;
Py_BEGIN_ALLOW_THREADS
n = read(fd, buffer, size);
Py_END_ALLOW_THREADS
if (n < 0) {
    return PyErr_SetFromErrno(PyExc_OSError);
}
return PyBytes_FromStringAndSize(buffer, n);
```

`ctypes` shows the same two cases. For a `CDLL`, "The Python global interpreter lock is released before calling any function exported by these libraries, and reacquired afterwards."[^8] For a `PyDLL`, "the Python GIL is not released during the function call".[^8] `PyGILState_Check` returns 1 when the thread holds the GIL:

```python run id=gil
import ctypes

holds = ctypes.pythonapi.PyGILState_Check()
released = ctypes.CDLL(None).PyGILState_Check()
print('PyDLL call holds the GIL:', holds)
print('CDLL call holds the GIL:', released)
```

```output for=gil
PyDLL call holds the GIL: 1
CDLL call holds the GIL: 0
```

## Pitfalls

Do not keep a borrowed reference across a call that can run Python code. The call can free the object, and the pointer then points to freed memory (rule r4). Take a new reference with `Py_INCREF`, or use a function that returns a new reference.

Give back each reference that you own. A missing `Py_DECREF` leaks the object. An extra `Py_DECREF` frees the object while other code uses it, and the program can crash later in an unrelated place.

Check the result of each call. A `NULL` or `-1` result means that an exception is set. Return the error to the caller, or clear it with `PyErr_Clear` if you handle it (rule r5).

Do not touch Python objects between `Py_BEGIN_ALLOW_THREADS` and `Py_END_ALLOW_THREADS`, or in a thread that has no attached thread state (rule r6).

Do not trust the exact value of `sys.getrefcount`. The documentation says: "do not rely on the returned value to be accurate, other than a value of 0 or 1."[^10] It also says that "some objects are immortal and have a very high refcount that does not reflect the actual number of references."[^10]

Do not use names that start with `_Py` or `PyUnstable` in code that must build on the next minor version without changes (rule r7).

In `ctypes`, set `restype` and `argtypes` for each C API function. With `restype = ctypes.py_object`, a function that returns a borrowed reference gives one reference too few, and the object can be freed while it is in use.

## Comparison

The Java Native Interface (JNI) solves the same problem in a different way. "The VM must keep track of all objects that have been passed to the native code, so that these objects are not freed by the garbage collector."[^12] "Local references are valid for the duration of a native method call, and are automatically freed after the native method returns. Global references remain valid until they are explicitly freed."[^12] In the Python C API, the C code counts each reference itself (rule r2), and there is no local reference that the runtime frees at the end of a call.

## References

[^1]: [Introduction](https://docs.python.org/3/c-api/intro.html)
[^2]: [Common Object Structures](https://docs.python.org/3/c-api/structures.html)
[^3]: [List Objects](https://docs.python.org/3/c-api/list.html)
[^4]: [Thread states and the global interpreter lock](https://docs.python.org/3/c-api/threads.html)
[^5]: [C API and ABI Stability](https://docs.python.org/3/c-api/stable.html)
[^6]: [PEP 731: C API Working Group Charter](https://peps.python.org/pep-0731/)
[^7]: [PEP 733: An Evaluation of Python's Public C API](https://peps.python.org/pep-0733/)
[^8]: [ctypes, A foreign function library for Python](https://docs.python.org/3/library/ctypes.html)
[^9]: [Built-in Functions](https://docs.python.org/3/library/functions.html)
[^10]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html)
[^11]: [Extending and Embedding the Python Interpreter](https://docs.python.org/3/extending/index.html)
[^12]: [Java Native Interface Specification: 2, Design Overview](https://docs.oracle.com/en/java/javase/25/docs/specs/jni/design.html)
[^13]: [Using the C API: Assorted topics](https://docs.python.org/3/extending/extending.html)

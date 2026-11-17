---
tag: 5CK9WZ
kind: feature
owner: python
slug: ctypes
name: The ctypes module
aliases: [ctypes, CDLL, PyDLL, argtypes, restype, ctypes.Structure, ctypes.POINTER, byref, CFUNCTYPE, ctypes callback, ctypes.util.find_library, use_errno, foreign function library]
area: languages/python/interop
level: advanced
summary: >-
  ctypes is a standard module that calls C functions in a shared library from Python, with no compiler. It cannot check a function's signature, so a wrong declaration can crash the process.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/library/ctypes.html", locator: "The Python Standard Library, ctypes, A foreign function library for Python"}
see_also: [python/the-python-c-api, python/extension-modules, python/the-limited-api-and-stable-abi, python/native-acceleration]
sources:
  - url: https://docs.python.org/3/library/ctypes.html
    title: ctypes, A foreign function library for Python
    locator: introduction and Warning; Calling functions; Fundamental data types; Specifying the required argument types; Return types; Passing pointers; Structures and unions; Pointers; Thread safety without the GIL; Callback functions; CDLL; find_library; c_byte
    accessed: 2026-11-17
  - url: https://openjdk.org/jeps/454
    title: "JEP 454: Foreign Function & Memory API"
    locator: header; Goals; Motivation; Linking Java code to foreign functions; Safety
    accessed: 2026-11-17
---

## Synopsis

```python run id=strlen
import ctypes

libc = ctypes.CDLL(None)
libc.strlen.argtypes = [ctypes.c_char_p]
libc.strlen.restype = ctypes.c_size_t
print(libc.strlen(b'hello'))
print(type(libc).__name__, type(libc.strlen).__name__)
```

```output for=strlen
5
CDLL _FuncPtr
```

`ctypes` is a module of the standard library that calls C functions in a shared library from Python. It needs no C compiler and no extension module. You load the library with `ctypes.CDLL`, get a function as an attribute of the library, give its argument types in `argtypes` and its result type in `restype`, and then call it like a Python function. The example calls `strlen` from the C library. On Linux and macOS, `CDLL(None)` gives the main program, and the main program is linked to the C library, so its functions are available. `ctypes` cannot check that the declaration matches the real C function, so a wrong declaration can crash the process.

## Semantics

**r1.** "ctypes is a foreign function library for Python. It provides C compatible data types, and allows calling functions in DLLs or shared libraries. It can be used to wrap these libraries in pure Python."[^1] "This is an optional module."[^1] The documentation starts with a warning: "Incorrect use can corrupt data and objects, reveal sensitive information, cause crashes, or otherwise compromise the running process."[^1]

**r2.** A `CDLL` object is a loaded library. "Functions in this library use the standard C calling convention, and are assumed to return int."[^1] On Linux, the name must be the full file name, for example `libc.so.6`: "Other platform-specific prefixes and suffixes (for example, lib, .so, .dylib, or version numbers) must be present in name; they are not added automatically."[^1] With the name `None`, "dlopen() is called with NULL, which opens the main program as a “library”."[^1] On Windows, `WinDLL` and `OleDLL` use the `stdcall` calling convention.[^1] `ctypes.util.find_library` finds a library by its short name. Python 3.15 made it soft deprecated: "This function is kept for use in cases where it works, but not expected to be updated for additional platforms and configurations."[^1] Example: [`strlen`](#strlen).

**r3.** "ctypes cannot determine a function’s calling convention or signature."[^1] "Without argtypes, the number of arguments is not checked. Calling a function with an incorrect calling convention or with incorrect arguments can corrupt memory or crash Python."[^1] With `argtypes`, `ctypes` converts each argument or raises an error. "By default functions are assumed to return the C int type. Other return types can be specified by setting the restype attribute of the function object."[^1] Example: [`declare`](#declare).

**r4.** Without `argtypes`, "None, integers, bytes objects and (unicode) strings are the only native Python objects that can directly be used as parameters in these function calls."[^1] "Python integers are passed as the platform’s default C int type, their value is masked to fit into the C type."[^1] For other C types, use the `ctypes` classes such as `c_int`, `c_double` and `c_char_p`. The integer types do not check the range: the constructor of `c_byte` "accepts an optional integer initializer; no overflow checking is done."[^1] Example: [`types`](#types).

**r5.** A C structure is a subclass of `ctypes.Structure`. "Each subclass must define a _fields_ attribute. _fields_ must be a list of 2-tuples, containing a field name and a field type."[^1] "By default, Structure and Union fields are laid out in the same way the C compiler does it."[^1] So the fields have the same padding as in C. A function can take or return a structure by value, but "Unions and structures with bit-fields should always be passed to functions by pointer."[^1] Example: [`struct`](#struct).

**r6.** A C function that writes to a pointer argument gets the address of a `ctypes` object. "ctypes exports the byref() function which is used to pass parameters by reference."[^1] `pointer()` makes a real pointer object, "so it is faster to use byref() if you don’t need the pointer object in Python itself".[^1] The `contents` of a pointer is a new Python object each time: "it constructs a new, equivalent object each time you retrieve an attribute".[^1] Examples: [`types`](#types), [`byref`](#byref).

**r7.** "ctypes allows creating C callable function pointers from Python callables. These are sometimes called callback functions."[^1] `CFUNCTYPE` makes the type of the callback from the result type and the argument types.[^1] The Python code must keep the callback object alive: "Make sure you keep references to CFUNCTYPE() objects as long as they are used from C code. ctypes doesn’t, and if you don’t, they may be garbage collected, crashing your program when a callback is made."[^1] Example: [`callback`](#callback).

**r8.** For a `CDLL`, "The Python global interpreter lock is released before calling any function exported by these libraries, and reacquired afterwards."[^1] So other Python threads can run while the C function runs. For a `PyDLL`, "the Python GIL is not released during the function call", so "this is only useful to call Python C API functions directly."[^1] On the free-threaded build, "reads and writes to a single object concurrently is safe, but not across multiple objects".[^1]

**r9.** A C function often reports an error in `errno`. With `use_errno=True`, "ctypes maintains a thread-local copy of the system’s errno variable".[^1] "The function ctypes.get_errno() returns the value of the ctypes private copy".[^1] Example: [`errno`](#errno).

## Examples

The integer types of `ctypes` wrap around in the same way as the C types (rule r4). A pointer can change the object that it points to (rule r6):

```python run id=types
import ctypes

print(ctypes.c_uint8(300).value, ctypes.c_int8(200).value)
print(ctypes.sizeof(ctypes.c_char), ctypes.sizeof(ctypes.c_int), ctypes.sizeof(ctypes.c_double))
number = ctypes.c_int(42)
pointer = ctypes.pointer(number)
pointer[0] = 7
print(number.value, pointer.contents.value, pointer.contents is number)
```

```output for=types
44 -56
1 4 8
7 7 False
```

The sizes are for Linux on x86-64. They come from the C compiler of the platform.

The function `strchr` returns a pointer into the string. With `restype = c_char_p`, `ctypes` makes a `bytes` object from it. With the default `restype`, the result is the address cut to a C `int`. The declared `argtypes` reject a `str` in place of `bytes`, and a call with too few arguments (rule r3):

```python run id=declare
import ctypes

libc = ctypes.CDLL(None)
libc.strchr.argtypes = [ctypes.c_char_p, ctypes.c_int]
libc.strchr.restype = ctypes.c_char_p
print(libc.strchr(b'abcdef', ord('d')))
try:
    libc.strchr('abcdef', ord('d'))
except ctypes.ArgumentError as e:
    print(type(e).__name__)
try:
    libc.strchr(b'abcdef')
except TypeError as e:
    print('TypeError:', e)
```

```output for=declare
b'def'
ArgumentError
TypeError: this function takes at least 2 arguments (1 given)
```

The C function `div` returns a structure `div_t` by value. The class `DivT` has the same fields, and `restype = DivT` tells `ctypes` to read the result as this structure. The class `Mixed` shows the padding of rule r5: the `double` starts at offset 8, not 1:

```python run id=struct
import ctypes

class DivT(ctypes.Structure):
    _fields_ = [('quot', ctypes.c_int), ('rem', ctypes.c_int)]

libc = ctypes.CDLL(None)
libc.div.argtypes = [ctypes.c_int, ctypes.c_int]
libc.div.restype = DivT
result = libc.div(17, 5)
print(result.quot, result.rem)

class Mixed(ctypes.Structure):
    _fields_ = [('flag', ctypes.c_char), ('value', ctypes.c_double)]

print(ctypes.sizeof(Mixed), Mixed.flag.offset, Mixed.value.offset)
```

```output for=struct
3 2
16 0 8
```

The function `strtol` writes the address of the first character that it did not read to its second argument, a `char **`. The code passes `byref(end)`, and then reads the rest of the string from `end`. `None` passes a `NULL` pointer (rule r6):

```python run id=byref
import ctypes

libc = ctypes.CDLL(None)
libc.strtol.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_char_p), ctypes.c_int]
libc.strtol.restype = ctypes.c_long
end = ctypes.c_char_p()
print(libc.strtol(b'42 apples', ctypes.byref(end), 10), end.value)
print(libc.strtol(b'ff', None, 16))
```

```output for=byref
42 b' apples'
255
```

The C function `qsort` calls a compare function for each pair of items. The decorator `CMPFUNC` makes a C function pointer from a Python function (rule r7). The name `by_value` keeps a reference to it while `qsort` runs:

```python run id=callback
import ctypes

libc = ctypes.CDLL(None)
CMPFUNC = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int))

@CMPFUNC
def by_value(a, b):
    return a[0] - b[0]

libc.qsort.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_size_t, CMPFUNC]
libc.qsort.restype = None
numbers = (ctypes.c_int * 6)(33, 5, 99, 1, 7, 5)
libc.qsort(numbers, len(numbers), ctypes.sizeof(ctypes.c_int), by_value)
print(list(numbers))
```

```output for=callback
[1, 5, 5, 7, 33, 99]
```

`chdir` returns -1 when it fails, and sets `errno`. The library is loaded with `use_errno=True`, so `ctypes.get_errno` gives the value (rule r9):

```python run id=errno
import ctypes
import errno

libc = ctypes.CDLL(None, use_errno=True)
libc.chdir.argtypes = [ctypes.c_char_p]
result = libc.chdir(b'/no/such/directory')
code = ctypes.get_errno()
print(result, errno.errorcode[code])
```

```output for=errno
-1 ENOENT
```

A bad pointer does not raise a Python exception. It stops the process. This example reads the memory at address 0 in a child process, so that the crash does not stop the example. The option `-X faulthandler` makes Python print the fatal error. The documentation says that "The faulthandler module can help debug crashes, such as segmentation faults produced by erroneous C library calls."[^1]

```python run id=crash
import signal
import subprocess
import sys

code = 'import ctypes; ctypes.string_at(0)'
child = subprocess.run([sys.executable, '-X', 'faulthandler', '-c', code], capture_output=True, text=True)
print(child.returncode == -signal.SIGSEGV)
print(child.stderr.splitlines()[0])
```

```output for=crash
True
Fatal Python error: Segmentation fault
```

The return code is the negative number of the signal, so this example is for Linux and other POSIX systems.

## Pitfalls

Set `argtypes` and `restype` for each function, from the C header. Without `restype`, a function that returns a pointer, a `long` or a `double` gives a wrong value, because the result is read as an `int` (rule r3).

Do not trust a declaration because a call works one time. `ctypes` cannot check it, and a wrong one can corrupt memory with no error (rule r3).

Do not give a `bytes` object to a C function that writes to its argument. The documentation says about such objects: "You should be careful, however, not to pass them to functions expecting pointers to mutable memory. If you need mutable memory blocks, ctypes has a create_string_buffer() function".[^1]

Keep a reference to each callback object while C code can call it (rule r7). If the object is freed, the next call from C crashes the program.

Do not pass a union or a structure with bit fields by value. Pass a pointer to it (rule r5).

Do not hard-code a library file name that is for one platform only, such as `libc.so.6`. Find the name for each platform, and do not depend on `find_library` in new code (rule r2).

For a library that you call often, or with many functions, think about a binding generator or an extension module. The page [Native acceleration](/python/native-acceleration/) compares the ways to call native code.

## Comparison

Java has a similar API in the standard library since JDK 22: the Foreign Function & Memory API of JEP 454.[^2] Its goal is to "Replace the brittle machinery of native methods and the Java Native Interface (JNI) with a concise, readable, and pure-Java API."[^2] Like `CDLL` with `argtypes` and `restype`, the Java code describes the C signature at run time. For `strlen`, it gives the descriptor `FunctionDescriptor.of(JAVA_LONG, ADDRESS)` to `Linker.downcallHandle`, and gets a method handle that it can call.[^2] Java also treats these calls as unsafe: "the unsafe methods in the FFM API are restricted. This means that their use is permitted, but by default causes a warning to be issued at run time."[^2] `ctypes` gives no such warning. A difference in memory is that Java allocates foreign memory in an arena with a lifetime, and "Exiting the try-with-resources block closes the arena".[^2]

## References

[^1]: [ctypes, A foreign function library for Python](https://docs.python.org/3/library/ctypes.html)
[^2]: [JEP 454: Foreign Function & Memory API](https://openjdk.org/jeps/454)

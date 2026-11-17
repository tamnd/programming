---
tag: PHT9X7
kind: feature
owner: python
slug: binding-generators
name: Binding generators
aliases: [binding generator, wrapper generator, cffi, Cython, pybind11, nanobind, PyO3, maturin, SWIG, Boost.Python, Argument Clinic, return value policy, type stubs for extension modules]
area: languages/python/interop
level: advanced
summary: >-
  A binding generator makes the glue code between Python and a C, C++ or Rust library from declarations, so you do not write C API calls by hand. Examples: cffi, Cython, pybind11, nanobind, PyO3, SWIG.
grammar: null
spec: {standard: null, url: "https://packaging.python.org/en/latest/guides/packaging-binary-extensions/", locator: "Python Packaging User Guide, Packaging binary extensions, Alternatives to handcoded wrapper modules"}
see_also: [python/extension-modules, python/the-python-c-api, python/ctypes, python/the-limited-api-and-stable-abi, python/native-acceleration]
sources:
  - url: https://packaging.python.org/en/latest/guides/packaging-binary-extensions/
    title: Packaging binary extensions
    locator: Alternatives to handcoded accelerator modules; Alternatives to handcoded wrapper modules; Alternatives for low level system access
    accessed: 2026-11-18
  - url: https://cffi.readthedocs.io/en/stable/overview.html
    title: CFFI documentation, Overview
    locator: Main mode of usage; Other CFFI modes; Simple example (ABI level, in-line); ABI versus API
    accessed: 2026-11-18
  - url: https://cython.readthedocs.io/en/latest/src/quickstart/overview.html
    title: Cython, an overview
    locator: first paragraphs
    accessed: 2026-11-18
  - url: https://cython.readthedocs.io/en/latest/src/tutorial/pure.html
    title: Cython, Pure Python Mode
    locator: introduction; Compiled switch
    accessed: 2026-11-18
  - url: https://pybind11.readthedocs.io/en/stable/index.html
    title: pybind11, Seamless interoperability between C++ and Python
    locator: About this project; Core features
    accessed: 2026-11-18
  - url: https://pybind11.readthedocs.io/en/stable/basics.html
    title: pybind11, First steps
    locator: Creating bindings for a simple function
    accessed: 2026-11-18
  - url: https://pybind11.readthedocs.io/en/stable/advanced/functions.html
    title: pybind11, Functions
    locator: Return value policies; Overload resolution order
    accessed: 2026-11-18
  - url: https://pybind11.readthedocs.io/en/stable/advanced/exceptions.html
    title: pybind11, Exceptions
    locator: Built-in C++ to Python exception translation
    accessed: 2026-11-18
  - url: https://pybind11.readthedocs.io/en/stable/advanced/misc.html
    title: pybind11, Miscellaneous
    locator: Global Interpreter Lock (GIL)
    accessed: 2026-11-18
  - url: https://nanobind.readthedocs.io/en/latest/index.html
    title: nanobind documentation
    locator: introduction
    accessed: 2026-11-18
  - url: https://nanobind.readthedocs.io/en/latest/why.html
    title: nanobind, Why another binding library?
    locator: So what is different?; Performance improvements; Major additions
    accessed: 2026-11-18
  - url: https://pyo3.rs/main/index.html
    title: The PyO3 user guide
    locator: Introduction; Usage; Using Rust from Python
    accessed: 2026-11-18
  - url: https://www.swig.org/Doc4.3/Introduction.html
    title: SWIG 4.3 documentation, Introduction
    locator: 2.1 What is SWIG?; 2.4 A SWIG example
    accessed: 2026-11-18
  - url: https://www.swig.org/Doc4.3/Python.html
    title: SWIG 4.3 documentation, SWIG and Python
    locator: 33.2.1 Running SWIG
    accessed: 2026-11-18
  - url: https://devguide.python.org/development-tools/clinic/
    title: Python Developer's Guide, Argument Clinic
    locator: introduction and Note; Basic concepts
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/inspect.html
    title: inspect, Inspect live objects
    locator: Introspecting callables with the Signature object, Note
    accessed: 2026-11-18
  - url: https://docs.python.org/3/c-api/buffer.html
    title: Buffer Protocol
    locator: introduction; Buffer structure
    accessed: 2026-11-18
  - url: https://rust-lang.github.io/rust-bindgen/introduction.html
    title: The bindgen User Guide, Introduction
    locator: Introduction
    accessed: 2026-11-18
  - url: https://github.com/openjdk/jextract
    title: openjdk/jextract, README
    locator: Jextract
    accessed: 2026-11-18
---

## Synopsis

```python run id=generate
import ctypes
import re

HEADER = """
size_t strlen(const char *s);
int abs(int n);
double atof(const char *s);
"""

C_TO_CTYPES = {"size_t": "c_size_t", "int": "c_int", "double": "c_double", "const char *": "c_char_p"}


def generate(header):
    out = ["import ctypes", "lib = ctypes.CDLL(None)"]
    for m in re.finditer(r"(.+?) (\w+)\((.*)\);", header):
        result, name, params = m.groups()
        types = [C_TO_CTYPES[re.sub(r"\w+$", "", p).strip()] for p in params.split(",")]
        out.append(f"{name} = lib.{name}")
        out.append(f"{name}.argtypes = [{', '.join('ctypes.' + t for t in types)}]")
        out.append(f"{name}.restype = ctypes.{C_TO_CTYPES[result]}")
    return "\n".join(out)


source = generate(HEADER)
print(source)
namespace = {}
exec(source, namespace)
print(namespace["strlen"](b"binding"), namespace["abs"](-7), namespace["atof"](b"2.5"))
```

```output for=generate
import ctypes
lib = ctypes.CDLL(None)
strlen = lib.strlen
strlen.argtypes = [ctypes.c_char_p]
strlen.restype = ctypes.c_size_t
abs = lib.abs
abs.argtypes = [ctypes.c_int]
abs.restype = ctypes.c_int
atof = lib.atof
atof.argtypes = [ctypes.c_char_p]
atof.restype = ctypes.c_double
7 7 2.5
```

A binding generator is a tool that writes the glue code between Python and a library in C, C++ or Rust. You give it declarations: a C header, C++ functions and classes, or Rust functions with attributes. It gives you a module that Python can import. The example is a very small generator. It reads three C prototypes and writes the `ctypes` declarations for them, and then the program runs the generated code. Real generators do the same work at a much larger scale, and most of them write C or C++ source that a compiler builds into an [extension module](/python/extension-modules/). The common tools are cffi, Cython, pybind11, nanobind, PyO3 and SWIG. CPython itself uses a generator, Argument Clinic, for the argument parsing of its built-in functions.

## Semantics

**r1.** The packaging guide says: "maintaining handcoded wrapper modules is tedious, so alternative approaches should be considered."[^1] A generator does not make the distribution of a binary module easier, "but they can significantly reduce the maintenance burden of keeping wrapper modules up to date."[^1] The result of most generators is still an extension module, so the module has the same build and platform problems as a module that you write by hand with [the C API](/python/the-python-c-api/). Example: [`generate`](#generate).

**r2.** A generator works at one of two levels. At the ABI level, the binding calls the machine code of the library and trusts the declarations. The guide says that `ctypes` "operates solely at the C ABI level, and thus has no automatic consistency checking between the interface actually being exported by the library and the one declared in the Python code."[^1] At the API level, the generator writes C code that a compiler compiles against the real header: "the above alternatives are all able to operate at the C API level, using C header files to ensure consistency between the interface exported by the library being wrapped and the one expected by the Python wrapper module."[^1] So the compiler finds a wrong declaration at the API level. At the ABI level, a wrong declaration gives a wrong value or a crash. Example: [`misdeclare`](#misdeclare).

**r3.** cffi takes C declarations in a string, in valid C syntax, and works at both levels. "CFFI can be used in one of four modes: “ABI” versus “API” level, each with “in-line” or “out-of-line” preparation (or compilation)."[^2] "In the in-line mode, everything is set up every time you import your Python code. In the out-of-line mode, you have a separate step of preparation (and possibly C compilation) that produces a module which your main program can then import."[^2] The ABI mode needs no compiler, but "it will crash if you call some function or access some fields of a structure that was slightly misdeclared in the cdef()."[^2] The documentation recommends the other mode: "If using a C compiler to install your module is an option, it is highly recommended to use the API mode instead. (It is also faster.)"[^2] In API mode, the build script calls `ffibuilder.cdef()` with the declarations and `ffibuilder.set_source()` with the `#include` of the header, and the build makes a module such as `_pi_cffi` with the objects `ffi` and `lib`.[^2] The packaging guide adds that cffi "is compatible with the PyPy JIT".[^1]

**r4.** Cython is a compiler for a language that is close to Python. Cython "is a programming language that makes writing C extensions for the Python language as easy as Python itself."[^3] "The source code gets translated into optimized C/C++ code and compiled as Python extension modules."[^3] Its main addition to Python is "support for optional static type declarations as part of the language."[^3] Cython has two syntax styles: the `.pyx` syntax with `cdef` declarations, and a pure Python syntax in `.py` files that uses type annotations and the special `cython` module.[^4] Code in the pure Python syntax can still run in the interpreter. The variable `cython.compiled` "is set to True when the compiler runs, and False in the interpreter."[^4] Plain Python code compiled with Cython "usually results only in a speed gain of about 20%-50%"; the large gains come from the static types.[^4]

**r5.** pybind11 is a C++ library, not a separate program. "pybind11 is a lightweight header-only library that exposes C++ types in Python and vice versa, mainly to create Python bindings of existing C++ code."[^5] It has the same goal as Boost.Python: "to minimize boilerplate code in traditional extension modules by inferring type information using compile-time introspection."[^5] The macro `PYBIND11_MODULE` "creates a function that will be called when an import statement is issued from within Python", and `m.def()` "generates binding code that exposes the add() function to Python."[^6] You do not write the types of the arguments: "all details regarding the function’s parameters and return value were automatically inferred using template metaprogramming."[^6] Examples: [`pybind11`](#pybind11), [`deduce`](#deduce).

**r6.** The types of a C++ function do not tell who owns a returned object. "Just by looking at the type information, it is not clear whether Python should take charge of the returned value and eventually free its resources, or if this is handled on the C++ side."[^7] pybind11 has return value policies for this, and "The default policy is return_value_policy::automatic."[^7] A wrong policy can crash the program. In the example of the documentation, a function returns a pointer to static data, and the automatic policy "causes pybind11 to assume ownership of the static _data instance."[^7] Then the delete of the Python object deletes the static data. The correct policy there is `return_value_policy::reference`. The documentation says: "Return value policies are tricky, and it’s very important to get them right."[^7]

**r7.** A C++ exception must not go through the C code of the interpreter. The binding catches it at the boundary and raises a Python exception. "When Python calls C++ code through pybind11, pybind11 provides a C++ exception handler that will trap C++ exceptions, translate them to the corresponding Python exception, and raise them so that Python code can handle them."[^8] In the table of pybind11, `std::exception` becomes `RuntimeError`, `std::invalid_argument` becomes `ValueError`, `std::out_of_range` becomes `IndexError` and `std::bad_alloc` becomes `MemoryError`.[^8] Example: [`translate`](#translate).

**r8.** A C++ name can have overloads, and Python has one function for each name. The binding keeps a list of the overloads and tries them in turn. pybind11 "determines which overload to call in two passes."[^7] The first pass tries each overload with no conversion of the arguments. "If no overload succeeds in the no-conversion first pass, a second pass is attempted in which argument conversion is allowed".[^7] So the order of the `m.def()` calls can change which overload runs.

**r9.** Generated code can release the global interpreter lock while the C++ code runs. In pybind11, "The classes gil_scoped_release and gil_scoped_acquire can be used to acquire and release the global interpreter lock in the body of a C++ function call."[^9] The code must take the lock again before it uses a Python object.[^9] This is one reason to use a compiled binding: the packaging guide says that some operations, "like releasing the Global Interpreter Lock", "are simply invalid when the interpreter is running code, even if a module like ctypes or cffi is used to obtain access to the relevant C API interfaces."[^1]

**r10.** nanobind is a newer library by the author of pybind11.[^11] "It is reminiscent of Boost.Python and pybind11 and uses near-identical syntax."[^10] "In contrast to these existing tools, nanobind is more efficient: bindings compile in a shorter amount of time, produce smaller binaries, and have better runtime performance."[^10] It supports fewer C++ features on purpose: "The codebase has to adapt to the binding tool and not the other way around, which allows nanobind to be simpler and faster."[^11] Its documentation gives the size of a wrapped object as "pybind11: 56 bytes, nanobind: 24 bytes."[^11] nanobind can target the [stable ABI](/python/the-limited-api-and-stable-abi/) on Python 3.12 and later versions, and it has a stub generator that makes type stubs as a part of the build.[^11]

**r11.** PyO3 is the binding library for Rust. It gives "Rust bindings for Python, including tools for creating native Python extension modules. Running and interacting with Python code from a Rust binary is also supported."[^12] The attribute `#[pyfunction]` makes a Python function from a Rust function, and `#[pymodule]` makes the module. The build tool is maturin: "maturin is a tool for building and publishing Rust-based Python packages with minimal configuration."[^12] The guide lists the Python implementations that PyO3 supports: CPython 3.9 or greater, PyPy 7.3 and GraalPy 25.4 or greater.[^12]

**r12.** SWIG is an older generator that is not specific to Python. "SWIG is a compiler that takes C/C++ declarations and creates the wrappers needed to access those declarations from other languages including Perl, Python, Tcl, Ruby, Guile, and Java."[^13] Its input is an interface file with the suffix `.i`. "The %module directive defines the name of the module that will be created by SWIG."[^13] For Python, `swig -python example.i` "creates two different files; a C/C++ source file example_wrap.c or example_wrap.cxx and a Python source file example.py."[^14] The C file holds "the low-level wrappers", and the Python file "contains high-level support code. This is the file that you will import to use the module."[^14]

**r13.** CPython uses Argument Clinic for its own built-in functions. "Argument Clinic is a preprocessor for CPython C files."[^15] It generates the argument parsing code and the introspection signature of each function from a block of declarations in a comment. The generated code goes into the C file after the input block, and "You should never modify the output of an Argument Clinic block".[^15] It is not a tool for other projects: "Argument Clinic is considered internal-only for CPython. Its use is not supported for files outside CPython".[^15] The signature that it generates is in the attribute `__text_signature__` of the function, and `inspect.signature` reads it. Not all functions have one: "in CPython, some built-in functions defined in C provide no metadata about their arguments."[^16] Examples: [`clinic`](#clinic), [`keywords`](#keywords), [`stubs`](#stubs).

**r14.** Many bindings move arrays between Python and C++ with the buffer protocol, and they do not copy the data. The protocol lets a type "expose information about their underlying buffer", and a consumer can "obtain a pointer to the raw underlying data of an object".[^17] Buffers can "be used as a zero-copy slicing mechanism."[^17] pybind11 uses this "for fast conversion between C++ matrix classes like Eigen and NumPy without expensive copy operations."[^5] nanobind uses "either DLPack or the buffer protocol to achieve zero-copy CPU/GPU array exchange".[^11] In Python, `memoryview` shows the format, the shape and the strides that the C++ side gets. Example: [`buffer`](#buffer).

**r15.** The output of a compiled binding is an extension module file. Its name must have one of the suffixes that the import system accepts, and the suffixes are different for each platform and Python version. So the build tool must get the suffix from the Python that the module is for, and not use a fixed name. Example: [`suffix`](#suffix).

## Examples

This is the first example of the pybind11 documentation. It is the complete source of a module named `example`.[^6] It needs the pybind11 headers and the Python headers to compile, and the C++ image of this site has neither, so the block does not run here.

```cpp norun reason=platform id=pybind11
#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m, py::mod_gil_not_used()) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function that adds two numbers");
}
```

After the build, `import example` and `example.add(1, 2)` work in Python. The tag `py::mod_gil_not_used()` lets a free-threaded build of CPython use the module with no GIL. The documentation warns that the tag "is also your promise that your code is thread safe."[^9]

pybind11 gets the types from the C++ function itself. A function template takes a pointer to the function, and template argument deduction finds the result type and the argument types. The next program uses the same method to print a Python signature for three C++ functions. It is a model of the method, not pybind11 code.

```cpp run id=deduce
#include <cstdio>
#include <string>

template <typename T> const char *py_name();
template <> const char *py_name<int>() { return "int"; }
template <> const char *py_name<double>() { return "float"; }
template <> const char *py_name<bool>() { return "bool"; }
template <> const char *py_name<std::string>() { return "str"; }

template <typename R, typename... Args>
std::string signature(R (*)(Args...)) {
    std::string s = "(";
    const char *sep = "";
    ((s += sep, s += py_name<Args>(), sep = ", "), ...);
    return s + ") -> " + py_name<R>();
}

int add(int i, int j) { return i + j; }
double scale(double x, int n) { return x * n; }
bool starts_with(std::string s, std::string prefix) { return s.rfind(prefix, 0) == 0; }

int main() {
    std::printf("add%s\n", signature(&add).c_str());
    std::printf("scale%s\n", signature(&scale).c_str());
    std::printf("starts_with%s\n", signature(&starts_with).c_str());
}
```

```output for=deduce
add(int, int) -> int
scale(float, int) -> float
starts_with(str, str) -> bool
```

A type with no specialization of `py_name` stops the compile, so the model cannot print a wrong name for a type that it does not know.

The next program is a model of the exception boundary of rule r7. Each call goes through a function that catches the C++ exceptions and gives the name of the Python exception that pybind11 raises for it.

```cpp run id=translate
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

// A binding catches every C++ exception at the boundary and raises a Python exception.
template <typename F>
std::string call_from_python(F f) {
    try {
        return "result " + std::to_string(f());
    } catch (const std::out_of_range &e) {
        return std::string("IndexError: ") + e.what();
    } catch (const std::invalid_argument &e) {
        return std::string("ValueError: ") + e.what();
    } catch (const std::exception &e) {
        return std::string("RuntimeError: ") + e.what();
    }
}

int main() {
    std::vector<int> v{10, 20, 30};
    std::printf("%s\n", call_from_python([&] { return v.at(1); }).c_str());
    std::printf("%s\n", call_from_python([&] { return v.at(5) > 0; }).c_str());
    std::printf("%s\n", call_from_python([] { return std::stoi("abc"); }).c_str());
    std::printf("%s\n", call_from_python([]() -> int { throw std::runtime_error("disk full"); }).c_str());
}
```

```output for=translate
result 20
IndexError: vector::_M_range_check: __n (which is 5) >= this->size() (which is 3)
ValueError: stoi
RuntimeError: disk full
```

The order of the `catch` clauses is important. `std::out_of_range` and `std::invalid_argument` are subclasses of `std::exception`, so they must come before it.

At the ABI level, nothing checks the declaration. Here the result type of `strlen` is one byte, but the header says `size_t`. The call works and gives a wrong number with no error: 300 is 44 modulo 256.

```python wrong id=misdeclare
import ctypes

libc = ctypes.CDLL(None)
strlen = libc.strlen
strlen.argtypes = [ctypes.c_char_p]
strlen.restype = ctypes.c_ubyte  # the header says size_t
print(strlen(b"x" * 300))
```

```output for=misdeclare
44
```

A generator at the API level compiles the call against the header, so the C compiler uses the real result type. This mistake cannot occur there.

Argument Clinic stores the signature of a built-in function as text. The first parameter, `$module`, is the module object that the C function gets. `inspect.signature` removes it.

```python run id=clinic
import inspect
import math

print(math.dist.__text_signature__)
print(inspect.signature(math.dist))
print(inspect.signature(math.isclose))
print(type(math.dist).__name__)
```

```output for=clinic
($module, p, q, /)
(p, q, /)
(a, b, *, rel_tol=1e-09, abs_tol=0.0)
builtin_function_or_method
```

The generated parser applies the signature. The `/` in the signature of `math.dist` makes `p` and `q` positional-only, so a call with keywords fails, but `math.isclose` accepts keywords.

```python fails expect=exception:TypeError id=keywords
import math

print(math.isclose(a=1.0, b=1.0 + 1e-12))
print(math.dist(p=(0, 0), q=(3, 4)))
```

The first call prints `True`. The second call raises `TypeError` with the message `math.dist() takes no keyword arguments`.

A stub generator reads such signatures from the compiled module and writes a `.pyi` file for type checkers and editors. The text signatures have the names and the defaults of the parameters, but no types. So a stub generator that only reads them writes stubs with no types. nanobind and PyO3 know the C++ or Rust types, so their stubs can have them.

```python run id=stubs
import inspect
import math

for name in ["dist", "gcd", "isclose", "sqrt"]:
    func = getattr(math, name)
    print(f"def {name}{inspect.signature(func)}: ...")
```

```output for=stubs
def dist(p, q, /): ...
def gcd(*integers): ...
def isclose(a, b, *, rel_tol=1e-09, abs_tol=0.0): ...
def sqrt(x, /): ...
```

A binding that takes an array gets a buffer: a pointer, the format of the items, the shape and the strides. `memoryview` shows the same fields in Python. The second view has the same memory as the first, with two dimensions, and no data is copied.

```python run id=buffer
import array

data = array.array("d", [1.0, 2.0, 3.0, 4.0, 5.0, 6.0])
view = memoryview(data)
print(view.format, view.itemsize, view.ndim, view.shape, view.strides)
grid = view.cast("B").cast("d", shape=[2, 3])
print(grid.ndim, grid.shape, grid.strides, grid.c_contiguous)
print(grid.tolist())
```

```output for=buffer
d 8 1 (6,) (8,)
2 (2, 3) (24, 8) True
[[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]]
```

The build of a binding must give the module file a suffix that the interpreter accepts. On Linux, the list has the suffix for the stable ABI, `.abi3.so`, and the plain `.so` at the end. The first item is the suffix of the version-specific file name, and `sysconfig` gives the same value to build tools.

```python run id=suffix
import importlib.machinery
import sys
import sysconfig

suffixes = importlib.machinery.EXTENSION_SUFFIXES
print(sys.platform)
print(".abi3.so" in suffixes, suffixes[-1])
print(sysconfig.get_config_var("EXT_SUFFIX") == suffixes[0])
```

```output for=suffix
linux
True .so
True
```

## Pitfalls

Do not use the ABI mode of cffi, or `ctypes`, for a large library when you can compile. A wrong declaration gives a wrong value or a crash, and no tool tells you (rule r2).

Give the correct return value policy in pybind11 for each function that returns a pointer or a reference. The default policy can take ownership of an object that C++ still owns, and the program crashes later, far from the cause (rule r6).

Do not let a C++ exception or a Rust panic go into the interpreter. Use the translation of the binding library, and register a translator for your own exception types (rule r7).

Release the GIL only around code that does not touch Python objects, and take it again before a callback into Python (rule r9).

Do not edit generated files, such as the output of Argument Clinic or the `example_wrap.c` of SWIG. Change the input and run the generator again (rules r12 and r13).

Do not use Argument Clinic outside CPython. Its documentation says that the next version "could be totally incompatible and break all your code."[^15]

A compiled binding is still a binary for one platform and one Python version, unless it targets the stable ABI. Build a wheel for each target, or use a tool that targets `abi3` (rules r1, r10 and r15).

## Comparison

| Tool | Input | Output | Level | Language of the library |
|---|---|---|---|---|
| `ctypes` | declarations in Python at run time | none, calls at run time | ABI | C |
| cffi, ABI mode | C declarations in a string | none, calls through libffi | ABI | C |
| cffi, API mode | C declarations and a C source string | a C extension module | API | C |
| Cython | `.pyx` files, or `.py` files with types | a C or C++ extension module | API | C, C++ |
| pybind11, nanobind | C++ binding code with templates | a C++ extension module | API | C++ |
| PyO3 | Rust code with attributes | a Rust extension module | API | Rust |
| SWIG | a `.i` interface file | a C or C++ wrapper and a `.py` file | API | C, C++ |

The table summarizes rules r2 to r12.

Other languages have binding generators that go the other way, from a C header to code in the language. "bindgen automatically generates Rust FFI bindings to C and C++ libraries."[^18] For Java, "`jextract` is a tool which mechanically generates Java bindings from native library headers", and the generated bindings use the Foreign Function & Memory API.[^19] These tools read the header once and write declarations, as the example [`generate`](#generate) does for `ctypes`. A Python binding has more work to do, because it also converts between Python objects and C values at each call.

## References

[^1]: [Packaging binary extensions](https://packaging.python.org/en/latest/guides/packaging-binary-extensions/)
[^2]: [CFFI documentation, Overview](https://cffi.readthedocs.io/en/stable/overview.html)
[^3]: [Cython, an overview](https://cython.readthedocs.io/en/latest/src/quickstart/overview.html)
[^4]: [Cython, Pure Python Mode](https://cython.readthedocs.io/en/latest/src/tutorial/pure.html)
[^5]: [pybind11, Seamless interoperability between C++ and Python](https://pybind11.readthedocs.io/en/stable/index.html)
[^6]: [pybind11, First steps](https://pybind11.readthedocs.io/en/stable/basics.html)
[^7]: [pybind11, Functions](https://pybind11.readthedocs.io/en/stable/advanced/functions.html)
[^8]: [pybind11, Exceptions](https://pybind11.readthedocs.io/en/stable/advanced/exceptions.html)
[^9]: [pybind11, Miscellaneous](https://pybind11.readthedocs.io/en/stable/advanced/misc.html)
[^10]: [nanobind documentation](https://nanobind.readthedocs.io/en/latest/index.html)
[^11]: [nanobind, Why another binding library?](https://nanobind.readthedocs.io/en/latest/why.html)
[^12]: [The PyO3 user guide](https://pyo3.rs/main/index.html)
[^13]: [SWIG 4.3 documentation, Introduction](https://www.swig.org/Doc4.3/Introduction.html)
[^14]: [SWIG 4.3 documentation, SWIG and Python](https://www.swig.org/Doc4.3/Python.html)
[^15]: [Python Developer's Guide, Argument Clinic](https://devguide.python.org/development-tools/clinic/)
[^16]: [inspect, Inspect live objects](https://docs.python.org/3/library/inspect.html)
[^17]: [Buffer Protocol](https://docs.python.org/3/c-api/buffer.html)
[^18]: [The bindgen User Guide, Introduction](https://rust-lang.github.io/rust-bindgen/introduction.html)
[^19]: [openjdk/jextract, README](https://github.com/openjdk/jextract)

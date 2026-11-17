---
tag: PRVD6E
kind: feature
owner: python
slug: embedding-python
name: Embedding Python
aliases: [embedding, embedded Python, embed the interpreter, Py_Initialize, Py_InitializeFromConfig, PyConfig, PyRun_SimpleString, Py_FinalizeEx, PyImport_AppendInittab, isolated configuration, python3-config --embed, sub-interpreters]
area: languages/python/interop
level: advanced
summary: >-
  An application embeds Python when its own main program starts the interpreter with the C API, runs Python code in it, exchanges values with it, and shuts it down.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/extending/embedding.html", locator: "Extending and Embedding the Python Interpreter, Embedding Python in Another Application"}
see_also: [python/the-python-c-api, python/extension-modules, python/ctypes, python/the-limited-api-and-stable-abi, python/binding-generators]
sources:
  - url: https://docs.python.org/3/extending/embedding.html
    title: Embedding Python in Another Application
    locator: introduction; Very High Level Embedding; Beyond Very High Level Embedding; Pure Embedding; Extending Embedded Python; Compiling and Linking under Unix-like systems
    accessed: 2026-11-18
  - url: https://docs.python.org/3/c-api/interp-lifecycle.html
    title: Interpreter initialization and finalization
    locator: Before Python initialization; Py_Initialize; Py_IsInitialized; Py_FinalizeEx; Py_GetVersion
    accessed: 2026-11-18
  - url: https://docs.python.org/3/c-api/veryhigh.html
    title: The Very High Level Layer
    locator: PyRun_SimpleStringFlags
    accessed: 2026-11-18
  - url: https://docs.python.org/3/c-api/init_config.html
    title: Python Initialization Configuration
    locator: PyConfig_Get; PyConfig fields argv, isolated, orig_argv, parse_argv; Isolated Configuration; Python Configuration
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions
    locator: sys.builtin_module_names; sys.orig_argv
    accessed: 2026-11-18
  - url: https://docs.python.org/3/c-api/import.html
    title: Importing Modules
    locator: PyImport_AppendInittab
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0741/
    title: "PEP 741: Python Configuration C API"
    locator: header; Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3/c-api/threads.html
    title: Thread states and the global interpreter lock
    locator: Using the C API from foreign threads; PyGILState_Check
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/ctypes.html
    title: ctypes, A foreign function library for Python
    locator: CDLL; PyDLL; ctypes.pythonapi
    accessed: 2026-11-18
  - url: https://docs.python.org/3/c-api/subinterpreters.html
    title: Multiple interpreters in a Python process
    locator: Py_NewInterpreterFromConfig
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/concurrent.interpreters.html
    title: concurrent.interpreters, Multiple interpreters in the same process
    locator: header
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/atexit.html
    title: atexit, Exit handlers
    locator: introduction
    accessed: 2026-11-18
  - url: https://www.lua.org/manual/5.4/manual.html
    title: Lua 5.4 Reference Manual
    locator: 1 Introduction; 4 The Application Program Interface
    accessed: 2026-11-18
---

## Synopsis

```python run id=simple
import ctypes

api = ctypes.pythonapi
print(api.Py_IsInitialized())
status = api.PyRun_SimpleString(b"answer = 6 * 7\nprint('inside:', answer)")
print("status", status, "answer", answer)
```

```output for=simple
1
inside: 42
status 0 answer 42
```

An application embeds Python when its own main program, in C or C++, starts the Python interpreter and gives it code to run. The application calls `Py_InitializeFromConfig` (or `Py_Initialize`) at the start, then runs code with functions such as `PyRun_SimpleString`, and at the end calls `Py_FinalizeEx`. One use is to let the users of the application write scripts for it in Python. A C program that embeds Python cannot run in the sandbox of this site, so the examples call the same C API functions from Python with [`ctypes`](/python/ctypes/). In the example, `ctypes.pythonapi` gives the C API of the running interpreter. `PyRun_SimpleString` runs a string in the module `__main__`, so the variable `answer` that the string makes is also a variable of the script.

## Semantics

**r1.** Embedding is the reverse of extending. "Embedding provides your application with the ability to implement some of the functionality of your application in Python rather than C or C++."[^1] "The difference is that when you extend Python, the main program of the application is still the Python interpreter, while if you embed Python, the main program may have nothing to do with Python".[^1] "So if you are embedding Python, you are providing your own main program. One of the things this main program has to do is initialize the Python interpreter."[^1]

**r2.** The interpreter must be initialized before the program uses the C API. `Py_Initialize` "should be called before using any other Python/C API functions", except a few functions that the documentation lists.[^2] It "initializes the table of loaded modules (sys.modules), and creates the fundamental modules builtins, __main__ and sys."[^2] It has no result: "it is a fatal error if the initialization fails."[^2] `Py_InitializeFromConfig` takes a `PyConfig` structure and gives a `PyStatus`, so the application can handle an error.[^1] `Py_IsInitialized` tells if the interpreter is ready.[^2] Examples: [`simple`](#simple), [`embed`](#embed).

**r3.** The very high level layer runs source code and does not give values back. `PyRun_SimpleString` "Executes the Python source code from command in the __main__ module".[^3] It "Returns 0 on success or -1 if an exception was raised. If there was an error, there is no way to get the exception information."[^3] The traceback goes to `sys.stderr`. "Note that if an otherwise unhandled SystemExit is raised, this function will not return -1, but exit the process".[^3] Example: [`simple-error`](#simple-error).

**r4.** To exchange values with Python code, the application uses the same functions as an extension module. "At the cost of having to write more C code, you can achieve almost anything."[^1] The direction of the conversions is the reverse: "the data conversion steps are simply swapped to accommodate the different direction of the cross-language transfer."[^1] A typical sequence imports a module with `PyImport_Import`, gets a function with `PyObject_GetAttrString`, makes the arguments, calls the function with `PyObject_CallObject`, and converts the result to a C value.[^1] Each new reference must be released with `Py_DECREF`. Example: [`call`](#call).

**r5.** The configuration has two presets. With the Python configuration, the embedded interpreter acts as the regular `python` program: "Environments variables and command line arguments are used to configure Python, whereas global configuration variables are ignored."[^4] The isolated configuration is for applications: it "ignores global configuration variables, environment variables, command line arguments (PyConfig.argv is not parsed) and user site directory."[^4] With it, "Signal handlers are not installed."[^4] The option `-I` of the `python` program gives the same isolated mode: `PyConfig.isolated` is "Set to 1 by the -I command line option."[^4] Example: [`isolated`](#isolated).

**r6.** An embedded interpreter has no command line of its own. `Py_Initialize` "does not set sys.argv; use the Python Initialization Configuration API for that."[^2] `PyConfig.argv` sets `sys.argv`, and its "first entry should refer to the script file to be executed rather than the executable hosting the Python interpreter."[^4] When `PyConfig.parse_argv` is 1, Python parses the list "the same way the regular Python parses command line arguments, and strip Python arguments from argv."[^4] `PyConfig.orig_argv` keeps the original list, and it is `sys.orig_argv` in Python.[^4] The documentation of `sys` says: "The elements of sys.orig_argv are the arguments to the Python interpreter, while the elements of sys.argv are the arguments to the user’s program."[^5] Example: [`argv`](#argv).

**r7.** The application can give its own functions to the Python code as a built-in module. "The Python API allows this by extending the embedded interpreter. That is, the embedded interpreter gets extended with routines provided by the application."[^1] `PyImport_AppendInittab` adds "a single module to the existing table of built-in modules", and "This should be called before Py_Initialize()."[^6] The name of the module is then in `sys.builtin_module_names`, which is "A tuple of strings containing the names of all modules that are compiled into this Python interpreter."[^5] Example: [`inittab`](#inittab).

**r8.** PEP 741 gives a second configuration API, for Python 3.14.[^7] Its aim is to "configure the Python initialization without relying on C structures and the ability to make ABI-compatible changes in the future."[^7] It adds `PyInitConfig_AddModule`, for the "feature previously referred to as the “inittab”", and the functions `PyConfig_Get` and `PyConfig_Set` "to get and set the current runtime configuration."[^7] `PyConfig_Get` gives an option as a Python object, and "Some options are read from the sys attributes."[^4] For example, it reads the option `argv` from `sys.argv`.[^4]

**r9.** A thread that the application makes, and not Python, has no thread state. "If you need to call Python code from these threads (often this will be part of a callback API provided by the aforementioned third-party library), you must first register these threads with the interpreter by creating a new thread state and attaching it."[^8] Python 3.15 gives `PyThreadState_Ensure` and `PyThreadState_Release` for this, and the documentation names `PyGILState_Ensure` as the older function for the same task.[^8] `PyGILState_Check` returns 1 if the current thread has an attached thread state that matches, and 0 if not.[^8] The functions of `ctypes.pythonapi` keep the GIL during the call, because it is a `PyDLL`: "the Python GIL is not released during the function call".[^9] A `CDLL` releases the GIL: "The Python global interpreter lock is released before calling any function exported by these libraries, and reacquired afterwards."[^9] Example: [`gil`](#gil).

**r10.** One process can have more than one interpreter. `Py_NewInterpreterFromConfig` makes a sub-interpreter, "an (almost) totally separate environment for the execution of Python code."[^10] It has "separate, independent versions of all imported modules, including the fundamental modules builtins, __main__ and sys."[^10] Python code can use the same feature with the module `concurrent.interpreters` of Python 3.14.[^11]

**r11.** `Py_FinalizeEx` shuts the interpreter down. "Normally the return value is 0. If there were errors during finalization (flushing buffered data), -1 is returned."[^2] The shutdown calls the exit handlers of `atexit`, which run "upon normal program termination" or "upon interpreter shutdown".[^12] The documentation lists caveats: "The destruction of modules and objects in modules is done in random order", "Dynamically loaded extension modules loaded by Python are not unloaded", and "Some extensions may not work properly if their initialization routine is called more than once".[^2] Example: [`finalize`](#finalize).

**r12.** The application must link to the Python library. "pythonX.Y-config --cflags will give you the recommended flags when compiling", and "pythonX.Y-config --ldflags --embed will give you the recommended flags when linking".[^1] The documentation recommends the full path of the script, "To avoid confusion between several Python installations".[^1] An application that loads the library at run time can check the version with `Py_GetVersion`. Its result "is available to Python code as sys.version."[^2] Example: [`version`](#version).

## Examples

This is the very high level example of the Python documentation, with the configuration API.[^1] It needs the Python headers and the Python library, and the C image of this site has neither, so the block does not run here. Build it with the flags of `python3-config --cflags` and `python3-config --ldflags --embed` (rule r12).

```c norun reason=platform id=embed
#define PY_SSIZE_T_CLEAN
#include <Python.h>

int
main(int argc, char *argv[])
{
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    /* optional but recommended */
    status = PyConfig_SetBytesString(&config, &config.program_name, argv[0]);
    if (PyStatus_Exception(status)) {
        goto exception;
    }

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        goto exception;
    }
    PyConfig_Clear(&config);

    PyRun_SimpleString("from time import time,ctime\n"
                       "print('Today is', ctime(time()))\n");
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    return 0;

  exception:
     PyConfig_Clear(&config);
     Py_ExitStatusException(status);
}
```

The program prints the date and time. For an application, `PyConfig_InitIsolatedConfig` is often the better preset, because the environment of the user does not change the behavior of the application (rule r5).

When the code raises an exception, `PyRun_SimpleString` prints the traceback and returns -1. The caller gets no exception object. Here `sys.stderr` is the same stream as `sys.stdout`, so the traceback is in the output.

```python run id=simple-error
import ctypes
import sys

sys.stderr = sys.stdout
status = ctypes.pythonapi.PyRun_SimpleString(b"1 / 0")
print("status", status)
```

```output for=simple-error
Traceback (most recent call last):
  File "<string>", line 1, in <module>
ZeroDivisionError: division by zero
status -1
```

The next program does the steps of rule r4 with three C API functions: import a module, get a function from it, and call the function. In C, the application then converts the result with a function such as `PyLong_AsLong`. With `restype = ctypes.py_object`, `ctypes` makes a Python object from the result. This short program does not release the references, which a C program must do.

```python run id=call
import ctypes

api = ctypes.pythonapi
api.PyImport_ImportModule.argtypes = [ctypes.c_char_p]
api.PyImport_ImportModule.restype = ctypes.py_object
api.PyObject_GetAttrString.argtypes = [ctypes.py_object, ctypes.c_char_p]
api.PyObject_GetAttrString.restype = ctypes.py_object
api.PyObject_CallOneArg.argtypes = [ctypes.py_object, ctypes.py_object]
api.PyObject_CallOneArg.restype = ctypes.py_object

module = api.PyImport_ImportModule(b"math")
func = api.PyObject_GetAttrString(module, b"factorial")
print(api.PyObject_CallOneArg(func, 20))
```

```output for=call
2432902008176640000
```

The isolated configuration ignores the environment. The program starts the `python` program two times with the variable `PYTHONPATH`, the second time with `-I`. Only the first interpreter adds the directory to `sys.path`.

```python run id=isolated
import os
import subprocess
import sys

env = dict(os.environ, PYTHONPATH="/opt/app/plugins")
code = "import sys; print(sys.flags.isolated, '/opt/app/plugins' in sys.path)"
for flags in [[], ["-I"]]:
    out = subprocess.run([sys.executable, *flags, "-c", code], env=env, capture_output=True, text=True)
    print(flags, out.stdout.strip())
```

```output for=isolated
[] 0 True
['-I'] 1 False
```

The `python` program fills `PyConfig.argv` and `PyConfig.orig_argv` from its command line, and parses the list because `parse_argv` is 1. The options `-X dev` and `-X utf8` go into the configuration, and only the arguments of the program stay in `sys.argv`.

```python run id=argv
import subprocess
import sys

code = "import sys; print(sys.orig_argv[1:5]); print(sys.argv); print(sys.flags.dev_mode, sys.flags.utf8_mode)"
out = subprocess.run([sys.executable, "-X", "dev", "-X", "utf8", "-c", code, "one", "two"], capture_output=True, text=True)
print(out.stdout, end="")
```

```output for=argv
['-X', 'dev', '-X', 'utf8']
['-c', 'one', 'two']
True 1
```

A module that the application adds with `PyImport_AppendInittab` is a built-in module, as `sys` is. The import system finds it with `BuiltinImporter` and not on `sys.path`.

```python run id=inittab
import importlib.machinery
import sys

print("sys" in sys.builtin_module_names, "builtins" in sys.builtin_module_names)
spec = importlib.machinery.BuiltinImporter.find_spec("sys")
print(spec.origin, spec.loader.__name__)
print(sys.modules["__main__"].__name__, "builtins" in sys.modules)
```

```output for=inittab
True True
built-in BuiltinImporter
__main__ True
```

The module of the embedding documentation is named `emb`, and the script then does `import emb` and calls `emb.numargs()`.[^1]

The functions of `ctypes.pythonapi` run with the GIL, so `PyGILState_Check` gives 1. The same function through a `CDLL` gives 0, because `CDLL` releases the GIL for the call, as a C library function would run with no thread state. A thread of the `threading` module has its own thread state.

```python run id=gil
import ctypes
import threading

with_gil = ctypes.pythonapi.PyGILState_Check
without_gil = ctypes.CDLL(None).PyGILState_Check
print("PyDLL", with_gil(), "CDLL", without_gil())

thread = threading.Thread(target=lambda: print("thread", with_gil()))
thread.start()
thread.join()
```

```output for=gil
PyDLL 1 CDLL 0
thread 1
```

The exit handlers run after the main module ends, while the interpreter shuts down. In an embedding application, they run in `Py_FinalizeEx`.

```python run id=finalize
import atexit

atexit.register(print, "atexit handler")
print("end of the script")
```

```output for=finalize
end of the script
atexit handler
```

`Py_GetVersion` gives the same string as `sys.version`. An application that loads the Python library at run time can read it before it uses other functions.

```python run id=version
import ctypes
import sys

get_version = ctypes.pythonapi.Py_GetVersion
get_version.restype = ctypes.c_char_p
print(get_version().decode() == sys.version)
```

```output for=version
True
```

## Pitfalls

Check the `PyStatus` of each configuration call and of `Py_InitializeFromConfig`, and call `PyConfig_Clear` on each path. `Py_Initialize` cannot report an error; it stops the process (rule r2).

Do not use `PyRun_SimpleString` when the application must know the error. It gives only -1, and an uncaught `SystemExit` ends the whole process (rule r3). Use the lower level functions, and look at the exception with the error functions of the C API.

Use the isolated configuration for an application, unless the users must control it with `PYTHON*` variables. With the Python configuration, a variable such as `PYTHONPATH` in the environment of the user changes what the application imports (rule r5).

Call `PyImport_AppendInittab` before the interpreter starts. A call after `Py_Initialize` is too late (rule r7).

Attach a thread state before a thread of the application calls Python, and release it after the call (rule r9).

Do not initialize and finalize Python many times in one process. Some extension modules do not work after the second initialization, and modules that Python loaded stay in memory (rule r11).

Do not mix the headers of one Python version with the library of another. Use the `python3-config` of the same installation, with its full path (rule r12).

## Comparison

Lua is designed for embedding. As an extension language, it has no main program of its own: it "works embedded in a host client, called the embedding program or simply the host."[^13] "The host program can invoke functions to execute a piece of Lua code, can write and read Lua variables, and can register C functions to be called by Lua code."[^13] These are the same three tasks as in rules r3, r4 and r7. A large difference is the state. "The Lua library is fully reentrant: it has no global variables. It keeps all information it needs in a dynamic structure, called the Lua state."[^13] Each Lua API function takes the state as an argument. Most CPython C API functions take no interpreter argument; they use the thread state that is attached to the current thread (rule r9). So a C application can have many Lua states with no extra setup, but it needs sub-interpreters and thread states to run more than one Python interpreter (rule r10).

## References

[^1]: [Embedding Python in Another Application](https://docs.python.org/3/extending/embedding.html)
[^2]: [Interpreter initialization and finalization](https://docs.python.org/3/c-api/interp-lifecycle.html)
[^3]: [The Very High Level Layer](https://docs.python.org/3/c-api/veryhigh.html)
[^4]: [Python Initialization Configuration](https://docs.python.org/3/c-api/init_config.html)
[^5]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html)
[^6]: [Importing Modules](https://docs.python.org/3/c-api/import.html)
[^7]: [PEP 741: Python Configuration C API](https://peps.python.org/pep-0741/)
[^8]: [Thread states and the global interpreter lock](https://docs.python.org/3/c-api/threads.html)
[^9]: [ctypes, A foreign function library for Python](https://docs.python.org/3/library/ctypes.html)
[^10]: [Multiple interpreters in a Python process](https://docs.python.org/3/c-api/subinterpreters.html)
[^11]: [concurrent.interpreters, Multiple interpreters in the same process](https://docs.python.org/3/library/concurrent.interpreters.html)
[^12]: [atexit, Exit handlers](https://docs.python.org/3/library/atexit.html)
[^13]: [Lua 5.4 Reference Manual](https://www.lua.org/manual/5.4/manual.html)

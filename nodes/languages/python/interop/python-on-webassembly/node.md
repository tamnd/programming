---
tag: B8HCA1
kind: feature
owner: python
slug: python-on-webassembly
name: Python on WebAssembly
aliases: [WebAssembly, Wasm, wasm32, Emscripten, WASI, wasm32-emscripten, wasm32-wasi, Pyodide, PyScript, micropip, Python in the browser, JsProxy, PyProxy]
area: languages/python/interop
level: advanced
summary: >-
  CPython can be compiled to WebAssembly for Emscripten, which runs in browsers and Node.js, and for WASI. Pyodide is a Python distribution for the browser that is built on the Emscripten port.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/library/intro.html", locator: "Introduction, Notes on availability, WebAssembly platforms"}
see_also: [python/embedding-python, python/ctypes, python/extension-modules, python/the-python-c-api, python/binding-generators]
sources:
  - url: https://docs.python.org/3/library/intro.html
    title: Introduction (The Python Standard Library)
    locator: Notes on availability; WebAssembly platforms
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0011/
    title: "PEP 11: CPython platform support"
    locator: Tier 2; Tier 3; WASI
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0776/
    title: "PEP 776: Emscripten Support"
    locator: Abstract; Emscripten toolchain; Pyodide's tools
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0816/
    title: "PEP 816: WASI Support"
    locator: Abstract; Motivation; Specification
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions
    locator: sys._emscripten_info; sys.platform
    accessed: 2026-11-18
  - url: https://wasi.dev/
    title: Introduction, WASI.dev
    locator: introduction
    accessed: 2026-11-18
  - url: https://pyodide.org/en/stable/
    title: Pyodide
    locator: What is Pyodide?
    accessed: 2026-11-18
  - url: https://pyodide.org/en/stable/usage/quickstart.html
    title: Pyodide, Getting started
    locator: Setup; Running Python code; Complete example
    accessed: 2026-11-18
  - url: https://pyodide.org/en/stable/usage/wasm-constraints.html
    title: Pyodide Python compatibility
    locator: Python Standard library; Modules with limited functionality; Synchronous HTTP requests support; Removed modules; Included but not working modules; Threading and multiprocessing
    accessed: 2026-11-18
  - url: https://pyodide.org/en/stable/usage/type-conversions.html
    title: Pyodide, Type translations
    locator: Type translations; Round trip conversions; Implicit conversions
    accessed: 2026-11-18
  - url: https://pyodide.org/en/stable/usage/loading-packages.html
    title: Pyodide, Loading packages
    locator: introduction; How to chose between micropip.install and pyodide.loadPackage?
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/WebAssembly/Reference/JavaScript_interface/instantiate_static
    title: WebAssembly.instantiate(), MDN
    locator: description; Parameters; Return value
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/asyncio-task.html
    title: Coroutines and Tasks
    locator: Sleeping
    accessed: 2026-11-18
---

## Synopsis

```python run id=platform
import sys

WEBASSEMBLY = ("emscripten", "wasi")


def on_webassembly():
    return sys.platform in WEBASSEMBLY


print(sys.platform, on_webassembly())
```

```output for=platform
linux False
```

WebAssembly (Wasm) is a binary format for programs that a sandboxed virtual machine runs, for example the one in a web browser. CPython can be compiled to WebAssembly for two platforms. Emscripten is a toolchain whose programs run in JavaScript runtimes, such as browsers and Node.js. WASI is a set of system interfaces that runtimes such as Wasmtime give to WebAssembly programs. Python code finds the platform in `sys.platform`, which is `'emscripten'` or `'wasi'` on these builds. The sandbox of this site runs CPython on Linux, so the example prints `linux False`. Pyodide is a Python distribution for the browser that is built on the Emscripten port, and it lets Python code and JavaScript code call each other.

## Semantics

**r1.** The two WebAssembly platforms give only a part of the operating system. "The WebAssembly platforms wasm32-emscripten (Emscripten) and wasm32-wasi (WASI) provide a subset of POSIX APIs."[^1] "WebAssembly runtimes and browsers are sandboxed and have limited access to the host and external resources."[^1] Thus "Any Python standard library module that uses processes, threading, networking, signals, or other forms of inter-process communication (IPC), is either not available or may not work as on other Unix-like systems."[^1] The behavior also depends on the toolchain version, on the runtime ("browser, NodeJS, wasmtime") and on the build flags of Python.[^1]

**r2.** The two platforms have different levels of support. PEP 11 puts WASI in tier 2, with the notes "WASI SDK, Wasmtime", and Emscripten in tier 3, with the note "emcc".[^2] PEP 11 also says that WASI was tier 3 for Python 3.11 and 3.12, and that it is tier 2 for Python 3.13 and later versions.[^2] PEP 776 made Emscripten a tier 3 platform for Python 3.14.[^3]

**r3.** Python code identifies the platform with `sys.platform`. Its table of known values includes `'emscripten'` for Emscripten and `'wasi'` for WASI.[^5] On Emscripten, `sys._emscripten_info` is "A named tuple holding information about the environment on the wasm32-emscripten platform", and "The named tuple is provisional and may change in the future."[^5] Its fields are `emscripten_version`, `runtime`, `pthreads` and `shared_memory`. The field `runtime` is a "Runtime string, e.g. browser user agent, 'Node.js v14.18.2', or 'UNKNOWN'", and `pthreads` is "True if Python is compiled with Emscripten pthreads support."[^5] Examples: [`platform`](#platform), [`thread`](#thread).

**r4.** A build without thread support can import `threading`, but the module does not work. Pyodide lists `multiprocessing`, `threading` and sockets as modules that "can be imported, but are not functional due to the limitations of the WebAssembly VM".[^9] "Because Pyodide does not support threading or multiprocessing, packages that use threading or multiprocessing will not work without a patch to disable it."[^9] Pyodide gives a function that tells if the platform can start a thread: on Emscripten it reads `sys._emscripten_info.pthreads`, and on other platforms it checks that `platform.machine()` is not `wasm32` or `wasm64`.[^9] Example: [`thread`](#thread).

**r5.** There are no child processes. "Process-related APIs are not available or always fail with an error. That includes APIs that spawn new processes (fork(), execve()), wait for processes (waitpid()), send signals (kill()), or otherwise interact with processes."[^1] "The subprocess is importable but does not work."[^1] Code that must also run on these platforms catches the error and uses a different path. Example: [`process`](#process).

**r6.** On Emscripten, a blocking call stops the host. "Emscripten does not permit blocking I/O. Other blocking operations like sleep() block the browser event loop."[^1] Code that waits must give control back to the event loop. In `asyncio`, "sleep() always suspends the current task, allowing other tasks to run", and "Setting the delay to 0 provides an optimized path to allow other tasks to run."[^13] A call such as `time.sleep` does not give control back, so no other task runs during it. Example: [`blocking`](#blocking).

**r7.** Sockets and the file system are limited. "The socket module is available, but is limited and behaves differently from other platforms."[^1] "On Emscripten, sockets are always non-blocking and require additional JavaScript code and helpers on the server to proxy TCP through WebSockets".[^1] "WASI snapshot preview 1 only permits sockets from an existing file descriptor."[^1] "Functions related to file descriptors, file permissions, file ownership, and links are limited and don’t support some operations. For example, WASI does not permit symlinks with absolute file names."[^1] "Some functions are stubs that either don’t do anything and always return hardcoded values."[^1]

**r8.** WASI is a set of interface standards for WebAssembly programs outside the browser. "The WebAssembly System Interface (WASI) is a group of standards-track API specifications for software compiled to the W3C WebAssembly (Wasm) standard."[^6] It has three milestone releases, 0.1, 0.2 and 0.3, which are "Sometimes you will see these referred to as Preview 1/2/3 or P1/P2/P3."[^6] "WASI applications run in a capability-based sandbox: a Wasm module or component starts with no ambient authority and can only do what the host explicitly grants."[^6] Runtimes that support WASI include Wasmtime, WAMR, WasmEdge, wazero and Wasmer.[^6]

**r9.** Each Python version supports one WASI version and one WASI SDK version. PEP 816 says that CPython must target "the WASI version and the WASI SDK version".[^4] "The former is the specification of WASI itself while the latter is a version of clang with a specific version of wasi-libc as the sysroot that allows for compiling CPython to WASI."[^4] The two versions in use at the first beta release are the versions "to be supported for the lifetime of that Python version", and PEP 11 records them.[^4] The table in PEP 11 gives WASI 0.1 for Python 3.11 to 3.15, WASI SDK 24 for Python 3.13 and 3.14, and WASI SDK 33 for Python 3.15.[^2] The coordination is necessary because "WASI SDK is NOT forwards- or backwards-compatible due to the ABI of wasi-libc not making any compatibility guarantees".[^4]

**r10.** Emscripten compiles C and C++ for JavaScript runtimes. "Emscripten is a complete open source compiler toolchain. It compiles C/C++ code into WebAssembly/JavaScript executables, for use in JavaScript runtimes, including browsers and Node.js."[^3] Its compiler `emcc` "is a C and C++ compiler, linker, and a sysroot with headers for the system libraries."[^3] "Node.js is the most convenient way to test Emscripten programs."[^3] To make an extension module for this platform, the build must cross compile it: "pyodide build can be used to cross compile Python packages to run on Emscripten."[^3]

**r11.** Pyodide is the usual way to run Python in a browser. The CPython documentation says: "For Python in the browser, users should consider Pyodide or PyScript. PyScript is built on top of Pyodide, which itself is built on top of CPython and Emscripten."[^1] "Pyodide is a Python distribution for the browser and Node.js based on WebAssembly."[^7] "Pyodide is a port of CPython to WebAssembly/Emscripten."[^7] A web page loads the file `pyodide.js`, which "defines a single async function called loadPyodide() which sets up the Python environment and returns the Pyodide top level namespace."[^8] The function `pyodide.runPython` runs a string of Python code, and "If the code ends in an expression, it returns the result of the expression, translated to JavaScript objects".[^8] Example: [`pyodide`](#pyodide).

**r12.** Pyodide removes or changes some standard modules. "Most of the Python standard library is functional, except for the modules listed in the sections below."[^9] Some modules "are removed from the standard library to reduce download size and since they currently wouldn’t work in the WebAssembly VM", among them `curses`, `dbm`, `fcntl`, `termios`, `tkinter` and `venv`.[^9] The modules `pty` and `tty` "are present but cannot be imported due to a dependency on the termios package which has been removed".[^9] In `ssl`, "All the methods that depend on OpenSSL will not work as expected or will raise NotImplementedError."[^9] Code that must run on Pyodide tests if a module can be imported. Example: [`detect`](#detect).

**r13.** Values that cross between Python and JavaScript are converted or proxied. "By “converting” an object we mean producing a new object in the target language which is the equivalent of the object from the source language".[^10] "By “proxying” an object we mean producing a special object in the target language that forwards requests to the source language."[^10] A JavaScript object in Python is a `JsProxy`, and a Python object in JavaScript is a `PyProxy`.[^10] "We implicitly convert immutable types but not mutable types. This ensures that mutable Python objects can be modified from JavaScript and vice-versa."[^10] Because JavaScript has no equivalent of `tuple` and `bytes`, Pyodide proxies these two types too.[^10] Example: [`convert`](#convert).

**r14.** An `int` becomes a JavaScript `Number` or a `BigInt`. "An int is converted to a Number if the absolute value is less than or equal to Number.MAX_SAFE_INTEGER otherwise it is converted to a BigInt."[^10] In the other direction, "A Number is converted to an int if the absolute value is less than or equal to Number.MAX_SAFE_INTEGER and its fractional part is zero. Otherwise, it is converted to a float."[^10] A `Number` is a 64-bit float, so it cannot keep all the integers above this limit. Examples: [`convert`](#convert), [`number`](#number).

**r15.** On the host side, JavaScript loads a WebAssembly module with `WebAssembly.instantiate`. "The primary overload takes the WebAssembly binary code, in the form of a typed array or ArrayBuffer, and performs both compilation and instantiation in one step."[^12] Its promise resolves to an object with a `module` and an `instance`, and the instance "contains all the Exported WebAssembly functions."[^12] The optional `importObject` gives the values that the module imports, "such as functions or WebAssembly.Memory objects".[^12] "There must be one matching property for each declared import of the compiled module or else a WebAssembly.LinkError is thrown."[^12] The loader in `pyodide.js` does this work for the Python interpreter. Examples: [`instantiate`](#instantiate), [`imports`](#imports), [`linkerror`](#linkerror).

**r16.** Pyodide installs packages from wheels. With `micropip`, "Any pure Python package with a wheel available on PyPi is supported."[^7] "Many packages with C, C++, and Rust extensions have also been ported for use with Pyodide."[^7] `micropip.install()` is "for pure Python packages with wheels as well as Pyodide packages (including Emscripten/wasm32 binary wheels)", and `pyodide.loadPackage()` is "for packages built with Pyodide."[^11] "After importing Pyodide, only packages from the standard library are available."[^8]

## Examples

Pyodide's own example of a web page. The page loads Pyodide from a CDN and runs Python code in the browser, so the block does not run in the sandbox of this site.[^8]

```javascript norun reason=platform id=pyodide
// In a web page, after <script src=".../pyodide.js"></script>
async function main() {
  let pyodide = await loadPyodide();
  console.log(pyodide.runPython(`
    import sys
    sys.version
  `));
  pyodide.runPython("print(1 + 2)");
}
main();
```

The `runPython` call gives back the value of the last expression, so the first `console.log` prints the version string of the Python in the page (rule r11).

The function of Pyodide that tells if the platform can start threads, with a worker count that depends on it.[^9] On this site's Linux, the result is `True`.

```python run id=thread
import platform
import sys


def can_start_thread():
    if sys.platform == "emscripten":
        return sys._emscripten_info.pthreads
    return platform.machine() not in ("wasm32", "wasm64")


workers = 4 if can_start_thread() else 1
print(can_start_thread(), workers)
```

```output for=thread
True 4
```

Code that starts a child process catches the error, because on the WebAssembly platforms the process APIs fail (rule r5). On Linux the child runs.

```python run id=process
import subprocess
import sys


def run_child(code):
    try:
        done = subprocess.run(
            [sys.executable, "-c", code], capture_output=True, text=True
        )
    except OSError as error:
        return f"no child process: {type(error).__name__}"
    return done.stdout.strip()


print(run_child("print('from the child')"))
```

```output for=process
from the child
```

Two tasks that pause with `asyncio.sleep(0)` take turns. When the pause is `time.sleep(0)`, the event loop does not get control, so the first task does all its steps before the second task starts (rule r6). In a browser, a long `time.sleep` stops the page in the same way.

```python run id=blocking
import asyncio
import time


async def cooperative():
    await asyncio.sleep(0)


async def blocking():
    time.sleep(0)


async def worker(name, log, pause):
    for step in range(2):
        log.append(f"{name}{step}")
        await pause()


async def main(pause):
    log = []
    await asyncio.gather(worker("a", log, pause), worker("b", log, pause))
    return log


print(asyncio.run(main(cooperative)))
print(asyncio.run(main(blocking)))
```

```output for=blocking
['a0', 'b0', 'a1', 'b1']
['a0', 'a1', 'b0', 'b1']
```

A test for a module that Pyodide removes (rule r12). The program gives a simpler interface when `termios` is not there.

```python run id=detect
try:
    import termios
except ImportError:
    termios = None


def terminal_size_api():
    if termios is None:
        return "no termios: use a plain text interface"
    return "termios: can set raw mode"


print(terminal_size_api())
```

```output for=detect
termios: can set raw mode
```

A model of the rules of Pyodide for the JavaScript type that a Python value becomes (rules r13 and r14). The real conversion happens inside Pyodide; this function only follows its table. The last line shows why large integers cannot be a `Number`: as a 64-bit float, 2**53 + 1 is equal to 2**53.

```python run id=convert
MAX_SAFE_INTEGER = 2**53 - 1


def js_type_of(value):
    if isinstance(value, bool):
        return "Boolean"
    if isinstance(value, int):
        return "Number" if abs(value) <= MAX_SAFE_INTEGER else "BigInt"
    if isinstance(value, float):
        return "Number"
    if isinstance(value, str):
        return "String"
    if value is None:
        return "undefined"
    return "PyProxy"


for value in (True, 7, 2**53, 1.5, "text", None, (1, 2), [1, 2]):
    print(repr(value), "->", js_type_of(value))
print(float(2**53 + 1) == float(2**53))
```

```output for=convert
True -> Boolean
7 -> Number
9007199254740992 -> BigInt
1.5 -> Number
'text' -> String
None -> undefined
(1, 2) -> PyProxy
[1, 2] -> PyProxy
True
```

The same limit on the JavaScript side. A `Number` loses the last digit of 2**53 + 1, and a `BigInt` keeps it (rule r14).

```javascript run id=number
console.log(Number.MAX_SAFE_INTEGER);
console.log(String(2 ** 53 + 1));
console.log(String(2n ** 53n + 1n));
console.log(typeof 7, typeof 7n);
```

```output for=number
9007199254740991
9007199254740992
9007199254740993
number bigint
```

The host side of WebAssembly, with a module of 41 bytes that exports a function `add` of two 32-bit integers (rule r15). The bytes are the header, the type of the function, the export and the code.

```javascript run id=instantiate
const bytes = new Uint8Array([
  0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x07, 0x01, 0x60, 0x02, 0x7f, 0x7f, 0x01, 0x7f,
  0x03, 0x02, 0x01, 0x00,
  0x07, 0x07, 0x01, 0x03, 0x61, 0x64, 0x64, 0x00, 0x00,
  0x0a, 0x09, 0x01, 0x07, 0x00, 0x20, 0x00, 0x20, 0x01, 0x6a, 0x0b,
]);

WebAssembly.instantiate(bytes).then(({ module, instance }) => {
  console.log(WebAssembly.Module.exports(module));
  console.log(instance.exports.add(2, 3));
});
```

```output for=instantiate
[ { name: 'add', kind: 'function' } ]
5
```

A module that imports the function `env.log` from the host and exports `run`, which calls `log(42)`. The host gives the function in the import object. This is how a WebAssembly program, such as the Python interpreter, gets access to the outside world: only through what the host gives it (rules r8 and r15).

```javascript run id=imports
const bytes = new Uint8Array([
  0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x08, 0x02, 0x60, 0x01, 0x7f, 0x00, 0x60, 0x00, 0x00,
  0x02, 0x0b, 0x01, 0x03, 0x65, 0x6e, 0x76, 0x03, 0x6c, 0x6f, 0x67, 0x00, 0x00,
  0x03, 0x02, 0x01, 0x01,
  0x07, 0x07, 0x01, 0x03, 0x72, 0x75, 0x6e, 0x00, 0x01,
  0x0a, 0x08, 0x01, 0x06, 0x00, 0x41, 0x2a, 0x10, 0x00, 0x0b,
]);

const importObject = {
  env: { log: (value) => console.log("the host got", value) },
};

WebAssembly.instantiate(bytes, importObject).then(({ instance }) => {
  instance.exports.run();
});
```

```output for=imports
the host got 42
```

The same module with an import object that does not give `env.log`. The promise is rejected with a `LinkError` (rule r15).

```javascript run id=linkerror
const bytes = new Uint8Array([
  0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x08, 0x02, 0x60, 0x01, 0x7f, 0x00, 0x60, 0x00, 0x00,
  0x02, 0x0b, 0x01, 0x03, 0x65, 0x6e, 0x76, 0x03, 0x6c, 0x6f, 0x67, 0x00, 0x00,
  0x03, 0x02, 0x01, 0x01,
  0x07, 0x07, 0x01, 0x03, 0x72, 0x75, 0x6e, 0x00, 0x01,
  0x0a, 0x08, 0x01, 0x06, 0x00, 0x41, 0x2a, 0x10, 0x00, 0x0b,
]);

WebAssembly.instantiate(bytes, { env: {} }).catch((error) => {
  console.log(error instanceof WebAssembly.LinkError, error.name);
});
```

```output for=linkerror
true LinkError
```

## Pitfalls

- **Modules that import but do not work.** On these platforms, `threading`, `multiprocessing` and `subprocess` can be imported, but they do not work.[^1][^9] A successful import is thus not a test of the feature. Use a test such as the function of Pyodide in [`thread`](#thread), or catch the error as in [`process`](#process).
- **Blocking calls in the browser.** "Other blocking operations like sleep() block the browser event loop."[^1] Code for the browser must wait with `await`, as in [`blocking`](#blocking).
- **Proxies that are not released.** "Any time a Python to JavaScript translation occurs, it may create a PyProxy. To avoid memory leaks, you must store the PyProxy and destroy() it when you are done with it."[^10]
- **Large integers.** An `int` above `Number.MAX_SAFE_INTEGER` becomes a `BigInt`, which is a different type in JavaScript. "If the browser does not support BigInt then a Number will be used instead. In this case, conversion of large integers from Python to JavaScript is lossy."[^10]
- **Network calls in the browser.** In Pyodide, "all network calls are done via the browser", so the program has "very little or no control over certificates, timeouts, proxies and other network related settings", and requests can be "blocked by CORS policies".[^9]
- **Data that the module needs.** "The zoneinfo package will only work if you install the timezone data using the tzdata package".[^9]
- **A different WASI SDK.** An extension module for WASI must use the WASI SDK version of its Python version, because the SDK has no compatibility guarantee across versions (rule r9).[^4]

## Comparison

On WebAssembly, the host program starts the interpreter and gives it all access to the outside world, as an application does when it [embeds Python](/python/embedding-python/) with the C API. Pyodide is that host for the browser and Node.js, and a WASI runtime such as Wasmtime is the host for WASI.[^6][^7] Python is not the only language with an Emscripten target: "The Rust language also maintains an Emscripten target."[^3] The interface between Python and JavaScript in Pyodide does the same work as a [binding generator](/python/binding-generators/) does for C: it converts the simple immutable values and proxies the other objects.[^10]

## References

[^1]: [Introduction (The Python Standard Library)](https://docs.python.org/3/library/intro.html)
[^2]: [PEP 11: CPython platform support](https://peps.python.org/pep-0011/)
[^3]: [PEP 776: Emscripten Support](https://peps.python.org/pep-0776/)
[^4]: [PEP 816: WASI Support](https://peps.python.org/pep-0816/)
[^5]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html)
[^6]: [Introduction, WASI.dev](https://wasi.dev/)
[^7]: [Pyodide](https://pyodide.org/en/stable/)
[^8]: [Pyodide, Getting started](https://pyodide.org/en/stable/usage/quickstart.html)
[^9]: [Pyodide Python compatibility](https://pyodide.org/en/stable/usage/wasm-constraints.html)
[^10]: [Pyodide, Type translations](https://pyodide.org/en/stable/usage/type-conversions.html)
[^11]: [Pyodide, Loading packages](https://pyodide.org/en/stable/usage/loading-packages.html)
[^12]: [WebAssembly.instantiate(), MDN](https://developer.mozilla.org/en-US/docs/WebAssembly/Reference/JavaScript_interface/instantiate_static)
[^13]: [Coroutines and Tasks](https://docs.python.org/3/library/asyncio-task.html)

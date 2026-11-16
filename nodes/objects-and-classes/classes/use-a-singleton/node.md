---
tag: 1X7NQW
kind: task
slug: use-a-singleton
name: Use a singleton
aliases: [singleton, single instance, global instance, lazy singleton, shared instance, getInstance, module singleton]
area: objects-and-classes/classes
level: core
summary: Give a program exactly one IdGenerator that is made on first use, safely across threads, with a module, a static holder, a lazy value or a once cell.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [static-members, classes-and-instances, define-a-class, method-concept, singleton-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Singleton_pattern
    title: Singleton pattern, Wikipedia
    locator: Introduction; Criticism
  - url: https://docs.python.org/3/tutorial/modules.html
    title: 6. Modules, The Python Tutorial
    locator: 6.1 More on Modules
  - url: https://nodejs.org/api/esm.html
    title: Modules, ECMAScript modules, Node.js documentation
    locator: URLs
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html
    title: Chapter 12, Execution, The Java Language Specification, Java SE 25 Edition
    locator: 12.4.1 When Initialization Occurs; 12.4.2 Detailed Initialization Procedure
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.lazy-1
    title: Lazy<T> Class, .NET API reference
    locator: Definition; Examples
  - url: https://pkg.go.dev/sync
    title: sync package, Go standard library
    locator: func OnceValue
  - url: https://doc.rust-lang.org/std/sync/struct.OnceLock.html
    title: OnceLock in std::sync, The Rust Standard Library
    locator: Description
  - url: https://en.cppreference.com/cpp/language/storage_duration
    title: Storage duration, cppreference.com
    locator: Static local variables
  - url: https://kotlinlang.org/docs/object-declarations.html
    title: Object declarations and expressions, Kotlin documentation
    locator: Object declarations
---

## Task

Give the program exactly one `IdGenerator`. The generator hands out consecutive ids, 1, 2, 3 and so on, so two parts of the program that get ids from it never get the same id. Every part of the program must get the same instance, the instance must be made on the first use and not before, and two threads that ask for it at the same time must still get one instance.

Then use it. The function `singleton_report(calls)` gets the instance `calls` times and takes one id each time. It returns three numbers: the last id minus the first id, how many times the generator was constructed, and how many distinct instances the calls returned. With one instance, the result is always `[calls - 1, 1, 1]`. For example, `singleton_report(3)` is `[2, 1, 1]`. The input `calls` is from 1 to 1,000,000. The tests also get the instance from 8 threads at the same time and check that all of them get the same object.

The singleton pattern "restricts the instantiation of a class to a singular instance".[^1] It is one of the Gang of Four patterns, and it is also the pattern that people criticize most: see [Pitfalls](#pitfalls) before you use it.

## Solutions

| Language | Where the instance lives | What makes it once | Thread-safe |
|---|---|---|---|
| Python | class attribute `_instance` | `instance()` classmethod with a lock | yes, double-checked lock |
| JavaScript | a module-level `const` | the module is evaluated once | one thread per realm |
| TypeScript | a module-level `const` | the module is evaluated once | one thread per realm |
| Java | a static field of a private holder class | class initialization | yes, by the JVM |
| C# | a static `Lazy<IdGenerator>` | `Lazy<T>.Value` | yes, by default |
| Go | a package variable `Instance = sync.OnceValue(...)` | `sync.OnceValue` | yes |
| Rust | `static INSTANCE: OnceLock<IdGenerator>` | `OnceLock::get_or_init` | yes |
| C | a `static` struct in the `.c` file | `call_once` from `<threads.h>` | yes |
| C++ | a `static` local variable in `instance()` | static local initialization | yes, by the language |

- **Python uses a class attribute and a lock.** `IdGenerator.instance()` checks `_instance`, takes a lock only when it is `None`, and checks again under the lock, because another thread can make the instance between the two checks. After the first call, no call takes the lock. A module is the other usual Python singleton: "each module is only imported once per interpreter session",[^2] and the statements at the top level of a module "are executed only the first time the module name is encountered in an import statement". A module-level `id_generator = IdGenerator()` is then one object for the whole program. The solution uses the class form because the test can count constructions and threads.
- **JavaScript and TypeScript use the module.** "ES modules are resolved and cached as URLs",[^3] so every `import` of `./id-generator.js` gets the same module and the same `idGenerator` object. The class is not exported, so other modules cannot call `new IdGenerator()`. Plain JavaScript code runs on one thread per realm, so no lock is necessary. A worker thread has its own realm and loads its own copy of the module, so it gets its own instance.
- **Java uses a holder class.** The instance is a `static final` field of a private nested class `Holder`. The JVM initializes a class "immediately before the first occurrence" of a use such as a read of a static field,[^4] so `Holder` is initialized on the first call of `instance()`, not when `IdGenerator` loads. Class initialization is done under a lock: "For each class or interface C, there is a unique initialization lock LC." So two threads cannot make two instances, and `instance()` itself has no lock or `volatile` field. The constructor is `private`, and `nextId` uses an `AtomicLong`.
- **C# uses `Lazy<T>`.** The reference describes `Lazy<T>` in one line, "Provides support for lazy initialization."[^5] It runs the factory on the first read of `Value`, and the default mode is thread-safe. The class is `sealed` and its constructor is `private`. `NextId` uses `Interlocked.Increment`, because `Lazy<T>` protects only the creation: "Lazy initialization is thread-safe, but it doesn't protect the object after creation."
- **Go uses `sync.OnceValue`.** "OnceValue returns a function that invokes f only once and returns the value returned by f",[^6] and the documentation adds: "The returned function may be called concurrently." The package variable `Instance` is that function, so `idgen.Instance()` returns the same pointer each time. The type is exported, but the constructor `newIdGenerator` is not. Go code can still write `idgen.IdGenerator{}`, so the singleton is a convention that the package documents, not a rule that the compiler enforces.
- **Rust uses a `static OnceLock`.** The documentation of `OnceLock` starts: "A synchronization primitive which can nominally be written to only once."[^7] `INSTANCE.get_or_init(IdGenerator::new)` runs `new` on the first call and returns a `&'static IdGenerator` after that. A `static` must be `Sync`, so the counter is an `AtomicU64`; a plain `u64` field that `next_id` changes would not compile.
- **C uses `call_once`.** The struct is defined only in `id_generator.c`, so other files can hold a `struct id_generator *` but cannot declare one. `call_once(&instance_once, make_instance)` runs `make_instance` once, even when several threads call it. The counter is an `atomic_long`.
- **C++ uses a static local variable.** `static IdGenerator g;` inside `instance()` is made on the first call. The language makes this safe: "If multiple threads attempt to initialize the same static local variable concurrently, the initialization occurs exactly once".[^8] This form is often called the Meyers singleton. The copy constructor and copy assignment are deleted, so code cannot copy the instance.

### Alternatives

- **A language construct.** Kotlin has the singleton in the language: "objects allow you to define a class and create an instance of it in a single step",[^9] and the documentation adds: "The initialization of an object declaration is thread-safe and done on first access." `object IdGenerator { ... }` replaces all of the code above.
- **Make it eagerly.** If the instance is cheap to make and the program always uses it, make it at startup: a Java `static final` field in `IdGenerator` itself, a Go package variable `var instance = &IdGenerator{}`, or a C# `static readonly` field. The language still makes it once, and the code has no lazy step.
- **Pass the instance instead.** Make one `IdGenerator` in `main` and pass it to the code that needs it, as a constructor parameter or a function parameter. The program still has one instance, but a test can pass its own instance. This is dependency injection, and it removes most of the pitfalls below.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `single-call`: one call makes the instance and takes id 1, so the result is `[0, 1, 1]`.
- `pair`: the second call must return the instance of the first call, not make another one. A missing check in `instance()` shows here.
- `limit`: 1,000,000 calls. After the first call, `instance()` must not take a lock or make an object, or this case is slow. The ids fit in 32 bits, but the solutions use 64-bit counters, because a real program can run for a long time.
- The tests call `singleton_report` more than once in one process, so the second call starts at a higher id. The report uses the difference of the ids, so the result does not change.
- The thread test gets the instance from 8 threads at the same time, and then checks that the generator was constructed once.

## Pitfalls

- **Global state.** "Some consider the singleton to be an anti-pattern that introduces global state into an application, often unnecessarily."[^1] Any code can get the instance, so a function that uses it does not show this in its parameters, and two functions that look independent can affect each other through it. "This increased coupling can introduce difficulties with unit testing."
- **State that leaks from one test to the next.** A singleton lives as long as the process, and a test runner runs many tests in one process. A change that one test makes is still there in the next test. Code then adds a reset that only tests call:

```python run id=state-leaks-between-tests
class Config:
    _instance = None

    @classmethod
    def instance(cls):
        if cls._instance is None:
            cls._instance = cls()
        return cls._instance

    def __init__(self):
        self.values = {}


def test_turns_on_debug():
    Config.instance().values["debug"] = True


def test_default_is_off():
    return Config.instance().values.get("debug", False) is False


test_turns_on_debug()
print(test_default_is_off())  # fails: the state of the first test is still there
Config._instance = None  # a reset that only tests call
print(test_default_is_off())
```

```output for=state-leaks-between-tests
False
True
```

The reset is in production code but is not part of its real interface, and a test that forgets to call it gives a result that depends on the order of the tests. Passing the instance as a parameter avoids the problem.
- **Python `__new__` without a guard in `__init__`.** A common Python singleton overrides `__new__` to return the stored instance. But each call of `Settings()` still calls `__init__` on the instance that `__new__` returns, so `__init__` resets the state:

```python run id=init-runs-again
class Settings:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self):
        self.values = {}  # runs again on every call of Settings()


a = Settings()
a.values["debug"] = True
b = Settings()
print(a is b, a.values)
```

```output for=init-runs-again
True {}
```

Use a classmethod such as `instance()`, a module, or a guard in `__init__`.
- **Double-checked locking without a memory barrier.** In Java and C++, a field that is checked outside the lock must be `volatile` or atomic. Without it, another thread can see the reference before it sees the fields that the constructor set. The holder class, `Lazy<T>`, `sync.OnceValue`, `OnceLock`, `call_once` and the C++ static local do this correctly, so use them instead of a hand-written check.
- **A singleton per class loader, process or worker.** "One instance" means one per Java class loader, one per JavaScript realm and one per process. Two web application class loaders, two worker threads or two server processes each have their own instance, so a singleton cannot hand out unique ids across processes. Use a database sequence or a UUID for that.
- **The instance does not make its methods thread-safe.** The once step protects only the creation. `next_id` needs its own lock or atomic counter in each solution.

## Related tasks

- [[define-a-class]] defines a class whose instances each have their own state.
- [[static-members]] explains the static fields and class methods that hold the instance.
- [[classes-and-instances]] explains how a class makes instances.
- [[singleton-pattern]] explains the design pattern and when to use it.

## References

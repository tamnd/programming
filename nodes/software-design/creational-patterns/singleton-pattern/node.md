---
tag: NB3KXE
kind: pattern
slug: singleton-pattern
name: Singleton pattern
aliases: [Singleton]
area: software-design/creational-patterns
level: core
summary: >-
  A class makes sure that it has only one instance, and gives the whole program one global way to reach it. The global access causes most of the problems of the pattern.
family: creational-design
polarity: positive
intent: "Make sure that a class has only one instance, and give one global point of access to it."
languages_where_built_in: {kotlin: kotlin/object-declarations}
refactorings: [inline-singleton, limit-instantiation-with-singleton]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides described the pattern in Design Patterns (1994). In 2008 Miško Hevery wrote a series of posts on the Google Testing Blog against singletons as global state, and in a 2009 interview Gamma said that he was in favour of dropping the pattern from the catalog.
see_also: [factory-method-pattern, strategy-pattern, unit-testing, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Singleton (Intent, Motivation, Consequences, Implementation, Known Uses, Related Patterns)
  - url: https://www.informit.com/articles/article.aspx?p=1404056
    title: Larry O'Brien, Design Patterns 15 Years Later, An Interview with Erich Gamma, Richard Helm, and Ralph Johnson, InformIT, October 22, 2009
    locator: Gamma's answer on the patterns to drop from a revised catalog (Singleton)
  - url: https://testing.googleblog.com/2008/08/by-miko-hevery-so-you-join-new-project.html
    title: Miško Hevery, Singletons are Pathological Liars, Google Testing Blog, August 17, 2008
    locator: the CreditCard test example and the conclusion on global state
  - url: https://testing.googleblog.com/2008/08/root-cause-of-singletons.html
    title: Miško Hevery, Root Cause of Singletons, Google Testing Blog, August 2008
    locator: the difference between the Singleton pattern and a single instance; global state is transitive; immutable singletons and loggers
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 6, Creation, Inline Singleton; Chapter 9, Protection, Limit Instantiation with Singleton (problem and solution of each)
  - url: https://learn.microsoft.com/en-us/dotnet/core/extensions/dependency-injection/service-lifetimes#singleton
    title: Service lifetimes, .NET dependency injection, Microsoft Learn
    locator: Singleton
  - url: https://martinfowler.com/articles/injection.html
    title: Martin Fowler, Inversion of Control Containers and the Dependency Injection pattern, January 2004
    locator: Using a Service Locator
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 49 (Singleton as Memoization)
  - url: https://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html
    title: Bacon, Bloch, Bogda and others, The "Double-Checked Locking is Broken" Declaration
    locator: the main text; Making it work for static singletons; Fixing Double-Checked Locking using Volatile
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Item 3, Enforce the singleton property with a private constructor or an enum type; Item 83, Use lazy initialization judiciously
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.4.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 12.4.1 When Initialization Occurs; 12.4.2 Detailed Initialization Procedure
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.9
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.9 Enum Classes (no instances other than the enum constants; reflective instantiation prohibited; serialization)
  - url: https://docs.python.org/3.14/faq/programming.html#how-do-i-share-global-variables-across-modules
    title: Programming FAQ, Python 3.14 documentation
    locator: How do I share global variables across modules?
  - url: https://docs.python.org/3.14/reference/import.html#the-module-cache
    title: The Python Language Reference, The import system, Python 3.14
    locator: 5.3.1 The module cache
  - url: https://docs.python.org/3.14/reference/datamodel.html#object.__new__
    title: The Python Language Reference, Data model, Python 3.14
    locator: object.__new__
  - url: https://docs.python.org/3.14/library/functools.html#functools.cache
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.cache and functools.lru_cache (thread safety note)
  - url: https://tc39.es/ecma262/#sec-HostLoadImportedModule
    title: ECMAScript Language Specification (draft)
    locator: 16.2.1.10 HostLoadImportedModule (the same result for the same referrer and module request)
  - url: https://nodejs.org/api/modules.html#caching
    title: Node.js documentation, Modules, CommonJS modules
    locator: Caching
  - url: https://kotlinlang.org/docs/object-declarations.html
    title: Kotlin documentation, Object declarations and expressions
    locator: Object declarations (initialization is thread-safe and done on first access)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/classes#1512-static-constructors
    title: C# language specification, Classes
    locator: 15.12 Static constructors
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.lazy-1
    title: .NET API reference, System.Lazy<T>
    locator: class description and remarks (thread safety modes)
  - url: https://pkg.go.dev/sync#OnceValue
    title: Go standard library, package sync
    locator: func OnceValue; type Once
  - url: https://go.dev/ref/spec#Package_initialization
    title: The Go Programming Language Specification
    locator: Package initialization; Program initialization
  - url: https://doc.rust-lang.org/std/sync/struct.LazyLock.html
    title: Rust standard library, std::sync::LazyLock
    locator: struct description
  - url: https://doc.rust-lang.org/reference/items/static-items.html#mutable-statics
    title: The Rust Reference, Static items
    locator: Mutable statics (rule items.static.mut.safety)
  - url: https://timsong-cpp.github.io/cppwp/n4950/stmt.dcl#3
    title: Working Draft, Programming Languages, C++ (N4950)
    locator: "[stmt.dcl] paragraph 3 (block variables with static storage duration, concurrent initialization)"
  - url: https://timsong-cpp.github.io/cppwp/n4950/basic.start.dynamic
    title: Working Draft, Programming Languages, C++ (N4950)
    locator: "[basic.start.dynamic] (unordered and indeterminately sequenced dynamic initialization of non-block variables)"
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/lang/Runtime.java#L124-L141
    title: OpenJDK jdk-25+36 source, java/lang/Runtime.java
    locator: currentRuntime, getRuntime and the private constructor, lines 124 to 141
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/Constructor.html#newInstance(java.lang.Object...)
    title: Java SE 25 API, java.lang.reflect.Constructor
    locator: newInstance (IllegalArgumentException if the constructor pertains to an enum class)
---

## Intent

Make sure that a class has only one instance, and give one global point of access to it.[^1]

The intent has two parts, and they are different things. The first part is a constraint on the number of instances. The second part is a way to find the instance from anywhere. Most of the debate about the pattern is about the second part.

## Problem

Some things exist once in a running program. There is one configuration that the program read at start, one connection pool for a database, one table of loaded plugins. Design Patterns gives a printer spooler, a file system and a window manager as examples.[^1] Two parts of the program can each make their own configuration object. Then a change made through one object does not show in the other, and the program reads the files twice.

The forces are these. There must be exactly one instance, and the type must enforce this, not a comment. Code in many places needs the instance, and it is not always convenient to pass it down. The instance can be expensive to create, so it should be created on first use, not at start. And in a program with threads, two threads that need it at the same moment must get the same instance.

## Solution

The class takes over the job of creating its instance. Its constructor is private, so no other code can call it. A static field holds the one instance, and a static operation returns it, and creates it on the first call.[^1]

```text
 Settings
 ------------------------------------------
 - instance: Settings   (static)
 - Settings()           (private constructor)
 + get(): Settings      (static)
     if instance is empty: instance = new Settings()
     return instance
 + level, set_level(...)  (the actual job of the class)
```

The participant is the Singleton class itself. Clients reach the instance only through the static operation.[^1] The class therefore has two jobs: its own work, and the control of its life cycle.

**Most languages have a construct that gives the single instance with less code.** The construct also solves the thread problem, because the runtime guarantees that the initialization runs once:

| Language | Construct | Guarantee |
|---|---|---|
| Python | a module | a module runs once; later imports get the same object from `sys.modules`[^13][^14] |
| JavaScript | an ES module | the host returns the same module record for the same request[^17][^18] |
| Kotlin | an `object` declaration | initialization is thread-safe and done on first access[^19] |
| Java | a static field of a nested holder class, or an `enum` with one constant | class initialization runs once, under a lock[^11][^12] |
| C# | a static constructor, or `Lazy<T>` | a static constructor runs at most once; `Lazy<T>` is thread-safe by default[^20][^21] |
| Go | a package-level variable, or `sync.OnceValue` | a package is initialized once; `OnceValue` calls its function once[^22][^23] |
| Rust | a `static` with `LazyLock` | initialized on first access, and other threads block until it is done[^24] |
| C++ | a function-local `static` | initialized the first time control passes the declaration; concurrent callers wait[^26] |

The Python FAQ states it directly. There is only one instance of each module, and a module is also the basis for the singleton design pattern.[^13] Norvig makes a similar point for dynamic languages: memoization of the creation function gives the same effect.[^8]

## Example

The example files show one program in each language. A `Settings` object prints "creating the settings" when it is created. The main code reads the level, `info`. A worker, on a second thread where the language has threads, gets the settings, and sets the level to `debug`. The main code reads the level again and sees `debug`, and the program checks that both got the same instance. The creation message appears once.

| Language | Form |
|---|---|
| Python | a module, `settings.py`, imported by the main module and by the worker |
| JavaScript | an ES module, imported statically by `main.js` and dynamically by the worker |
| TypeScript | a class with a `private` constructor and a static accessor |
| Java | the holder class idiom |
| Kotlin | an `object` declaration |
| C# | a `Lazy<T>` in a static field |
| Go | `sync.OnceValue` |
| Rust | `static LazyLock<Mutex<Settings>>` |
| C++ | a function-local static in a static member function |

## Consequences

Design Patterns lists these benefits.[^1] Access to the instance is controlled, because the class owns it. The name space stays clean: the pattern is an improvement over a global variable that any code can overwrite. The class can be subclassed, and the application can choose the subclass. The same mechanism can allow a fixed number of instances greater than one. It is more flexible than a class with only static operations, which cannot be subclassed or replaced.

The costs come from the global access, and from the hand-written lazy creation.

**Hidden dependencies.** A function that calls `Settings.get()` inside its body depends on the settings, but its signature does not say so. Hevery shows this with a credit card test that passes in the full suite and throws a `NullPointerException` when it runs alone. The card used a database, a queue and a processor through singletons, and the suite had set them up in another test. He calls such APIs liars, because they hide what they need, and he concludes that singletons are global state.[^3]

**State that leaks between tests.** The instance lives as long as the program. A test that changes it changes it for the tests that run after it, so the result depends on the order of the tests:

```python wrong id=test-order
class Settings:
    level = "info"  # one shared value, like the field of a singleton


def test_debug_logging():
    Settings.level = "debug"
    return Settings.level == "debug"


def test_default_level():
    return Settings.level == "info"


for name, order in [("first order", [test_default_level, test_debug_logging]),
                    ("second order", [test_debug_logging, test_default_level])]:
    results = [f"{test.__name__} {'pass' if test() else 'FAIL'}" for test in order]
    print(f"{name}: {', '.join(results)}")
```

```output for=test-order
first order: test_default_level pass, test_debug_logging pass
second order: test_debug_logging pass, test_default_level FAIL
```

The fix that Hevery gives is to pass the dependencies to the code that needs them.[^3] Each test then makes its own object:

```python run id=test-order-fixed
class Settings:
    def __init__(self, level="info"):
        self.level = level


def test_debug_logging():
    settings = Settings()
    settings.level = "debug"
    return settings.level == "debug"


def test_default_level():
    settings = Settings()
    return settings.level == "info"


for name, order in [("first order", [test_default_level, test_debug_logging]),
                    ("second order", [test_debug_logging, test_default_level])]:
    results = [f"{test.__name__} {'pass' if test() else 'FAIL'}" for test in order]
    print(f"{name}: {', '.join(results)}")
```

```output for=test-order-fixed
first order: test_default_level pass, test_debug_logging pass
second order: test_debug_logging pass, test_default_level pass
```

**Lazy creation and threads.** The check "is the instance empty?" and the creation are two steps. Two threads can both pass the check before either one stores the instance. The barrier below forces that order, so the program makes two instances every time it runs:

```python wrong id=race
import threading


class Settings:
    created = 0

    def __init__(self):
        Settings.created += 1


_instance = None
_both_checked = threading.Barrier(2)


def get_settings():
    global _instance
    if _instance is None:
        _both_checked.wait()  # both threads have seen None
        _instance = Settings()
    return _instance


threads = [threading.Thread(target=get_settings) for _ in range(2)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(f"instances created: {Settings.created}")
```

```output for=race
instances created: 2
```

The fix is to make the check and the creation one step, with a lock:

```python run id=race-fixed
import threading


class Settings:
    created = 0

    def __init__(self):
        Settings.created += 1


_instance = None
_lock = threading.Lock()


def get_settings():
    global _instance
    with _lock:
        if _instance is None:
            _instance = Settings()
        return _instance


threads = [threading.Thread(target=get_settings) for _ in range(8)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(f"instances created: {Settings.created}")
```

```output for=race-fixed
instances created: 1
```

Programs often add a first check before the lock, so that later calls do not take the lock. This is double-checked locking, and it is not safe in every language. In Java, double-checked locking on a plain field is broken: another thread can see the reference before it sees the fields of the object. The Declaration signed by Bacon, Bloch, Pugh and others explains why. It gives two fixes: a `volatile` field, or a static field in a separate holder class. The class initialization of the JVM makes the second fix safe.[^9][^11] `functools.cache` on a function with no arguments is not a strict once in Python either. Its documentation says that the wrapped function can run more than once. This happens if a second thread calls it before the first call has finished.[^16] The language constructs in the table above avoid all of this.

**Python `__new__` and `__init__`.** A common Python singleton overrides `__new__` to return the stored instance. But when `__new__` returns an instance of the class, Python then calls `__init__` on it, on every call.[^15] So each call to `Settings()` resets the state:

```python wrong id=new-init
class Settings:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            print("creating the settings")
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self):
        self.level = "info"


first = Settings()
first.level = "debug"
second = Settings()
print(first is second, first.level)
```

```output for=new-init
creating the settings
True info
```

There is one instance, as the identity check shows, but the `debug` level is lost. A module, or a function that returns a stored object, does not have this trap.

**Uniqueness can be broken from outside.** In Java, a private constructor does not stop reflection: `setAccessible(true)` lets code call it. Bloch names reflection and serialization as the two ways to get a second instance, and recommends an enum with one constant.[^10] The Java Language Specification says three things about an enum class. It has no instances other than its constants. Reflective instantiation of enum classes is prohibited. Serialization does not create duplicates.[^12] The program below makes a second instance of a class singleton, and fails to do so for an enum singleton:

```java run id=reflection
import java.lang.reflect.Constructor;

public class Main {
    static final class ClassSingleton {
        static final ClassSingleton INSTANCE = new ClassSingleton();

        private ClassSingleton() {}
    }

    enum EnumSingleton {
        INSTANCE
    }

    public static void main(String[] args) throws Exception {
        Constructor<ClassSingleton> c = ClassSingleton.class.getDeclaredConstructor();
        c.setAccessible(true);
        ClassSingleton second = c.newInstance();
        System.out.println("class: same instance: " + (second == ClassSingleton.INSTANCE));

        Constructor<EnumSingleton> e =
                EnumSingleton.class.getDeclaredConstructor(String.class, int.class);
        e.setAccessible(true);
        try {
            e.newInstance("SECOND", 1);
        } catch (IllegalArgumentException ex) {
            System.out.println("enum: " + ex.getMessage());
        }
    }
}
```

```output for=reflection
class: same instance: false
enum: Cannot reflectively create enum objects
```

The `IllegalArgumentException` for an enum class is part of the specification of `Constructor.newInstance`.[^29]

**Order of initialization.** Design Patterns warns that C++ does not define the order in which it initializes global objects across translation units. So a singleton held in a global object can be used before it exists.[^1] The C++ standard still leaves the dynamic initialization of such non-block variables unordered or indeterminately sequenced.[^27] A function-local static avoids the problem, because it is initialized when control first passes through it.[^26] The same problem appears between singletons: when the constructor of one singleton calls another, the order of creation is hidden in the code. Hevery's credit card example needed the database before the queue, and the queue before the processor, and nothing in the API showed it.[^3]

**Rust makes the global mutable state explicit.** A `static mut` can be read or written only in an `unsafe` block.[^25] A safe global with state needs a type such as `LazyLock<Mutex<T>>`, so the lock is visible at every use. This does not remove the hidden dependency, but it does stop a data race.

## When not to use it

The fair summary of the criticism separates the two parts of the intent. Hevery makes this distinction himself. A class with one instance in a program is fine: the application creates it once and passes it to the code that needs it. The problem is the Singleton pattern in the narrow sense, a private constructor with a global static accessor, because it makes global state.[^4] Global state is transitive: any object that the singleton holds is also global.[^4] He accepts two cases. A singleton that is immutable, and that holds only immutable objects, has no state to leak. A logger is acceptable in practice, because information flows into it and the program's behaviour does not depend on what it holds.[^4]

Gamma is one of the four authors. In a 2009 interview he said that he was in favour of dropping Singleton from a revised catalog. His reason was that its use is almost always a design smell.[^2]

So do not use the pattern in these cases:

- **To avoid passing an argument.** Perhaps the singleton exists because passing the object through three layers is tedious. Then the dependency is real, and the signatures should show it. Kerievsky's Inline Singleton describes the fix. Code needs an object but does not need a global point of access to it. Move the object to a class that stores it and gives access to it, and delete the singleton.[^5]
- **In a program with a dependency injection container.** The container can give one instance to every consumer without a global accessor. The .NET documentation says it plainly. If the app requires singleton behaviour, let the service container manage the lifetime, and do not implement the singleton design pattern.[^6] A service registered with `AddSingleton` must be thread-safe, because many requests share it.[^6]
- **For state that tests need to control.** A test cannot replace a static accessor without extra hooks. If tests need a fake, pass the object in.
- **When "one" is only true today.** One database, one window and one user become two databases, a multi-window app and a server with many users. A singleton turns that change into a search through the whole program.

Use it, or a language construct that gives it, when three things are true. The instance is stateless or immutable. It truly belongs to the process. Access goes in one direction, as for a logger or a cache of constant data. Even then, prefer to create it at the top of the program, the place where the objects are wired together, and pass it down. Kerievsky's own refactoring towards the pattern, Limit Instantiation with Singleton, starts from a performance problem. Code creates many instances of an object, and that uses too much memory or slows the system.[^5] It does not start from a need for global access.

## Variants and related patterns

**A registry of singletons.** Design Patterns discusses how a singleton class can be subclassed: the base class looks up the instance to use in a registry, by name.[^1] Fowler's service locator is a related idea: one object that knows how to get the services of the application. He notes that his locator is a registry, not a singleton, even though he reaches it through a singleton in his example.[^7]

**Dependency injection with a singleton lifetime.** The container creates one instance and gives it to every consumer through a constructor. This keeps the "one instance" part of the intent and drops the "global access" part.[^6]

**Abstract Factory, Builder and Prototype.** Design Patterns notes that these patterns can use Singleton, and that a concrete factory is often a singleton.[^1] The [[factory-method-pattern]] page shows a creator that is passed in instead.

**Memoization.** A function with no arguments that caches its result is a lazy singleton. Norvig presents Singleton as memoization in dynamic languages.[^8] Go `sync.OnceValue` and Python `functools.cache` both have this shape, with the thread caveat above for `functools.cache`.[^16][^22]

**Static classes.** A class with only static members also has no second instance. Design Patterns notes that it is less flexible, because a static operation cannot be overridden and the class cannot be replaced by a subclass.[^1] Its state is still global.

## Known uses

- OpenJDK: `java.lang.Runtime` keeps one instance in a private static final field, returns it from `getRuntime`, and has a private constructor, in [Runtime.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/lang/Runtime.java#L124-L141).[^28]
- Design Patterns records the `ChangeSet current` of Smalltalk-80 and the `Session` object of InterViews as known uses.[^1]

## References

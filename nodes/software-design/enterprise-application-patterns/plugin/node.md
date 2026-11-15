---
tag: DJR162
kind: pattern
slug: plugin
name: Plugin
aliases: [Plugin pattern, plug-in, runtime linking by configuration]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A plugin factory reads a configuration when the program starts and creates the implementation that it names, so the program links classes during configuration and not at compile time.
family: architectural
polarity: positive
intent: "Link classes during configuration rather than compilation."
history:
  year: 2002
  people: [David Rice, Matt Foemmel]
  note: David Rice and Matt Foemmel wrote the pattern for Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [separated-interface, dependency-injection, service-locator, registry, factory-method-pattern, inversion-of-control, test-double, gateway]
sources:
  - url: https://martinfowler.com/eaaCatalog/plugin.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Plugin
    locator: pattern summary, by David Rice and Matt Foemmel
  - url: https://packaging.python.org/en/latest/specifications/entry-points/
    title: Python Packaging User Guide, Entry points specification
    locator: Data model
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/ServiceLoader.html
    title: Java SE 21 API, java.util.ServiceLoader
    locator: class description, Deploying service providers on the class path
  - url: https://pkg.go.dev/database/sql
    title: Go standard library, database/sql
    locator: func Register
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Plugin
---

## Intent

A plugin "Links classes during configuration rather than compilation."[^1] The code uses an interface. A configuration, which is read when the program starts, names the class that implements it. A plugin factory creates that class.

## Problem

A [[separated-interface]] lets one interface have several implementations, for example a key generator that is a counter in memory for unit tests and a database sequence in production. Something must choose the implementation. The usual answer is a factory with a conditional statement that tests the mode of the program.

With many factories, each one has its own conditional. A new deployment, such as tests against an in-memory database, needs a change in each factory, then a new build and a new deployment. The catalog says: "Configuration shouldn't be scattered throughout your application, nor should it require a rebuild or redeployment."[^1]

## Solution

"Plugin solves both problems by providing centralized, runtime configuration."[^1] The parts are:

- **The interface.** The code uses only the interface, such as `KeyGenerator`.
- **The implementations.** Each one implements the interface. The code that uses the interface does not import them.
- **The configuration.** For each deployment, a file or a table maps each interface to the name of an implementation. It is the only place that names the classes.
- **The plugin factory.** It reads the configuration, finds the class by its name, creates an object and checks that the object implements the interface. A language with reflection loads the class by name. In other languages, each plugin registers a constructor under its name.

```text
  test.config:        KeyGenerator = CounterKeyGenerator
  production.config:  KeyGenerator = SequenceKeyGenerator
  code:  PluginFactory(config).get("KeyGenerator")  -->  a KeyGenerator
```

In the following program, the configuration names a function as `module:attribute`, and `load` imports the module and gets the function. This is the form that Python entry points use.[^2]

```python run id=load-by-reference
import importlib


def load(reference):  # "module:attribute", the form of a Python entry point
    module_name, _, attribute = reference.partition(":")
    return getattr(importlib.import_module(module_name), attribute)


for configured in ["json:dumps", "pprint:pformat"]:  # read from a configuration file
    serialize = load(configured)
    print(configured, "->", serialize({"id": 7}))
```

```output
json:dumps -> {"id": 7}
pprint:pformat -> {'id': 7}
```

## Example

Each example has a `KeyGenerator` interface with two implementations: `CounterKeyGenerator` for tests and `SequenceKeyGenerator`, which stands for a database sequence, for production. Three configurations name the implementation. The program gives each configuration to a `PluginFactory` and asks for three keys. The test configuration gives keys 1 to 3 and the production configuration gives keys 1001 to 1003. The third configuration names a class that does not exist, and the factory gives a clear error.

## Consequences

**Benefits.**

- **One place for the configuration.** A new deployment is a new configuration, with no change of the code.
- **No new build.** The program chooses the implementation when it starts, so the same build runs in tests and in production.
- **New implementations from outside.** A package that the program does not know when it is built can add an implementation, if the program loads it by name.

**Liabilities.**

**Without a plugin, each factory has its own test of the mode, and they can disagree.** In the following program, a new mode `staging` is added to one factory but not to the other. The staging program sends no real mail, but it uses the database sequence of production.

```python wrong id=factories-disagree-on-the-mode
MODE = "staging"  # a new deployment mode


def key_generator():  # one factory
    if MODE == "test":
        return "counter in memory"
    return "database sequence"


def mailer():  # another factory, in another module
    if MODE in ("test", "staging"):  # only this factory knows the new mode
        return "fake mailer"
    return "real mailer"


print(f"{MODE}: {key_generator()} + {mailer()}")
```

```output
staging: database sequence + fake mailer
```

Give each mode one configuration that names the implementation of each interface, and let one plugin factory read it.

**Errors move to the start of the program.** A wrong class name in the configuration is not found by the compiler. The factory must check the configuration when the program starts, and give an error that names the missing plugin.

**Harder to follow.** To know which class runs, a reader must look at the configuration and not at the code.

**Loading code by name is a risk.** If an attacker can change the configuration, the program loads the code that the attacker chooses. Keep the configuration as safe as the code.

## When not to use it

**When there is one implementation.** A plugin factory and a configuration add work for nothing.

**When the choice never changes after the build.** If the build already knows the implementation, for example a library for one platform, a compile-time choice is simpler and the compiler checks it.

## Variants and related patterns

**Service providers.** Java has a standard plugin mechanism. "A service is a well-known interface or class for which zero, one, or many service providers exist."[^3] A JAR file names its providers in a provider-configuration file in the directory `META-INF/services`, and `ServiceLoader` loads them.[^3]

**Entry points.** "Entry points are a mechanism for an installed distribution to advertise components it provides to be discovered and used by other code."[^2] The specification adds: "Applications can use entry points to load plugins".[^2]

**Registration by name.** In Go, "Register makes a database driver available by the provided name."[^4] Each driver package registers itself, and the program chooses a driver by the name in its configuration.

**Related patterns.** [[dependency-injection]] containers often read the same kind of configuration and then give each object its dependencies. A [[service-locator]] or a [[registry]] can keep the objects that the plugin factory creates. A plugin is a common way to choose a [[service-stub]] in tests in place of a [[gateway]] to a real service. The book shows the pattern with a separated interface and a factory that reads a properties file.[^5]

## Known uses

- Java `ServiceLoader` finds the providers of a service on the class path or the module path.[^3]
- Python tools such as Pygments load plugins from entry points of other installed packages.[^2]
- Go database drivers register themselves with `sql.Register`.[^4]

## References

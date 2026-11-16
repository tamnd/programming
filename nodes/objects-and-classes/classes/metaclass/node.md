---
tag: 7KBXXM
kind: concept
slug: metaclass
name: Metaclass
aliases: [metaclass, meta-class, class of a class, metaobject, type of a type, eigenclass, singleton class]
area: objects-and-classes/classes
level: advanced
summary: >-
  A metaclass is the class of a class: it decides how a class statement makes a class object, and what that class object can do at run time.
applies_to: core-languages
viz: optional
history:
  year: 1980
  people: [Alan Kay, Dan Ingalls, Adele Goldberg]
  note: Smalltalk-80 gave each class its own metaclass, so a class could have its own methods and variables. Python later made type the metaclass of its classes and let code replace it.
see_also: [classes-and-instances, static-members, class-inheritance, runtime-reflection, syntactic-macros, registry, data-class, enumeration]
same_as:
  wikipedia: Metaclass
sources:
  - url: https://en.wikipedia.org/wiki/Metaclass
    title: Metaclass, Wikipedia
    locator: Introduction; In Ruby; In Objective-C; Support in languages and tools
  - url: https://docs.python.org/3/reference/datamodel.html
    title: 3. Data model, The Python Language Reference
    locator: 3.3.3 Customizing class creation; 3.3.3.1 Metaclasses; 3.3.3.6 Creating the class object; 3.3.4 Customizing instance and subclass checks
  - url: https://en.wikipedia.org/wiki/Smalltalk
    title: Smalltalk, Wikipedia
    locator: History; Reflection
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Class.html
    title: Class (Java SE 21 & JDK 21), API documentation
    locator: Class Class<T>, description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/hasInstance
    title: Symbol.hasInstance, JavaScript reference, MDN
    locator: Introduction
  - url: https://peps.python.org/pep-0487/
    title: PEP 487, Simpler customisation of class creation
    locator: Abstract; Background
  - url: https://docs.python.org/3/library/enum.html
    title: enum, Support for enumerations, The Python Standard Library
    locator: Data types, class enum.EnumType
  - url: https://peps.python.org/pep-3115/
    title: PEP 3115, Metaclasses in Python 3000
    locator: Abstract
---

## Idea

In a language where a class is an object, the class must itself have a class. That class of a class is a metaclass. Wikipedia puts it this way: "Unlike ordinary classes, which define the behaviors of objects, metaclasses specify the behaviors of classes and their instances."[^1] A metaclass decides how a class is made, what its class statement does, and what a class object can do at run time.

Python is the main language with metaclasses in everyday code. "By default, classes are constructed using type()",[^2] so `type` is the metaclass of most classes, and a subclass of `type` is a custom metaclass. Its `__new__` runs each time a class statement that uses it runs. The usual jobs are to register each subclass, to check the class body, and to add methods. In Python:

```python run id=metaclass-registers-classes
class PluginMeta(type):
    registry = {}

    def __new__(mcls, name, bases, namespace):
        if bases and "run" not in namespace:  # checks each subclass, not the base
            raise TypeError(f"{name} must define run()")
        cls = super().__new__(mcls, name, bases, namespace)
        if bases:
            mcls.registry[name.lower()] = cls
        return cls


class Plugin(metaclass=PluginMeta):
    pass


class Upper(Plugin):
    def run(self, text):
        return text.upper()


class Reverse(Plugin):
    def run(self, text):
        return text[::-1]


print(type(Upper).__name__, type(PluginMeta).__name__, type(type).__name__)
print(sorted(PluginMeta.registry), PluginMeta.registry["reverse"]().run("abc"))
try:

    class Broken(Plugin):
        pass

except TypeError as e:
    print(e)
```

```output for=metaclass-registers-classes
PluginMeta type type
['reverse', 'upper'] cba
Broken must define run()
```

`Upper` is an instance of `PluginMeta`, and `PluginMeta` is an instance of `type`. The type of `type` is `type`, so the chain stops there. `Plugin` declares the metaclass, and its subclasses get the same metaclass. So each class statement below `Plugin` runs `PluginMeta.__new__`, and a class without `run` fails when it is defined, not when it is first used.

## How it works

- **A class statement is a call.** Python runs the class body in a new namespace, and then "the class object is created by calling metaclass(name, bases, namespace, **kwds)".[^2] So the metaclass gets the name, the base classes and the members, and it returns the class object. It can change the members before it makes the class, or refuse to make the class.
- **The metaclass is inherited.** "The class creation process can be customized by passing the metaclass keyword argument in the class definition line, or by inheriting from an existing class that included such an argument."[^2] When the bases have different metaclasses, Python uses the most derived one, and fails if no candidate is a subclass of all the others.
- **Methods of the metaclass are methods of the class.** An attribute lookup on a class looks in the metaclass too, in the same way that a lookup on an instance looks in its class. A metaclass `__call__` controls what `Upper()` does, and `__instancecheck__` controls `isinstance`; "the metaclass abc.ABCMeta implements these methods"[^2] to give abstract base classes.
- **Smalltalk has one metaclass for each class.** In Smalltalk, "Each class is an instance of the metaclass of that class",[^3] and "Metaclasses in turn are also objects, and are all instances of a class named Metaclass." The class methods of a class are the methods of its metaclass. Ruby has the same structure with singleton classes, which it also calls eigenclasses.[^1]
- **No metaclass, only a class object.** Java, C#, Go and many other languages give each type an object at run time for reflection, but code cannot give a class a different metaclass. Java says: "Instances of the class Class represent classes and interfaces in a running Java application",[^4] and "Class has no public constructor." The class of every class is the same fixed type.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | a subclass of `type`, set with `metaclass=` in the class statement; `__init_subclass__` for simpler cases |
| JavaScript | absent | a class is a function object; `static` blocks, `Symbol.hasInstance` and functions that take a class cover some uses |
| TypeScript | absent | as JavaScript; decorators run a function on a class when it is defined |
| Java | absent | `java.lang.Class` for reflection; annotation processors generate code at compile time |
| Kotlin | absent | `KClass` for reflection; compiler plugins and annotation processors |
| C# | absent | `System.Type` for reflection; source generators generate code at compile time |
| Go | absent | `reflect.Type` for reflection; `go generate` for code |
| Rust | absent | derive macros and procedural macros generate code for a type at compile time |
| C | absent | no classes |
| C++ | absent | templates, `constexpr` and the preprocessor generate code at compile time |
| Bash | n/a | no classes |
| PowerShell | absent | `[type]` objects for reflection; classes cannot have a custom metaclass |
| SQL | n/a | no classes; the system catalog describes tables as rows |
| Regex | n/a | no classes |
| HTML | n/a | no classes |
| CSS | n/a | no classes |

- **Java and C#** keep a class object at run time, but its class is fixed. Wikipedia says: "Java features java.lang.Class for introspection, similar to metaclasses, but are not actually metaclasses."[^1] Code can read a class and make instances by reflection, but it cannot run its own code when a class is defined. Annotation processors in Java and source generators in C# do some of the same jobs at compile time.
- **JavaScript** has no hook that runs when a class is defined, but a class is a value. A function that takes a class and returns it does the work of a metaclass that registers or checks classes. Some static methods also change how the language uses a class: "The instanceof operator looks up this symbol on its right-hand operand for the method used to determine if the constructor object recognizes an object as its instance."[^5]
- **Rust, Go and C++** have no class objects to customize. Code that a metaclass makes at run time in Python, a derive macro, `go generate` or a template makes at compile time.

## Variations and related ideas

- **`__init_subclass__` and class decorators in Python.** Most metaclasses register or check subclasses, and Python 3.6 added a simpler hook for that job. PEP 487 says: "Currently, customising class creation requires the use of a custom metaclass", and it adds `__init_subclass__`, which "should be easier to understand and use than implementing a custom metaclass".[^6] A class decorator changes one class after it is made. Use one of these two before a metaclass.
- **Metaclasses in the standard library.** `abc.ABCMeta` makes abstract base classes, and "EnumType is the metaclass for enum enumerations".[^7] It makes the members, stops duplicates and lets code iterate over an enum class.
- **Metaobject protocols.** "Each programming language adheres to its own metaobject protocol, which are the rules that determine interactions among objects, classes, and metaclasses."[^1] Common Lisp, through the Common Lisp Object System, Objective-C, Ruby and Smalltalk all have metaclasses, with different amounts of control.
- **Reflection.** Reading a class at run time is [runtime reflection](/runtime-reflection/). A metaclass goes one step further, because it changes how a class is made.
- **Code generation at compile time.** [Syntactic macros](/syntactic-macros/), Rust derive macros and C# source generators make members for a type before the program runs, so the result is checked by the compiler.

## Pitfalls

- **Metaclass conflicts.** A class with two bases from two libraries fails if each library uses its own metaclass. PEP 487 gives the cause: "there is no automatic way to combine metaclasses",[^6] so code needs a third metaclass that is a subclass of both.

```python run id=metaclass-conflict
class OrmMeta(type):
    pass


class UiMeta(type):
    pass


class Model(metaclass=OrmMeta):
    pass


class Widget(metaclass=UiMeta):
    pass


try:

    class ModelWidget(Model, Widget):
        pass

except TypeError as e:
    print(e)


class OrmUiMeta(OrmMeta, UiMeta):  # a metaclass that is a subclass of both
    pass


class ModelWidget(Model, Widget, metaclass=OrmUiMeta):
    pass


print(type(ModelWidget).__name__)
```

```output for=metaclass-conflict
metaclass conflict: the metaclass of a derived class must be a (non-strict) subclass of the metaclasses of all its bases
OrmUiMeta
```

- **Hidden behaviour.** A metaclass changes every class below the base class that names it, and the class statements do not show this. A reader of `class Upper(Plugin)` cannot see that the class was checked and registered. Put the rule in the documentation of the base class.
- **The base class also goes through the metaclass.** `__new__` runs for the base class too, so a check for subclasses must skip it, as `if bases` does above.
- **Import order decides the registry.** A registry that a metaclass fills holds only the classes whose modules were imported. A plugin in a module that nothing imports is not in it.
- **Too much power for the job.** Most uses need only `__init_subclass__`, a class decorator or a [data class](/data-class/). A metaclass is harder to read and to combine, so keep it for frameworks that must change how classes are made.

## Cost

A metaclass costs time when a class is made, which is once for each class statement, usually at import time. It does not change the cost of a method call on an instance, because instances do not see the metaclass. A metaclass with its own `__call__` adds a Python-level call to each instance creation, and a metaclass `__getattr__` adds work to each failed attribute lookup on the class. Memory: one extra class object for each metaclass, and in Smalltalk one metaclass object for each class.

## History

"Smalltalk-80 added metaclasses",[^3] to keep the rule that everything is an object, by "associating properties and behavior with individual classes". Objective-C took its metaclasses from Smalltalk.[^1] Python made `type` the metaclass of classes, and Python 3.0 changed the syntax to the `metaclass=` keyword in the class statement: PEP 3115 "proposes changing the syntax for declaring metaclasses, and alters the semantics for how classes with metaclasses are constructed".[^8] Python 3.6 then added `__init_subclass__` and `__set_name__` from PEP 487, so many programs no longer need a custom metaclass.

## References

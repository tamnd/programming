---
tag: JM1Q0V
kind: pattern
slug: mixin-pattern
name: Mixin
aliases: [mix-in, mixin class, mixin pattern, trait mixin]
area: software-design/structural-patterns
level: core
summary: >-
  A mixin is a class that holds methods for other classes to use, without being their parent in the sense of "is a". A class mixes in several mixins to reuse their behaviour.
family: structural-design
polarity: positive
intent: "Put one reusable piece of behaviour in a class that is not meant to stand alone, so that other classes can add the behaviour by mixing the class in, without a relation of specialization."
also_known_as: [Mix-in]
history:
  year: 1990
  people: [Howard Cannon, Gilad Bracha, William Cook]
  note: Mixins first appeared in the Flavors object system of Lisp Machine Lisp, developed by Howard Cannon, and took their name from Steve's Ice Cream Parlor in Somerville, Massachusetts. In an OOPSLA 90 paper, Bracha and Cook described the inheritance of Smalltalk, Beta and CLOS as special forms of mixin inheritance.
see_also: [class-inheritance, composition-over-inheritance, interfaces, delegation-pattern, decorator-pattern, template-method-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Mixin
    title: Mixin, Wikipedia
    locator: Introduction; History; Definition; Advantages; Implementations
  - url: https://docs.ruby-lang.org/en/3.4/Comparable.html
    title: module Comparable, Documentation for Ruby 3.4
    locator: module description
  - url: https://docs.python.org/3.14/library/socketserver.html
    title: The Python Standard Library, socketserver, Python 3.14
    locator: Server Creation Notes; ForkingMixIn and ThreadingMixIn
  - url: https://docs.python.org/3.14/howto/mro.html
    title: The Python 2.3 Method Resolution Order, Python 3.14 documentation
    locator: The beginning; linearization
  - url: https://docs.scala-lang.org/tour/mixin-class-composition.html
    title: Class Composition with Mixins, Tour of Scala
    locator: whole page
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/socketserver.py#L678-L723
    title: CPython v3.14.0 source, Lib/socketserver.py
    locator: ThreadingMixIn, ThreadingUDPServer and ThreadingTCPServer, lines 678 to 723
---

## Intent

Put one reusable piece of behaviour in a class that is not meant to stand alone. Other classes add the behaviour by mixing the class in. A mixin contains methods for use by other classes without having to be their parent class.[^1]

This page is about the design use of mixins. The language features that support them, such as multiple inheritance, modules and traits, differ from one language to the next.

## Problem

Several unrelated classes need the same small behaviour. A `Version`, a `Money` amount and a `Date` all need `<`, `>` and `between`, and each can compare itself with one method. If each class writes the operators, the code repeats. If all three inherit from a common `Comparable` base class, the base class takes the one superclass that a single-inheritance language allows, and the hierarchy says that a version is a kind of comparable thing, which is not a useful relation.

The forces are these:

- The behaviour is small and can be built on one or two methods of the class.
- Unrelated classes need it, and one class may need several such behaviours.
- The relation between the class and the behaviour is "has this feature", not "is a kind of".

## Solution

Write the behaviour as a mixin. The mixin has methods but usually no state, and it requires the class to supply one or more methods. A class that needs the behaviour mixes the mixin in. Ruby's `Comparable` is a standard example: the class must define the `<=>` operator, and `Comparable` uses it to implement `<`, `<=`, `==`, `>=`, `>` and `between?`.[^2]

```text
 mixin Comparable                       class Version
   requires: compare(other)  <------------ defines compare(other)
   provides: less_than(other)              mixes in Comparable
             between(lo, hi)
                     |
                     v
 v1.less_than(v2)  -> Comparable.less_than -> self.compare(v2)
```

Wikipedia gives the difference from inheritance: the class reuses the functionality of the mixin, but not as a means of specialization, so the relation is not a rigid "is a".[^1] A mixin can also be seen as an interface with implemented methods.[^1]

The mechanism depends on the language:[^1]

| Language | Mechanism |
|---|---|
| Python | multiple inheritance from a class with a `Mixin` name, ordered by the method resolution order[^3][^4] |
| Ruby | `include` of a module, such as `Comparable`[^2] |
| Scala | traits; a class has one superclass but many mixins, with `extends` and `with`[^5] |
| JavaScript and TypeScript | a function that copies methods to a prototype, or a function that returns a subclass |
| Java and C# | default methods in an interface |
| C++ | multiple inheritance, often with the curiously recurring template pattern (CRTP) |
| Rust | a trait with default methods |
| Go | no mixins; generic functions over an interface |

## Example

The example files show one program in each language. A `Comparable` mixin requires `compare` and provides `less_than` and `between`. `Version` defines only `compare`. The program prints:

```text
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

The comparison is numeric, so version 1.10 comes after version 1.2.

## Consequences

The benefits:[^1]

- A class gets common functionality from several classes, without the complex semantics of full multiple inheritance.
- Shared code lives in one mixin, not in a copy in each class.
- A class takes only the features it wants, not every feature of a parent.

The costs:

- **Hidden requirements.** A mixin depends on methods that the class must supply. If the class does not supply them, the error comes at the first call, or, in a language with static types, at compile time.
- **Name clashes.** Two mixins can define a method with the same name. The language then picks one by a rule, and the other is hidden. In Python, the rule is the linearization of the class, which the documentation also calls the MRO.[^4]

Here two mixins define `dump`, and the class uses the first one in its list of bases. The log is never written:

```python wrong id=name-clash
class JsonMixin:
    def dump(self):
        return f'json: {{"title": "{self.title}"}}'


class LogMixin:
    def dump(self):
        return f"log: wrote {self.title}"


class Report(JsonMixin, LogMixin):
    def __init__(self, title):
        self.title = title


print(Report("q3").dump())
print([c.__name__ for c in Report.__mro__])
```

```output for=name-clash
json: {"title": "q3"}
['Report', 'JsonMixin', 'LogMixin', 'object']
```

The fix is to give each mixin method a distinct name, or to make the mixins cooperate. In a cooperative mixin, each method does its work and then calls `super()`, which goes to the next class in the MRO, not to the parent of the mixin:

```python run id=cooperative
class Base:
    def setup(self):
        return ["base"]


class LockMixin(Base):
    def setup(self):
        return ["lock"] + super().setup()


class CacheMixin(Base):
    def setup(self):
        return ["cache"] + super().setup()


class Service(LockMixin, CacheMixin):
    pass


print(Service().setup())
print([c.__name__ for c in Service.__mro__])
```

```output for=cooperative
['lock', 'cache', 'base']
['Service', 'LockMixin', 'CacheMixin', 'Base', 'object']
```

Both mixins inherit from `Base`, which is the diamond shape. The MRO puts `Base` once, after both mixins, so each `setup` runs once.

## When not to use it

Do not use a mixin when the behaviour needs much state of its own. A separate object that the class holds and calls, as in [[delegation-pattern]], keeps the state apart and can change at run time.

Do not stack many mixins on one class. Each mixin adds methods and requirements, and the reader must know the order of all of them to know which method runs. [[composition-over-inheritance]] gives the general advice.

Do not use a mixin to share code between classes that are really kinds of one thing. A normal base class says that more clearly.

## Variants and related patterns

**Traits.** A trait is a mixin that the language checks. Scala classes compose traits as mixins, and a class can have only one superclass but many mixins.[^5]

**Interface with default methods.** Java and C# let an interface supply method bodies. The interface then works as a mixin with no state.

**Template method.** A mixin that calls a method the class must supply is close to the [[template-method-pattern]], but the mixin is not the parent of the class in the design.

**Decorator.** A [[decorator-pattern]] adds behaviour to one object at run time. A mixin adds behaviour to a class when the class is defined.

## Known uses

- Python: [Lib/socketserver.py at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/socketserver.py#L678-L723) defines `ThreadingMixIn`, a mix-in class that handles each request in a new thread. It overrides `process_request`, and the module defines `ThreadingUDPServer` as `class ThreadingUDPServer(ThreadingMixIn, UDPServer)`.[^6][^3]
- Ruby: `Comparable` gives the comparison operators and `between?` to each class that defines `<=>`.[^2]

## References

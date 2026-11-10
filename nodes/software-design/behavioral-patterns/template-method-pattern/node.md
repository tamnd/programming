---
tag: NEW-template-method-pattern
kind: pattern
slug: template-method-pattern
name: Template Method pattern
aliases: [Template Method, hook method, non-virtual interface]
area: software-design/behavioral-patterns
level: core
summary: >-
  A base class method fixes the steps of an algorithm and calls other methods for the steps that vary. Subclasses override those steps and leave the order of the steps alone.
family: behavioral-design
polarity: positive
intent: "Define the skeleton of an algorithm in one operation, and let subclasses redefine some of its steps without changing the structure of the algorithm."
also_known_as: []
refactorings: [form-template-method]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994). In his article Virtuality (2001), Herb Sutter gave C++ guidelines based on it, and he later named the related idiom the Non-Virtual Interface idiom.
see_also: [strategy-pattern, factory-method-pattern, iterators, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Template Method (Intent, Motivation, Participants, Collaborations, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (first-class functions make Command, Strategy, Template-Method and Visitor simpler)
  - url: https://martinfowler.com/bliki/CallSuper.html
    title: Martin Fowler, CallSuper, bliki, August 11, 2005
    locator: the whole entry (call super as a minor smell; the template method with a hook as the fix)
  - url: http://www.gotw.ca/publications/mill18.htm
    title: Herb Sutter, Virtuality, C/C++ Users Journal 19(9), September 2001
    locator: Guidelines 1 to 4 (non-virtual interfaces; private virtual functions; protected only when derived classes call the base; the destructor)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.3.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.4.3.3 final Methods
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/classes#1564-virtual-methods
    title: C# language specification, Classes
    locator: 15.6.4 Virtual methods
  - url: https://kotlinlang.org/docs/inheritance.html#overriding-methods
    title: Kotlin documentation, Inheritance
    locator: Overriding methods
  - url: https://docs.python.org/3.14/library/typing.html#typing.final
    title: The Python Standard Library, typing, Python 3.14
    locator: typing.final
  - url: https://docs.python.org/3.14/library/abc.html#abc.abstractmethod
    title: The Python Standard Library, abc, Python 3.14
    locator: abc.abstractmethod
  - url: https://doc.rust-lang.org/book/ch10-02-traits.html#default-implementations
    title: The Rust Programming Language, chapter 10.2
    locator: Default Implementations
  - url: https://doc.rust-lang.org/std/iter/index.html#implementing-iterator
    title: Rust standard library, std::iter
    locator: Implementing Iterator
  - url: https://pkg.go.dev/sort#Sort
    title: Go standard library, sort
    locator: func Sort; type Interface
  - url: https://docs.python.org/3.14/library/socketserver.html#socketserver.BaseRequestHandler.setup
    title: The Python Standard Library, socketserver, Python 3.14
    locator: BaseRequestHandler.setup, handle and finish
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/socketserver.py#L760-L777
    title: CPython v3.14.0 source, Lib/socketserver.py
    locator: BaseRequestHandler.__init__, setup, handle and finish, lines 760 to 777
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/io/InputStream.java#L283-L308
    title: OpenJDK jdk-25+36 source, java/io/InputStream.java
    locator: abstract read() at line 182; read(byte[], int, int), lines 283 to 308
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/AbstractList.java#L30-L45
    title: OpenJDK jdk-25+36 source, java/util/AbstractList.java
    locator: class documentation, lines 30 to 45
  - url: https://docs.python.org/3.14/library/collections.abc.html#collections-abstract-base-classes
    title: The Python Standard Library, collections.abc, Python 3.14
    locator: Collections Abstract Base Classes (the table of abstract methods and mixin methods)
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 8, Generalization, Form Template Method
  - url: https://peps.python.org/pep-0591/
    title: PEP 591, Adding a final qualifier to typing
    locator: The final decorator
---

## Intent

Define the skeleton of an algorithm in an operation, and defer some steps to subclasses. Subclasses then redefine those steps without changing the structure of the algorithm.[^1] The base class owns the order. The subclasses own the details.

## Problem

A program loads records from text in two formats. One is CSV, with lines such as `pen,3`. The other is key and value, with lines such as `pen=3`. Both loaders must strip each line, skip blank lines and comments, parse the rest, and filter the result. Only the parse step, and in one case the filter, differ.

The first version has two loaders, each with its own loop. The loops are copies, and copies drift apart. A fix in one is missed in the other. Here, the key and value loader does not skip comments, and a comment becomes a record:

```python wrong id=copied-loop
def load_csv(lines):
    records = []
    for line in lines:
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        name, count = line.split(",")
        records.append((name, int(count)))
    return records


def load_key_value(lines):
    records = []
    for line in lines:
        line = line.strip()
        if not line:  # the copy lacks the comment check
            continue
        name, _, count = line.partition("=")
        if int(count) > 0:
            records.append((name, int(count)))
    return records


print(load_csv(["# stock", "pen,3", "book,1"]))
print(load_key_value(["pen=3", "#cup=5"]))
```

```output for=copied-loop
[('pen', 3), ('book', 1)]
[('pen', 3), ('#cup', 5)]
```

Design Patterns uses an application framework. The base class opens a document in fixed steps: check that it can be opened, make the document object, add it to the application, and read it. Each kind of application changes some of those steps.[^1] The forces are these. Several classes share an algorithm and differ in a few steps. The shared part must exist once. And the order of the steps must not change from one class to the next.

## Solution

Design Patterns names two participants.[^1]

- **AbstractClass** defines the template method, which calls the steps in order. It declares the steps that vary as primitive operations.
- **ConcreteClass** implements the primitive operations for one case.

The base class calls the subclass, not the other way round. Design Patterns calls this inverted control structure the Hollywood principle.[^1] The subclass gives the steps and does not call the template.

The steps come in two kinds. A **primitive operation**, often abstract, has no default, and each subclass must give it. In the loader, `parse` is one. A **hook** has a default, often one that does nothing, and a subclass may override it. In the loader, `accept` is a hook whose default keeps every record. Design Patterns asks the author of the base class to say which steps are hooks and which must be overridden, since a subclass author cannot tell otherwise.[^1]

The template method itself should not be overridden. Languages state this in different ways:

| Language | How the template is kept fixed | How the steps are marked |
|---|---|---|
| Java | `final` on the template method[^5] | `protected abstract` and `protected` methods |
| C# | methods are not virtual unless marked, so the template is fixed by default[^6] | `abstract` and `virtual` |
| Kotlin | members are final unless marked `open`[^7] | `abstract` and `open` |
| C++ | a public non-virtual function[^4] | private virtual functions |
| Python | `typing.final`, checked by type checkers only[^8][^19] | `abc.abstractmethod` for the primitive operations[^9] |
| Rust | no way; a provided trait method can be replaced[^10] | required methods and provided methods |
| TypeScript, JavaScript | no way | `abstract` in TypeScript; a method that throws in JavaScript |
| Go | the template is a function, which cannot be overridden | an interface, and an optional interface for a hook |

In C++, Sutter's guidelines turn the pattern into a rule of class design. Prefer to make interfaces non-virtual, using Template Method. Prefer to make virtual functions private. Make a virtual function protected only if derived classes need to call the base version.[^4] In a note added to the article, Sutter names this the Non-Virtual Interface idiom, or NVI. He describes it as a more restricted idiom with a form similar to Template Method.[^4] The C++ example file follows it.

## Example

The example files load the CSV lines `# stock`, `pen,3`, an empty line and `book,1`, and the key and value lines `pen=3`, `# sold out` and `cup=0`. The key and value loader overrides the hook to drop records with a count of zero. Each file prints `csv: pen:3 book:1` and `kv: pen:3`:

| Language | Form |
|---|---|
| Python | an abstract base class, with `@final` on the template[^8] |
| JavaScript | a base class whose `parse` throws until a subclass overrides it |
| TypeScript | an `abstract` class with `protected` steps |
| Java | a `final` template, a `protected abstract` step and a `protected` hook[^5] |
| C# | a non-virtual template, an `abstract` step and a `virtual` hook[^6] |
| C++ | the non-virtual interface idiom with private virtual steps[^4] |
| Go | a function that takes an interface, and an optional interface for the hook[^12] |
| Rust | a trait whose provided method calls its required method[^10] |

## Consequences

Design Patterns calls template methods a fundamental technique for code reuse, and notes that they are important in class libraries, because they factor out the common behaviour.[^1] The loop of the loader exists once. A fix to the comment rule reaches every format. A new format is one class with one method.

The costs come from inheritance. The steps are bound when the subclass is written, so a loader cannot change its parse step while the program runs. Each combination of steps needs a class. A loader with two parse formats and two filters needs four subclasses, or a second mechanism. The subclass depends on the base class's protected interface, and a change to the order of the steps can break subclasses that relied on it.

**Call super.** A base class that asks subclasses to override a method and call the base version from it has a weak form of the pattern. Fowler calls this the call super smell. When a programmer must remember to make a call each time, the API is at fault. The fix is a template method that makes the call itself and then calls a hook.[^3] This subclass forgets the call, and the comment check is lost:

```python wrong id=call-super
class Loader:
    def clean(self, line):
        """Subclasses that override this must call super().clean(line)."""
        line = line.strip()
        return None if not line or line.startswith("#") else line

    def load(self, lines):
        records = []
        for line in lines:
            line = self.clean(line)
            if line is not None:
                records.append(self.parse(line))
        return records


class UpperCsvLoader(Loader):
    def clean(self, line):
        return line.strip().upper()  # forgot super().clean(line)

    def parse(self, line):
        name, count = line.split(",")
        return name, int(count)


print(UpperCsvLoader().load(["pen,3", "# note,0", "book,1"]))
```

```output for=call-super
[('PEN', 3), ('# NOTE', 0), ('BOOK', 1)]
```

The fix keeps the check in `load` and calls a hook, such as `transform(line)`, that returns the line unchanged by default. The subclass then overrides the hook and cannot skip the check. The example files have this shape: the base class strips and filters, and `parse` sees only the lines that passed.

**A template that a subclass replaces.** If the template method can be overridden, a subclass can change the order of the steps and break the contract. In Java, `final` stops this at compile time:[^5]

```java fails expect=compile id=final-template
import java.util.List;

public class Main {
    abstract static class Loader {
        public final List<String> load(List<String> lines) {
            return lines.stream().map(this::parse).toList();
        }

        protected abstract String parse(String line);
    }

    static final class CsvLoader extends Loader {
        @Override
        public List<String> load(List<String> lines) { // error: load is final in Loader
            return lines;
        }

        @Override
        protected String parse(String line) {
            return line;
        }
    }

    public static void main(String[] args) {
        System.out.println(new CsvLoader().load(List.of("pen,3")));
    }
}
```

**Do not call the steps from a constructor.** A template method that runs in the base class constructor calls steps of a subclass that is not fully built. C++ and Java handle this in different ways, and both surprise. The [[factory-method-pattern]] page shows both cases, since a factory method is often a step of a template method.

## When not to use it

Do not use inheritance when the steps vary on their own. If parse formats and filters combine freely, pass them as two values. Two subclasses for two cases become four for four combinations, and the count grows with each axis.

Do not use it when the step must change at run time, or when the caller chooses it. Pass a function or a strategy object.

Do not use it in a language where a function value does the job with less code, and the template has only one or two steps. The comparison with Strategy below shows the function form.

Do not make a deep hierarchy of templates that call templates. A reader must then follow each step up and down the class tree to see what runs.

## Variants and related patterns

### Template Method and Strategy

Both patterns let part of an algorithm vary. Design Patterns states the difference: template methods use inheritance to vary part of an algorithm, and strategies use delegation to vary the entire algorithm.[^1] Norvig lists both among the patterns that first-class functions make less complex.[^2] With functions, the steps of a template become parameters, and the template becomes a function:

```python run id=steps-as-functions
def load(lines, parse, accept=lambda record: True):
    records = []
    for line in lines:
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        record = parse(line)
        if accept(record):
            records.append(record)
    return records


def csv(line):
    name, count = line.split(",")
    return name, int(count)


def key_value(line):
    name, _, count = line.partition("=")
    return name, int(count)


print(load(["# stock", "pen,3", "book,1"], csv))
print(load(["pen=3", "cup=0"], key_value, accept=lambda record: record[1] > 0))
print(load(["pen,3", "cup,0"], csv, accept=lambda record: record[1] > 0))  # a combination with no class
```

```output for=steps-as-functions
[('pen', 3), ('book', 1)]
[('pen', 3)]
[('pen', 3)]
```

The last call combines the CSV parser with the filter of the other format. The class form would need a third subclass for it. The table below compares the two forms:

| Aspect | Template Method | Steps as functions or strategies |
|---|---|---|
| binding time | when the subclass is written | when the caller passes the values; can change at run time |
| what varies | some steps; the structure is fixed | any step; a strategy can replace the whole algorithm |
| combinations of steps | one subclass for each | free, with no extra types |
| access to the base class's state | yes, through protected members | only what the template passes as arguments |
| defaults for steps | hook methods | default parameter values |
| a missing step | a compile error, or an error when the object is made | an error at the call |

Choose the class form when the steps share state with the template, or when a class must give several related steps together. Choose functions when the steps are independent and few. The [[strategy-pattern]] page covers the delegation side.

**Iterator-style traits.** In Rust, a trait with one required method and many provided methods is a set of template methods. `Iterator` is the main case. An implementation gives `next`, and the provided methods build on it.[^10][^11] Python's `collections.abc` classes do the same with mixin methods. A class that inherits from `Sequence` and gives `__getitem__` and `__len__` gets `__contains__`, `__iter__`, `index` and `count`.[^17] See [[iterators]].

**Go's function templates.** `sort.Sort` takes a value of the interface `sort.Interface`, with `Len`, `Less` and `Swap`. The sort algorithm is fixed in the function, and the value gives the steps.[^12]

**Factory Method.** A factory method is often a step of a template method. The template calls it to get an object that it then uses.[^1] See [[factory-method-pattern]].

**Refactoring.** Kerievsky's refactoring Form Template Method starts from two methods in sibling subclasses that do similar steps in the same order. It moves the shared order into a template in the base class, and leaves the differing steps in the subclasses.[^18]

## Known uses

- CPython: [BaseRequestHandler.__init__ in socketserver.py at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/socketserver.py#L760-L777) calls `setup()`, then `handle()` in a `try` block, then `finish()` in a `finally` block. All three are hooks with empty defaults, and a handler class overrides `handle`.[^13][^14]
- OpenJDK: `java.io.InputStream` declares `read()` as abstract. [read(byte[], int, int) at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/io/InputStream.java#L283-L308) calls `read()` in a loop, so a subclass that gives the one-byte read gets the bulk read. The documentation encourages subclasses to give a more efficient bulk read.[^15]
- OpenJDK: [AbstractList at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/AbstractList.java#L30-L45) documents that an unmodifiable list needs only `get(int)` and `size()`. Iteration, search and the other methods are built on those two.[^16]
- Rust: `std::iter::Iterator` has one required method, `next`, and the rest are provided.[^11]

## References

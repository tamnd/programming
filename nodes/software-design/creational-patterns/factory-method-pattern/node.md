---
tag: 0PK16R
kind: pattern
slug: factory-method-pattern
name: Factory Method pattern
aliases: [Factory Method, virtual constructor]
area: software-design/creational-patterns
level: core
summary: >-
  A base class does its work through an abstract product and gets the product from a method that subclasses override. Each subclass decides which concrete class to create.
family: creational-design
polarity: positive
intent: "Define an operation that creates an object, and let subclasses decide which class to create, so that the code that uses the object does not name a concrete class."
also_known_as: [Virtual Constructor]
refactorings: [introduce-polymorphic-creation-with-factory-method]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Virtual Constructor as a second name. In a 2009 interview, Gamma proposed to generalize it to Factory in a revised catalog.
see_also: [strategy-pattern, singleton-pattern, extract-function, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Factory Method (Intent, Also Known As, Motivation, Participants, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (first-class types make Abstract-Factory, Flyweight, Factory-Method, State, Proxy and Chain-Of-Responsibility simpler)
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 6, Creation, Introduce Polymorphic Creation with Factory Method (problem and solution)
  - url: https://www.informit.com/articles/article.aspx?p=1404056
    title: Larry O'Brien, Design Patterns 15 Years Later, An Interview with Erich Gamma, Richard Helm, and Ralph Johnson, InformIT, October 22, 2009
    locator: Gamma's draft of a revised catalog (generalize Factory Method to Factory; add Dependency Injection)
  - url: https://timsong-cpp.github.io/cppwp/n4950/class.cdtor#4
    title: Working Draft, Programming Languages, C++ (N4950)
    locator: "[class.cdtor] paragraph 4 (virtual calls during construction)"
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 12.5 Creation of New Class Instances (the note after the procedure, on overriding methods during creation)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.8.7
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.8.7 Constructor Body (prologue and epilogue)
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Item 1, Consider static factory methods instead of constructors; Item 19, Design and document for inheritance or else prohibit it
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Supplier.html
    title: Java SE 25 API, java.util.function.Supplier
    locator: interface description
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.13
    title: The Java Language Specification, Java SE 25 Edition
    locator: "15.13 Method Reference Expressions (ClassType :: new)"
  - url: https://docs.python.org/3.14/library/collections.html#collections.defaultdict.default_factory
    title: The Python Standard Library, collections, Python 3.14
    locator: defaultdict.default_factory
  - url: https://go.dev/doc/effective_go#generality
    title: Effective Go
    locator: Interfaces and other types, Generality
  - url: https://rust-lang.github.io/api-guidelines/predictability.html#c-ctor
    title: Rust API Guidelines, Predictability
    locator: C-CTOR, Constructors are static, inherent methods
  - url: https://doc.rust-lang.org/book/ch10-02-traits.html#default-implementations
    title: The Rust Programming Language, chapter 10.2
    locator: Using Default Implementations
  - url: https://kotlinlang.org/docs/coding-conventions.html#factory-functions
    title: Kotlin coding conventions
    locator: Factory functions
  - url: https://docs.python.org/3.14/library/abc.html#abc.abstractmethod
    title: The Python Standard Library, abc, Python 3.14
    locator: abc.abstractmethod
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/AbstractCollection.java#L104-L116
    title: OpenJDK jdk-25+36 source, java/util/AbstractCollection.java
    locator: contains, lines 104 to 116
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/ArrayList.java#L1029-L1031
    title: OpenJDK jdk-25+36 source, java/util/ArrayList.java
    locator: iterator, lines 1029 to 1031
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Abstract Factory (Implementation, factories as singletons)
---

## Intent

Define an operation that creates an object, and let subclasses decide which class to create. The code that uses the object works with an interface and does not name a concrete class.[^1]

## Problem

An exporter writes a table of rows. It writes a header, then one line for each row. The steps are the same for every format, but the class that formats the lines is different. One class writes CSV, and another writes text for a person to read. A user of a library will want a third format that the authors did not plan.

The direct design creates the writer inside `export` with a conditional on a format name, or with a call to one concrete constructor. Either way, the exporter names every concrete writer. A third format needs a change to the exporter. The library cannot ship the export loop and let its users supply the format.

The forces are these. The algorithm that uses the object is fixed and belongs in one place. The concrete class of the object is not known when that algorithm is written. The class must be chosen by code that comes later, often by a subclass in another module. Kerievsky describes a related starting point: classes in a hierarchy implement a method in the same way, except for the step that creates an object.[^3]

## Solution

Put the creation step in its own method, the **factory method**, and call it from the algorithm. The base class, the **creator**, declares the factory method with the type of the abstract **product**. Each **concrete creator** overrides the factory method and returns a **concrete product**.[^1]

```text
 Creator (Exporter)                      Product (Writer)
 ------------------------------          ----------------
 export(rows):                           header()
   w = create_writer()   ---creates-->   row(name, count)
   w.header(); w.row(...) ...                  ^
 create_writer(): Writer  (abstract)           |
        ^                              +-------+-------+
        |                              |               |
  +-----+--------+                 CsvWriter       TextWriter
  |              |                     ^               ^
 CsvExporter  TextExporter             |               |
 create_writer() --------------------->+               |
                create_writer() ---------------------->+
```

The participants in Design Patterns are Product, ConcreteProduct, Creator and ConcreteCreator.[^1] The creator depends only on the product interface. Only the concrete creator names the concrete product.

The method that calls the factory method is often a template method. It fixes the order of the steps and leaves one step, the creation, to subclasses. Design Patterns notes this link in its Related Patterns section for Factory Method.[^1]

Design Patterns describes two main forms. In the first, the creator is abstract and has no default product. In the second, the creator is concrete and returns a default product, and subclasses override it only to change it. A **parameterized factory method** takes an argument, such as a format name, and returns one of several products. Subclasses can then extend or override the choice for some values of the argument.[^1]

**In languages where classes and functions are values, the subclass is often not needed.** If a class is a value, the creator can store the class and call it. If a function is a value, the creator can store a function that returns a product. Norvig counts Factory Method among the six patterns that first-class types make simpler.[^2] The standard libraries use this form:

| Language | Factory as a value |
|---|---|
| Python | a class is callable, and `collections.defaultdict` takes a `default_factory` such as `list`[^11] |
| Java | `java.util.function.Supplier<T>`, with a constructor reference such as `CsvWriter::new`[^9][^10] |
| Go | a constructor function that returns an interface, such as `crc32.NewIEEE` returning `hash.Hash32`[^12] |
| Rust | a trait with a default method that calls a required factory method[^14] |
| Kotlin | a top-level factory function, which the coding conventions describe[^15] |

The factory-as-a-value form is the [[strategy-pattern]] applied to creation. The subclass form fixes the choice when the subclass is written. The value form lets the choice change when the creator is built.

## Example

The example files show one program in each language. An exporter writes the rows `("pen", 3)` and `("book", 1)`. The CSV exporter makes a CSV writer, and the text exporter makes a text writer. The method `export` is the same for both and calls the factory method once:

| Language | Form |
|---|---|
| Python | an abstract base class with `abc.abstractmethod`[^16] |
| JavaScript | a base class whose `createWriter` throws until a subclass overrides it |
| TypeScript | an `abstract` class and an `abstract` method |
| Java | an abstract class, and concrete products as records |
| Kotlin | an `abstract` class with an `abstract fun` |
| C# | an `abstract` class with an `abstract` method |
| C++ | a pure virtual function that returns `std::unique_ptr<Writer>` |
| Go | an interface with a `NewWriter` method, and `Export` as a function that takes it |
| Rust | a trait with an associated type, a required `create_writer` and a default `export` |

## Consequences

Design Patterns lists these benefits.[^1] The code that uses the product does not name a concrete class, so it works with any class that implements the product interface. The factory method is a hook: a subclass can give an extended version of an object with no change to the creator. A factory method can connect parallel class hierarchies. In the example, each exporter knows which writer goes with it, and nothing else in the program needs that knowledge.

The main cost is the subclass. A client may have to subclass the creator only to create one particular product.[^1] Each format in the example needs two classes: a writer and an exporter. When the exporter adds nothing but the choice of the writer, the second class is noise. Pass the writer's class or a factory function to the exporter instead, as in the variant below.

**Do not call the factory method from the constructor of the creator.** The creator is not fully built when its constructor runs, and languages handle a virtual call at that time in two different ways. In C++, a virtual call from a constructor calls the version of the class whose constructor runs, not the override of the more derived class.[^5] The text exporter below gets a CSV writer:

```cpp wrong id=ctor-call-cpp
#include <iostream>
#include <memory>
#include <string>

struct Writer {
    virtual ~Writer() = default;
    virtual std::string row(const std::string& name, int count) const = 0;
};

struct CsvWriter : Writer {
    std::string row(const std::string& name, int count) const override {
        return name + "," + std::to_string(count);
    }
};

struct TextWriter : Writer {
    std::string row(const std::string& name, int count) const override {
        return name + ": " + std::to_string(count);
    }
};

class Exporter {
public:
    Exporter() : writer_(create_writer()) {}  // a virtual call in the constructor
    virtual ~Exporter() = default;
    std::string export_row(const std::string& name, int count) const {
        return writer_->row(name, count);
    }

protected:
    virtual std::unique_ptr<Writer> create_writer() const {
        return std::make_unique<CsvWriter>();
    }

private:
    std::unique_ptr<Writer> writer_;
};

class TextExporter : public Exporter {
protected:
    std::unique_ptr<Writer> create_writer() const override {
        return std::make_unique<TextWriter>();
    }
};

int main() {
    TextExporter exporter;
    std::cout << exporter.export_row("pen", 3) << "\n";
}
```

```output for=ctor-call-cpp
pen,3
```

In Java, the call goes to the override, but the override runs before the fields of the subclass have their values. The Java Language Specification states this: overriding methods are used even before the object is completely initialized.[^6] Here the separator is still `null`:

```java wrong id=ctor-call-java
public class Main {
    interface Writer {
        String row(String name, int count);
    }

    abstract static class Exporter {
        private final Writer writer;

        Exporter() {
            writer = createWriter(); // an overridable call in the constructor
        }

        abstract Writer createWriter();

        String exportRow(String name, int count) {
            return writer.row(name, count);
        }
    }

    static final class TextExporter extends Exporter {
        private final String separator;

        TextExporter(String separator) {
            this.separator = separator;
        }

        @Override
        Writer createWriter() {
            String sep = separator;
            return (name, count) -> name + sep + count;
        }
    }

    public static void main(String[] args) {
        System.out.println(new TextExporter(": ").exportRow("pen", 3));
    }
}
```

```output for=ctor-call-java
pennull3
```

Bloch gives the general rule for classes designed for inheritance: constructors must not invoke overridable methods.[^8] The usual fix is to call the factory method later, in the operation that needs the product, as the example files do in `export`. Design Patterns describes a lazy accessor in C++ for the same reason: it creates the product on first use.[^1] Java has one more fix for this case. A constructor can assign its fields in the prologue, which is the statements before the `super()` call. Then the fields have their values when the superclass constructor runs.[^6][^7]

```java run id=ctor-prologue-java
public class Main {
    interface Writer {
        String row(String name, int count);
    }

    abstract static class Exporter {
        private final Writer writer;

        Exporter() {
            writer = createWriter();
        }

        abstract Writer createWriter();

        String exportRow(String name, int count) {
            return writer.row(name, count);
        }
    }

    static final class TextExporter extends Exporter {
        private final String separator;

        TextExporter(String separator) {
            this.separator = separator; // the prologue runs before super()
            super();
        }

        @Override
        Writer createWriter() {
            String sep = separator;
            return (name, count) -> name + sep + count;
        }
    }

    public static void main(String[] args) {
        System.out.println(new TextExporter(": ").exportRow("pen", 3));
    }
}
```

```output for=ctor-prologue-java
pen: 3
```

**A static factory method is a different thing.** Bloch's Item 1 recommends static methods such as `valueOf` or `of` in place of public constructors. He notes that a static factory method is not the Factory Method pattern of Design Patterns.[^8] A static method cannot be overridden, so it gives a named constructor and a choice of the returned class, but not the subclass hook. In Rust, the convention is that a constructor is a static, inherent method, often named `new`, so the same distinction applies.[^13]

## When not to use it

Do not use the pattern when there is one product class and no plan for another. A direct constructor call is clearer, and the reader sees which class runs.

Do not add a creator subclass whose only job is to choose a product. Pass the product's class, a constructor reference, or a factory function to the creator. This keeps one class for each format instead of two.

Do not use a factory method to hide a choice that the caller should make. When the caller knows the concrete class and needs its specific methods, the abstract return type only forces a cast later.

A program where every `new` goes through a factory method has many classes and little behaviour. Kerievsky's catalog starts from duplicated creation code, and the refactoring is worth its cost when that duplication is real.[^3]

## Variants and related patterns

**Factory as a parameter.** The creator takes a callable that makes the product. In Python a class is such a callable, so the exporter needs no subclasses:

```python run id=factory-parameter
class CsvWriter:
    def row(self, name, count):
        return f"{name},{count}"


class TextWriter:
    def __init__(self, separator=": "):
        self.separator = separator

    def row(self, name, count):
        return f"{name}{self.separator}{count}"


class Exporter:
    def __init__(self, make_writer):
        self.make_writer = make_writer

    def export(self, rows):
        writer = self.make_writer()
        return "\n".join(writer.row(name, count) for name, count in rows)


rows = [("pen", 3), ("book", 1)]
print(Exporter(CsvWriter).export(rows))
print(Exporter(lambda: TextWriter(" = ")).export(rows))
```

```output for=factory-parameter
pen,3
book,1
pen = 3
book = 1
```

**Abstract Factory.** An abstract factory is an object with several factory methods, one for each product of a family. Design Patterns notes that abstract factories are often implemented with factory methods.[^1]

**Template Method.** The operation that calls the factory method is usually a template method, and the factory method is one of its hook steps.[^1]

**Prototype.** Design Patterns contrasts the two. Prototype does not need a subclass of the creator, because it copies an instance that the client gives. But it often needs an initialize operation on the product.[^1]

**Dependency injection.** A container that builds the objects of a program and passes them in is a general factory, configured in one place. In his 2009 draft of a revised catalog, Gamma proposed to generalize Factory Method to Factory and to add Dependency Injection as a pattern.[^4]

**Singleton.** A concrete factory often has one instance in a program. Design Patterns makes this point in its section on Abstract Factory.[^19] The [[singleton-pattern]] page explains why passing the factory in is often better than a global instance.

## Known uses

- OpenJDK: `java.util.AbstractCollection` declares `iterator()` as abstract. [contains in AbstractCollection.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/AbstractCollection.java#L104-L116) calls `iterator()` and walks the result, so `contains`, `toString` and other methods work for every collection. [ArrayList.iterator at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/ArrayList.java#L1029-L1031) overrides it and returns its own iterator class. The collections and their iterators are parallel hierarchies.[^17][^18]

## References

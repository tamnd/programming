---
tag: F0YZKX
kind: concept
slug: nested-and-inner-classes
name: Nested and inner classes
aliases: [nested class, inner class, static nested class, member class, local class, anonymous class, nested type, enclosing instance, Outer.this]
area: objects-and-classes/classes
level: core
summary: >-
  A nested class is declared inside another class. A static nested class is only grouped by name, and an inner class also holds a reference to an object of the outer class.
applies_to: core-languages
viz: optional
history:
  year: 1997
  note: BETA had nested classes first. Java 1.1 added inner, local and anonymous classes; C++ and C# nested classes have no outer instance.
see_also: [classes-and-instances, define-a-class, static-members, closures, encapsulation-and-visibility, method-concept]
same_as:
  wikipedia: Inner_class
sources:
  - url: https://docs.oracle.com/javase/tutorial/java/javaOO/nested.html
    title: Nested Classes, The Java Tutorials
    locator: Nested Classes; Inner Classes; Serialization
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
    title: Chapter 8, Classes, The Java Language Specification, Java SE 25 Edition
    locator: 8.1.1.4 static Classes; 8.1.3 Inner Classes and Enclosing Instances
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/nested-types
    title: Nested types, C# programming guide
    locator: Introduction
  - url: https://kotlinlang.org/docs/nested-classes.html
    title: Nested and inner classes, Kotlin documentation
    locator: Inner classes
  - url: https://en.cppreference.com/cpp/language/nested_types
    title: Nested classes, cppreference.com
    locator: Explanation
  - url: https://en.wikipedia.org/wiki/Inner_class
    title: Inner class, Wikipedia
    locator: Programming languages; GUI code
---

## Idea

A nested class is a class declared inside another class. The name of the inner one is then a member of the outer one, such as `Playlist.Cursor`, and the nested class can use the private members of the outer class. Languages differ on one question: does an object of the nested class belong to an object of the outer class?

- A **static nested class** does not. It is a class like any other, grouped under the name of the outer class. The Java tutorial says: "Nested classes that are declared static are called static nested classes."[^1]
- An **inner class** does. Each inner object holds a hidden reference to one outer object, and its methods can read the fields of that object as if they were its own. "Non-static nested classes are called inner classes."

Java has both, and two more kinds that are declared in a method body: a local class, which has a name, and an anonymous class, which is declared and made in one expression. In Java:

```java run id=static-nested-inner-anonymous
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Main {
  static class Playlist {
    private final String name;
    private final List<String> songs = new ArrayList<>();

    Playlist(String name) {
      this.name = name;
    }

    // A static nested class: no Playlist instance; it gets data through parameters.
    static class Stats {
      final int count;

      Stats(Playlist p) {
        count = p.songs.size(); // may read the private field through a reference
      }
    }

    // An inner class: each Cursor belongs to one Playlist and reads its fields.
    class Cursor implements Iterator<String> {
      private int next = 0;

      public boolean hasNext() {
        return next < songs.size(); // songs is Playlist.this.songs
      }

      public String next() {
        return name + ": " + songs.get(next++);
      }
    }

    void add(String song) {
      songs.add(song);
    }
  }

  public static void main(String[] args) {
    Playlist p = new Playlist("road");
    p.add("one");
    p.add("two");

    Playlist.Stats s = new Playlist.Stats(p); // made without an outer instance
    Playlist.Cursor c = p.new Cursor(); // made from an outer instance
    System.out.println(s.count);
    while (c.hasNext()) {
      System.out.println(c.next());
    }

    String prefix = "> "; // effectively final, so the class below can use it
    // An anonymous class: declared and instantiated in one expression.
    Iterator<String> loud = new Iterator<>() {
      private final Iterator<String> in = p.new Cursor();

      public boolean hasNext() {
        return in.hasNext();
      }

      public String next() {
        return prefix + in.next().toUpperCase();
      }
    };
    while (loud.hasNext()) {
      System.out.println(loud.next());
    }
  }
}
```

```output for=static-nested-inner-anonymous
2
road: one
road: two
> ROAD: ONE
> ROAD: TWO
```

`Stats` is a static nested class: `new Playlist.Stats(p)` needs no `Playlist` object, and the constructor gets the playlist as a parameter. `Cursor` is an inner class: `p.new Cursor()` makes a cursor that belongs to `p`, and its methods read `songs` and `name` of that playlist. The anonymous class at the end implements `Iterator` in place and uses the local variable `prefix`.

## How it works

- **The outer reference.** An inner object has a hidden field that points to the outer object. The JLS calls it the immediately enclosing instance, and it "is determined when the object is created".[^2] Inside the inner class, the name `songs` means `Playlist.this.songs`. A static nested class has no such field: "Just as a static method of a class has no current instance of the class in its body, a static nested class has no immediately enclosing instance in its body."
- **Making an inner object.** An inner object needs an outer object. In an instance method of `Playlist`, `new Cursor()` uses `this`. Elsewhere, Java code writes `p.new Cursor()`, and Kotlin code writes `p.Cursor()`.
- **Access to private members.** A nested class is a member of the outer class, so it can use the private members of the outer class, and the outer class can use the private members of the nested class. C# says that a nested type "can access private and protected members of the containing type, including any inherited protected members".[^3]
- **Captured locals.** A local class, an anonymous class or a lambda in Java can use a local variable of the method only if the variable is final or effectively final. The object gets a copy of the value, so the rule keeps the copy and the variable equal.
- **The compiled form.** The JVM has no nested classes. The compiler makes a separate class file for each one, such as `Main$Playlist$Cursor.class`, and for an inner class it adds the outer reference as a constructor parameter and a field.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Java | native | static nested, inner member, local and anonymous classes; `Outer.this`; `outer.new Inner()` |
| Kotlin | native | nested classes by default; `inner class` for an outer reference; `object :` expressions |
| C# | native | nested types with no outer instance; private by default |
| C++ | native | nested classes with no outer `this`; local classes in functions |
| Python | idiom | a class statement in a class body makes a class attribute; no outer instance |
| JavaScript | idiom | a class expression in a static field; a class in a function closes over its variables |
| TypeScript | idiom | as JavaScript; namespaces merged with a class for nested type names |
| Go | idiom | separate types in the package; local types without methods; closures |
| Rust | idiom | modules group types; local items in functions; closures |
| C | absent | a struct inside a struct does not make a nested scope for its name |
| Bash | n/a | no classes |
| PowerShell | absent | classes cannot be nested |
| SQL | n/a | no classes |
| Regex | n/a | no classes |
| HTML | n/a | no classes |
| CSS | n/a | no classes |

- **Kotlin** reverses the Java default. A class inside a class is nested and has no outer object; the keyword `inner` adds one: "A nested class marked as inner can access the members of its outer class. Inner classes carry a reference to an object of an outer class".[^4]
- **C#** nested types are like Java static nested classes. "Regardless of whether the outer type is a class, interface, or struct, nested types default to private."[^3] For the outer object, C# code does by hand what a Java inner class does: "To access the containing type, pass it as an argument to the constructor of the nested type."
- **C++** nested classes have access to the private names of the outer class, but no outer object: "it is otherwise independent and has no special access to the this pointer of the enclosing class".[^5]

## Variations and related ideas

- **Anonymous classes and lambdas.** An anonymous class that implements an interface with one method can be a lambda in Java, Kotlin and C#, which is shorter. An anonymous class is still useful when the object needs fields or more than one method. Kotlin uses an object expression, `object : MouseAdapter() { ... }`.
- **Local classes and closures.** A local class is declared in a method and can use its variables. In JavaScript, Python, Go and Rust, a closure does the same job, and a class declared in a JavaScript function is itself a closure.
- **Nested types for grouping.** A nested type gives a name such as `Map.Entry` or `Playlist.Cursor`, which shows that the type exists for its outer type. The Java tutorial gives this reason: "It is a way of logically grouping classes that are only used in one place".[^1] Go and Rust use the module or package for grouping instead.
- **Builders and iterators.** The usual nested classes are a static nested `Builder` that makes the outer object, and an inner iterator or cursor that walks over the outer object's data.

## Pitfalls

- **An inner object keeps its outer object alive.** The hidden reference stops the garbage collector from freeing the outer object while the inner object is reachable. A listener or a callback made as an inner or anonymous class keeps a large outer object in memory. Make a nested class static unless it needs the outer object.
- **Serialization.** "Serialization of inner classes, including local and anonymous classes, is strongly discouraged."[^1] The serialized form contains the hidden outer field and names that the compiler makes, and these can differ between compilers.
- **Shadowed names.** A field of the inner class with the same name as a field of the outer class hides it. Write `Outer.this.name` in Java or `this@Outer.name` in Kotlin for the outer one.
- **A dangling outer pointer in C++.** A C++ nested object that keeps a pointer or a reference to its outer object does not keep that object alive. If the outer object is destroyed first, the pointer is invalid.
- **The outer class body is not a scope in Python.** A method of a nested class cannot see the names of the outer class body; it must write `Playlist.LIMIT`. The nested class also has no outer object; code must pass one.
- **Effectively final in Java.** A local variable that a local class, an anonymous class or a lambda uses cannot change after its assignment. Code that must change a counter from inside uses a field, an array of one element or an `AtomicInteger`.

## Cost

A static nested class costs the same as a top-level class. An inner object has one more field, the outer reference, so it uses one pointer more of memory and its constructor has one more parameter. Each nested, local or anonymous class is a separate class for the runtime, which must load it before its first use. Access from an inner class to a private member of the outer class is a field access; older Java compilers made extra accessor methods for it.

## History

Simula 67 and its successor BETA let a class contain other classes: "BETA language introduced this notion of nested classes."[^6] "Inner classes became a feature of the Java programming language starting with version 1.1", in 1997, together with local and anonymous classes. Their first big use was event handling: "Local inner classes are often used in Java to define callbacks for GUI code." The JLS defines the kinds today: "An inner class is a nested class that is not explicitly or implicitly static."[^2] C++ and C# chose nested classes without an outer object, and Kotlin made that the default and added the `inner` keyword. Java 8 added lambdas, which replaced most anonymous classes with one method.

## References

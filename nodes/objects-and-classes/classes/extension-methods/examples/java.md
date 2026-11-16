```java run id=utility-class-and-default-method
public class Main {
  static final class Texts { // a utility class: the usual answer in Java
    private Texts() {}

    static int wordCount(String text) {
      String t = text.strip();
      return t.isEmpty() ? 0 : t.split("\\s+").length;
    }
  }

  interface Shape {
    double area();

    default String describe() { // a default method: the author of the interface adds it
      return getClass().getSimpleName() + " " + area();
    }
  }

  record Square(double side) implements Shape {
    public double area() {
      return side * side;
    }
  }

  public static void main(String[] args) {
    System.out.println(Texts.wordCount("Hello static methods"));
    System.out.println(new Square(3).describe());
  }
}
```

```output for=utility-class-and-default-method
3
Square 9.0
```

- Verdict: absent. Java has no extension methods. A static method in a utility class, such as `Texts.wordCount(s)`, is the usual answer, and libraries such as Lombok and Manifold add extension methods with compiler plugins.
- A `default` method adds a method to every class that implements an interface, but only the author of the interface can add it, and a class can override it.

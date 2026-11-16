```java run id=class-object-reflection
public class Main {
  interface Plugin {
    String run(String text);
  }

  static final class Upper implements Plugin {
    public String run(String text) {
      return text.toUpperCase();
    }
  }

  public static void main(String[] args) throws Exception {
    Class<?> c = Upper.class; // the object that describes the class at run time
    System.out.println(c.getSimpleName() + " " + c.getClass().getName());
    System.out.println(Class.class.getClass() == Class.class); // the class of every class is Class
    System.out.println(Plugin.class.isAssignableFrom(c));
    Plugin p = (Plugin) c.getDeclaredConstructor().newInstance(); // make an instance by reflection
    System.out.println(p.run("abc"));
  }
}
```

```output for=class-object-reflection
Upper java.lang.Class
true
true
ABC
```

- Verdict: absent. Each class has a `Class` object at run time, and the class of every `Class` object is `Class`. Code can read a class and make instances by reflection, but it cannot change how a class is made.
- To check or register classes when they are defined, Java code uses an annotation processor at compile time, or `ServiceLoader`, which reads a list of implementations from the class path.

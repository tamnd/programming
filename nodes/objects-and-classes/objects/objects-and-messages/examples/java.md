```java run id=objects-and-primitives
import java.util.ArrayList;

public class Main {
    interface Greeter {
        String greet(String name);
    }

    record Point(int x, int y) {}

    public static void main(String[] args) {
        Object[] things = {42, "hi", new int[] {1, 2}, new Point(1, 2), new ArrayList<String>()};
        for (Object t : things) {
            System.out.print(t.getClass().getSimpleName() + " ");
        }
        System.out.println();

        int n = 42; // a primitive value, not an object
        Object boxed = n; // boxing makes an Integer object
        System.out.println(boxed.getClass().getName() + " " + boxed.equals(42));

        Greeter polite = new Greeter() { // an object of an anonymous class
            public String greet(String name) {
                return "Good day, " + name;
            }
        };
        Greeter brief = name -> "Hi " + name;
        System.out.println(polite.greet("ana") + " / " + brief.greet("bo"));

        Point p = new Point(1, 2);
        Point q = p; // copies the reference, not the object
        System.out.println((p == q) + " " + (p == new Point(1, 2)));
    }
}
```

```output for=objects-and-primitives
Integer String int[] Point ArrayList 
java.lang.Integer true
Good day, ana / Hi bo
true false
```

- Verdict: native. In Java an object is a class instance or an array, and variables of reference types hold references to objects. The eight primitive types, such as `int` and `double`, are not objects: they have no methods and no identity. Assigning a primitive to `Object` boxes it in a wrapper object such as `Integer`. Every object knows its class through `getClass`.
- An anonymous class creates one object of a class that has no name, and a lambda creates an object of a functional interface. `==` on references compares identity, so `p == q` is true after `q = p`, and a new `Point` with the same values is a different object.

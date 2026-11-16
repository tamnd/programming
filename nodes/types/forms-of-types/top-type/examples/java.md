```java run id=object-root
import java.util.ArrayList;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Object> values = new ArrayList<>();
    values.add(42); // boxed to Integer
    values.add("hi");
    values.add(new int[] {1, 2});
    values.add(new StringBuilder("sb"));
    for (Object v : values) {
      System.out.println(v.getClass().getSimpleName());
    }

    Object o = values.get(0);
    if (o instanceof Integer n) {
      System.out.println(n + 1);
    }
    try {
      String s = (String) o;
      System.out.println(s);
    } catch (ClassCastException e) {
      System.out.println("ClassCastException");
    }
    System.out.println(Object.class.getSuperclass() + " " + String.class.getSuperclass().getName());
  }
}
```

```output for=object-root
Integer
String
int[]
StringBuilder
43
ClassCastException
null java.lang.Object
```

- Verdict: native. `Object` is the root of the class hierarchy, so a `List<Object>` can hold a string, an array and a builder. An `int` is not an object, so `add(42)` boxes it to an `Integer`. `instanceof` with a pattern tests the type and casts in one step, and a plain cast to the wrong type throws `ClassCastException`. `Object` has no superclass.

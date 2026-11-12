```java run id=primitive-and-reference-values
import java.util.ArrayList;
import java.util.List;

public class Main {
    static void change(int n, List<String> list) {
        n = 99;
        list.add("added");
        list = new ArrayList<>();
        list.add("lost");
    }

    public static void main(String[] args) {
        // A primitive value is the number itself; a reference value points to an object.
        int n = 1;
        List<String> list = new ArrayList<>(List.of("a"));
        change(n, list);
        System.out.println(n + " " + list);

        // Copying a reference value gives two references to one object.
        int[] a = {1, 2};
        int[] b = a;
        b[0] = 9;
        System.out.println(a[0] + " " + (a == b));

        // == on references compares identity; equals compares the values.
        String s1 = new String("java");
        String s2 = new String("java");
        System.out.println((s1 == s2) + " " + s1.equals(s2));

        // The null reference refers to no object.
        String none = null;
        System.out.println(none == null);
        try {
            System.out.println(none.length());
        } catch (NullPointerException e) {
            System.out.println("NullPointerException");
        }

        // A primitive value boxes into an object, and the box unboxes to the value.
        Object boxed = 42;
        int back = (Integer) boxed;
        System.out.println(boxed.getClass().getSimpleName() + " " + back);
    }
}
```

```output for=primitive-and-reference-values
1 [a, added]
9 true
false true
true
NullPointerException
Integer 42
```

- Verdict: native. A method gets copies of the values of its arguments: it cannot change the caller's `int` or the caller's variable, but it can change the object that a reference value points to. Two array variables can hold one reference. `==` on two `String` objects compares references, and `equals` compares text. The `null` reference refers to no object. `42` boxes into an `Integer` and unboxes back.

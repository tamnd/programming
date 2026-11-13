```java run id=fixed-length-objects
import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        // new int[5] makes five components with the default value 0.
        int[] a = new int[5];
        a[0] = 7;
        a[4] = 9;
        System.out.println(Arrays.toString(a) + " " + a.length);

        // An index outside 0 to length - 1 throws an exception.
        try {
            a[5] = 1;
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
        }

        // An array is an object. Assignment copies the reference.
        int[] b = a;
        b[1] = 1;
        int[] c = a.clone();
        c[1] = 2;
        System.out.println(a[1] + " " + c[1] + " " + (a == b));

        // == compares references. Arrays.equals compares the components.
        int[] d = {7, 1, 0, 0, 9};
        System.out.println((a == d) + " " + Arrays.equals(a, d));

        // Arrays are covariant, so the store check runs when the program runs.
        Object[] objects = new String[2];
        try {
            objects[0] = 42;
        } catch (ArrayStoreException e) {
            System.out.println("ArrayStoreException: " + e.getMessage());
        }
    }
}
```

```output
[7, 0, 0, 0, 9] 5
Index 5 out of bounds for length 5
1 2 true
false true
ArrayStoreException: java.lang.Integer
```

- Verdict: native. An array is an object with a fixed length and default values. Every index is checked. Assignment shares the array, clone copies it, and == compares references while Arrays.equals compares the components. A String[] stored in an Object[] variable rejects an Integer at run time.

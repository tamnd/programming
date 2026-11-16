```java run id=definite-assignment
public class Main {
    static int count;
    static String name;

    public static void main(String[] args) {
        int[] scores = new int[3];
        System.out.println(count + " " + name + " " + scores[1]);

        int limit;
        if (args.length > 0) {
            limit = args.length;
        } else {
            limit = 10;
        }
        System.out.println(limit);

        final int size;
        size = 4;
        System.out.println(size);
    }
}
```

```output for=definite-assignment
0 null 0
10
4
```

- Verdict: native. Fields and array elements get a default value: 0 for numbers and `null` for references. A local variable has no default value. The compiler checks that each path gives `limit` a value before the read, and rejects the program if one path does not. A `final` local variable can get its one value after the declaration.

```java run id=field-defaults
public class Main {
    static byte smallNumber;
    static double ratio;
    static char letter;
    static boolean flag;
    static Object thing;

    public static void main(String[] args) {
        System.out.println(smallNumber + " " + ratio + " " + (int) letter + " " + flag + " " + thing);
        String[] names = new String[2];
        boolean[] seen = new boolean[2];
        System.out.println(names[0] + " " + seen[1]);
    }
}
```

```output for=field-defaults
0 0.0 0 false null
null false
```

- Verdict: native. Each field and each array element gets the default value of its type: zero for numbers, the character `'\u0000'` for `char`, `false` for `boolean` and `null` for references. Local variables get no default value: the compiler requires an assignment before a read.

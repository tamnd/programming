```java run id=all-paths
public class Main {
    public static void main(String[] args) {
        int code = args.length;
        String label;
        switch (code) {
            case 0 -> label = "none";
            case 1 -> label = "one";
            default -> label = "many";
        }
        System.out.println(label);

        String[] names = new String[1];
        System.out.println(names[0] == null);
    }
}
```

```output for=all-paths
none
true
```

- Verdict: native. A local variable has no value until an assignment, and the compiler checks every path to a read. Each case of the switch assigns `label`, so the read compiles. Without the `default` case, the compiler rejects the program, because `label` might not have a value. Array elements and fields have default values, such as `null`, so the compiler does not check them.

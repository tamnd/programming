```java run id=return-value-java
public class Main {
    static int absolute(int n) {
        if (n < 0) {
            return -n;
        }
        return n;
    }

    public static void main(String[] args) {
        System.out.println(absolute(-7));
        System.out.println(absolute(5) + 1);
    }
}
```

```output for=return-value-java
7
6
```

- Verdict: native. The result type comes before the method name, and the compiler checks that each path ends with a `return`.

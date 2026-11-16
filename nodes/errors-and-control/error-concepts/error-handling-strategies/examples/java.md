```java run id=strategy-java
public class Main {
    static int divide(int a, int b) {
        if (b == 0) {
            throw new IllegalArgumentException("division by zero");
        }
        return a / b;
    }

    public static void main(String[] args) {
        int[][] pairs = {{10, 2}, {10, 0}};
        for (int[] p : pairs) {
            try {
                System.out.println(p[0] + " / " + p[1] + " = " + divide(p[0], p[1]));
            } catch (IllegalArgumentException e) {
                System.out.println(p[0] + " / " + p[1] + ": error: " + e.getMessage());
            }
        }
    }
}
```

```output for=strategy-java
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: exceptions. A bad argument is a bug of the caller, so the code throws an unchecked exception. An error that the caller can be expected to recover from, such as a missing file, is a checked exception, which the method must declare with `throws`.

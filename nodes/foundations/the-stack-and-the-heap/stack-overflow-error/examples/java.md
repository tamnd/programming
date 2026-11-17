```java run id=deep
public class Main {
    static int depth(int n) {
        return n == 0 ? 0 : 1 + depth(n - 1); // one frame for each step
    }

    static int loop(int n) {
        int count = 0;
        while (n > 0) { // one frame for the whole loop
            n--;
            count++;
        }
        return count;
    }

    public static void main(String[] args) {
        System.out.println("depth(100) = " + depth(100));
        try {
            depth(1_000_000);
        } catch (StackOverflowError e) {
            System.out.println("depth(1000000): StackOverflowError");
        }
        System.out.println("loop(1000000) = " + loop(1_000_000));
    }
}
```

```output for=deep
depth(100) = 100
depth(1000000): StackOverflowError
loop(1000000) = 1000000
```

- Verdict: native. The JVM throws `StackOverflowError` when a thread needs more stack than it may have. It is an `Error`, not an `Exception`, so `catch (Exception e)` does not catch it.
- A `Thread` constructor takes a stack size, but its effect depends on the platform.

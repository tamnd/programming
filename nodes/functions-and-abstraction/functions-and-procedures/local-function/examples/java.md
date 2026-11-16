```java run id=local-function-java
import java.util.function.IntUnaryOperator;

public class Main {
    static int sumOfScaled(int[] values, int factor) {
        IntUnaryOperator scale = x -> x * factor;

        int total = 0;
        for (int v : values) {
            total += scale.applyAsInt(v);
        }
        return total;
    }

    public static void main(String[] args) {
        System.out.println(sumOfScaled(new int[] {1, 2, 3}, 10));
        System.out.println(sumOfScaled(new int[] {4}, 2));
    }
}
```

```output for=local-function-java
60
8
```

- Verdict: idiom. A Java method cannot contain a method. A local variable holds a lambda, which can read the effectively final parameter `factor`.

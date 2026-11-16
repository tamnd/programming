```java run id=first-class-function-java
import java.util.Map;
import java.util.function.IntUnaryOperator;

public class Main {
    static int twice(int x) {
        return x * 2;
    }

    static int applyTwice(IntUnaryOperator g, int x) {
        return g.applyAsInt(g.applyAsInt(x));
    }

    static IntUnaryOperator adder(int n) {
        return x -> x + n;
    }

    public static void main(String[] args) {
        IntUnaryOperator f = Main::twice;
        System.out.println(f.applyAsInt(5));

        Map<String, IntUnaryOperator> ops = Map.of("double", Main::twice, "square", x -> x * x);
        System.out.println(ops.get("square").applyAsInt(4));

        System.out.println(applyTwice(Main::twice, 3));

        IntUnaryOperator add5 = adder(5);
        System.out.println(add5.applyAsInt(10));
    }
}
```

```output for=first-class-function-java
10
16
12
15
```

- Verdict: native. A method is not a value, but the method reference `Main::twice` and the lambda `x -> x * x` are. Their type is the functional interface `IntUnaryOperator`, and the call is `applyAsInt`.

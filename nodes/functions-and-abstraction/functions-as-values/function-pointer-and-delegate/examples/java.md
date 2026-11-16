```java run id=function-pointer-and-delegate-java
import java.util.function.IntBinaryOperator;

public class Main {
    static int add(int a, int b) {
        return a + b;
    }

    static int mul(int a, int b) {
        return a * b;
    }

    static int apply(IntBinaryOperator f, int a, int b) {
        return f.applyAsInt(a, b);
    }

    public static void main(String[] args) {
        IntBinaryOperator op = Main::add;
        System.out.println(op.applyAsInt(6, 7));
        op = Main::mul;
        System.out.println(op.applyAsInt(6, 7));
        System.out.println(apply((a, b) -> a - b, 6, 7));
    }
}
```

```output for=function-pointer-and-delegate-java
13
42
-1
```

- Verdict: native. Java has no function pointers. The method references `Main::add` and `Main::mul` make objects of the functional interface `IntBinaryOperator`, and the call is `applyAsInt`.

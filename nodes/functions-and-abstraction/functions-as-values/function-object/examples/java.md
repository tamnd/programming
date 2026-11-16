```java run id=function-object-java
import java.util.function.IntUnaryOperator;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Main {
    static class Multiplier implements IntUnaryOperator {
        private final int factor;
        int calls = 0;

        Multiplier(int factor) {
            this.factor = factor;
        }

        @Override
        public int applyAsInt(int x) {
            calls++;
            return x * factor;
        }
    }

    public static void main(String[] args) {
        Multiplier triple = new Multiplier(3);
        System.out.println(triple.applyAsInt(5));
        String out = IntStream.of(1, 2, 3)
                .map(triple)
                .mapToObj(String::valueOf)
                .collect(Collectors.joining(" "));
        System.out.println(out);
        System.out.println(triple.calls);
    }
}
```

```output for=function-object-java
15
3 6 9
4
```

- Verdict: idiom. Java has no call operator for objects. The class implements the functional interface `IntUnaryOperator`, so `IntStream.map` takes the object. A direct call uses the name of the method, `applyAsInt`.

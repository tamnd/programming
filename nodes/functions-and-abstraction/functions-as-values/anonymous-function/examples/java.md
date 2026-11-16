```java run id=anonymous-function-java
import java.util.ArrayList;
import java.util.List;
import java.util.function.IntUnaryOperator;
import java.util.stream.Collectors;

public class Main {
    public static void main(String[] args) {
        List<String> words = new ArrayList<>(List.of("house", "car", "bike"));
        words.sort((a, b) -> Integer.compare(a.length(), b.length()));
        System.out.println(String.join(" ", words));

        String squares = List.of(1, 2, 3).stream()
                .map(x -> String.valueOf(x * x))
                .collect(Collectors.joining(" "));
        System.out.println(squares);

        int n = 5;
        IntUnaryOperator add = x -> x + n;
        System.out.println(add.applyAsInt(10));
    }
}
```

```output for=anonymous-function-java
car bike house
1 4 9
15
```

- Verdict: native. The lambdas `(a, b) -> ...`, `x -> ...` and `x -> x + n` are anonymous functions. A lambda can use a local variable such as `n` only if the variable does not change after its assignment.

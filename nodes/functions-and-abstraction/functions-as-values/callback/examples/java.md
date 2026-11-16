```java run id=callback-java
import java.util.ArrayList;
import java.util.List;
import java.util.function.IntConsumer;
import java.util.stream.Collectors;

public class Main {
    static void repeat(int n, IntConsumer callback) {
        for (int i = 1; i <= n; i++) {
            callback.accept(i);
        }
    }

    static void tick(int i) {
        System.out.println("tick " + i);
    }

    public static void main(String[] args) {
        List<Integer> xs = new ArrayList<>(List.of(3, 1, 2));
        xs.sort((a, b) -> Integer.compare(b, a));
        System.out.println(xs.stream().map(String::valueOf).collect(Collectors.joining(" ")));
        repeat(2, Main::tick);
    }
}
```

```output for=callback-java
3 2 1
tick 1
tick 2
```

- Verdict: native. `List.sort` takes a `Comparator`, and `repeat` takes an `IntConsumer`. A lambda and the method reference `Main::tick` make objects of these functional interfaces.

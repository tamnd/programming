```java run id=infinite
import java.util.List;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class Main {
    static boolean isPrime(int n) {
        for (int d = 2; d * d <= n; d++) {
            if (n % d == 0) {
                return false;
            }
        }
        return true;
    }

    public static void main(String[] args) {
        List<Integer> primes = Stream.iterate(2, n -> n + 1)
                .filter(Main::isPrime)
                .limit(10)
                .toList();
        System.out.println(primes);

        System.out.println(Stream.iterate(1, x -> 2 * x).limit(8).toList());

        int firstSquare = IntStream.iterate(1, n -> n + 1)
                .map(n -> n * n)
                .filter(s -> s > 50)
                .findFirst()
                .getAsInt();
        System.out.println("first square over 50: " + firstSquare);

        System.out.println(Stream.generate(() -> "ab").limit(3).toList());
    }
}
```

```output for=infinite
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
[1, 2, 4, 8, 16, 32, 64, 128]
first square over 50: 64
[ab, ab, ab]
```

- Verdict: stdlib. `Stream.iterate(seed, f)` and `Stream.generate(supplier)` make infinite streams. `limit` and `findFirst` are short-circuiting operations, so the pipeline stops after a finite number of elements.
- Without `limit`, a terminal operation such as `toList` or `count` on these streams never returns.

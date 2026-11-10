```java run id=checker
import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;
import java.util.function.Predicate;

public class Main {
    /** xorshift32 with Marsaglia's seed: the same numbers on every run. */
    static final class Rng {
        private int state = (int) 2463534242L; // Java has no unsigned int

        int next() {
            state ^= state << 13;
            state ^= state >>> 17; // >>> shifts in zeros, as on an unsigned value
            state ^= state << 5;
            return state;
        }

        long between(long lo, long hi) {
            return lo + Long.remainderUnsigned(Integer.toUnsignedLong(next()), hi - lo + 1);
        }
    }

    record Pair(int items, int perPage) {
        @Override
        public String toString() {
            return "(" + items + ", " + perPage + ")";
        }
    }

    /** Runs a property on 100 generated cases and shrinks the first failure. */
    static <T> void check(String name, Function<Rng, T> generate, Predicate<T> holds, Function<T, List<T>> shrink) {
        Rng rng = new Rng();
        for (int trial = 1; trial <= 100; trial++) {
            T c = generate.apply(rng);
            if (holds.test(c)) {
                continue;
            }
            System.out.println(name + ": falsified on test " + trial + " by " + c);
            int steps = 0;
            boolean shrunk = true;
            while (shrunk) {
                shrunk = false;
                for (T smaller : shrink.apply(c)) {
                    if (!holds.test(smaller)) {
                        c = smaller;
                        shrunk = true;
                        steps++;
                        break;
                    }
                }
            }
            System.out.println(name + ": shrunk in " + steps + " steps to " + c);
            return;
        }
        System.out.println(name + ": passed 100 tests");
    }

    /** Gives lo, then x - d for d = (x - lo) / 2, / 4, ..., 1. */
    static List<Integer> shrinkInt(int x, int lo) {
        List<Integer> out = new ArrayList<>();
        if (x > lo) {
            out.add(lo);
        }
        for (int d = (x - lo) / 2; d > 0; d /= 2) {
            out.add(x - d);
        }
        return out;
    }

    static int pages(int items, int perPage) {
        return items / perPage; // bug: drops the last page when it is not full
    }

    public static void main(String[] args) {
        check("round trip",
            rng -> (int) rng.between(Integer.MIN_VALUE, Integer.MAX_VALUE),
            x -> Integer.parseInt(Integer.toString(x)) == x,
            x -> List.of());

        check("pages",
            rng -> new Pair((int) rng.between(0, 1000), (int) rng.between(1, 50)),
            p -> pages(p.items(), p.perPage()) * p.perPage() >= p.items(),
            p -> {
                List<Pair> out = new ArrayList<>();
                for (int n : shrinkInt(p.items(), 0)) {
                    out.add(new Pair(n, p.perPage()));
                }
                for (int n : shrinkInt(p.perPage(), 1)) {
                    out.add(new Pair(p.items(), n));
                }
                return out;
            });
    }
}
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

- Verdict: library. jqwik is a property testing library for the JVM, with a focus on Java and Kotlin.
- Java has no unsigned `int`. The state is stored in an `int` with the same bits, `>>>` shifts in zeros, and `Integer.toUnsignedLong` and `Long.remainderUnsigned` read the bits as an unsigned number.
- `Function` and `Predicate` are the standard functional interfaces, and the lambdas implement them. The generic method `check` boxes the `int` cases as `Integer`.
- A record gives the pair `equals`, `hashCode` and accessors. The program overrides `toString` so that the output matches the other languages.

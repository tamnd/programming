```java run id=schema
import java.util.ArrayList;
import java.util.List;

public class Main {
    // The first two names are PIT mutators; the other two change a variable and a constant.
    enum Mutant { NONE, CONDITIONALS_BOUNDARY, NEGATE_CONDITIONALS, COMPARE_BEST_TO_BEST, START_AT_0 }

    record Case(int[] xs, int want) {}

    static int argmax(int[] xs, Mutant m) {
        int best = 0;
        for (int i = m == Mutant.START_AT_0 ? 0 : 1; i < xs.length; i++) {
            int left = m == Mutant.COMPARE_BEST_TO_BEST ? xs[best] : xs[i];
            boolean bigger = switch (m) {
                case CONDITIONALS_BOUNDARY -> left >= xs[best];
                case NEGATE_CONDITIONALS -> left <= xs[best];
                default -> left > xs[best];
            };
            if (bigger) {
                best = i;
            }
        }
        return best;
    }

    static boolean passes(Mutant m, List<Case> suite) {
        return suite.stream().allMatch(c -> argmax(c.xs(), m) == c.want());
    }

    public static void main(String[] args) {
        List<Case> weak = List.of(
                new Case(new int[] {1, 2, 3}, 2),
                new Case(new int[] {1, 3, 2}, 1),
                new Case(new int[] {3, 1, 2}, 0));
        List<Case> strong = new ArrayList<>(weak);
        strong.add(new Case(new int[] {2, 2, 1}, 0));

        if (!passes(Mutant.NONE, strong)) {
            throw new AssertionError("the original must pass first");
        }
        System.out.printf("%-24s%-8s%s%n", "mutant", "3 tests", "4 tests");
        int weakKilled = 0, strongKilled = 0;
        for (Mutant m : Mutant.values()) {
            if (m == Mutant.NONE) {
                continue;
            }
            boolean weakDead = !passes(m, weak), strongDead = !passes(m, strong);
            weakKilled += weakDead ? 1 : 0;
            strongKilled += strongDead ? 1 : 0;
            System.out.printf("%-24s%-8s%s%n", m, weakDead ? "killed" : "lived", strongDead ? "killed" : "lived");
        }
        System.out.printf("%-24s%d of 4  %d of 4%n", "killed", weakKilled, strongKilled);
    }
}
```

```output
mutant                  3 tests 4 tests
CONDITIONALS_BOUNDARY   lived   killed
NEGATE_CONDITIONALS     killed  killed
COMPARE_BEST_TO_BEST    killed  killed
START_AT_0              lived   lived
killed                  2 of 4  3 of 4
```

- Verdict: library. The JDK has no mutation tool. PIT is the common one for Java and the JVM.
- The first two mutants use PIT's rules. `CONDITIONALS_BOUNDARY` turns `>` into `>=`. `NEGATE_CONDITIONALS` turns `>` into `<=`, not `<`, so this row differs from the Python table. Both rules are on by default in PIT.
- PIT changes the bytecode that the compiler made, not the source. Its documentation says that this is in general much faster and easier to add to a build, but that a bytecode change can be hard to map back to a change in the source.
- The `switch` expression over the enum is the mutant schema. A `default` branch keeps the original `>` for the other mutants.

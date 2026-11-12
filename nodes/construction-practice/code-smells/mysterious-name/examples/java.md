```java run id=rename-in-place
import java.util.List;

public class Main {
    record Call(int minutes) {}

    static double calc(List<Call> l, int x) {
        int n = 0;
        for (Call e : l) {
            if (e.minutes() > x) n += e.minutes();
        }
        return n / 60.0;
    }

    static double billableHours(List<Call> calls, int freeMinutes) {
        int billedMinutes = 0;
        for (Call call : calls) {
            if (call.minutes() > freeMinutes) billedMinutes += call.minutes();
        }
        return billedMinutes / 60.0;
    }

    static void check(String name, java.util.function.BiFunction<List<Call>, Integer, Double> f) {
        List<Call> calls = List.of(new Call(5), new Call(30), new Call(45));
        if (f.apply(calls, 10) != 1.25) throw new AssertionError(name);
        if (f.apply(List.of(), 10) != 0.0) throw new AssertionError(name);
        System.out.println(name + ": tests pass");
    }

    public static void main(String[] args) {
        check("calc", Main::calc);
        check("billableHours", Main::billableHours);
    }
}
```

```output
calc: tests pass
billableHours: tests pass
```

- The record `Call` already has a good name for its component, so the rename touches only the method, its parameters and its locals. In a typed language, the compiler finds every caller, so a rename of a method is safe within one build.
- The Rename refactoring of IntelliJ IDEA changes the declaration and every reference to it.
- `n / 60` with an `int` would truncate to 1. The literal `60.0` makes the division a `double` division. A name such as `billedMinutes` makes it clearer that the unit changes here, from minutes to hours.

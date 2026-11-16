```java run id=match-guards-java
public class Main {
    static String describe(Integer n) {
        return switch (n) {
            case Integer k when k == 0 -> "zero";
            case Integer k when k < 0 -> "negative";
            case Integer k when k < 10 && k % 2 == 0 -> "small even";
            case Integer k when k < 10 -> "small odd";
            case Integer k -> "large";
        };
    }

    public static void main(String[] args) {
        StringBuilder out = new StringBuilder();
        for (int n : new int[] {-5, 0, 4, 7, 12}) {
            if (out.length() > 0) {
                out.append(", ");
            }
            out.append(describe(n));
        }
        System.out.println(out);
    }
}
```

```output for=match-guards-java
negative, zero, small even, small odd, large
```

- Verdict: native. A pattern case label can have a guard after `when`. The selector is an `Integer`, so each label is the type pattern `Integer k` with a guard. The last label has no guard, so the switch is exhaustive.

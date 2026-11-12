```java run id=conditional-and-or
import java.util.Map;

public class Main {
    static int calls = 0;

    static boolean check(boolean v) {
        calls++;
        return v;
    }

    public static void main(String[] args) {
        // && and || skip the right operand when the left one decides the result.
        boolean r1 = check(false) && check(true);
        System.out.println(r1 + " calls=" + calls);
        calls = 0;
        // & and | on booleans evaluate both operands.
        boolean r2 = check(false) & check(true);
        System.out.println(r2 + " calls=" + calls);

        // The guard pattern: the right side runs only when it is safe.
        String s = null;
        System.out.println(s != null && s.length() > 3);

        Map<String, Integer> stock = Map.of("pen", 4);
        String key = "ink";
        System.out.println(stock.containsKey(key) && stock.get(key) > 0);

        // The conditional operator also evaluates only one branch.
        int d = 0;
        System.out.println(d == 0 ? "no division" : String.valueOf(10 / d));
    }
}
```

```output for=conditional-and-or
false calls=1
false calls=2
false
false
no division
```

- Verdict: native. `&&` and `||` skip the right operand when the left one decides the result, and `&` and `|` on `boolean` evaluate both, so the second call runs. A null test before `s.length()` and a `containsKey` test before `get` are the usual guards. The conditional operator evaluates only the branch that it selects, so the division by zero does not run.

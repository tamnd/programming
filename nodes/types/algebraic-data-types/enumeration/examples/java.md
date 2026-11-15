```java run id=light
import java.util.Locale;

public class Main {
    enum Light {
        RED, AMBER, GREEN;

        Light after() {
            return switch (this) {
                case RED -> GREEN;
                case GREEN -> AMBER;
                case AMBER -> RED;
            };
        }

        String lower() {
            return name().toLowerCase(Locale.ROOT);
        }
    }

    public static void main(String[] args) {
        for (Light light : Light.values()) {
            System.out.println(light.lower() + " " + light.ordinal() + " then " + light.after().lower());
        }
        System.out.println("by name: " + Light.valueOf("AMBER").ordinal());
    }
}
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
by name: 1
```

- Verdict: native. An `enum` is a class with exactly one instance for each constant. It can have methods, such as `after`.
- `values()` lists the constants in declaration order, and `ordinal()` gives the position.
- The `switch` expression covers every constant, so it needs no `default` branch.

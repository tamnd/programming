```java run id=count
import java.util.ArrayList;
import java.util.List;

public class Main {
    enum Light { RED, AMBER, GREEN }

    // Product: a flag AND a light.
    record Pair(boolean flag, Light light) {}

    // Sum: a flag OR a light.
    sealed interface Choice permits Flag, Lamp {}
    record Flag(boolean flag) implements Choice {}
    record Lamp(Light light) implements Choice {}

    static String yn(boolean b) {
        return b ? "yes" : "no";
    }

    static String name(Light l) {
        return l.name().toLowerCase();
    }

    static String show(Choice c) {
        return switch (c) {
            case Flag f -> "flag:" + yn(f.flag());
            case Lamp l -> "light:" + name(l.light());
        };
    }

    public static void main(String[] args) {
        boolean[] flags = {false, true};
        List<Pair> pairs = new ArrayList<>();
        List<Choice> choices = new ArrayList<>();
        for (boolean f : flags) {
            for (Light l : Light.values()) {
                pairs.add(new Pair(f, l));
            }
            choices.add(new Flag(f));
        }
        for (Light l : Light.values()) {
            choices.add(new Lamp(l));
        }

        System.out.println("pairs: " + pairs.size());
        List<String> p = new ArrayList<>();
        for (Pair pair : pairs) {
            p.add(yn(pair.flag()) + "/" + name(pair.light()));
        }
        System.out.println(String.join(" ", p));
        System.out.println("choices: " + choices.size());
        System.out.println(String.join(" ", choices.stream().map(Main::show).toList()));
    }
}
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: native. A `record` is a product. A `sealed` interface whose permitted subclasses are records is a sum of products.
- The `switch` in `show` has no `default`. The compiler accepts it because `Choice` permits only `Flag` and `Lamp`.

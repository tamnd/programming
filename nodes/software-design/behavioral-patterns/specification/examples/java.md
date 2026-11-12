```java run id=containers
import java.util.List;
import java.util.function.Predicate;
import java.util.stream.Collectors;

record Container(String name, boolean refrigerated, boolean clean) {}

public class Main {
    static final List<Container> CONTAINERS = List.of(
            new Container("c1", true, true),
            new Container("c2", true, false),
            new Container("c3", false, true));

    static String select(Predicate<Container> spec) {
        return CONTAINERS.stream().filter(spec).map(Container::name).collect(Collectors.joining(", "));
    }

    public static void main(String[] args) {
        Predicate<Container> refrigerated = Container::refrigerated;
        Predicate<Container> clean = Container::clean;

        System.out.println("meat: " + select(refrigerated.and(clean)));
        System.out.println("not refrigerated: " + select(refrigerated.negate()));
        System.out.println("refrigerated or clean: " + select(refrigerated.or(clean)));
    }
}
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: stdlib. `java.util.function.Predicate` has the default methods `and`, `or` and `negate`, so a predicate is a specification that combines.

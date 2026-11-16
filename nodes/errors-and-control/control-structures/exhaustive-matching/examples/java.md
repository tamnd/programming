```java run id=exhaustive-matching-java
public class Main {
    enum Light { RED, YELLOW, GREEN }

    static String action(Light light) {
        // A switch expression must be exhaustive. No default is needed here.
        return switch (light) {
            case RED -> "stop";
            case YELLOW -> "slow down";
            case GREEN -> "go";
        };
    }

    public static void main(String[] args) {
        for (Light light : Light.values()) {
            System.out.println(light.name().toLowerCase() + ": " + action(light));
        }
    }
}
```

```output for=exhaustive-matching-java
red: stop
yellow: slow down
green: go
```

- Verdict: native. A switch expression must be exhaustive, and a switch on an enum is exhaustive when it names each constant. The switch has no `default`, so a new constant gives a compile-time error. If a new constant reaches a switch that was not compiled again, the default that the compiler added throws an exception.

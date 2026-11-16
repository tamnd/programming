```java run id=refutable-java
public class Main {
    record Pair(String key, String value) {}

    static Pair parse(String line) {
        int i = line.indexOf('=');
        return i < 0 ? null : new Pair(line.substring(0, i), line.substring(i + 1));
    }

    public static void main(String[] args) {
        for (String line : new String[] {"width=80", "oops", "height=24"}) {
            // A record pattern after instanceof fails for null.
            if (parse(line) instanceof Pair(String key, String value)) {
                System.out.println(key + " -> " + value);
            } else {
                System.out.println("skip: " + line);
            }
        }
    }
}
```

```output for=refutable-java
width -> 80
skip: oops
height -> 24
```

- Verdict: native. Java uses patterns only in `instanceof` and in `switch`, where a failed match is handled. A record pattern does not match `null`, so `parse` returns `null` for a line without `=`. The JLS calls a pattern that each value of a type matches unconditional.

```java run id=named-steps
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class Main {
    record Request(String path, int status, int ms) {}

    static List<String> slowFailuresLoop(List<Request> log) {
        List<Request> failed = new ArrayList<>();
        for (Request r : log) if (r.status() >= 500) failed.add(r);
        failed.sort(Comparator.comparingInt(Request::ms).reversed());
        List<String> paths = new ArrayList<>();
        for (int i = 0; i < failed.size() && i < 3; i++) paths.add(failed.get(i).path());
        return paths;
    }

    static List<String> slowFailures(List<Request> log) {
        return log.stream()
                .filter(r -> r.status() >= 500)
                .sorted(Comparator.comparingInt(Request::ms).reversed())
                .limit(3)
                .map(Request::path)
                .toList();
    }

    public static void main(String[] args) {
        List<Request> log = List.of(
                new Request("/a", 200, 40), new Request("/b", 503, 900), new Request("/c", 500, 120),
                new Request("/d", 502, 2500), new Request("/e", 500, 300), new Request("/f", 404, 10));
        if (!slowFailures(log).equals(slowFailuresLoop(log))) throw new AssertionError("differ");
        if (!slowFailures(log).equals(List.of("/d", "/b", "/e"))) throw new AssertionError(slowFailures(log));

        // Laziness: with no terminal operation, the filter never runs.
        int[] calls = {0};
        log.stream().filter(r -> { calls[0]++; return true; });
        if (calls[0] != 0) throw new AssertionError("ran early");
        System.out.println("replace loop with pipeline: tests pass");
    }
}
```

```output
replace loop with pipeline: tests pass
```

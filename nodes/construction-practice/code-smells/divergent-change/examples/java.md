```java run id=split-phase-report
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    record Sale(String region, int amount) {}

    // Before: the computation and the format are mixed.
    static String reportBefore(List<Sale> sales, String format) {
        boolean csv = format.equals("csv");
        int total = 0;
        Map<String, Integer> byRegion = new LinkedHashMap<>();
        StringBuilder out = new StringBuilder(csv ? "region,amount\n" : "");
        for (Sale s : sales) {
            total += s.amount();
            byRegion.merge(s.region(), s.amount(), Integer::sum);
        }
        byRegion.forEach((r, a) -> out.append(csv ? r + "," + a + "\n" : r + ": " + a + "\n"));
        out.append(csv ? "total," + total + "\n" : "Total: " + total + "\n");
        return out.toString();
    }

    // After: phase 1 computes a summary, phase 2 renders it.
    record Summary(Map<String, Integer> byRegion, int total) {}

    static Summary summarize(List<Sale> sales) {
        Map<String, Integer> byRegion = new LinkedHashMap<>();
        for (Sale s : sales) byRegion.merge(s.region(), s.amount(), Integer::sum);
        int total = byRegion.values().stream().mapToInt(Integer::intValue).sum();
        return new Summary(byRegion, total);
    }

    interface Renderer { String render(Summary s); }

    static final Map<String, Renderer> RENDERERS = Map.of(
        "text", s -> {
            StringBuilder b = new StringBuilder();
            s.byRegion().forEach((r, a) -> b.append(r).append(": ").append(a).append("\n"));
            return b.append("Total: ").append(s.total()).append("\n").toString();
        },
        "csv", s -> {
            StringBuilder b = new StringBuilder("region,amount\n");
            s.byRegion().forEach((r, a) -> b.append(r).append(",").append(a).append("\n"));
            return b.append("total,").append(s.total()).append("\n").toString();
        });

    static String report(List<Sale> sales, String format) {
        return RENDERERS.get(format).render(summarize(sales));
    }

    public static void main(String[] args) {
        List<Sale> sales = List.of(new Sale("north", 100), new Sale("south", 50), new Sale("north", 25));
        for (String f : List.of("text", "csv")) {
            if (!report(sales, f).equals(reportBefore(sales, f))) throw new AssertionError(f);
        }
        if (!report(sales, "text").equals("north: 125\nsouth: 50\nTotal: 175\n")) throw new AssertionError("text");
        if (summarize(sales).total() != 175) throw new AssertionError("total");
        System.out.println("report: tests pass");
    }
}
```

```output
report: tests pass
```

- `LinkedHashMap` keeps the order of insertion. A `HashMap` would give the regions in an order that the tests could not rely on.
- `Map.merge` with `Integer::sum` adds the amount to the value for the region, or stores it if the region is not there yet.
- The `Renderer` interface is the contract of phase 2. A class in another package can add a format without access to the code of phase 1.

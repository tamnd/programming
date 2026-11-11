```java run id=orders
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    record Order(String customer, int amount) {            // a value type
        static Order parse(String line) {                  // a pure function
            String[] p = line.split(",");
            return new Order(p[0].strip(), Integer.parseInt(p[1].strip()));
        }
    }

    sealed interface Report permits Plain, Csv {           // one interface, two formats
        String line(String name, int total);
    }
    record Plain() implements Report {
        public String line(String name, int total) { return name + " " + total; }
    }
    record Csv() implements Report {
        public String line(String name, int total) { return name + "," + total; }
    }

    public static void main(String[] args) {
        String text = "ana, 30\nbo, 70\nana, 25\ncy, 10\nbo, 5";
        Map<String, Integer> totals = new HashMap<>();
        for (String line : text.split("\n")) {             // an imperative loop
            Order o = Order.parse(line);
            totals.merge(o.customer(), o.amount(), Integer::sum);
        }
        List<Map.Entry<String, Integer>> big = totals.entrySet().stream()
            .filter(e -> e.getValue() > 50)                // a functional pipeline
            .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
            .toList();
        for (Report r : List.of(new Plain(), new Csv())) {
            for (var e : big) System.out.println(r.line(e.getKey(), e.getValue()));
        }
    }
}
```

```output for=orders
bo 75
ana 55
bo,75
ana,55
```

- Verdict: native. Java began as a class-based object-oriented language, and it has since gained lambdas, streams, records, sealed interfaces and pattern matching, which support a functional style with algebraic data types. Every function still lives in a class, so a pure function is a static method, as `Order.parse` is.

```java run id=static-methods
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    static String[] splitWords(String text) {
        return text.toLowerCase().split("\\s+");
    }

    static Map<String, Integer> countWords(String[] words) {
        Map<String, Integer> counts = new TreeMap<>();
        for (String w : words) {
            counts.merge(w, 1, Integer::sum);
        }
        return counts;
    }

    static List<Map.Entry<String, Integer>> top(Map<String, Integer> counts, int n) {
        List<Map.Entry<String, Integer>> entries = new ArrayList<>(counts.entrySet());
        entries.sort((a, b) -> b.getValue() - a.getValue());
        return entries.subList(0, n);
    }

    static void report(String text, int n) {
        for (var e : top(countWords(splitWords(text)), n)) {
            System.out.println(String.format("%-5s %d", e.getKey(), e.getValue()));
        }
    }

    public static void main(String[] args) {
        report("The cat and the hat and the bat", 2);
    }
}
```

```output for=static-methods
the   3
and   2
```

- Verdict: idiom. Java has no free functions. A class with only `static` methods acts as a module of procedures, as `java.lang.Math` does.

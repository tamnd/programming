```java run id=stream-pipeline
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    record Article(String title, int words, List<String> tags) {}

    static int calls = 0;

    static int score(Article a) {
        calls++;
        return a.words();
    }

    public static void main(String[] args) {
        List<Article> articles = List.of(
                new Article("Intro to NoSQL", 1200, List.of("nosql", "databases")),
                new Article("Event sourcing", 800, List.of("architecture")),
                new Article("Graph stores", 1500, List.of("nosql", "graphs")),
                new Article("Refactoring loops", 600, List.of("refactoring")),
                new Article("Key-value stores", 900, List.of("nosql")));

        // A stream pipeline: a source, intermediate operations and a terminal operation.
        List<Article> nosql = articles.stream().filter(a -> a.tags().contains("nosql")).toList();
        System.out.println(nosql.stream()
                .sorted(Comparator.comparingInt(Article::words).reversed())
                .limit(2)
                .map(Article::title)
                .toList());

        // map and reduce.
        System.out.println(nosql.stream().map(Article::words).reduce(0, Integer::sum));

        // flatMap and groupingBy: the number of articles for each tag.
        Map<String, Long> counts = articles.stream()
                .flatMap(a -> a.tags().stream())
                .collect(Collectors.groupingBy(t -> t, TreeMap::new, Collectors.counting()));
        System.out.println(counts);

        // A list between the steps makes the pipeline eager: every score is computed.
        List<Integer> all = articles.stream().map(Main::score).toList();
        List<Integer> eager = all.stream().filter(s -> s > 1000).limit(1).toList();
        System.out.println("eager " + eager + " " + calls);

        // One stream is lazy: each item goes through all the steps, and limit stops the work.
        calls = 0;
        List<Integer> lazy = articles.stream().map(Main::score).filter(s -> s > 1000).limit(1).toList();
        System.out.println("lazy " + lazy + " " + calls);
    }
}
```

```output
[Graph stores, Intro to NoSQL]
3600
{architecture=1, databases=1, graphs=1, nosql=3, refactoring=1}
eager [1200] 5
lazy [1200] 1
```

- Verdict: stdlib. A stream pipeline has a source, intermediate operations such as filter, sorted, map and flatMap, and a terminal operation such as toList, reduce or collect. Collectors.groupingBy with a TreeMap and Collectors.counting gives the counts in key order. A toList between map and filter makes the pipeline eager, so score runs 5 times. One stream is lazy, and limit(1) stops it after the first match.

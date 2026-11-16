```java run id=graph
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    public static void main(String[] args) {
        String[][] edges = {{"a", "b"}, {"a", "c"}, {"b", "c"}, {"c", "d"}, {"d", "a"}};

        Map<String, List<String>> graph = new LinkedHashMap<>();
        for (String[] edge : edges) {
            graph.computeIfAbsent(edge[0], k -> new ArrayList<>()).add(edge[1]);
            graph.computeIfAbsent(edge[1], k -> new ArrayList<>());
        }

        graph.forEach((node, neighbors) ->
                System.out.println(node + " -> " + String.join(" ", neighbors)));

        System.out.println("edge a->c: " + yesNo(hasEdge(graph, "a", "c")));
        System.out.println("edge c->a: " + yesNo(hasEdge(graph, "c", "a")));

        long inDegree = graph.values().stream()
                .flatMap(List::stream)
                .filter("c"::equals)
                .count();
        int edgeCount = graph.values().stream().mapToInt(List::size).sum();
        System.out.println("in-degree of c: " + inDegree);
        System.out.println("edges: " + edgeCount);
    }

    static boolean hasEdge(Map<String, List<String>> graph, String x, String y) {
        return graph.getOrDefault(x, List.of()).contains(y);
    }

    static String yesNo(boolean value) {
        return value ? "yes" : "no";
    }
}
```

```output for=graph
a -> b c
b -> c
c -> d
d -> a
edge a->c: yes
edge c->a: no
in-degree of c: 2
edges: 5
```

- Verdict: idiom. A `LinkedHashMap` keeps the nodes in insertion order, and `computeIfAbsent` creates each list once.
- For graph algorithms and weighted edges, a library such as JGraphT has graph types.

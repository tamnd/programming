```cpp run id=graph
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::pair<std::string, std::string>> edges{
        {"a", "b"}, {"a", "c"}, {"b", "c"}, {"c", "d"}, {"d", "a"}};

    std::map<std::string, std::vector<std::string>> graph;
    for (const auto& [source, target] : edges) {
        graph[source].push_back(target);
        graph[target];
    }

    for (const auto& [node, neighbors] : graph) {
        std::cout << node << " ->";
        for (const auto& n : neighbors) {
            std::cout << ' ' << n;
        }
        std::cout << '\n';
    }

    auto has_edge = [&graph](const std::string& x, const std::string& y) {
        auto it = graph.find(x);
        return it != graph.end() &&
               std::find(it->second.begin(), it->second.end(), y) != it->second.end();
    };
    std::cout << "edge a->c: " << (has_edge("a", "c") ? "yes" : "no") << '\n';
    std::cout << "edge c->a: " << (has_edge("c", "a") ? "yes" : "no") << '\n';

    long in_degree = 0;
    std::size_t edge_count = 0;
    for (const auto& entry : graph) {
        in_degree += std::count(entry.second.begin(), entry.second.end(), "c");
        edge_count += entry.second.size();
    }
    std::cout << "in-degree of c: " << in_degree << '\n';
    std::cout << "edges: " << edge_count << '\n';
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

- Verdict: idiom. A `std::map` from each node to a `std::vector` of neighbors is an adjacency list. The map keeps its keys sorted.
- `graph[target];` creates an empty list for a node with no outgoing edges.

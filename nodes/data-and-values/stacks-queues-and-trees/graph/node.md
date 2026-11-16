---
tag: AVZZ4W
kind: concept
slug: graph
name: Graph
aliases: [graph, graph data structure, network, adjacency list, adjacency matrix, directed graph, undirected graph, vertices and edges]
area: data-and-values/stacks-queues-and-trees
level: core
summary: >-
  A graph is a set of nodes joined by edges, with no root and no limit on links. Programs store it as an adjacency list, a map from each node to its neighbors, or as a matrix.
applies_to: core-languages
viz: optional
formal: none
see_also: [tree-concept, map-and-dictionary, set-concept, multimap, hash-table, dependency-resolution, recursion, queue-concept]
same_as:
  wikidata: Q2479726
  wikipedia: Graph_(abstract_data_type)
sources:
  - url: https://en.wikipedia.org/wiki/Graph_(abstract_data_type)
    title: Graph (abstract data type), Wikipedia
    locator: the introduction; Operations; Common data structures for graph representation
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/graphlib.html
    title: graphlib, Functionality to operate with graph-like structures, Python documentation
    locator: TopologicalSorter; CycleError
    accessed: 2026-11-16
  - url: https://networkx.org/documentation/stable/reference/introduction.html
    title: Introduction, NetworkX documentation
    locator: NetworkX Basics; Graphs
    accessed: 2026-11-16
  - url: https://jgrapht.org/
    title: JGraphT
    locator: the home page
    accessed: 2026-11-16
  - url: https://www.boost.org/doc/libs/1_89_0/libs/graph/doc/index.html
    title: The Boost Graph Library (BGL), Boost 1.89.0
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.rs/petgraph/latest/petgraph/
    title: Crate petgraph, docs.rs
    locator: the crate description
    accessed: 2026-11-16
---

## Idea

A graph is a set of nodes and a set of links between them. Wikipedia: "A graph data structure consists of a finite (and possibly mutable) set of vertices (also called nodes or points), together with a set of unordered pairs of these vertices for an undirected graph or a set of ordered pairs for a directed graph."[^1] A tree is a graph with strict rules. A general graph has no root, a node can have many parents, and the links can form cycles.

Package dependencies, road maps, social networks, links between web pages and calls between functions are graphs. This program stores which part of an app needs which other part. It is a map from each node to a set of its neighbors.

```python run id=idea
from graphlib import TopologicalSorter

# Each key needs the parts in its set.
needs = {
    "app": {"web", "db"},
    "web": {"auth"},
    "auth": {"db"},
    "db": set(),
}

print("app needs:", sorted(needs["app"]))
print("does web need db?", "yes" if "db" in needs["web"] else "no")

used_by = {name: set() for name in needs}
for name, parts in needs.items():
    for part in parts:
        used_by[part].add(name)
print("db is used by:", sorted(used_by["db"]))

print("build order:", list(TopologicalSorter(needs).static_order()))
```

```output for=idea
app needs: ['db', 'web']
does web need db? no
db is used by: ['app', 'auth']
build order: ['db', 'auth', 'web', 'app']
```

The map gives the parts that a node needs at once. To find the parts that need a node, the program builds a second map, here `used_by`.

## How it works

**Vertices and edges.** The nodes are also called vertices, and the links are edges. In a directed graph each edge has a direction, from one node to another. In an undirected graph an edge joins two nodes both ways. "A graph data structure may also associate to each edge some edge value, such as a symbolic label or a numeric attribute (cost, capacity, length, etc.)."[^1] A graph with such values is a weighted graph.

**Operations.** Wikipedia lists the basic operations.[^1] Among them:

- `adjacent(G, x, y)` "tests whether there is an edge from the vertex x to the vertex y";
- `neighbors(G, x)` "lists all vertices y such that there is an edge from the vertex x to the vertex y";
- `add_vertex`, `remove_vertex`, `add_edge` and `remove_edge`.

**Adjacency list.** "Vertices are stored as records or objects, and every vertex stores a list of adjacent vertices."[^1] In most languages this is a map from a node to a list or a set of nodes. A set makes the edge test fast: Wikipedia notes that hash tables for the neighbor sets give "an amortized average time complexity of O(1) to test adjacency of two given vertices".[^1]

**Adjacency matrix.** "A two-dimensional matrix, in which the rows represent source vertices and columns represent destination vertices."[^1] The nodes are numbered 0 to n minus 1, and cell `[i][j]` is true, or holds the weight, when there is an edge from i to j. "Only the cost for one edge can be stored between each pair of vertices."[^1]

**Which one.** "Adjacency lists are generally preferred for the representation of sparse graphs, while an adjacency matrix is preferred if the graph is dense".[^1] Road maps and dependency graphs have few edges per node, so an adjacency list suits them.

**Edge list.** A plain list of `(from, to)` pairs is the simplest form. It is a good format to read and write, and programs usually turn it into an adjacency list before they work on it.

The algorithms that walk and search graphs are on the algorithm site: [graph representation](https://algorithm.tamnd.com/graph-representation/), [breadth-first search](https://algorithm.tamnd.com/breadth-first-search/), [depth-first search](https://algorithm.tamnd.com/depth-first-search/) and [topological sort](https://algorithm.tamnd.com/topological-sort/).

## In languages

No core language has a general graph type in its standard library. A program builds an adjacency list from a map and lists or sets, or it uses a graph library.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | a `dict` of `set` or `list`; `graphlib.TopologicalSorter`[^2]; the NetworkX library[^3] |
| JavaScript | idiom | a `Map` from node to an array or a `Set` |
| TypeScript | idiom | as JavaScript, with `Map<string, Set<string>>` |
| Java | idiom | a `Map<V, List<V>>`; the JGraphT library[^4] |
| C | idiom | an adjacency matrix of numbered nodes, or arrays of neighbor indexes |
| C++ | idiom | a `std::vector<std::vector<int>>` or a map of vectors; the Boost Graph Library[^5] |
| C# | idiom | a `Dictionary<TKey, List<TKey>>` |
| Go | idiom | a `map[string][]string` |
| Rust | idiom | a `HashMap` or `BTreeMap` of `Vec`; the petgraph crate[^6] |
| Kotlin | idiom | a `Map` of `List`, as in Java |
| PowerShell | idiom | a hashtable of arrays |
| Bash | idiom | an associative array from node to a string of neighbor names |
| SQL | idiom | an edge table with two node columns, and a node table |
| Regex | n/a | a pattern has no data structures |
| HTML/CSS | n/a | markup and style rules have no data structures |

**Python.** The `graphlib` module has one class. `TopologicalSorter` "Provides functionality to topologically sort a graph of hashable nodes."[^2] Its input is a dictionary "where the keys are nodes and the values are iterables of all predecessors of that node in the graph".[^2] NetworkX is a library with graph types. Its documentation says that "The graph internal data structures are based on an adjacency list representation and implemented using Python dictionary datastructures."[^3]

**Java.** JGraphT is "a Java library of graph theory data structures and algorithms".[^4] In it, "any object can be used for vertex and edge types, with full type safety via generics".[^4]

**C++.** The Boost Graph Library defines "a generic interface that allows access to a graph's structure, but hides the details of the implementation."[^5]

**Rust.** "petgraph is a graph data structure library."[^6] "Both nodes and edges can have arbitrary associated data, and edges may be either directed or undirected."[^6]

The example files build the same directed graph of four nodes and five edges. They print the neighbors, test two edges and count the edges into one node.

## Variations and related ideas

- **Directed acyclic graph.** A directed graph with no cycles. Build systems and package managers use one for dependencies. A topological order exists only for such a graph: "A complete topological ordering is possible if and only if the graph has no directed cycles".[^2] See [dependency-resolution](/dependency-resolution/).
- **Multigraph.** More than one edge can join the same two nodes. NetworkX has `MultiGraph`, "A flexible graph class that allows multiple undirected edges between pairs of nodes."[^3] A [multimap](/multimap/) can store one.
- **Tree.** A connected graph where each node except the root has one parent. See [tree-concept](/tree-concept/).
- **Implicit graph.** The edges are computed, not stored. The squares of a game board and the states of a puzzle are graphs of this kind.
- **Graph databases.** Some databases store nodes and edges as their main model and query paths directly.

## Pitfalls

### An undirected edge stored in one direction

In an adjacency list, an undirected edge must appear in the lists of both nodes. If the program adds it only once, the graph becomes directed by mistake.

```python wrong id=python-one-way
friends = {"ana": set(), "ben": set(), "cho": set()}


def add_friend(a, b):
    friends[a].add(b)


add_friend("ana", "ben")
print("ben in ana's friends:", "yes" if "ben" in friends["ana"] else "no")
print("ana in ben's friends:", "yes" if "ana" in friends["ben"] else "no")
```

```output for=python-one-way
ben in ana's friends: yes
ana in ben's friends: no
```

Add both directions in one function, so no caller can forget the second one:

```python run id=python-both-ways
friends = {"ana": set(), "ben": set(), "cho": set()}


def add_friend(a, b):
    friends[a].add(b)
    friends[b].add(a)


add_friend("ana", "ben")
print("ana in ben's friends:", "yes" if "ana" in friends["ben"] else "no")
```

```output for=python-both-ways
ana in ben's friends: yes
```

### Walking a graph as if it were a tree

A recursive walk over the children of a tree ends at the leaves. In a graph with a cycle, the same walk never ends, because it comes back to a node it has already seen.

```python wrong id=python-cycle
links = {"home": ["about"], "about": ["contact"], "contact": ["home"]}


def count_pages(page):
    return 1 + sum(count_pages(nxt) for nxt in links[page])


try:
    print(count_pages("home"))
except RecursionError:
    print("RecursionError")


def count_pages_seen(start):
    seen = set()
    stack = [start]
    while stack:
        page = stack.pop()
        if page not in seen:
            seen.add(page)
            stack.extend(links[page])
    return len(seen)


print("pages:", count_pages_seen("home"))
```

```output for=python-cycle
RecursionError
pages: 3
```

Keep a set of visited nodes. Without it, a graph that has no cycle but has shared nodes is still slow, because the walk visits a shared node once for each path to it.

### A matrix for a sparse graph

An adjacency matrix has one cell for each pair of nodes, whether or not an edge exists. Wikipedia gives the storage of a matrix as the square of the number of vertices, and the storage of an adjacency list as the number of vertices plus the number of edges.[^1] A road map with a million junctions and a few roads at each junction needs a million million cells as a matrix, but only a few million entries as an adjacency list.

## Cost

Wikipedia gives these costs, with V vertices and E edges.[^1]

| Operation | Adjacency list | Adjacency matrix |
|---|---|---|
| store the graph | V + E | V squared |
| add a vertex | constant | V squared |
| add an edge | constant | constant |
| remove a vertex | E | V squared |
| remove an edge | V | constant |
| test if x and y are adjacent | V | constant |

With hash sets for the neighbors, the edge test and the edge removal become constant on average.[^1]

## History

The Python `graphlib` documentation marks the module's class with "Added in version 3.9."[^2]

## References

---
tag: T7XMZM
kind: concept
slug: tree-concept
name: Tree
aliases: [tree, tree data structure, rooted tree, node tree, parent and child nodes, hierarchy]
area: data-and-values/stacks-queues-and-trees
level: intro
summary: >-
  A tree is a set of nodes where each node has one parent, except the root, and any number of children. File systems, the DOM, syntax trees and menus are trees.
applies_to: core-languages
viz: optional
formal: none
see_also: [recursion, abstract-syntax-tree, recursive-type, pointers-and-references, ordered-map, list-files-in-a-directory, json, class-inheritance]
same_as:
  wikidata: Q223655
  wikipedia: Tree_(abstract_data_type)
sources:
  - url: https://en.wikipedia.org/wiki/Tree_(abstract_data_type)
    title: Tree (abstract data type), Wikipedia
    locator: the introduction; Terminology; Traversal and search methods; Representations; Applications
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction
    title: Introduction to the DOM, MDN Web Docs
    locator: What is the DOM?
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/ast.html
    title: ast, Abstract syntax trees, Python documentation
    locator: the introduction; ast.walk
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Node
    title: Node, MDN Web Docs
    locator: Instance properties
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/pathlib.html
    title: pathlib, Object-oriented filesystem paths, Python documentation
    locator: Path.walk
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/TreeMap.html
    title: TreeMap (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/index.html
    title: Module std::collections, The Rust Standard Library
    locator: the module description
    accessed: 2026-11-16
  - url: https://docs.python.org/3/tutorial/controlflow.html
    title: More Control Flow Tools, The Python Tutorial
    locator: Default Argument Values
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions, Python documentation
    locator: sys.getrecursionlimit
    accessed: 2026-11-16
---

## Idea

A tree stores items in a hierarchy. Each item is a node. A node can have children, and each child has exactly one parent. The one node with no parent is the root. Wikipedia: "Each node in the tree can be connected to many children (depending on the type of tree), but must be connected to exactly one parent".[^1]

A folder of files is a tree. This program builds one from nodes that hold a list of children, prints it, and adds up the sizes.

```python run id=idea
from dataclasses import dataclass, field


@dataclass
class Node:
    name: str
    size: int = 0
    children: list = field(default_factory=list)


def show(node, depth=0):
    label = node.name + "/" if node.children else node.name
    print("  " * depth + label)
    for child in node.children:
        show(child, depth + 1)


def total_size(node):
    return node.size + sum(total_size(child) for child in node.children)


root = Node("project", children=[
    Node("README.md", 120),
    Node("src", children=[Node("main.py", 300), Node("util.py", 80)]),
    Node("tests", children=[Node("test_main.py", 150)]),
])
show(root)
print("total size:", total_size(root))
```

```output for=idea
project/
  README.md
  src/
    main.py
    util.py
  tests/
    test_main.py
total size: 650
```

Both functions call themselves on each child. Each child is the root of a smaller tree, so recursion fits the shape.

## How it works

**Words.** The root is the top node. A node with no children is a leaf. The depth of a node is the length of the path from the root to it. Wikipedia: "The height of a node is the length of the longest downward path to a leaf from that node."[^1] Nodes with the same parent are siblings. Each node and all the nodes below it form a subtree.[^1]

**No cycles.** Because each node has one parent, no node can be below itself. Wikipedia: "These constraints mean there are no cycles". Each child is also the root of its own subtree, "making recursion a useful technique for tree traversal."[^1]

**How programs store a tree.**

- A node object holds its data and a list of its children. This is the most common form. Wikipedia: "In working memory, nodes are typically dynamically allocated records with pointers to their children, their parents, or both".[^1]
- A node holds a pointer to its first child and a pointer to its next sibling. Each node then has the same fixed size.
- A table holds one row per node, with the id of the parent. "In relational databases, nodes are typically represented as table rows, with indexed row IDs facilitating pointers between parents and children."[^1]
- An array holds the nodes at fixed positions. A binary heap stores a complete binary tree like this.[^1] See [priority-queue](/priority-queue/).

A parent pointer makes it fast to go up from a node to the root. Without it, a program must search from the root to find the parent of a node.

**Walks.** A program visits every node with a walk.[^1]

- Pre-order: "each parent node is traversed before its children".[^1] The Idea example prints in pre-order.
- Post-order: "the children are traversed before their respective parents".[^1] The size total needs the children first.
- Level-order: the root, then its children, then their children. This walk uses a queue. See [queue-concept](/queue-concept/).

The walk algorithms and their proofs are on the algorithm site: [tree traversal](https://algorithm.tamnd.com/tree-traversal/), [binary search tree](https://algorithm.tamnd.com/binary-search-tree/).

**Where trees appear.** Wikipedia lists file systems, class hierarchies, abstract syntax trees, and the DOM trees of XML and HTML documents.[^1] MDN: "The DOM represents a document with a logical tree."[^2]

## In languages

No core language has a general tree type in its standard library. A program declares a node type with a list of children. Some libraries give trees for one purpose.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | a class with a `children` list; `ast` for syntax trees[^3] |
| JavaScript | idiom | objects with a `children` array; the DOM `Node` in browsers[^4] |
| TypeScript | idiom | a recursive interface with `children: Node[]` |
| Java | idiom | a class with a `List` of children |
| C | idiom | a struct with child pointers, or first-child and next-sibling pointers |
| C++ | idiom | a struct with a `std::vector` of children |
| C# | idiom | a class with a `List<T>` of children |
| Go | idiom | a struct with a slice of child pointers |
| Rust | idiom | a struct with a `Vec` of children |
| Kotlin | idiom | a data class with a `List` of children |
| PowerShell | idiom | nested hashtables or custom objects |
| Bash | n/a | no nested data; the file system is a tree that `find` walks |
| SQL | idiom | a table with a parent id column[^1] |
| Regex | n/a | a pattern has no data structures |
| HTML/CSS | n/a | nested elements form the DOM tree, which scripts walk[^2] |

**Python.** "The ast module helps Python applications to process trees of the Python abstract syntax grammar."[^3] `ast.walk` will "Recursively yield all descendant nodes in the tree starting at node (including node itself), in no specified order."[^3] For folders, `Path.walk` will "Generate the file names in a directory tree by walking the tree either top-down or bottom-up."[^5]

**JavaScript.** In a browser, each DOM node has `childNodes`, which "Returns a live NodeList containing all the children of this node".[^4] `parentNode` "Returns a Node that is the parent of this node."[^4]

**Java and Rust.** `TreeMap` is "A Red-Black tree based NavigableMap implementation."[^6] Rust has `BTreeMap` and `BTreeSet` among its maps and sets.[^7] These types use a tree inside, but they are sorted maps and sets. A program cannot see or walk their nodes. See [ordered-map](/ordered-map/).

The example files build the same folder tree in each language. They print it, count the files, add up the sizes and find the height.

## Variations and related ideas

- **Binary tree.** Each node has at most two children, left and right. "Binary trees are a commonly used type, which constrain the number of children for each parent to at most two."[^1]
- **Ordered and unordered children.** In a file tree, the order of the children often does not matter. In a syntax tree, it does: the two operands of `a - b` must not swap. Wikipedia: "Typically siblings have an order, with the first one conventionally drawn on the left."[^1]
- **Search trees.** A binary search tree, a red-black tree and a B-tree keep keys in order for fast lookup. They are algorithms, and they live on the algorithm site.
- **Forest.** "A set of one or more disjoint trees."[^1]
- **Recursive types.** A tree type refers to itself: a node holds nodes. See [recursive-type](/recursive-type/).
- **Graph.** When a node can have more than one parent, or the links can form a cycle, the structure is a graph. See [graph](/graph/).

## Pitfalls

### One node under two parents

If a program puts the same node object under two parents, the structure is no longer a tree. A change under one parent shows under the other, and a walk visits the node twice. Wikipedia notes that in file systems, "symbolic links create non-tree graphs, as do multiple hard links to the same file or directory".[^1]

```javascript wrong id=javascript-shared-node
const logo = { name: "logo.png", size: 50, children: [] };
const site = {
  name: "site",
  size: 0,
  children: [
    { name: "home", size: 0, children: [logo] },
    { name: "about", size: 0, children: [logo] },
  ],
};

logo.name = "logo-v2.png";
console.log(site.children.map((page) => page.children[0].name).join(", "));

const total = (node) => node.size + node.children.reduce((sum, child) => sum + total(child), 0);
console.log("total size:", total(site));
```

```output for=javascript-shared-node
logo-v2.png, logo-v2.png
total size: 100
```

The file has a size of 50, but the total counts it twice. Copy the node for the second parent, or treat the structure as a graph and keep a set of visited nodes.

### A shared default list in Python

In Python, a default argument value is created once. The tutorial warns: "The default value is evaluated only once. This makes a difference when the default is a mutable object such as a list".[^8] A node class with `children=[]` gives every node the same list.

```python wrong id=python-default-children
class Node:
    def __init__(self, name, children=[]):
        self.name = name
        self.children = children


root = Node("root")
a = Node("a")
root.children.append(a)
print("children of a:", [child.name for child in a.children])
print("same list:", "yes" if a.children is root.children else "no")
```

```output for=python-default-children
children of a: ['a']
same list: yes
```

The node `a` is now its own child. Use `None` as the default and create the list in the body, or use `field(default_factory=list)` as in the Idea example.

### Deep trees and recursion

A recursive walk uses one stack frame per level. A tree that is very deep, such as a long chain of nodes, can go over the limit. Python's recursion limit is "the maximum depth of the Python interpreter stack. This limit prevents infinite recursion from causing an overflow of the C stack and crashing Python."[^9]

```python wrong id=python-deep-tree
class Node:
    def __init__(self, children=None):
        self.children = children or []


def height(node):
    return max((1 + height(child) for child in node.children), default=0)


def height_with_stack(root):
    best = 0
    stack = [(root, 0)]
    while stack:
        node, depth = stack.pop()
        best = max(best, depth)
        stack.extend((child, depth + 1) for child in node.children)
    return best


node = Node()
for _ in range(5000):
    node = Node([node])

try:
    print("recursive height:", height(node))
except RecursionError:
    print("recursive height: RecursionError")
print("height with a stack:", height_with_stack(node))
```

```output for=python-deep-tree
recursive height: RecursionError
height with a stack: 5000
```

For trees that can be deep, such as parsed input from users, walk with an explicit stack. See [stack-data-structure](/stack-data-structure/).

## Cost

| Operation | Children lists | With parent pointers |
|---|---|---|
| visit every node | linear | linear |
| go to a child | constant | constant |
| go to the parent | linear (search from the root) | constant |
| path from a node to the root | linear | proportional to the depth |
| add a leaf under a known node | constant (amortized) | constant (amortized) |

A recursive walk also uses stack space in proportion to the height of the tree.

## History

The `Path.walk` method of Python's `pathlib` was "Added in version 3.12."[^5]

## References

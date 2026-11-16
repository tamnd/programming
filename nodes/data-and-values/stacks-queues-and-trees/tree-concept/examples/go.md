```go run id=tree
package main

import (
	"fmt"
	"strings"
)

type node struct {
	name     string
	size     int
	children []*node
}

func file(name string, size int) *node {
	return &node{name: name, size: size}
}

func dir(name string, children ...*node) *node {
	return &node{name: name, children: children}
}

func show(n *node, depth int) {
	indent := strings.Repeat("  ", depth)
	if len(n.children) == 0 {
		fmt.Printf("%s%s %d\n", indent, n.name, n.size)
		return
	}
	fmt.Printf("%s%s/\n", indent, n.name)
	for _, child := range n.children {
		show(child, depth+1)
	}
}

func countFiles(n *node) int {
	if len(n.children) == 0 {
		return 1
	}
	count := 0
	for _, child := range n.children {
		count += countFiles(child)
	}
	return count
}

func totalSize(n *node) int {
	total := n.size
	for _, child := range n.children {
		total += totalSize(child)
	}
	return total
}

func height(n *node) int {
	best := 0
	for _, child := range n.children {
		if h := 1 + height(child); h > best {
			best = h
		}
	}
	return best
}

func main() {
	root := dir("project",
		file("README.md", 120),
		dir("src", file("main.py", 300), file("util.py", 80)),
		dir("tests", file("test_main.py", 150)),
	)
	show(root, 0)
	fmt.Printf("files %d, total size %d, height %d\n", countFiles(root), totalSize(root), height(root))
}
```

```output for=tree
project/
  README.md 120
  src/
    main.py 300
    util.py 80
  tests/
    test_main.py 150
files 4, total size 650, height 2
```

- Verdict: idiom. A struct with a slice of `*node` pointers is a node.
- `show` prints the directories with a slash, and the files with their size.

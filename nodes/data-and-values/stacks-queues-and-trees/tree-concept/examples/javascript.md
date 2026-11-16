```javascript run id=tree
const file = (name, size) => ({ name, size, children: [] });
const dir = (name, children) => ({ name, size: 0, children });

function show(node, depth = 0) {
  const indent = "  ".repeat(depth);
  if (node.children.length > 0) {
    console.log(`${indent}${node.name}/`);
    for (const child of node.children) show(child, depth + 1);
  } else {
    console.log(`${indent}${node.name} ${node.size}`);
  }
}

const countFiles = (node) =>
  node.children.length === 0 ? 1 : node.children.reduce((sum, child) => sum + countFiles(child), 0);

const totalSize = (node) => node.size + node.children.reduce((sum, child) => sum + totalSize(child), 0);

const height = (node) => Math.max(0, ...node.children.map((child) => 1 + height(child)));

const root = dir("project", [
  file("README.md", 120),
  dir("src", [file("main.py", 300), file("util.py", 80)]),
  dir("tests", [file("test_main.py", 150)]),
]);
show(root);
console.log(`files ${countFiles(root)}, total size ${totalSize(root)}, height ${height(root)}`);
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

- Verdict: idiom. Plain objects with a `children` array form the tree.
- `Math.max(0, ...)` returns 0 for a leaf, because a leaf has no children to spread.

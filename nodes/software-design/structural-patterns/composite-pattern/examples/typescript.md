```typescript run id=tree
type Entry =
  | { readonly kind: "file"; readonly name: string; readonly size: number }
  | { readonly kind: "dir"; readonly name: string; readonly children: readonly Entry[] };

function size(entry: Entry): number {
  switch (entry.kind) {
    case "file":
      return entry.size;
    case "dir":
      return entry.children.reduce((total, child) => total + size(child), 0);
  }
}

function lines(entry: Entry, depth = 0): string[] {
  const indent = "  ".repeat(depth);
  switch (entry.kind) {
    case "file":
      return [`${indent}${entry.name} ${entry.size}`];
    case "dir":
      return [`${indent}${entry.name}/ ${size(entry)}`, ...entry.children.flatMap((child) => lines(child, depth + 1))];
  }
}

const file = (name: string, size: number): Entry => ({ kind: "file", name, size });
const dir = (name: string, ...children: Entry[]): Entry => ({ kind: "dir", name, children });

const project = dir("project", file("readme.md", 200), dir("src", file("main.go", 1200), file("util.go", 300)));
console.log(lines(project).join("\n"));
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. A discriminated union in place of a class hierarchy. The `kind` property tells the cases apart, and a `switch` on it narrows the type in each branch.
- Each operation is a function with one case for each kind of entry. With `strict` on, the compiler reports a function that does not return in every case. So a third kind of entry is a compile error in each function that does not handle it.
- This is the other side of the expression problem. A new operation is one new function. A new kind of entry changes every function (see the node).

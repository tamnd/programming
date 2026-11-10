```javascript run id=tree
class File {
  constructor(name, size) {
    this.name = name;
    this.bytes = size;
  }

  size() {
    return this.bytes;
  }

  lines(depth = 0) {
    return [`${"  ".repeat(depth)}${this.name} ${this.bytes}`];
  }
}

class Directory {
  constructor(name, ...children) {
    this.name = name;
    this.children = children;
  }

  size() {
    return this.children.reduce((total, child) => total + child.size(), 0);
  }

  lines(depth = 0) {
    return [`${"  ".repeat(depth)}${this.name}/ ${this.size()}`, ...this.children.flatMap((child) => child.lines(depth + 1))];
  }
}

const project = new Directory(
  "project",
  new File("readme.md", 200),
  new Directory("src", new File("main.go", 1200), new File("util.go", 300)),
);
console.log(project.lines().join("\n"));
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. Two classes with the same two methods. No base class is needed, because a call such as `child.size()` works on any object that has the method.
- Nothing declares the component interface. A child that lacks `lines` fails only when the program reaches it. The TypeScript file names the two cases in a type.
- `flatMap` joins the lines of all children into one array, so each level returns a flat list.

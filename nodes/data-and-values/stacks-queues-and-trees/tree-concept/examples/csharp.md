```csharp run id=tree
var root = Node.Dir("project",
    Node.File("README.md", 120),
    Node.Dir("src", Node.File("main.py", 300), Node.File("util.py", 80)),
    Node.Dir("tests", Node.File("test_main.py", 150)));

Show(root, 0);
Console.WriteLine("files " + CountFiles(root) + ", total size " + TotalSize(root) + ", height " + Height(root));

static void Show(Node node, int depth)
{
    string indent = new string(' ', depth * 2);
    if (node.Children.Count == 0)
    {
        Console.WriteLine(indent + node.Name + " " + node.Size);
        return;
    }
    Console.WriteLine(indent + node.Name + "/");
    foreach (Node child in node.Children)
    {
        Show(child, depth + 1);
    }
}

static int CountFiles(Node node) =>
    node.Children.Count == 0 ? 1 : node.Children.Sum(child => CountFiles(child));

static int TotalSize(Node node) => node.Size + node.Children.Sum(child => TotalSize(child));

static int Height(Node node) =>
    node.Children.Count == 0 ? 0 : node.Children.Max(child => 1 + Height(child));

record Node(string Name, int Size, List<Node> Children)
{
    public static Node File(string name, int size) => new(name, size, new List<Node>());

    public static Node Dir(string name, params Node[] children) => new(name, 0, new List<Node>(children));
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

- Verdict: idiom. A record with a `List<Node>` of children is a node.
- LINQ `Sum` and `Max` combine the results of the children. `Height` checks for a leaf first, because a leaf has no children to compare.

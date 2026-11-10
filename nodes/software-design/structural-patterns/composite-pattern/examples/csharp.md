```csharp run id=tree
using System.Text;

Entry project = new Directory("project",
    new File("readme.md", 200),
    new Directory("src", new File("main.go", 1200), new File("util.go", 300)));
var output = new StringBuilder();
project.Write(output, 0);
Console.Write(output);

abstract class Entry(string name)
{
    public string Name { get; } = name;
    public abstract long Size();
    public abstract void Write(StringBuilder output, int depth);
}

sealed class File(string name, long size) : Entry(name)
{
    public override long Size() => size;

    public override void Write(StringBuilder output, int depth) =>
        output.AppendLine($"{new string(' ', 2 * depth)}{Name} {size}");
}

sealed class Directory(string name, params Entry[] children) : Entry(name)
{
    private readonly List<Entry> _children = [.. children];

    public override long Size() => _children.Sum(child => child.Size());

    public override void Write(StringBuilder output, int depth)
    {
        output.AppendLine($"{new string(' ', 2 * depth)}{Name}/ {Size()}");
        foreach (var child in _children)
        {
            child.Write(output, depth + 1);
        }
    }
}
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. The classic form, with an abstract class for the component and primary constructors for the data.
- Each operation is an abstract method, so a new kind of entry is one class. A new operation changes every class. This is the side of the expression problem that object-oriented code favours.
- `File` here is the class of this program, not `System.IO.File`. Name lookup finds the types that the program declares before the namespaces of the implicit `using` directives, so the program compiles with no clash.

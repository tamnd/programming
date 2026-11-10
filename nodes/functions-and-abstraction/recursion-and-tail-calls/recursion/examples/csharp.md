```csharp run id=tree
static int Total(Tree tree) => tree switch
{
    Leaf leaf => leaf.Value,
    Branch branch => Total(branch.Left) + Total(branch.Right),
    _ => throw new ArgumentException("unknown tree"),
};

static int Depth(int n) => n == 0 ? 0 : 1 + Depth(n - 1);

Console.WriteLine(Total(new Branch(new Leaf(1), new Branch(new Leaf(2), new Leaf(3)))));
Console.WriteLine(Depth(1000));

abstract record Tree;
record Leaf(int Value) : Tree;
record Branch(Tree Left, Tree Right) : Tree;
```

```output for=tree
6
1000
```

- Verdict: native. A method or a local function can call itself.
- A stack overflow in .NET is not an exception that the program can handle. A `try`/`catch` block cannot catch `StackOverflowException`, and the runtime ends the process.
- For this reason, code that recurses on input of unknown depth should check the depth itself, or use a loop with an explicit `Stack<T>`.

```csharp fails expect=runtime id=overflow
static int Depth(int n) => n == 0 ? 0 : 1 + Depth(n - 1);

try
{
    Console.WriteLine(Depth(100_000_000));
}
catch (Exception error)
{
    Console.WriteLine("never printed: " + error.GetType().Name);
}
```

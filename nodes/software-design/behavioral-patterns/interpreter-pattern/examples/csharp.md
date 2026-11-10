```csharp run id=expr
var e = new Mul(new Var("x"), new Add(new Var("y"), new Num(2)));
Console.WriteLine($"expression: {e.Show()}");
Report(e, new() { ["x"] = 3, ["y"] = 4 });
Report(e, new() { ["x"] = 5, ["y"] = 1 });

var f = new Add(new Var("z"), new Num(1));
Console.WriteLine($"expression: {f.Show()}");
Report(f, new() { ["x"] = 3, ["y"] = 4 });

static void Report(Expr e, Dictionary<string, int> env)
{
    var label = string.Join(" ", env.Select(kv => $"{kv.Key}={kv.Value}"));
    try
    {
        Console.WriteLine($"{label}: {e.Evaluate(env)}");
    }
    catch (UnknownVariableException err)
    {
        Console.WriteLine($"{label}: error: {err.Message}");
    }
}

// AbstractExpression: each node evaluates and shows itself.
abstract record Expr
{
    public abstract int Evaluate(IReadOnlyDictionary<string, int> env);
    public abstract string Show();

    // A sum inside a product needs parentheses.
    protected static string Wrap(Expr e) => e is Add ? $"({e.Show()})" : e.Show();
}

record Num(int Value) : Expr
{
    public override int Evaluate(IReadOnlyDictionary<string, int> env) => Value;
    public override string Show() => Value.ToString();
}

record Var(string Name) : Expr
{
    public override int Evaluate(IReadOnlyDictionary<string, int> env) =>
        env.TryGetValue(Name, out var value) ? value : throw new UnknownVariableException(Name);
    public override string Show() => Name;
}

record Add(Expr Left, Expr Right) : Expr
{
    public override int Evaluate(IReadOnlyDictionary<string, int> env) =>
        Left.Evaluate(env) + Right.Evaluate(env);
    public override string Show() => $"{Left.Show()} + {Right.Show()}";
}

record Mul(Expr Left, Expr Right) : Expr
{
    public override int Evaluate(IReadOnlyDictionary<string, int> env) =>
        Left.Evaluate(env) * Right.Evaluate(env);
    public override string Show() => $"{Wrap(Left)} * {Wrap(Right)}";
}

class UnknownVariableException(string name) : Exception($"unknown variable {name}");
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. This is the classic form of the pattern: an abstract class with one abstract method for each operation, and one subclass for each rule.
- Records give the constructor, the properties and value equality, so two trees with the same shape compare equal with `==`.
- A `throw` expression lets `Var.Evaluate` stay one expression.
- C# also has switch expressions with type patterns, which give the function-per-operation form of the TypeScript, Java and Rust examples. Without a sealed hierarchy, the compiler cannot check that such a switch covers every node type.

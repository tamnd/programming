```csharp run id=expr
Expr tree = new Mul(new Add(new Num(2), new Num(3)), new Num(4));
Console.WriteLine($"show: {Ops.Show(tree)}");
Console.WriteLine($"evaluate: {Ops.Evaluate(tree)}");
Console.WriteLine($"nodes: {Ops.Count(tree)}");

abstract record Expr;
sealed record Num(int Value) : Expr;
sealed record Add(Expr Left, Expr Right) : Expr;
sealed record Mul(Expr Left, Expr Right) : Expr;

static class Ops
{
    // Each operation is one switch expression with an arm for each kind of node.
    public static string Show(Expr e) => e switch
    {
        Num(var v) => v.ToString(),
        Add(var l, var r) => $"({Show(l)} + {Show(r)})",
        Mul(var l, var r) => $"({Show(l)} * {Show(r)})",
        _ => throw new ArgumentException($"unknown node {e}"),
    };

    public static int Evaluate(Expr e) => e switch
    {
        Num(var v) => v,
        Add(var l, var r) => Evaluate(l) + Evaluate(r),
        Mul(var l, var r) => Evaluate(l) * Evaluate(r),
        _ => throw new ArgumentException($"unknown node {e}"),
    };

    public static int Count(Expr e) => e switch
    {
        Num => 1,
        Add(var l, var r) => 1 + Count(l) + Count(r),
        Mul(var l, var r) => 1 + Count(l) + Count(r),
        _ => throw new ArgumentException($"unknown node {e}"),
    };
}
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

- Verdict: idiom. Records are the node types, and a `switch` expression with positional patterns gives each operation an arm for each type. A positional record has a `Deconstruct` method, so `Add(var l, var r)` binds the children.
- `Expr` is an abstract record, so other code can add a subtype. The compiler cannot prove that three arms cover every subtype, so a switch without the `_` arm gets warning CS8509. With the `_` arm, a missing case fails at run time and not at compile time.
- The classic form with `Accept` and an `IVisitor<R>` interface works too. It is the form to choose when the compiler must report a missing operation, because each visitor class must implement every method of the interface.

```csharp run id=meaning-as-a-function
using System;
using System.Collections.Generic;

// Denotational style: the meaning of each expression is a function from an
// environment to a value, built once from the syntax tree.
static Func<Dictionary<string, int>, int> Meaning(Expr e) => e switch
{
    Num n => _ => n.Value,
    Var v => env => env[v.Name],
    Add a => Combine(Meaning(a.Left), Meaning(a.Right), (x, y) => x + y),
    Mul m => Combine(Meaning(m.Left), Meaning(m.Right), (x, y) => x * y),
    _ => throw new ArgumentException("unknown expression"),
};

static Func<Dictionary<string, int>, int> Combine(
    Func<Dictionary<string, int>, int> f, Func<Dictionary<string, int>, int> g, Func<int, int, int> op)
    => env => op(f(env), g(env));

// 2 * x + y, as a tree
Expr tree = new Add(new Mul(new Num(2), new Var("x")), new Var("y"));
var meaning = Meaning(tree);
foreach (var (x, y) in new[] { (1, 1), (5, 0), (10, -3) })
{
    Console.WriteLine($"x={x,-3} y={y,-3} 2 * x + y = {meaning(new() { ["x"] = x, ["y"] = y })}");
}

abstract record Expr;
record Num(int Value) : Expr;
record Var(string Name) : Expr;
record Add(Expr Left, Expr Right) : Expr;
record Mul(Expr Left, Expr Right) : Expr;
```

```output for=meaning-as-a-function
x=1   y=1   2 * x + y = 3
x=5   y=0   2 * x + y = 10
x=10  y=-3  2 * x + y = 17
```

- Verdict: absent. The C# specification, standardized as ECMA-334, is written in English. The example follows the denotational method: `Meaning` maps each node of the syntax tree to a function from an environment to a value, and the meaning of a compound expression is built only from the meanings of its parts. The tree is turned into a function once, and the function then runs on each environment.

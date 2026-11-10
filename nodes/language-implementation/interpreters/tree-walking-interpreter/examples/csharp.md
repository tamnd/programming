```csharp run id=interpreter
using System;
using System.Collections.Generic;

Stmt program = new Block([
    new Let("n", new Num(5)),
    new Let("acc", new Num(1)),
    new While(new Bin('<', new Num(0), new Var("n")), new Block([
        new Let("step", new Var("n")),
        new Assign("acc", new Bin('*', new Var("acc"), new Var("step"))),
        new Assign("n", new Bin('-', new Var("n"), new Num(1))),
    ])),
    new Print(new Var("acc")),
]);

var interp = new Interpreter();
interp.Execute(program, new Env(null));
Console.WriteLine($"nodes visited: {interp.Nodes}");
Console.WriteLine($"variable lookups: {interp.Lookups}, parent links followed: {interp.Links}");

abstract record Expr;
record Num(long Value) : Expr;
record Var(string Name) : Expr;
record Bin(char Op, Expr Left, Expr Right) : Expr;

abstract record Stmt;
record Let(string Name, Expr Init) : Stmt;
record Assign(string Name, Expr Value) : Stmt;
record Block(List<Stmt> Body) : Stmt;
record While(Expr Cond, Stmt Body) : Stmt;
record Print(Expr Value) : Stmt;

class Env(Env? parent)
{
    public Dictionary<string, long> Vars { get; } = new();
    public Env? Parent { get; } = parent;
}

class Interpreter
{
    public int Nodes, Lookups, Links;

    Env Find(Env env, string name)
    {
        Lookups++;
        for (Env? e = env; e != null; e = e.Parent)
        {
            if (e.Vars.ContainsKey(name)) return e;
            Links++;
        }
        throw new InvalidOperationException($"undefined variable '{name}'");
    }

    public long Evaluate(Expr node, Env env)
    {
        Nodes++;
        switch (node)
        {
            case Num(var value):
                return value;
            case Var(var name):
                return Find(env, name).Vars[name];
            case Bin(var op, var left, var right):
                long a = Evaluate(left, env);
                long b = Evaluate(right, env);
                return op switch
                {
                    '+' => a + b,
                    '-' => a - b,
                    '*' => a * b,
                    '<' => a < b ? 1 : 0,
                    _ => throw new InvalidOperationException($"unknown operator {op}"),
                };
            default:
                throw new InvalidOperationException($"unknown expression {node}");
        }
    }

    public void Execute(Stmt node, Env env)
    {
        Nodes++;
        switch (node)
        {
            case Let(var name, var init):
                env.Vars[name] = Evaluate(init, env);
                break;
            case Assign(var name, var value):
                long result = Evaluate(value, env);
                Find(env, name).Vars[name] = result;
                break;
            case Block(var body):
                var inner = new Env(env);
                foreach (var stmt in body) Execute(stmt, inner);
                break;
            case While(var cond, var body):
                while (Evaluate(cond, env) != 0) Execute(body, env);
                break;
            case Print(var value):
                Console.WriteLine(Evaluate(value, env));
                break;
            default:
                throw new InvalidOperationException($"unknown statement {node}");
        }
    }
}
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. Positional records model the nodes, and positional patterns such as `Bin(var op, var left, var right)` take them apart in a `switch`.
- C# has no closed hierarchy for records, so the compiler cannot prove that the cases are complete. Each `switch` ends with a `default` that throws.
- `Env` uses a primary constructor. The `Dictionary` of each environment becomes garbage when its block ends.

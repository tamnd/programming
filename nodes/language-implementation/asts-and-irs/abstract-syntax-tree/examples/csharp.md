```csharp run id=parse-walk
using System;
using System.Collections.Generic;
using System.Linq.Expressions;

Expression<Func<int, int>> lambda = x => 1 + 2 * (3 - x);

Console.WriteLine(lambda.Body);
var printer = new Printer();
printer.Visit(lambda.Body);
Console.WriteLine($"nodes: {printer.Count}");
Console.WriteLine($"names: {string.Join(", ", printer.Names)}");
Func<int, int> f = lambda.Compile();
Console.WriteLine($"value at x = 1: {f(1)}");

class Printer : ExpressionVisitor
{
    int depth;
    public int Count;
    public List<string> Names = new();

    public override Expression? Visit(Expression? node)
    {
        if (node is null) return null;
        Count++;
        string label = node switch
        {
            ConstantExpression c => $"Constant {c.Value}",
            ParameterExpression p => $"Parameter {p.Name}",
            _ => node.NodeType.ToString(),
        };
        Console.WriteLine(new string(' ', 2 * depth) + label);
        if (node is ParameterExpression pe) Names.Add(pe.Name!);
        depth++;
        var result = base.Visit(node);
        depth--;
        return result;
    }
}
```

```output for=parse-walk
(1 + (2 * (3 - x)))
Add
  Constant 1
  Multiply
    Constant 2
    Subtract
      Constant 3
      Parameter x
nodes: 7
names: x
value at x = 1: 5
```

- Verdict: stdlib. The C# compiler builds an expression tree when a lambda is assigned to `Expression<TDelegate>`. The program parses nothing at run time; the parser is the compiler.
- `ExpressionVisitor` calls `Visit` for each node. The override prints the node and then lets the base class visit the children.
- `ToString` prints the body with full parentheses, and `Compile` turns the tree into a delegate that runs.
- Expression trees cover expressions only. A statement lambda cannot be converted, so a full language needs its own AST, as in the Java example.

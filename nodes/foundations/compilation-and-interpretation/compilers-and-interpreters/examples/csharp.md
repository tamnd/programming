```csharp run id=expression-trees
using System;
using System.Linq.Expressions;

var x = Expression.Parameter(typeof(int), "x");
var body = Expression.Add(Expression.Multiply(x, Expression.Constant(2)), Expression.Constant(1));
var tree = Expression.Lambda<Func<int, int>>(body, x); // code as a data structure
Console.WriteLine(tree);

Func<int, int> f = tree.Compile(); // compiled at run time to IL, then by the JIT to machine code
Console.WriteLine(f(20));

Expression<Func<int, int>> quoted = n => n * n; // the C# compiler builds the tree
Console.WriteLine($"{quoted.Body.NodeType} {quoted.Compile()(7)}");
```

```output for=expression-trees
x => ((x * 2) + 1)
41
Multiply 49
```

- Verdict: native. The C# compiler translates source to common intermediate language (CIL) in an assembly, and the runtime's JIT compiler translates each method to machine code the first time it runs. Ahead-of-time compilation to native code is also possible.
- An expression tree is code held as data. A program can build one from nodes, or the C# compiler builds one from a lambda assigned to `Expression<...>`. `Compile` turns the tree into CIL at run time, and the JIT then compiles that, so a formula known only at run time runs as fast as code written by hand.

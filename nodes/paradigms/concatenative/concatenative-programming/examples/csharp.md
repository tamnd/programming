```csharp run id=immutable-stack
using System;
using System.Collections.Immutable;
using System.Linq;

Word Push(int n) => s => s.Push(n);
Word Lift2(Func<int, int, int> f) => s =>
{
    s = s.Pop(out int b).Pop(out int a);
    return s.Push(f(a, b));
};
Word Seq(params Word[] words) => s => words.Aggregate(s, (acc, w) => w(acc));

Word dup = s => s.Push(s.Peek());
Word mul = Lift2((a, b) => a * b);
Word sub = Lift2((a, b) => a - b);
Word square = Seq(dup, mul);

var result = Seq(Push(7), square, Push(9), sub)(ImmutableStack<int>.Empty);
Console.WriteLine(string.Join(" ", result));

var before = ImmutableStack<int>.Empty.Push(1);
var after = Seq(Push(2), Push(3))(before);
Console.WriteLine($"{string.Join(" ", before)} | {string.Join(" ", after)}");

delegate ImmutableStack<int> Word(ImmutableStack<int> s);
```

```output for=immutable-stack
40
1 | 3 2 1
```

- Verdict: absent. C# applies methods to arguments. With `ImmutableStack<T>`, each word is a pure function that returns a new stack and leaves the old one as it was, so `before` still holds only 1 after the program ran on it. The stack lists its top item first. The .NET runtime runs CIL, the bytecode of a stack machine.

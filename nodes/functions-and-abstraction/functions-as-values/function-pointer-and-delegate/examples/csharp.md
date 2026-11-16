```csharp run id=function-pointer-and-delegate-csharp
using System;

delegate int BinOp(int a, int b);

class Program
{
    static int Add(int a, int b)
    {
        return a + b;
    }

    static int Mul(int a, int b)
    {
        return a * b;
    }

    static int Apply(Func<int, int, int> f, int a, int b)
    {
        return f(a, b);
    }

    static void Main()
    {
        BinOp op = Add;
        Console.WriteLine(op(6, 7));
        op = Mul;
        Console.WriteLine(op(6, 7));
        Console.WriteLine(Apply((a, b) => a - b, 6, 7));
    }
}
```

```output for=function-pointer-and-delegate-csharp
13
42
-1
```

- Verdict: native. `BinOp` is a delegate type. The variable `op` holds the method `Add`, then `Mul`. `Apply` takes the library delegate type `Func<int, int, int>`.

```csharp run id=default-and-out
using System;

int[] counts = new int[3];
string[] names = new string[2];
Console.WriteLine($"{counts[0]} {names[0] == null}");
Console.WriteLine($"{default(int)} {default(bool)} {default(double)}");

int number;
if (int.TryParse("42", out number))
{
    Console.WriteLine(number);
}
```

```output for=default-and-out
0 True
0 False 0
42
```

- Verdict: native. Array elements and fields are initially assigned with the default value of their type, which `default(T)` also gives. A local variable is initially unassigned, and the compiler rejects a read before an assignment on every path. An `out` argument counts as an assignment, so `number` has a value after the call.

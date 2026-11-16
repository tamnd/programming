```csharp run id=object-and-boxing
using System;
using System.Collections.Generic;

var values = new List<object> { 42, "hi", 3.5, new[] { 1, 2 }, DateTime.UnixEpoch };
foreach (object v in values)
{
    Console.WriteLine(v switch
    {
        int n => $"int {n + 1}",
        string s => $"string {s.ToUpper()}",
        double d => $"double {d * 2}",
        int[] a => $"int[] {a.Length}",
        _ => v.GetType().Name,
    });
}

// Boxing copies the value, so a change to i does not change the boxed copy.
int i = 1;
object boxed = i;
i = 2;
Console.WriteLine($"{i} {boxed} {typeof(int).BaseType} {typeof(ValueType).BaseType}");
try
{
    string str = (string)boxed;
    Console.WriteLine(str);
}
catch (InvalidCastException)
{
    Console.WriteLine("InvalidCastException");
}
```

```output for=object-and-boxing
int 43
string HI
double 7
int[] 2
DateTime
2 1 System.ValueType System.Object
InvalidCastException
```

- Verdict: native. `object` is `System.Object`, and every type, value types included, inherits from it, as the base types of `int` and `ValueType` show. A `switch` expression with type patterns tests the type of each value. When `i` becomes an `object`, it is boxed: the box holds a copy, so it keeps the value 1 after `i` changes. A cast to the wrong type throws `InvalidCastException`.

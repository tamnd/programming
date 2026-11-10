```csharp run id=inspect
using System;
using System.Reflection;

object p = new Point(3, 4);
Type t = p.GetType();
Console.WriteLine($"type: {t.Name}");
foreach (PropertyInfo prop in t.GetProperties())
{
    Console.WriteLine($"{prop.Name} : {prop.PropertyType.Name} = {prop.GetValue(p)}");
}
MethodInfo sum = t.GetMethod("Sum")!;
Console.WriteLine($"Sum() = {sum.Invoke(p, null)}");

record Point(int X, int Y)
{
    public int Sum() => X + Y;
}
```

```output for=inspect
type: Point
X : Int32 = 3
Y : Int32 = 4
Sum() = 7
```

`GetType` returns the run-time type of the value. `GetProperties` and `GetMethod` find its members by name, and `GetValue` and `Invoke` use them. The order of `GetProperties` is the metadata order of the assembly since .NET 7. Earlier versions gave no order.

```csharp fails expect=exception:NullReferenceException id=typo
using System;

object p = new Point(3, 4);
var sum = p.GetType().GetMethod("Summ");
Console.WriteLine(sum!.Invoke(p, null));

record Point(int X, int Y)
{
    public int Sum() => X + Y;
}
```

`GetMethod` returns `null` when no method has the name. The `!` tells the compiler not to warn about it, so the typo shows up only as an exception at run time.

```csharp run id=wrapped
using System;
using System.Reflection;

var m = typeof(Parser).GetMethod("Parse")!;
try
{
    m.Invoke(null, new object[] { "x" });
}
catch (TargetInvocationException e)
{
    Console.WriteLine($"outer: {e.GetType().Name}");
    Console.WriteLine($"inner: {e.InnerException!.GetType().Name}");
}

static class Parser
{
    public static int Parse(string s) => int.Parse(s);
}
```

```output for=wrapped
outer: TargetInvocationException
inner: FormatException
```

An exception thrown by the method that `Invoke` calls arrives wrapped in `TargetInvocationException`. A `catch (FormatException)` around the call would not catch it.

- Verdict: native. `System.Type` and the namespace `System.Reflection` describe assemblies, types, fields, properties, methods and attributes at run time.
- Reflection and trimming do not mix well. The trimmer removes code that it cannot see used. A type loaded by a name that comes from input at run time cannot be analyzed, and the SDK reports a trim warning.
- Generics checked at compile time are the first fix that the .NET documentation gives for a trim warning.

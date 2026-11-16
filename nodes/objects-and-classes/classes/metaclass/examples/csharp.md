```csharp run id=type-object-reflection
using System;
using System.Linq;

Type t = typeof(Upper); // the object that describes the class at run time
Console.WriteLine($"{t.Name} {t.GetType().Name}");
var names = typeof(IPlugin).Assembly.GetTypes()
    .Where(x => x.IsClass && typeof(IPlugin).IsAssignableFrom(x))
    .Select(x => x.Name)
    .OrderBy(n => n);
Console.WriteLine(string.Join(" ", names)); // find the plugins by reflection
var p = (IPlugin)Activator.CreateInstance(t)!;
Console.WriteLine(p.Run("abc"));

interface IPlugin
{
    string Run(string text);
}

class Upper : IPlugin
{
    public string Run(string text) => text.ToUpperInvariant();
}

class Reverse : IPlugin
{
    public string Run(string text) => new string(text.Reverse().ToArray());
}
```

```output for=type-object-reflection
Upper RuntimeType
Reverse Upper
ABC
```

- Verdict: absent. `typeof(Upper)` gives the `Type` object of the class, and its own type is `RuntimeType`, a class that code cannot replace.
- Reflection finds the classes that implement `IPlugin` and makes one with `Activator.CreateInstance`. A source generator can do the same search at compile time and write the registry as code.

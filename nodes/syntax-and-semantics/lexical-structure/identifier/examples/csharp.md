```csharp run id=verbatim-identifiers-and-nameof
using System.Reflection;

// @ makes a keyword usable as a name. The @ is not part of the name.
int @class = 1;
var @event = new { @class };
Console.WriteLine($"{@class} {@event}");

// Contextual keywords such as var, async and record are names outside their context.
int var = 2, async = 3;
Console.WriteLine(var + async);

// A Unicode escape in a name is the same name.
int abc = 4;
Console.WriteLine(\u0061bc);

// nameof gives the name of a variable, type or member as a string, checked by the compiler.
Console.WriteLine(nameof(abc) + " " + nameof(Point.X));

// After compilation, the names of members are metadata that reflection reads.
var fields = typeof(Point).GetFields(BindingFlags.Public | BindingFlags.Instance);
Console.WriteLine(string.Join(" ", fields.Select(f => f.Name)));

class Point
{
    public int X = 1, @Y = 2;
}
```

```output for=verbatim-identifiers-and-nameof
1 { class = 1 }
5
4
abc X
X Y
```

- Verdict: native. The prefix `@` makes a keyword usable as a name, and the `@` is not part of the name, so the anonymous type has a property `class`. Contextual keywords such as `var` and `async` are names outside their context. `nameof` gives a name as a string that the compiler checks, and reflection reads the names of members from metadata.

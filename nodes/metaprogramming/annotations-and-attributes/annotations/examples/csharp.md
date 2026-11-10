```csharp run id=custom
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;

[AttributeUsage(AttributeTargets.Method)]
class CommandAttribute : Attribute
{
    public string Name { get; }
    public string Help { get; set; } = "";

    public CommandAttribute(string name)
    {
        Name = name;
    }
}

class Tool
{
    [Command("hello", Help = "print a greeting")]
    public string Hello() => "hello, world";

    [Command("time")]
    public string Time() => "noon";

    public string Helper() => "not a command";
}

class Program
{
    [Conditional("TRACE_ON")]
    static void Trace(string message) => Console.WriteLine("trace: " + message);

    static void Main()
    {
        var tool = new Tool();
        var commands = typeof(Tool).GetMethods()
            .Select(m => (Method: m, Attr: m.GetCustomAttribute<CommandAttribute>()))
            .Where(p => p.Attr != null)
            .OrderBy(p => p.Attr!.Name);
        foreach (var (method, attr) in commands)
        {
            Trace("calling " + method.Name);
            Console.WriteLine($"{attr!.Name} -> {method.Invoke(tool, null)} [help: \"{attr.Help}\"]");
        }
    }
}
```

```output for=custom
hello -> hello, world [help: "print a greeting"]
time -> noon [help: ""]
```

An attribute is a class that derives from `Attribute`. The constructor parameter `name` is a positional parameter, and the property `Help` is a named parameter. `[Command(...)]` drops the suffix `Attribute`. The program reads the attributes through reflection. `Trace` has the `Conditional` attribute, and the symbol `TRACE_ON` is not defined, so the compiler leaves out each call to it.

```csharp fails expect=compile id=obsolete
using System;

class Account
{
    [Obsolete("use Deposit", true)]
    public void Add(decimal amount) => Balance += amount;

    public void Deposit(decimal amount) => Balance += amount;

    public decimal Balance { get; private set; }
}

class Program
{
    static void Main()
    {
        var a = new Account();
        a.Add(10m);
        Console.WriteLine(a.Balance);
    }
}
```

`Obsolete` with `true` as its second argument makes each use of the method a compile error. Without the second argument, the use is a warning.

```csharp fails expect=compile id=usage
using System;

[AttributeUsage(AttributeTargets.Method)]
class CommandAttribute : Attribute
{
}

[Command]
class Tool
{
}

class Program
{
    static void Main()
    {
        Console.WriteLine(new Tool());
    }
}
```

`AttributeUsage` limits the attribute to methods. The program puts it on a class, and the compiler rejects it.

- Verdict: native. Attributes are classes that derive from `System.Attribute`. The compiler stores them in the metadata of the assembly, and reflection reads them at run time.
- Some attributes change what the compiler does: `Obsolete` adds a warning or an error, and `Conditional` removes calls.
- Source generators read attributes at compile time and add source files, in the same role as Java annotation processors.

```csharp run id=redacting-logger
using System;
using System.Reflection;

[AttributeUsage(AttributeTargets.Class, Inherited = false)]
sealed class SensitiveAttribute : Attribute { }

record Username(string Value);

[Sensitive]
record Password(string Value);

[Sensitive]
record ApiKey(string Value);

class Program
{
    static void Log(string label, object item, string value)
    {
        bool marked = item.GetType().GetCustomAttribute<SensitiveAttribute>() != null;
        Console.WriteLine($"{label}: {(marked ? "[redacted]" : value)}");
    }

    static void Main()
    {
        var user = new Username("ann");
        var password = new Password("hunter2");
        var key = new ApiKey("k-123");
        Log("user", user, user.Value);
        Log("password", password, password.Value);
        Log("api key", key, key.Value);
    }
}
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. The .NET rule CA1040 advises a custom attribute in place of an empty interface when the test happens at run time. `Inherited = false` means that a subclass is not marked unless it declares the attribute too.

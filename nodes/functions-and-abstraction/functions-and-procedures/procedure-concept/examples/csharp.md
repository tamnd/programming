```csharp run id=procedure-csharp
using System;

class Program
{
    static void Greet(string name)
    {
        Console.WriteLine("Hello, " + name + "!");
    }

    static void Main()
    {
        Greet("Ada");
        Greet("Alan");
    }
}
```

```output for=procedure-csharp
Hello, Ada!
Hello, Alan!
```

- Verdict: native. A method with the return type `void` is a procedure.

```csharp run id=copy
using System;

static void Copy(string src, string dst, bool fail)
{
    using var a = new FakeFile(src);
    using var b = new FakeFile(dst);
    if (fail) throw new InvalidOperationException($"cannot read {a.Name}");
    Console.WriteLine($"copy {a.Name} to {b.Name}");
}

static string Label()
{
    string text = "try";
    try
    {
        return text;
    }
    finally
    {
        text = "finally";
        Console.WriteLine($"finally ran; text is now {text}");
    }
}

Copy("a.txt", "b.txt", false);
try
{
    Copy("a.txt", "c.txt", true);
}
catch (InvalidOperationException e)
{
    Console.WriteLine($"caught: {e.Message}");
}
Console.WriteLine($"Label() returned {Label()}");

sealed class FakeFile : IDisposable
{
    public string Name { get; }

    public FakeFile(string name)
    {
        Name = name;
        Console.WriteLine($"open {name}");
    }

    public void Dispose() => Console.WriteLine($"close {Name}");
}
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt
close a.txt
open a.txt
open c.txt
close c.txt
close a.txt
caught: cannot read a.txt
finally ran; text is now finally
Label() returned try
```

- Verdict: native. `try` with `finally` runs on every exit. A `using` declaration disposes its object at the end of the enclosing scope, and several objects are disposed in reverse order of declaration.
- The compiler turns `using` into `try` and `finally`, so a `return` or an exception in the scope still calls `Dispose`.
- The `return` in `Label` computes its value before `finally` runs.

C# does not let control leave a `finally` block. A `return` there is the compile error CS0157, so the bug of the other languages cannot be written.

```csharp fails expect=compile id=finally-return
using System;

static string Save(string record)
{
    try
    {
        if (record.Length == 0) throw new ArgumentException("empty record");
        return "saved";
    }
    finally
    {
        return "done";
    }
}

Console.WriteLine(Save(""));
```

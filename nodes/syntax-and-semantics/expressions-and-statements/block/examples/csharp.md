```csharp run id=using-and-checked-blocks
int x = 1;
{
    int y = x + 1;  // y is local to this block
    Console.WriteLine($"y = {y}");
}

// using declares a resource that is disposed at the end of the block.
{
    using var r = new Resource("a");
    Console.WriteLine("in the block");
}
Console.WriteLine("after the block");

// checked and unchecked are blocks that change the rules for overflow.
int big = int.MaxValue;
unchecked { Console.WriteLine($"unchecked: {big + 1}"); }
try { checked { Console.WriteLine(big + 1); } }
catch (OverflowException) { Console.WriteLine("checked: OverflowException"); }

// A lambda can have a block body with statements and a return.
Func<int, string> name = n => { var s = n.ToString(); return "n" + s; };
Console.WriteLine(name(x));

class Resource(string id) : IDisposable
{
    public void Dispose() => Console.WriteLine($"disposed {id}");
}
```

```output for=using-and-checked-blocks
y = 2
in the block
disposed a
after the block
unchecked: -2147483648
checked: OverflowException
n1
```

- Verdict: native. A block makes a scope for `y`. A `using` declaration disposes the resource at the end of its block, so `disposed a` comes before `after the block`. `checked` and `unchecked` blocks choose what an overflow does. A lambda can have a block body.

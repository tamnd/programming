```csharp run id=boxing-and-references
using System;

object[] things = { 42, "hi", new[] { 1, 2 }, 3.5m, new Point(1, 2) };
foreach (var t in things)
{
    Console.Write(t.GetType().Name + " ");
}
Console.WriteLine();

int n = 1;
object box = n; // boxing copies the value into a new object
n = 2;
Console.WriteLine($"{n} {box} {box is int}");

var ana = new { Name = "ana", Age = 30 }; // an anonymous type
Console.WriteLine($"{ana} {ana.GetType().Name.StartsWith("<>")}");

var a = new Counter();
var b = a; // a second reference to the same object
b.Inc();
Console.WriteLine($"{a.Count} {ReferenceEquals(a, b)}");

record Point(int X, int Y);

class Counter
{
    public int Count { get; private set; }

    public void Inc() => Count++;
}
```

```output for=boxing-and-references
Int32 String Int32[] Decimal Point 
2 1 True
{ Name = ana, Age = 30 } True
1 True
```

- Verdict: native. Every C# type derives from `object`, value types included, so a list of `object` can hold an `int`, a string, an array and a record, and each knows its type through `GetType`. Assigning a value type to `object` boxes it: the runtime copies the value into a new object on the heap, so the box keeps 1 after `n` changes to 2.
- An anonymous type creates an object with read-only properties and a class name that the compiler generates. Variables of class types hold references, so `b` and `a` reach one `Counter`, and `ReferenceEquals` confirms it.

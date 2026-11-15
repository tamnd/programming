```csharp run id=list
using System;

static int Total(IntList xs) => xs switch
{
    Cons c => c.Head + Total(c.Tail),
    _ => 0,
};

static int Length(IntList xs) => xs switch
{
    Cons c => 1 + Length(c.Tail),
    _ => 0,
};

IntList xs = new Cons(1, new Cons(2, new Cons(3, new Nil())));
Console.WriteLine($"sum: {Total(xs)}");
Console.WriteLine($"length: {Length(xs)}");
Console.WriteLine($"record: {xs}");

abstract record IntList;
sealed record Nil : IntList;
sealed record Cons(int Head, IntList Tail) : IntList;
```

```output for=list
sum: 6
length: 3
record: Cons { Head = 1, Tail = Cons { Head = 2, Tail = Cons { Head = 3, Tail = Nil { } } } }
```

- Verdict: native. A record class holds references, so `Cons` can have a field of the base type `IntList`.
- The generated `ToString` of a record prints each property, so it shows the nested list.

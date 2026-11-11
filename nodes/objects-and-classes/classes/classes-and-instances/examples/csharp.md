```csharp run id=classes-structs-and-records
var a = new Account("ana", 100);
var b = a;  // a class is a reference type: b and a are the same object
b.Deposit(50);
Console.WriteLine($"{a.Balance} {ReferenceEquals(a, b)} {Account.Opened}");

var p = new Point(1, 2);
var q = p;  // a struct is a value type: q is a copy
q.X = 9;
Console.WriteLine($"{p.X} {q.X}");

var r1 = new Money(5, "EUR");
var r2 = new Money(5, "EUR");
Console.WriteLine($"{r1} {r1 == r2}");  // records compare by value

class Account(string owner, decimal balance)
{
    public static int Opened { get; private set; }

    public string Owner { get; } = owner;
    public decimal Balance { get; private set; } = Count(balance);

    private static decimal Count(decimal b)
    {
        Opened++;
        return b;
    }

    public void Deposit(decimal amount) => Balance += amount;
}

struct Point(int x, int y)
{
    public int X = x;
    public int Y = y;
}

record Money(decimal Amount, string Currency);
```

```output for=classes-structs-and-records
150 True 1
1 9
Money { Amount = 5, Currency = EUR } True
```

```csharp wrong id=copied-struct
var points = new List<Point> { new(1, 2) };
var p = points[0];  // the indexer returns a copy of the struct
p.Move(10);
Console.WriteLine($"{points[0].X} {p.X}");

struct Point(int x, int y)
{
    public int X = x;
    public int Y = y;

    public void Move(int dx) => X += dx;  // a mutable struct: changes only this copy
}
```

```output for=copied-struct
1 11
```

- Verdict: native. C# has two kinds of types with fields and methods. A `class` is a reference type: assigning an instance copies the reference, so `b` and `a` name one object. A `struct` is a value type: assigning it copies the fields, so a change to `q` leaves `p` alone. A `record` is a class whose equality compares the values of its properties. A primary constructor in the type header gives parameters that the members can use, and static members belong to the type.
- The wrong block changes a struct that it read from a list. The indexer returns a copy, so `Move` changes the copy and the list keeps the old value. Avoid mutable structs, or write the changed copy back with `points[0] = p`.

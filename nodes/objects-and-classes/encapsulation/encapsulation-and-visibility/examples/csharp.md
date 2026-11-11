```csharp run id=accessors
var a = new Account("ana");
a.Deposit(100);
a.Withdraw(30);
Console.WriteLine($"{a.Owner} {a.Balance} [{string.Join(", ", a.History)}]");
try
{
    a.Withdraw(500);
}
catch (InvalidOperationException e)
{
    Console.WriteLine(e.Message);
}
Console.WriteLine(a.History is List<long>);  // the caller sees only IReadOnlyList

sealed class Account(string owner)
{
    private readonly List<long> history = [];

    public string Owner { get; } = owner;      // read-only after construction
    public long Balance { get; private set; }  // public get, private set
    public IReadOnlyList<long> History => history.AsReadOnly();

    public void Deposit(long amount)
    {
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(amount);
        history.Add(amount);
        Balance += amount;
    }

    public void Withdraw(long amount)
    {
        if (amount > Balance) throw new InvalidOperationException("insufficient funds");
        history.Add(-amount);
        Balance -= amount;
    }
}
```

```output for=accessors
ana 70 [100, -30]
insufficient funds
False
```

```csharp fails expect=compile id=private-setter
var a = new Account();
a.Balance = 1_000_000;  // the set accessor is private
Console.WriteLine(a.Balance);

class Account
{
    public long Balance { get; private set; }
}
```

- Verdict: native. C# has `private`, `protected`, `internal`, `protected internal`, `private protected` and `public` access for members, and `file` for types, and each accessor of a property can have its own level, as in `{ get; private set; }`. A property with only `get` can be set only in the constructor or an initializer. `AsReadOnly` wraps a list in a view that has no methods to change it, so the caller cannot even cast it back to `List<long>`.
- The failing block assigns `Balance` from outside the class. The property is public, but its set accessor is private, so the compiler rejects the assignment with error CS0272.

```csharp run id=accounts
using System;

Account a = new Checking("bo", 100);
a.Deposit(50);
Console.WriteLine(a.Describe());
Console.WriteLine(typeof(Checking).BaseType!.Name);

class Account
{
    public string Owner { get; }
    public decimal Balance { get; private set; }

    public Account(string owner)
    {
        Owner = owner;
        Console.WriteLine("Account constructor");
    }

    public void Deposit(decimal n) => Balance += n;
    public virtual string Describe() => $"{GetType().Name} of {Owner}: {Balance}";
}

sealed class Checking : Account
{
    readonly decimal overdraft;

    public Checking(string owner, decimal overdraft) : base(owner) // the base constructor runs first
    {
        this.overdraft = overdraft;
        Console.WriteLine("Checking constructor");
    }

    public override string Describe() => base.Describe() + $" (overdraft {overdraft})";
}
```

```output for=accounts
Account constructor
Checking constructor
Checking of bo: 50 (overdraft 100)
Account
```

```csharp fails expect=compile id=sealed
class Account { }

sealed class Checking : Account { }

class Premium : Checking { } // a sealed class cannot be a base class
```

- Verdict: native. A class names one base class after a colon and inherits its members. The constructor calls a base constructor with `: base(...)`, and the base constructor runs first, as the output shows. A member can be extended with `base.Describe()`, but only a `virtual` or `abstract` member can be overridden. `private set` lets subclasses read `Balance` but not set it.
- `sealed` stops further inheritance, and the failing block is rejected with error CS0509. Sealing classes that are not designed for extension avoids the fragile base class problem, and it lets the runtime call their methods directly. A record can inherit from another record, and a struct cannot inherit from any type.

```csharp run id=account
using System;
using System.Diagnostics;

var account = new Account(100);
account.Withdraw(30);
Console.WriteLine($"balance {account.Balance}");
try
{
    account.Withdraw(500);
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine($"rejected: {e.ParamName} = {e.ActualValue}");
}
#if DEBUG
Console.WriteLine("DEBUG is defined");
#else
Console.WriteLine("DEBUG is not defined");
#endif

class Account(int balance)
{
    public int Balance { get; private set; } = balance;

    public void Withdraw(int amount)
    {
        if (amount <= 0 || amount > Balance)
        {
            throw new ArgumentOutOfRangeException(nameof(amount), amount, "cannot withdraw");
        }
        int old = Balance;
        Balance -= amount;
        Debug.Assert(Balance == old - amount, "postcondition");
        Debug.Assert(Balance >= 0, "invariant");
    }
}
```

```output
balance 70
rejected: amount = 500
DEBUG is defined
```

- Verdict: stdlib. `Debug.Assert` checks a condition and reports a failure. It carries `[Conditional("DEBUG")]`, so the compiler drops each call when the symbol `DEBUG` is not defined.
- `Trace.Assert` is the version that stays in release builds.
- The public method checks its argument with an exception, which runs in every build. `Debug.Assert` checks the postcondition and the invariant.

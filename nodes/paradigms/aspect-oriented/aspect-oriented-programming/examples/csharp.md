```csharp run id=dispatch-proxy
using System.Reflection;

IAccount account = Audit<IAccount>.Wrap(new Account());
account.Deposit(10);
account.Withdraw(4);
Console.WriteLine($"balance {account.Balance}");

public interface IAccount
{
    int Balance { get; }
    void Deposit(int amount);
    void Withdraw(int amount);
}

class Account : IAccount
{
    public int Balance { get; private set; }
    public void Deposit(int amount) => Balance += amount;
    public void Withdraw(int amount) => Balance -= amount;
}

public class Audit<T> : DispatchProxy where T : class
{
    T target = null!;

    public static T Wrap(T target)
    {
        var proxy = Create<T, Audit<T>>();
        ((Audit<T>)(object)proxy).target = target;
        return proxy;
    }

    protected override object? Invoke(MethodInfo? method, object?[]? args)
    {
        if (method!.Name is "Deposit" or "Withdraw")
            Console.WriteLine($"before {method.Name}({args![0]})");
        return method.Invoke(target, args);
    }
}
```

```output for=dispatch-proxy
before Deposit(10)
before Withdraw(4)
balance 6
```

- Verdict: stdlib. `System.Reflection.DispatchProxy` makes a class at run time that implements an interface and sends each call to `Invoke`. The pointcut here is the test on the method name. The property `Balance` is the method `get_Balance`, so it is not advised. Libraries such as Castle DynamicProxy, and compile-time weavers such as PostSharp and Metalama, give more complete AOP.

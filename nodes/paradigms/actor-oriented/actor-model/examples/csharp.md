```csharp run id=channel-actor
using System.Threading.Channels;

var account = new Account();
await Parallel.ForAsync(0, 100, async (i, ct) => await account.Send(new Deposit(10)));
await account.Send(new Withdraw(1));
Console.WriteLine($"balance {await account.Ask()}");

abstract record Msg;
record Deposit(int Amount) : Msg;
record Withdraw(int Amount) : Msg;
record GetBalance(TaskCompletionSource<int> Reply) : Msg;

class Account
{
    readonly Channel<Msg> mailbox = Channel.CreateUnbounded<Msg>(new UnboundedChannelOptions { SingleReader = true });
    int balance; // only the loop uses this field

    public Account() => Task.Run(Loop);

    public ValueTask Send(Msg m) => mailbox.Writer.WriteAsync(m);

    public async Task<int> Ask()
    {
        var reply = new TaskCompletionSource<int>(TaskCreationOptions.RunContinuationsAsynchronously);
        await Send(new GetBalance(reply));
        return await reply.Task;
    }

    async Task Loop()
    {
        await foreach (var m in mailbox.Reader.ReadAllAsync())
        {
            switch (m)
            {
                case Deposit d: balance += d.Amount; break;
                case Withdraw w when w.Amount <= balance: balance -= w.Amount; break;
                case GetBalance g: g.Reply.SetResult(balance); break;
            }
        }
    }
}
```

```output for=channel-actor
balance 999
```

- Verdict: library. `System.Threading.Channels` gives a mailbox, and one task that reads it is an actor. Microsoft Orleans gives virtual actors, called grains, that the runtime creates, places on servers and restores on demand. Akka.NET ports Akka to .NET.

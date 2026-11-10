```csharp run id=invoices
// The composition root.
var service = new ReminderService(new FixedClock(40), new ConsoleMailer());
Invoice[] invoices = [new(17, "ana@example.com", 31), new(18, "bo@example.com", 45)];
var sent = invoices.Count(service.Remind);
Console.WriteLine($"reminders sent: {sent}");

// A test wires the same class to a recording double.
var recorder = new RecordingMailer();
new ReminderService(new FixedClock(50), recorder).Remind(invoices[1]);
Console.WriteLine($"recorded: {recorder.Sent[0]}");

interface IClock { int Today(); }

interface IMailer { void Send(string to, string text); }

record Invoice(int Number, string Customer, int Due);

// A primary constructor takes the collaborators.
sealed class ReminderService(IClock clock, IMailer mailer)
{
    public bool Remind(Invoice invoice)
    {
        var late = clock.Today() - invoice.Due;
        if (late <= 0) return false;
        mailer.Send(invoice.Customer, $"invoice {invoice.Number} is {late} days overdue");
        return true;
    }
}

sealed class FixedClock(int day) : IClock
{
    public int Today() => day;
}

sealed class ConsoleMailer : IMailer
{
    public void Send(string to, string text) => Console.WriteLine($"to {to}: {text}");
}

sealed class RecordingMailer : IMailer
{
    public List<string> Sent { get; } = [];
    public void Send(string to, string text) => Sent.Add($"to {to}: {text}");
}
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. Interfaces describe the roles, and a primary constructor receives the collaborators. The parameters `clock` and `mailer` are in scope in every member of the class.
- The .NET libraries for hosted applications include a container in the package Microsoft.Extensions.DependencyInjection. It calls this same constructor. The package is not part of the base library, so this program wires the objects by hand.
- `TimeProvider` in the base library is a clock abstraction that a class can receive in its constructor, for the same reason as `IClock` here.

```csharp run id=simple-factory
using System;

foreach (var channel in new[] { "email", "sms", "push", "fax" })
{
    INotifier notifier;
    try
    {
        notifier = NotifierFactory.Make(channel);
    }
    catch (ArgumentException e)
    {
        Console.WriteLine(e.Message);
        continue;
    }
    Console.WriteLine($"{notifier.GetType().Name}: {notifier.Send("ann", "order shipped")}");
}

interface INotifier
{
    string Send(string user, string text);
}

sealed class EmailNotifier : INotifier
{
    public string Send(string user, string text) => $"mail to {user}@example.com: {text}";
}

sealed class SmsNotifier : INotifier
{
    public string Send(string user, string text) => $"text to the phone of {user}: {text}";
}

sealed class PushNotifier : INotifier
{
    public string Send(string user, string text) => $"push to the app of {user}: {text}";
}

static class NotifierFactory
{
    // The one place that knows the concrete classes.
    public static INotifier Make(string channel) => channel switch
    {
        "email" => new EmailNotifier(),
        "sms" => new SmsNotifier(),
        "push" => new PushNotifier(),
        _ => throw new ArgumentException($"unknown channel: {channel}"),
    };
}
```

```output
EmailNotifier: mail to ann@example.com: order shipped
SmsNotifier: text to the phone of ann: order shipped
PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A static class with a `switch` expression is the simple factory, and the return type is the interface.
- In an application that uses a dependency injection container, the container can take this role: it maps an interface to a class from configuration.

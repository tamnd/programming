```csharp run id=max-deliveries
const int MaxDeliveries = 3;

var queue = new Queue<Message>();
foreach (var b in new[] { "pay-1 10", "pay-2 x", "pay-3 5" })
{
    queue.Enqueue(new Message(b));
}
var deadLetters = new List<Message>();
int processed = 0;
while (queue.TryDequeue(out var msg))
{
    msg.Deliveries++;
    try
    {
        Handle(msg.Body);
        processed++;
    }
    catch (FormatException e)
    {
        Console.WriteLine($"fail {msg.Name} delivery {msg.Deliveries}: {e.Message}");
        if (msg.Deliveries < MaxDeliveries)
        {
            queue.Enqueue(msg);
        }
        else
        {
            msg.Reason = e.Message;
            msg.Source = "payments";
            deadLetters.Add(msg);
        }
    }
}
foreach (var msg in deadLetters)
{
    Console.WriteLine($"dead letter {msg.Name}: deliveries={msg.Deliveries} reason={msg.Reason} source={msg.Source}");
}
Console.WriteLine($"processed {processed}, dead letters {deadLetters.Count}");

static void Handle(string body)
{
    var parts = body.Split(' ');
    if (!int.TryParse(parts[1], out _))
    {
        throw new FormatException("bad amount " + parts[1]);
    }
    Console.WriteLine($"ok {parts[0]} amount {parts[1]}");
}

sealed class Message(string body)
{
    public string Body { get; } = body;
    public string Name => Body.Split(' ')[0];
    public int Deliveries { get; set; }
    public string? Reason { get; set; }
    public string? Source { get; set; }
}
```

```output
ok pay-1 amount 10
fail pay-2 delivery 1: bad amount x
ok pay-3 amount 5
fail pay-2 delivery 2: bad amount x
fail pay-2 delivery 3: bad amount x
dead letter pay-2: deliveries=3 reason=bad amount x source=payments
processed 2, dead letters 1
```

- Verdict: idiom. .NET has no dead letter channel. A `Queue<T>` is the queue, a property of the message is the delivery count, and a `List<T>` is the dead letter channel.
- `Handle` throws `FormatException`, and only that exception counts as a failure of the message. Other exceptions stop the program.
- With Azure Service Bus, `ServiceBusReceiver.DeadLetterMessageAsync` moves a message to the dead-letter subqueue with a reason and a description, and the service moves a message there itself when the delivery count is more than `MaxDeliveryCount`.

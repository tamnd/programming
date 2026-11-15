```csharp run id=tap-audit
using System;
using System.Collections.Generic;
using System.Linq;

var tap = new WireTap();
var orders = new (string Id, string Card, int Total)[] { ("o1", "4111111111111111", 30), ("o2", "5500000000000004", 12),
                                                         ("o3", "4111111111111111", 7), ("o4", "340000000000009", 45) };
foreach (var o in orders)
{
    tap.Send(new Message(o.Id, o.Card, o.Total, new Dictionary<string, string> { ["source"] = "web" }));
    if (o.Id == "o3")
    {
        tap.Enabled = false; // an operator turns the tap off
    }
}
var seen = new List<Message>();
while (tap.Main.TryDequeue(out var m))
{
    seen.Add(m);
    Console.WriteLine($"main: {m.Id} card {m.Card} total {m.Total}");
}
int audited = 0;
while (tap.Tap.TryDequeue(out var m))
{
    var masked = m with { Card = new string('*', m.Card.Length - 4) + m.Card[^4..] };
    masked.Headers["audit"] = "yes";
    audited++;
    Console.WriteLine($"audit: {masked.Id} card {masked.Card} total {masked.Total}");
}
Console.WriteLine($"main: {seen.Count} messages, audit: {audited} messages (tap off after o3)");
bool unchanged = seen.All(x => !x.Card.Contains('*') && !x.Headers.ContainsKey("audit"));
Console.WriteLine($"main messages unchanged: {unchanged.ToString().ToLowerInvariant()}");

record Message(string Id, string Card, int Total, Dictionary<string, string> Headers);

class WireTap
{
    public Queue<Message> Main { get; } = new();
    public Queue<Message> Tap { get; } = new();
    public bool Enabled { get; set; } = true;

    public void Send(Message m)
    {
        if (Enabled)
        {
            // "with" copies the record, but the dictionary would be shared; copy it too.
            Tap.Enqueue(m with { Headers = new Dictionary<string, string>(m.Headers) });
        }
        Main.Enqueue(m);
    }
}
```

```output
main: o1 card 4111111111111111 total 30
main: o2 card 5500000000000004 total 12
main: o3 card 4111111111111111 total 7
main: o4 card 340000000000009 total 45
audit: o1 card ************1111 total 30
audit: o2 card ************0004 total 12
audit: o3 card ************1111 total 7
main: 4 messages, audit: 3 messages (tap off after o3)
main messages unchanged: true
```

- Verdict: idiom. Two `Queue<Message>` objects are the channels, and `Send` adds a copy to the tap queue.
- A `with` expression copies a record, but it copies the reference to the dictionary, so the tap makes a new `Dictionary` too.
- The masked card goes into a new record, so the copy on the audit channel is not changed in place.

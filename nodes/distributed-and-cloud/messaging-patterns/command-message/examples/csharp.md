```csharp run id=dispatch-commands
using System.Text.Json;
using System.Text.Json.Serialization;

var stock = new Dictionary<string, int> { ["A1"] = 5 };
var channel = new Queue<byte[]>(); // the channel carries bytes

void Send(Command c) => channel.Enqueue(JsonSerializer.SerializeToUtf8Bytes(c));

Send(new ReserveStock("A1", 2));
Send(new ChargeCard(42, 1999));
Send(new ReserveStock("A1", 9));

while (channel.TryDequeue(out var raw))
{
    var command = JsonSerializer.Deserialize<Command>(raw);
    Console.WriteLine(command switch
    {
        ReserveStock r when stock[r.Sku] < r.Qty =>
            $"reserve {r.Qty} of {r.Sku}: not enough stock, {stock[r.Sku]} left",
        ReserveStock r => $"reserve {r.Qty} of {r.Sku}: {stock[r.Sku] -= r.Qty} left",
        ChargeCard c => $"charge {c.Cents} cents for order {c.Order}: done",
        _ => "invalid message",
    });
}

// The "command" property of the JSON names the command.
[JsonPolymorphic(TypeDiscriminatorPropertyName = "command")]
[JsonDerivedType(typeof(ReserveStock), "ReserveStock")]
[JsonDerivedType(typeof(ChargeCard), "ChargeCard")]
abstract record Command;

record ReserveStock(string Sku, int Qty) : Command;

record ChargeCard(int Order, int Cents) : Command;
```

```output
reserve 2 of A1: 3 left
charge 1999 cents for order 42: done
reserve 9 of A1: not enough stock, 3 left
```

- Verdict: stdlib. `System.Text.Json` writes each command as JSON with a property `command` that names its type, and reads it back as the right record.
- The `switch` expression on the type of the record is the dispatcher. A JSON body with an unknown name in `command` makes `Deserialize` throw, so the receiver does not run it.
- The records are immutable, so a handler cannot change a command after it arrives.

```csharp run id=claim-report
using System.Security.Cryptography;
using System.Text;

const int Threshold = 1024;                 // payloads larger than this go to the store
var store = new Dictionary<string, byte[]>(); // stands for a blob store or a database
var channel = new List<string>();

string Hash(byte[] b) => "sha256:" + Convert.ToHexString(SHA256.HashData(b)).ToLowerInvariant();

void Send(string kind, byte[] payload)
{
    string message, extra = "";
    if (payload.Length <= Threshold)
    {
        message = $"{kind};inline;{Encoding.UTF8.GetString(payload)}";
    }
    else
    {
        var key = Hash(payload);
        store[key] = payload; // store first, then publish the claim check
        message = $"{kind};claim;{key};{payload.Length}";
        extra = $" for {payload.Length} bytes in the store";
    }
    channel.Add(message);
    Console.WriteLine($"sent {kind}: {message.Length} bytes on the channel{extra}");
}

string Receive(string message)
{
    var parts = message.Split(';', 3);
    var (kind, mode, rest) = (parts[0], parts[1], parts[2]);
    if (mode == "inline") return $"received {kind}: {rest}";
    var claim = rest.Split(';');
    var key = claim[0];
    if (!store.TryGetValue(key, out var payload))
        return $"received {kind}: claim {key[..19]}... not found, to the dead letter channel";
    if (Hash(payload) != key || payload.Length != int.Parse(claim[1]))
        return $"received {kind}: payload does not match the claim check";
    var rows = payload.Count(b => b == (byte)'\n');
    return $"received {kind}: {rows} rows, hash {key[7..19]} ok";
}

var report = new StringBuilder();
for (int i = 0; i < 12000; i++) report.Append($"row {i:D5};widget;12.50\n");
Send("invoice", Encoding.UTF8.GetBytes("INV-17 total 12.50"));
Send("report", Encoding.ASCII.GetBytes(report.ToString()));
foreach (var m in channel) Console.WriteLine(Receive(m));

// The lifecycle rule of the store removes the payload; then the broker delivers the claim again.
store.Clear();
Console.WriteLine(Receive(channel[1]));
```

```output
sent invoice: 33 bytes on the channel
sent report: 91 bytes on the channel for 276000 bytes in the store
received invoice: INV-17 total 12.50
received report: 12000 rows, hash 63b98860796d ok
received report: claim sha256:63b98860796d... not found, to the dead letter channel
```

- Verdict: idiom. `SHA256.HashData` and `Convert.ToHexString` make the key; a `Dictionary<string, byte[]>` stands for the blob store.
- `TryGetValue` gives the missing payload as a normal case, which goes to the dead letter channel.
- `store.Clear()` plays the lifecycle rule that removes the payload.

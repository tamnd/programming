```csharp run id=lock-file-with-hashes
// A lock file records the exact version and the SHA-256 hash of each package, and an install checks both.
using System.Security.Cryptography;
using System.Text;

// name -> version -> the bytes of the package
var registry = new Dictionary<string, Dictionary<string, byte[]>>
{
    ["log"] = new() { ["1.1.0"] = "log 1.1.0"u8.ToArray(), ["1.2.0"] = "log 1.2.0"u8.ToArray() },
    ["web"] = new() { ["2.0.0"] = "web 2.0.0"u8.ToArray(), ["2.0.1"] = "web 2.0.1"u8.ToArray() },
};
// name -> the major version that the project accepts
var manifest = new SortedDictionary<string, int>(StringComparer.Ordinal) { ["log"] = 1, ["web"] = 2 };

string Sha256(byte[] data) => Convert.ToHexStringLower(SHA256.HashData(data))[..16];

List<Entry> Resolve()
{
    var chosen = manifest
        .Select(m => new Entry(m.Key, registry[m.Key].Keys.Where(v => Version.Parse(v).Major == m.Value).MaxBy(Version.Parse)!, ""))
        .ToList();
    Console.WriteLine("resolve: " + string.Join(", ", chosen.Select(e => $"{e.Name} {e.Version}")));
    return chosen;
}

List<Entry> Lock(List<Entry> chosen)
{
    Console.WriteLine("lock file:");
    var lockfile = chosen.Select(e => e with { Digest = Sha256(registry[e.Name][e.Version]) }).ToList();
    foreach (var e in lockfile)
    {
        Console.WriteLine($"  {e.Name} {e.Version} sha256 {e.Digest}");
    }
    return lockfile;
}

void Install(List<Entry> lockfile)
{
    Console.WriteLine("install from the lock file:");
    foreach (var e in lockfile)
    {
        var actual = Sha256(registry[e.Name][e.Version]);
        Console.WriteLine(actual == e.Digest
            ? $"  {e.Name} {e.Version} ok"
            : $"  {e.Name} {e.Version} error: expected sha256 {e.Digest}, got {actual}");
    }
}

var lockfile = Lock(Resolve());
registry["log"]["1.3.0"] = Encoding.UTF8.GetBytes("log 1.3.0");
Console.WriteLine("log 1.3.0 is published");
Resolve();
Install(lockfile);
registry["web"]["2.0.1"] = Encoding.UTF8.GetBytes("web 2.0.1 with other code");
Console.WriteLine("web 2.0.1 is changed on the registry");
Install(lockfile);

record Entry(string Name, string Version, string Digest);
```

```output
resolve: log 1.2.0, web 2.0.1
lock file:
  log 1.2.0 sha256 bc5900c6e46c5b3f
  web 2.0.1 sha256 a72ce45f235c38b5
log 1.3.0 is published
resolve: log 1.3.0, web 2.0.1
install from the lock file:
  log 1.2.0 ok
  web 2.0.1 ok
web 2.0.1 is changed on the registry
install from the lock file:
  log 1.2.0 ok
  web 2.0.1 error: expected sha256 a72ce45f235c38b5, got 6745f93439ad7399
```

- Verdict: idiom. `SHA256.HashData` hashes a byte array in one call, and `Convert.ToHexStringLower` gives lower-case hexadecimal text.
- The literal `"log 1.1.0"u8` is a UTF-8 byte span, and `ToArray` copies it into an array.
- `e with { Digest = ... }` copies a record with one new value, so the chosen entries stay as they were.

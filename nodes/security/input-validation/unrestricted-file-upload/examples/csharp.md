```csharp run id=checked-upload
using System.Security.Cryptography;
using System.Text;

const int MaxBytes = 1_000_000;
// Each allowed extension, the type to serve it with, and its first bytes.
var allowed = new Dictionary<string, (string Type, byte[][] Signatures)>
{
    [".png"] = ("image/png", [Convert.FromHexString("89504E470D0A1A0A")]),
    [".jpg"] = ("image/jpeg", [Convert.FromHexString("FFD8FF")]),
    [".gif"] = ("image/gif", [Encoding.ASCII.GetBytes("GIF87a"), Encoding.ASCII.GetBytes("GIF89a")]),
};

string SaveUpload(string store, string fileName, byte[] data)
{
    if (data.Length > MaxBytes) throw new InvalidDataException("the file is too large");
    var ext = Path.GetExtension(fileName).ToLowerInvariant();
    if (!allowed.TryGetValue(ext, out var rule)) throw new InvalidDataException($"the extension {ext} is not allowed");
    if (!rule.Signatures.Any(s => data.AsSpan().StartsWith(s))) throw new InvalidDataException($"the content is not {rule.Type}");
    // The server picks the name. Only the checked extension comes from the client.
    var name = Convert.ToHexStringLower(SHA256.HashData(data))[..16] + ext;
    File.WriteAllBytes(Path.Combine(store, name), data);
    return name;
}

// The store is a private directory outside the web root.
var store = Directory.CreateTempSubdirectory("uploads").FullName;
byte[] png = [.. Convert.FromHexString("89504E470D0A1A0A"), .. new byte[16]];
(string Name, byte[] Data)[] uploads =
[
    ("notes.html", Encoding.UTF8.GetBytes("<h1>hello</h1>")),
    ("notes.png", Encoding.UTF8.GetBytes("<h1>hello</h1>")),
    ("photo.php.PNG", png),
];
foreach (var (fileName, data) in uploads)
{
    try
    {
        Console.WriteLine($"{fileName}: saved as {SaveUpload(store, fileName, data)}");
    }
    catch (InvalidDataException e)
    {
        Console.WriteLine($"{fileName}: rejected, {e.Message}");
    }
}
Directory.Delete(store, recursive: true);
```

```output for=checked-upload
notes.html: rejected, the extension .html is not allowed
notes.png: rejected, the content is not image/png
photo.php.PNG: saved as f731c2528f4184b3.png
```

`SaveUpload` checks the size, the extension and the first bytes, and names the file after its SHA-256 hash.

- Verdict: stdlib. In ASP.NET Core, `IFormFile` holds an upload. Its `FileName` and `ContentType` come from the client, so check them and never use `FileName` as a path.
- `Path.GetExtension` returns only the last extension, so `photo.php.PNG` gives `.PNG`.
- CodeQL flags file uploads in ASP.NET code with the query `cs/web/file-upload`.

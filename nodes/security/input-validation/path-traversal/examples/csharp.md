```csharp wrong id=combine
var root = "/srv/app/public";
foreach (var name in new[] { "report.txt", "../secret.txt", "/etc/hosts" })
{
    Console.WriteLine(Path.Combine(root, name));
}
```

```output for=combine
/srv/app/public/report.txt
/srv/app/public/../secret.txt
/etc/hosts
```

```csharp run id=full-path
static string? LocalPath(string root, string name)
{
    var fullRoot = Path.GetFullPath(root) + Path.DirectorySeparatorChar;
    var full = Path.GetFullPath(Path.Join(fullRoot, name));
    return full.StartsWith(fullRoot, StringComparison.Ordinal) ? full : null;
}

foreach (var name in new[] { "report.txt", "../secret.txt", "/etc/hosts", "../public-old/a.txt" })
{
    Console.WriteLine($"{name}: {LocalPath("/srv/app/public", name) ?? "refused"}");
}
```

```output for=full-path
report.txt: /srv/app/public/report.txt
../secret.txt: refused
/etc/hosts: /srv/app/public/etc/hosts
../public-old/a.txt: refused
```

- Verdict: stdlib. `Path.Combine` returns the second path when it is absolute. Its documentation says to call `Path.Join` or `Path.TryJoin` when later arguments come from the user. `Path.Join` keeps the first path.
- `Path.GetFullPath` makes the path absolute and removes `.` and `..` by text. The prefix check adds a separator to the root, so `/srv/app/public-old` does not pass for `/srv/app/public`.
- `GetFullPath` does not follow symbolic links. Where other users can create links, resolve them with `FileSystemInfo.ResolveLinkTarget` or open the file and check its final path.
- On Windows, use `StringComparison.OrdinalIgnoreCase`, because the file system ignores case.
- CodeQL query `cs/path-injection` follows input from a request to file system calls.

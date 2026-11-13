```csharp run id=filesysteminfo
using System;
using System.IO;

File.WriteAllText("data.txt", "hello");
File.SetUnixFileMode("data.txt",
    UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.GroupRead);
File.SetLastWriteTimeUtc("data.txt", DateTimeOffset.FromUnixTimeSeconds(1700000000).UtcDateTime);
File.CreateSymbolicLink("link", "data.txt");

// A FileInfo for a link describes the link itself.
var link = new FileInfo("link");
Console.WriteLine($"link: target {link.LinkTarget}, size {link.Length}");
Console.WriteLine($"link attributes: {link.Attributes}");

// ResolveLinkTarget gives a FileSystemInfo for the target.
var target = (FileInfo)link.ResolveLinkTarget(returnFinalTarget: true)!;
Console.WriteLine($"target: {target.Name}, size {target.Length}");
Console.WriteLine($"mode: {target.UnixFileMode}");
Console.WriteLine($"mtime: {target.LastWriteTimeUtc:yyyy-MM-ddTHH:mm:ssZ}");
Console.WriteLine($"attributes: {target.Attributes}");
```

```output for=filesysteminfo
link: target data.txt, size 8
link attributes: ReparsePoint
target: data.txt, size 5
mode: GroupRead, UserWrite, UserRead
mtime: 2023-11-14T22:13:20Z
attributes: Normal
```

- Verdict: stdlib. `FileSystemInfo` is the base class of `FileInfo` and `DirectoryInfo`, with `Attributes`, the three times, `UnixFileMode` and `LinkTarget`. On a symbolic link, the properties describe the link, and `ResolveLinkTarget(returnFinalTarget: true)` gives the target. `Attributes` is a set of `FileAttributes` flags from Windows, and on Unix, .NET makes the flags from the file type and the mode. .NET has no property for the link count or the inode number.

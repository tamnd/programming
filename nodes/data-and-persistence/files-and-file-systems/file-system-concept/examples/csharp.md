```csharp run id=drives-and-links
using System.IO;

Directory.CreateDirectory("tree/src");
File.WriteAllText("tree/src/main.txt", "main\n");
File.WriteAllText("tree/readme.txt", "read me\n");

// DirectoryInfo lists FileSystemInfo objects: DirectoryInfo or FileInfo.
foreach (var e in new DirectoryInfo("tree").EnumerateFileSystemInfos().OrderBy(e => e.Name, StringComparer.Ordinal))
{
    Console.WriteLine($"{e.Name} {(e is DirectoryInfo ? "dir" : "file")}");
}

// File.Move with the same directory is a rename; the content stays.
File.Move("tree/readme.txt", "tree/README.txt");
Console.WriteLine($"size after move: {new FileInfo("tree/README.txt").Length}");

// A symbolic link is a file that holds a path. .NET 10 has no method that makes a hard link.
File.CreateSymbolicLink("tree/main-link.txt", "src/main.txt");
var link = new FileInfo("tree/main-link.txt");
Console.WriteLine($"link target: {link.LinkTarget}");
Console.WriteLine($"is reparse point: {link.Attributes.HasFlag(FileAttributes.ReparsePoint)}");

// DriveInfo describes the volumes. On Unix the root directory is one of them.
var root = DriveInfo.GetDrives().FirstOrDefault(d => d.Name == "/");
Console.WriteLine($"root volume ready: {root?.IsReady}");
Console.WriteLine($"separator: {Path.DirectorySeparatorChar}");

try
{
    File.ReadAllText("tree/missing.txt");
}
catch (FileNotFoundException e)
{
    Console.WriteLine($"missing: {e.GetType().Name}");
}
```

```output for=drives-and-links
readme.txt file
src dir
size after move: 8
link target: src/main.txt
is reparse point: True
root volume ready: True
separator: /
missing: FileNotFoundException
```

- Verdict: stdlib. `DirectoryInfo.EnumerateFileSystemInfos` gives a `DirectoryInfo` or a `FileInfo` for each entry. `File.Move` in one directory is a rename. `File.CreateSymbolicLink` makes a symbolic link, and `LinkTarget` reads its path, but .NET 10 has no method that makes a hard link. `DriveInfo.GetDrives` lists the volumes, and on Unix the root directory `/` is one of them.

```csharp run id=symbolic-link-target
using System;
using System.IO;

File.WriteAllText("data.txt", "alpha\n");
File.CreateSymbolicLink("link1.txt", "data.txt");
File.CreateSymbolicLink("link2.txt", "link1.txt");

var info = new FileInfo("link2.txt");
Console.WriteLine($"LinkTarget:    {info.LinkTarget}");
Console.WriteLine($"one step:      {info.ResolveLinkTarget(returnFinalTarget: false)?.Name}");
Console.WriteLine($"final target:  {info.ResolveLinkTarget(returnFinalTarget: true)?.Name}");
Console.WriteLine($"Length:        {info.Length}");

File.Delete("data.txt");
Console.WriteLine($"File.Exists:   {File.Exists("link2.txt")}");
Console.WriteLine($"final exists:  {info.ResolveLinkTarget(returnFinalTarget: true)?.Exists}");
```

```output for=symbolic-link-target
LinkTarget:    link1.txt
one step:      link1.txt
final target:  data.txt
Length:        9
File.Exists:   True
final exists:  False
```

- Verdict: stdlib. `File.CreateSymbolicLink` and `Directory.CreateSymbolicLink` make links, and `LinkTarget` and `ResolveLinkTarget` read them. The `File` class has no call that makes a hard link, so a program calls the system (`link` or `CreateHardLink`). On Unix, a `FileInfo` for a link describes the link: `Length` is 9, the length of the target string `link1.txt`, not the 6 bytes of `data.txt`. `File.Exists` gives `True` for a link whose target is gone, so check the final target.

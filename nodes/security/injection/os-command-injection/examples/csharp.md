```csharp wrong id=sh-c
using System.Diagnostics;

static void LookUp(string name)
{
    var psi = new ProcessStartInfo("sh") { RedirectStandardOutput = true };
    psi.Arguments = "-c \"echo looking up " + name + "\"";
    using var p = Process.Start(psi)!;
    Console.Write(p.StandardOutput.ReadToEnd());
    p.WaitForExit();
}

LookUp("report.txt");
LookUp("report.txt; echo INJECTED");
```

```output for=sh-c
looking up report.txt
looking up report.txt
INJECTED
```

```csharp run id=argument-list
using System.Diagnostics;

static void LookUp(string name)
{
    var psi = new ProcessStartInfo("echo") { RedirectStandardOutput = true };
    psi.ArgumentList.Add("looking up");
    psi.ArgumentList.Add(name);
    using var p = Process.Start(psi)!;
    Console.Write(p.StandardOutput.ReadToEnd());
    p.WaitForExit();
}

LookUp("report.txt");
LookUp("report.txt; echo INJECTED");
```

```output for=argument-list
looking up report.txt
looking up report.txt; echo INJECTED
```

- Verdict: stdlib. `ProcessStartInfo.ArgumentList` holds one item per argument, so each value stays one argument. No shell parses the arguments unless the program starts one, as the first example does.
- `ProcessStartInfo.Arguments` is one string, and the string is split into arguments. A space or a quote in a value changes the split, even with no shell. Prefer `ArgumentList`.
- CodeQL query `cs/command-line-injection` follows input to `Process.Start`.

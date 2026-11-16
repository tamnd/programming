```csharp run id=child
using System;
using System.Diagnostics;

const string Script =
    "echo \"child sees GREETING=$GREETING\"; GREETING=changed; echo \"child set GREETING=$GREETING\"; exit 3";

string greeting = "hello";
var info = new ProcessStartInfo("sh") { RedirectStandardOutput = true };
info.ArgumentList.Add("-c");
info.ArgumentList.Add(Script);
info.Environment["GREETING"] = greeting;
using var child = Process.Start(info)!;
string output = child.StandardOutput.ReadToEnd();
child.WaitForExit();
Console.Write(output);
Console.WriteLine($"child exit status: {child.ExitCode}");
Console.WriteLine($"child pid differs: {child.Id != Environment.ProcessId}");
Console.WriteLine($"parent greeting: {greeting}");
```

```output for=child
child sees GREETING=hello
child set GREETING=changed
child exit status: 3
child pid differs: True
parent greeting: hello
```

- Verdict: stdlib. `System.Diagnostics.Process` starts, watches and stops processes; `ProcessStartInfo` describes the program to start.
- `ArgumentList` passes each argument as it is, with no quoting rules to apply by hand.
- `Environment` in `ProcessStartInfo` starts as a copy of the environment of the parent. The change goes only to the child.
- `Process` holds operating system handles, so the `using` declaration disposes of it.

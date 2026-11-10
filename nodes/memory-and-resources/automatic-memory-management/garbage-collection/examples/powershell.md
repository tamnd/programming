```powershell run id=cycle
class Item {
    [Item] $Peer
}

function New-Cycle {
    $a = [Item]::new()
    $b = [Item]::new()
    $a.Peer = $b
    $b.Peer = $a
    [WeakReference]::new($a)
}

$probe = New-Cycle
"alive while referenced: $($probe.IsAlive)"
[GC]::Collect()
[GC]::WaitForPendingFinalizers()
"collected after gc: $(-not $probe.IsAlive)"
```

```output
alive while referenced: True
collected after gc: True
```

- Verdict: native. PowerShell runs on .NET, so its objects and script classes are freed by the .NET collector.
- `[GC]::Collect()` calls the .NET method directly. `[GC]::WaitForPendingFinalizers()` waits until the finalizers of the collected objects have run.
- `New-Cycle` returns only the weak reference, so no variable of the script keeps `$a` or `$b` alive.

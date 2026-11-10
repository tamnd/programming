```powershell run id=counter
$gate = [object]::new()
$total = [int[]]::new(1)

1..4 | ForEach-Object -Parallel {
    $gate = $using:gate
    $total = $using:total
    foreach ($i in 1..1000) {
        [System.Threading.Monitor]::Enter($gate)
        try {
            $total[0]++
        }
        finally {
            [System.Threading.Monitor]::Exit($gate)
        }
    }
}
"total: $($total[0])"
```

```output
total: 4000
```

- Verdict: idiom. PowerShell has no lock statement. `ForEach-Object -Parallel` runs each script block on its own thread, and the .NET class `System.Threading.Monitor` gives the lock.
- `$using:` passes a reference to a .NET object, so all blocks share `$gate` and the array `$total`. Shared objects that the blocks change must be thread-safe, or a lock must protect them.
- `try` and `finally` make sure that `Monitor.Exit` runs, as the C# `lock` statement does.
- For collections, the documentation uses thread-safe types such as `ConcurrentDictionary` instead of a lock. By default, at most 5 blocks run at a time, and `-ThrottleLimit` changes this number.

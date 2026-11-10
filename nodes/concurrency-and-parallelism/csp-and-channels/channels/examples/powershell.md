```powershell run id=blocking
$jobs = [System.Collections.Concurrent.BlockingCollection[int]]::new(2) # bound 2
$consumer = Start-ThreadJob {
    $total = 0
    foreach ($v in ($using:jobs).GetConsumingEnumerable()) { $total += $v }
    $total
}
1..2 | ForEach-Object -ThrottleLimit 2 -Parallel {
    $q = $using:jobs
    foreach ($i in 1..3) { $q.Add($_ * 10 * $i) } # Add waits while the collection is full
}
$jobs.CompleteAdding() # both producers have finished
"total: $(Receive-Job $consumer -Wait -AutoRemoveJob)"
try { $jobs.Add(1) } catch { $_.Exception.InnerException.GetType().Name }

$item = 0
"TryTake on an empty collection: $($jobs.TryTake([ref]$item, 50))"
```

```output for=blocking
total: 180
InvalidOperationException
TryTake on an empty collection: False
```

- Verdict: stdlib. PowerShell has no channel of its own, but a script can use the .NET class `BlockingCollection<T>`. `ForEach-Object -Parallel` and `Start-ThreadJob` run script blocks on other threads, and `$using:` passes the same object to them.
- `CompleteAdding` closes the collection. `GetConsumingEnumerable` then ends after the last item, and a later `Add` throws.

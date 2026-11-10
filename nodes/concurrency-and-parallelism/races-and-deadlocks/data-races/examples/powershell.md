```powershell run id=concurrent
$counts = [System.Collections.Concurrent.ConcurrentDictionary[string, int]]::new()
1..2 | ForEach-Object -ThrottleLimit 2 -Parallel {
    $shared = $using:counts
    $addOne = [Func[string, int, int]] { param($key, $value) $value + 1 }
    for ($i = 0; $i -lt 10000; $i++) {
        [void]$shared.AddOrUpdate('total', 1, $addOne)
    }
}
Write-Output $counts['total']
```

```output for=concurrent
20000
```

```powershell wrong id=ref-copy
$box = [int[]]::new(1)
1..2 | ForEach-Object -ThrottleLimit 2 -Parallel {
    $shared = $using:box
    for ($i = 0; $i -lt 10000; $i++) {
        [void][System.Threading.Interlocked]::Increment([ref]$shared[0])
    }
}
Write-Output $box[0]
```

```output for=ref-copy
0
```

- Verdict: stdlib. `ForEach-Object -Parallel` runs script blocks on separate threads. When the blocks change shared state, the documentation asks for thread-safe objects, such as the `System.Collections.Concurrent` types.
- `ConcurrentDictionary` locks its writes. `AddOrUpdate` calls the update delegate outside the lock and can call it more than once, so the delegate must have no side effects. Here it only returns `$value + 1`.
- The second program passes `[ref]$shared[0]` to `Interlocked.Increment`. PowerShell makes a reference to a copy of the element, so the array keeps 0. This is a bug of the program, not a data race.

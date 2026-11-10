```powershell run id=parallel
$a = [object]::new()
$b = [object]::new()
$bothHold = [System.Threading.Barrier]::new(2)
$bothTried = [System.Threading.Barrier]::new(2)

$jobs = @(
    @{ Name = 't1'; First = $a; Second = $b },
    @{ Name = 't2'; First = $b; Second = $a }
)
$results = $jobs | ForEach-Object -ThrottleLimit 2 -Parallel {
    $job = $_
    $hold = $using:bothHold
    $tried = $using:bothTried
    [System.Threading.Monitor]::Enter($job.First)
    try {
        $hold.SignalAndWait()
        $got = [System.Threading.Monitor]::TryEnter($job.Second, [timespan]::FromMilliseconds(200))
        if ($got) { [System.Threading.Monitor]::Exit($job.Second) }
        $tried.SignalAndWait()
    }
    finally {
        [System.Threading.Monitor]::Exit($job.First)
    }
    "$($job.Name) got its second lock: $got"
}
$results | Sort-Object
```

```output for=parallel
t1 got its second lock: False
t2 got its second lock: False
```

- Verdict: stdlib. `ForEach-Object -Parallel` runs each script block in its own runspace on its own thread. The scripts share .NET objects through `$using:` and the input objects, so .NET locks work across them.
- PowerShell has no `lock` statement. `[System.Threading.Monitor]::Enter` and `Exit` in a `try` and `finally` do the same work, and `TryEnter` with a timeout gives the try-lock.
- The output of parallel script blocks comes in the order they finish. `Sort-Object` makes it the same on each run.

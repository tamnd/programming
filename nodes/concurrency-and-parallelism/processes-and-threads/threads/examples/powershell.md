```powershell run id=sums
$results = 0..3 | ForEach-Object -ThrottleLimit 4 -Parallel {
    $start = $_ * 250
    $sum = 0
    foreach ($n in $start..($start + 249)) {
        $sum += $n
    }
    [pscustomobject]@{ Index = $_; Sum = $sum }
}

$sorted = $results | Sort-Object Index
$sorted | ForEach-Object { "thread $($_.Index): $($_.Sum)" }
"total: $(($sorted | Measure-Object Sum -Sum).Sum)"

$offset = 1000
$shifted = 1..3 | ForEach-Object -Parallel { $_ + $using:offset } | Sort-Object
$shifted -join ", "
```

```output for=sums
thread 0: 31125
thread 1: 93625
thread 2: 156125
thread 3: 218625
total: 499500
1001, 1002, 1003
```

- Verdict: stdlib. `ForEach-Object -Parallel` runs a script block for each input on a pool of threads in the same process. `-ThrottleLimit` sets the size of the pool, with 5 as the default.
- Each script block runs in its own runspace, so it does not see the variables of the caller. The `$using:` modifier passes a value in.
- The output comes back in the order that the threads finish, so the program sorts it.

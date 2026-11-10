```powershell run id=tree
function Get-Total($Item) {
    if ($Item -is [int]) {
        return $Item
    }
    $sum = 0
    foreach ($child in $Item) {
        $sum += Get-Total $child
    }
    $sum
}

function Get-Factorial([int]$N) {
    if ($N -le 1) {
        return 1
    }
    $N * (Get-Factorial ($N - 1))
}

Get-Total @(1, @(2, 3), @(@(4), 5))
Get-Factorial 10
```

```output for=tree
15
3628800
```

- Verdict: native. A function can call itself by name, and each call gets its own scope for its variables.
- A function call in PowerShell does much more work than a call in a compiled language, so keep recursion shallow.
- A function writes every value that it does not capture to its output. A recursive function must keep stray values out of the output, or the caller gets them too.

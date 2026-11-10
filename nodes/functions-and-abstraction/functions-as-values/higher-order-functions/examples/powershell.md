```powershell run id=hof
function Invoke-Twice {
    param([scriptblock]$Action, $Value)
    & $Action (& $Action $Value)
}

function New-Composed {
    param([scriptblock]$F, [scriptblock]$G)
    { param($x) & $F (& $G $x) }.GetNewClosure()
}

$words = 'pear', 'fig', 'banana'
Invoke-Twice { param($n) $n * 3 } 2
($words | ForEach-Object { $_.Length }) -join ', '
($words | Where-Object { $_.Length -gt 3 }) -join ', '
($words | Sort-Object { $_.Length }) -join ', '
$addThenDouble = New-Composed { param($n) $n * 2 } { param($n) $n + 1 }
& $addThenDouble 5
```

```output for=hof
18
4, 3, 6
pear, banana
fig, pear, banana
12
```

- Verdict: native. A script block is a value of type `ScriptBlock`. A function can take it as a parameter, store it and return it.
- The call operator `&` runs a script block with arguments. `ForEach-Object`, `Where-Object` and `Sort-Object` take a script block and see the current item as `$_`.
- A returned script block does not keep the variables of the function that made it. `GetNewClosure()` copies them, so the composed block can see `$F` and `$G` (see [[closures]]).

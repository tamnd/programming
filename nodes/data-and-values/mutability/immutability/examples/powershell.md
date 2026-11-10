```powershell run id=readonly
Set-Variable -Name Limit -Value 10 -Option ReadOnly
try {
    $Limit = 20
} catch {
    Write-Output $_.Exception.GetType().Name
}
Write-Output $Limit

Set-Variable -Name Limit -Value 30 -Force
Write-Output $Limit

Set-Variable -Name Items -Value ([System.Collections.ArrayList]@(1, 2)) -Option Constant
[void]$Items.Add(3)
Write-Output $Items.Count
```

```output for=readonly
SessionStateUnauthorizedAccessException
10
30
3
```

- Verdict: native. `Set-Variable -Option ReadOnly` makes a variable that a plain assignment cannot change. The `-Force` parameter can still change it, and the variable can be deleted.
- `-Option Constant` makes a variable that cannot be changed or deleted. The option is valid only when the variable is created.
- Both options protect the variable, not the object in it. Here the constant `$Items` holds an `ArrayList`, and `Add` still changes the list.
- .NET immutable types, such as `string` and the types in `System.Collections.Immutable`, give immutable values.

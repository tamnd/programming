```powershell run id=null
$value = $null
Write-Output "[$value]"
Write-Output ($null -eq $value)

$ages = @{ ada = 36 }
$age = $ages['bob']
Write-Output ($null -eq $age)
Write-Output ($age ?? 0)

$items = 'one', $null, 'three'
Write-Output $items.Count

$array = @($null)
Write-Output ([bool]($array -eq $null))
Write-Output ([bool]($array -ne $null))
Write-Output ($null -eq $array)
```

```output
[]
True
True
0
3
False
False
False
```

- Verdict: native. `$null` is an automatic variable that holds a null value. A missing key in a hashtable, or a variable that was never assigned, gives `$null`.
- In a string, `$null` becomes an empty string, so `"[$value]"` prints `[]`. In a collection, `$null` counts as an element, so `$items.Count` is 3.
- Calling a method on `$null` raises the error "You cannot call a method on a null-valued expression".
- Put `$null` on the left of `-eq` and `-ne`. With an array on the left, `-eq` and `-ne` filter the array and do not test it. For `@($null)` both filters give a result that is false, as the output shows. `$null -eq $array` tests the array itself.
- The `??` operator gives a default value when the left operand is `$null`.

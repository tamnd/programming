```powershell run id=change
function Set-Copy($n) {
    $n = 42
    "inside Set-Copy: $n"
}

function Set-Reference([ref]$n) {
    $n.Value = 42
}

$x = 1
Set-Copy $x
"after Set-Copy: $x"
Set-Reference ([ref]$x)
"after Set-Reference: $x"

$table = @{ count = 1 }
$alias = $table
$alias.count = 100
"table.count: $($table.count)"
"same object: $([object]::ReferenceEquals($table, $alias))"
```

```output for=change
inside Set-Copy: 42
after Set-Copy: 1
after Set-Reference: 42
table.count: 100
same object: True
```

- Verdict: native. A variable holds a .NET value or a reference to a .NET object. A hashtable is an object, so `$alias` and `$table` share it.
- A parameter receives a copy of the argument. An integer is a value type, so `Set-Copy` changes only its own copy.
- `[ref]` is the type accelerator for `System.Management.Automation.PSReference`. The function reads and writes the variable of the caller through the `Value` property.
- The call must wrap the variable in `[ref]` and parentheses, as in `Set-Reference ([ref]$x)`.

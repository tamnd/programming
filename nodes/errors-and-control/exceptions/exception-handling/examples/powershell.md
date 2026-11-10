```powershell run id=ports
class ConfigException : System.Exception {
    ConfigException([string] $message, [System.Exception] $inner) : base($message, $inner) {}
}

function ConvertTo-Port([string] $Text) {
    try {
        $port = [int]::Parse($Text)
    } catch [System.FormatException] {
        throw [ConfigException]::new("not a number: $Text", $_.Exception)
    }
    if ($port -lt 1 -or $port -gt 65535) {
        throw [ConfigException]::new("out of range: $port", $null)
    }
    $port
}

$checked = 0
foreach ($text in '8080', '70000', 'abc') {
    try {
        "port $(ConvertTo-Port $text)"
    } catch [ConfigException] {
        $inner = $_.Exception.InnerException
        $cause = if ($inner) { $inner.GetType().Name } else { 'none' }
        "error: $($_.Exception.Message) (cause: $cause)"
    } finally {
        $checked++
    }
}
"checked $checked"
```

```output
port 8080
error: out of range: 70000 (cause: none)
error: not a number: abc (cause: FormatException)
checked 3
```

- Verdict: native. `throw` makes a terminating error, and `try` with typed `catch` blocks and `finally` handles it. A `trap` statement is another kind of handler, for a whole scope.
- In a `catch` block, `$_` is an `ErrorRecord`. Its `Exception` property holds the .NET exception.
- `[int]::Parse` is a .NET method, so it throws `FormatException`, and the `catch` block can match its type.
- Most cmdlet errors are non-terminating. They go to the error stream and do not reach `catch`, unless `-ErrorAction Stop` turns them into terminating errors (see Pitfalls on the concept page).

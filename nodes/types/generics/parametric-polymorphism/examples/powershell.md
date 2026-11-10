```powershell run id=list
$numbers = [System.Collections.Generic.List[int]]::new()
$numbers.Add(3)
$numbers.Add("9")
$numbers.Add(4.0)
$numbers -join ", "
$numbers.GetType().GenericTypeArguments[0].Name
$empty = [array]::Empty[string]()
$empty.GetType().Name
try {
    $numbers.Add("nine")
} catch {
    $_.Exception.GetType().Name
}
```

```output for=list
3, 9, 4
Int32
String[]
MethodException
```

- Verdict: stdlib. PowerShell uses the generic types and methods of .NET. The type arguments go in square brackets, as in `List[int]`.
- A call such as `[array]::Empty[string]()` gives the type argument of a generic method explicitly.
- PowerShell converts each argument to the type argument when it can. Here the string `"9"` and the number `4.0` become `int` values, and only `"nine"` fails.
- The type argument exists at run time, because .NET keeps it.

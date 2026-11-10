```powershell run id=countdown
$e = @(3, 2, 1).GetEnumerator()
while ($e.MoveNext()) {
    Write-Output $e.Current
}

foreach ($n in 'a', 'b') {
    Write-Output $n
}

$table = @{ one = 1; two = 2 }
Write-Output (@(1, 2, 3) | Measure-Object).Count
Write-Output ($table | Measure-Object).Count
Write-Output ($table.GetEnumerator() | Measure-Object).Count
```

```output for=countdown
3
2
1
a
b
3
1
2
```

- Verdict: native. PowerShell uses the .NET enumerator protocol: `GetEnumerator()`, `MoveNext()` and `Current`.
- The pipeline enumerates any value that implements `IEnumerable` and sends the items one at a time. So the array gives three objects to `Measure-Object`.
- A hashtable is an exception. The pipeline sends it as one object, so the count is 1. Call `GetEnumerator()` to send its entries one at a time.
- A string is also sent as one object, not as a sequence of characters.

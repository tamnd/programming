```powershell run id=types
$value = 42
$value.GetType().Name
$value = "abc"
$value.GetType().Name

[int]$count = "42"
$count + 1
$count.GetType().Name
try {
    $count = "abc"
} catch {
    $_.Exception.GetType().Name
}

"5" + 2
5 + "2"
```

```output
Int32
String
43
Int32
ArgumentTransformationMetadataException
52
7
```

- Verdict: native. PowerShell variables are loosely typed. A variable can hold an object of any .NET type, and the type goes with the object.
- A type in square brackets before the variable fixes its type. Each assignment converts the value to that type, and an assignment fails when the value cannot be converted.
- The left operand decides the operation of `+`. A string on the left concatenates, and a number on the left converts the right operand to a number.

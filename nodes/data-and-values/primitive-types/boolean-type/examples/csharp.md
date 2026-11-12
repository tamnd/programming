```csharp run id=nullable-three-valued
// bool has no conversion from int; a comparison gives a bool.
int x = 5;
bool nonZero = x != 0;
Console.WriteLine($"{nonZero} {x > 3 && x < 10} {!nonZero} {true ^ false}");

// bool? adds null, and & and | follow three-valued logic.
bool? unknown = null;
bool? t = true, f = false;
string Show(bool? v) => v?.ToString() ?? "null";
Console.WriteLine($"{Show(unknown & f)} {Show(unknown & t)} {Show(unknown | t)} {Show(unknown | f)}");

// Parsing and conversion.
Console.WriteLine($"{bool.Parse("True")} {bool.TryParse("yes", out var parsed)} {parsed} {Convert.ToBoolean(5)} {Convert.ToInt32(true)}");

// A bool takes 1 byte, and its default value is false.
Console.WriteLine($"{sizeof(bool)} {default(bool)}");
```

```output for=nullable-three-valued
True True False True
False null True null
True False False True 1
1 False
```

- Verdict: native. A comparison gives a `bool`, and there is no conversion from `int`. `bool?` adds `null`, and `&` and `|` follow three-valued logic: `null & false` is `False` and `null | true` is `True`, but `null & true` and `null | false` are `null`. `bool.TryParse("yes")` fails, and `Convert.ToBoolean(5)` is `True`. `sizeof(bool)` is 1.

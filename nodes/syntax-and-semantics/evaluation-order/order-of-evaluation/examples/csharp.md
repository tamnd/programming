```csharp run id=trace
static int F(string name, int value)
{
    Console.WriteLine($"evaluate {name}={value}");
    return value;
}

int i = 1;
int result = F("F", i) + F("G", i++) * F("H", i);
Console.WriteLine(result);

int a = 9;
a += (a = 3);
Console.WriteLine(a);
```

```output for=trace
evaluate F=1
evaluate G=1
evaluate H=2
3
12
```

- Verdict: native. The C# specification states that the operands of an expression are evaluated from left to right. Its example is `F(i) + G(i++) * H(i)`: `F` gets the old value of `i`, `G` gets the old value, and `H` gets the new value.
- Precedence decides only that the multiplication happens before the addition. It does not move the call to `F` to a later point.
- The left operand of a compound assignment is evaluated once and first, as in Java.

```csharp wrong id=unchecked
static bool OrderFits(int price, int quantity, int budget)
{
    int total = price * quantity; // wraps on overflow
    return total <= budget;
}

Console.WriteLine($"3 items: {OrderFits(100, 3, 1000)}");
Console.WriteLine($"42949673 items: {OrderFits(100, 42949673, 1000)}");
```

```output for=unchecked
3 items: True
42949673 items: True
```

The default context is unchecked. The product loses its high-order bits and becomes 4.

```csharp run id=checked
static bool OrderFits(int price, int quantity, int budget)
{
    if (price < 0 || quantity < 0)
    {
        return false;
    }
    try
    {
        return checked(price * quantity) <= budget;
    }
    catch (OverflowException e)
    {
        Console.WriteLine($"refused: {e.Message}");
        return false;
    }
}

Console.WriteLine($"3 items: {OrderFits(100, 3, 1000)}");
Console.WriteLine($"42949673 items: {OrderFits(100, 42949673, 1000)}");
```

```output for=checked
3 items: True
refused: Arithmetic operation resulted in an overflow.
42949673 items: False
```

- Verdict: native. A `checked` expression or block throws `System.OverflowException` on overflow. An overflow in a constant expression is a compile-time error.
- The default context is unchecked, and it discards the high-order bits. The project option `CheckForOverflowUnderflow` changes the default for a whole project.
- `System.Numerics.BigInteger` has no fixed width.

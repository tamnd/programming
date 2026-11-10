```csharp norun reason=fragment id=cart file=CartTests.cs
using Xunit;

public record Item(int Price, int Quantity);

public static class Cart
{
    public static int Total(IEnumerable<Item> items)
    {
        var total = 0;
        foreach (var item in items)
        {
            if (item.Quantity < 0)
            {
                throw new ArgumentOutOfRangeException(nameof(items), $"negative quantity: {item.Quantity}");
            }
            total += item.Price * item.Quantity;
        }
        return total;
    }
}

public class CartTests
{
    [Fact]
    public void EmptyCartIsZero()
    {
        Assert.Equal(0, Cart.Total([]));
    }

    [Theory]
    [InlineData(250, 2, 500)]
    [InlineData(100, 1, 100)]
    public void MultipliesPriceByQuantity(int price, int quantity, int expected)
    {
        Assert.Equal(expected, Cart.Total([new Item(price, quantity)]));
    }

    [Fact]
    public void RejectsNegativeQuantity()
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => Cart.Total([new Item(250, -1)]));
    }
}
```

The command `dotnet test`, in a test project that references the packages `xunit`, `xunit.runner.visualstudio` and `Microsoft.NET.Test.Sdk`, prints at the end:

```text
Passed!  - Failed:     0, Passed:     4, Skipped:     0, Total:     4, Duration: 12 ms - Cart.Tests.dll (net10.0)
```

- Verdict: library. .NET has no test framework in its base class library. xUnit.net, NUnit and MSTest are packages, and `dotnet test` runs any of them through the test platform.
- `[Fact]` marks a test with no parameters. `[Theory]` marks a test that runs once for each `[InlineData]` row, so the report counts 4 tests for the 3 methods.
- xUnit.net makes a separate instance of the test class for each test. The constructor is the setup step, and `Dispose` is the teardown step.
- This block does not run on this page, because it needs a test project and packages from NuGet, and the runner of this site builds one file with the standard library only.

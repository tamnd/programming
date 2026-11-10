```csharp run id=invoice
using System.Text;

var items = new List<Item> { new("pen", 250, 2), new("pad", 1205, 1) };
var expected = "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05";
var versions = new (string Name, Func<string, List<Item>, string> Make)[]
{
    ("InvoiceBefore", Invoices.InvoiceBefore),
    ("InvoiceAfter", Invoices.InvoiceAfter),
};
foreach (var (name, make) in versions)
{
    if (make("Ada", items) != expected || make("Bob", []) != "Invoice for Bob\nTotal: 0.00")
    {
        throw new Exception($"{name} failed");
    }
    Console.WriteLine($"{name}: tests pass");
}
Console.WriteLine(Invoices.InvoiceAfter("Ada", items));

record Item(string Name, int Price, int Quantity);

static class Invoices
{
    public static string InvoiceBefore(string customer, List<Item> items)
    {
        var text = new StringBuilder($"Invoice for {customer}\n");
        foreach (var item in items)
        {
            text.Append($"  {item.Name}: {item.Quantity} x {item.Price / 100}.{item.Price % 100:D2}\n");
        }
        // compute the total
        var total = 0;
        foreach (var item in items)
        {
            total += item.Price * item.Quantity;
        }
        text.Append($"Total: {total / 100}.{total % 100:D2}");
        return text.ToString();
    }

    static string FormatCents(int cents) => $"{cents / 100}.{cents % 100:D2}";

    static int TotalCents(List<Item> items) => items.Sum(item => item.Price * item.Quantity);

    public static string InvoiceAfter(string customer, List<Item> items)
    {
        var text = new StringBuilder($"Invoice for {customer}\n");
        foreach (var item in items)
        {
            text.Append($"  {item.Name}: {item.Quantity} x {FormatCents(item.Price)}\n");
        }
        text.Append($"Total: {FormatCents(TotalCents(items))}");
        return text.ToString();
    }
}
```

```output for=invoice
InvoiceBefore: tests pass
InvoiceAfter: tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

- Verdict: idiom. The refactoring makes a method, often `private` and `static`, or a local function inside the source method.
- A fragment that assigns two locals can return a tuple, `(int Count, int Total)`, or use `out` parameters.
- Expression-bodied members (`=>`) suit a short extracted method such as `FormatCents`.
- Visual Studio has Extract Method as one command.

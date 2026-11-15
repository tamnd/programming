```csharp run id=translate-orders
using System;
using System.Text.RegularExpressions;

var partner = new[]
{
    "P-17;Lan Pham;12.50;03/11/2026",
    "P-18;Hoa Le;8;05/11/2026",
    "P-19;Minh Tran;7,25;04/11/2026",
};

foreach (var line in partner)
{
    try
    {
        var o = Translator.Translate(line);
        Console.WriteLine($"order {o.Id}: customer={o.Customer} total_cents={o.TotalCents} date={o.Date}");
    }
    catch (FormatException e)
    {
        Console.WriteLine($"invalid: {line} ({e.Message})");
    }
}

// The format of the shop.
record Order(int Id, string Customer, int TotalCents, string Date);

static class Translator
{
    static readonly Regex Amount = new(@"^(\d+)(?:\.(\d\d))?$");

    // Translate one partner line (id;customer;amount;day/month/year) into an Order.
    public static Order Translate(string line)
    {
        var f = line.Split(';');
        if (f.Length != 4) throw new FormatException("want 4 fields");
        var m = Amount.Match(f[2]);
        if (!m.Success) throw new FormatException("bad amount " + f[2]);
        int cents = int.Parse(m.Groups[1].Value) * 100 + (m.Groups[2].Success ? int.Parse(m.Groups[2].Value) : 0);
        var d = f[3].Split('/');
        if (d.Length != 3) throw new FormatException("bad date " + f[3]);
        int id = int.Parse(f[0].StartsWith("P-") ? f[0][2..] : f[0]);
        return new Order(id, f[1], cents, $"{d[2]}-{d[1]}-{d[0]}");
    }
}
```

```output
order 17: customer=Lan Pham total_cents=1250 date=2026-11-03
order 18: customer=Hoa Le total_cents=800 date=2026-11-05
invalid: P-19;Minh Tran;7,25;04/11/2026 (bad amount 7,25)
```

- Verdict: idiom. A positional record is the target format, and a static method maps a partner line to a new record.
- The method throws `FormatException` with a reason; `int.Parse` throws the same type for a bad id, so one `catch` handles all bad lines.
- The range `f[0][2..]` removes the prefix of the partner id.

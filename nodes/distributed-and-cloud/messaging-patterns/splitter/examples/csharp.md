```csharp run id=split-order
using System;
using System.Collections.Generic;
using System.Linq;

var orders = new List<(string Id, Line[] Lines)>
{
    ("A7", new[] { new Line("widget", 2), new Line("gadget", 1), new Line("widget", 5) }),
    ("B9", Array.Empty<Line>()),
};
var channel = new Dictionary<string, string> { ["widget"] = "inventory.widgets", ["gadget"] = "inventory.gadgets" };

foreach (var (id, lines) in orders)
{
    Console.WriteLine($"order {id} has {lines.Length} lines");
    foreach (var m in Splitter.Split(id, lines))
    {
        if (m.Line is null)
        {
            Console.WriteLine($"  {id} 0/0 empty order -> orders.empty");
            continue;
        }
        Console.WriteLine($"  {id} {m.Seq}/{m.Size} {m.Line.Kind} x{m.Line.Qty} -> {channel[m.Line.Kind]}");
    }
}

record Line(string Kind, int Qty);

// One message from the splitter, with the headers that an aggregator needs.
record Msg(string CorrelationId, int Seq, int Size, Line? Line);

static class Splitter
{
    public static IEnumerable<Msg> Split(string orderId, Line[] lines)
    {
        if (lines.Length == 0)
        {
            // One message with size 0, so that a later aggregator is not left waiting.
            return new[] { new Msg(orderId, 0, 0, null) };
        }
        return lines.Select((line, i) => new Msg(orderId, i + 1, lines.Length, line));
    }
}
```

```output
order A7 has 3 lines
  A7 1/3 widget x2 -> inventory.widgets
  A7 2/3 gadget x1 -> inventory.gadgets
  A7 3/3 widget x5 -> inventory.widgets
order B9 has 0 lines
  B9 0/0 empty order -> orders.empty
```

- Verdict: idiom. The overload of `Select` with an index makes one record for each line, with the sequence number `i + 1`.
- `Select` is lazy, so the parts are made as the loop reads them; the size is known here because the input is an array.
- The nullable `Line?` in the record marks the message for an empty order.

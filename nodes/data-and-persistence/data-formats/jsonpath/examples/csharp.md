```csharp run id=get-path
using System.Text.Json.Nodes;

JsonNode doc = JsonNode.Parse("""{"store": {"book": [{"title": "Moby Dick", "price": 8.99}], "a b": {"it's": 1}}}""")!;

// The query $..* in a few lines: every descendant of the root, each node before its descendants.
foreach (JsonNode node in Descendants(doc))
{
    string value = node is JsonValue ? node.ToJsonString() : node.GetType().Name;
    Console.WriteLine($"{node.GetPath(),-26} {value}");
}

static IEnumerable<JsonNode> Descendants(JsonNode node)
{
    IEnumerable<JsonNode?> children = node switch
    {
        JsonObject o => o.Select(member => member.Value),
        JsonArray a => a,
        _ => [],
    };
    foreach (JsonNode? child in children)
    {
        if (child is null) continue;
        yield return child;
        foreach (JsonNode d in Descendants(child)) yield return d;
    }
}
```

```output for=get-path
$.store                    JsonObject
$.store.book               JsonArray
$.store.book[0]            JsonObject
$.store.book[0].title      "Moby Dick"
$.store.book[0].price      8.99
$.store['a b']             JsonObject
$.store['a b']['it's']     1
```

- Verdict: stdlib. `System.Text.Json.Nodes` has no JSONPath query function, but the method `JsonNode.GetPath` returns the path of a node as a string. The walk above visits each node before its descendants, as the descendant segment of RFC 9535 does. The output shows that `GetPath` is not a Normalized Path: it uses dot notation for simple names, and it does not escape the apostrophe in `it's`, so the path `$.store['a b']['it's']` is not a well-formed JSONPath query. Use the path for messages and logs, and do not parse it again as a query.

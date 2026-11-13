```csharp run id=resolve
using System.Text.Json.Nodes;
using System.Text.RegularExpressions;

JsonNode? doc = JsonNode.Parse("""{"items": [{"id": 7, "tags": ["new"]}], "a/b": null}""");
foreach (string p in new[] { "/items/0/id", "/items/0/tags", "/a~1b", "/items/7", "/missing" })
{
    try
    {
        JsonNode? v = Resolve(doc, p);
        Console.WriteLine($"{p} {v?.ToJsonString() ?? "null"}");
    }
    catch (KeyNotFoundException e)
    {
        Console.WriteLine($"{p} error: {e.Message}");
    }
}

// Resolve returns the node that an RFC 6901 pointer identifies. A JSON null is a C# null.
static JsonNode? Resolve(JsonNode? doc, string pointer)
{
    if (pointer == "") return doc;
    if (!pointer.StartsWith('/')) throw new FormatException($"bad pointer {pointer}");
    JsonNode? value = doc;
    foreach (string raw in pointer[1..].Split('/'))
    {
        string token = raw.Replace("~1", "/").Replace("~0", "~");
        if (value is JsonObject obj && obj.TryGetPropertyValue(token, out JsonNode? member))
            value = member;
        else if (value is JsonArray arr && Regex.IsMatch(token, "^(0|[1-9][0-9]*)$") && int.TryParse(token, out int i) && i < arr.Count)
            value = arr[i];
        else
            throw new KeyNotFoundException($"no value at token {token}");
    }
    return value;
}
```

```output for=resolve
/items/0/id 7
/items/0/tags ["new"]
/a~1b null
/items/7 error: no value at token 7
/missing error: no value at token missing
```

- Verdict: stdlib. `System.Text.Json.Nodes` gives a mutable tree of `JsonObject`, `JsonArray` and `JsonValue` nodes, and the resolver walks it one token at a time. The library has no JSON Pointer function, so the code follows the evaluation steps of RFC 6901. A JSON `null` is a C# `null` in this tree, so the resolver uses `TryGetPropertyValue` to tell a member with the value `null` (`/a~1b`) from a member that does not exist (`/missing`).

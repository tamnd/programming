```csharp run id=test-op
using System.Text.Json.Nodes;

JsonNode doc = JsonNode.Parse("""{"price": 10, "active": true, "tags": ["a", "b"]}""")!;
string[] patches =
[
    """[{"op": "test", "path": "/price", "value": 10.0}, {"op": "replace", "path": "/price", "value": 12}]""",
    """[{"op": "test", "path": "/active", "value": 1}, {"op": "replace", "path": "/active", "value": false}]""",
    """[{"op": "test", "path": "/tags", "value": ["b", "a"]}]""",
];
foreach (string text in patches)
{
    try
    {
        doc = Apply(doc, JsonNode.Parse(text)!.AsArray());
        Console.WriteLine($"ok    {doc.ToJsonString()}");
    }
    catch (InvalidOperationException e)
    {
        Console.WriteLine($"error {e.Message}");
    }
}

// Apply runs the test and replace operations of RFC 6902 on a copy of doc.
static JsonNode Apply(JsonNode doc, JsonArray patch)
{
    JsonNode result = doc.DeepClone();
    foreach (JsonNode? op in patch)
    {
        string path = (string)op!["path"]!;
        JsonObject parent = result.AsObject();
        string name = path[1..];
        if (!parent.TryGetPropertyValue(name, out JsonNode? current))
            throw new InvalidOperationException($"no value at {path}");
        switch ((string)op["op"]!)
        {
            case "test":
                if (!JsonNode.DeepEquals(current, op["value"]))
                    throw new InvalidOperationException($"test failed at {path}");
                break;
            case "replace":
                parent[name] = op["value"]?.DeepClone();
                break;
            default:
                throw new InvalidOperationException("this example has only test and replace");
        }
    }
    return result;
}
```

```output for=test-op
ok    {"price":12,"active":true,"tags":["a","b"]}
error test failed at /active
error test failed at /tags
```

- Verdict: stdlib. `System.Text.Json.Nodes` parses the document and the patch into nodes, and `DeepClone` gives the copy that the operations change, so a failed patch leaves `doc` as it was. `JsonNode.DeepEquals` "Compares the values of two nodes, including the values of all descendant nodes". In the run it treats `10` and `10.0` as equal, it does not treat `true` and `1` as equal, and it compares array elements in order. These are the results that the `test` rules of RFC 6902 require. .NET has no JSON Patch type in the base library, so the example handles only `test` and `replace` on members of the root object. For a full implementation, ASP.NET Core web APIs use the NuGet package `Microsoft.AspNetCore.JsonPatch.SystemTextJson`.

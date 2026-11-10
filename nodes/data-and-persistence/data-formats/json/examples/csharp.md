```csharp run id=parse-emit
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Text.Json.Serialization;

const string text = """{"id": 9007199254740993, "total": 0.10, "tags": ["new"], "paid": false}""";

Order order = JsonSerializer.Deserialize(text, AppJson.Default.Order)!;
Console.WriteLine($"{order.Id} {order.Total} {string.Join(",", order.Tags)} {order.Paid}");
Console.WriteLine(JsonSerializer.Serialize(order, AppJson.Default.Order));

JsonNode tree = JsonNode.Parse(text)!;
Console.WriteLine(tree["id"]!.GetValue<long>());
Console.WriteLine(tree["id"]!.GetValue<double>());

[JsonSourceGenerationOptions(PropertyNamingPolicy = JsonKnownNamingPolicy.CamelCase)]
[JsonSerializable(typeof(Order))]
partial class AppJson : JsonSerializerContext;

record Order(long Id, decimal Total, string[] Tags, bool Paid);
```

```output for=parse-emit
9007199254740993 0.10 new False
{"id":9007199254740993,"total":0.10,"tags":["new"],"paid":false}
9007199254740993
9007199254740992
```

```csharp run id=round-trip
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Text.Json.Serialization;

const string repeated = """{"total": 1, "total": 2}""";
Console.WriteLine(JsonNode.Parse(repeated)!.ToJsonString());
Console.WriteLine(JsonSerializer.Deserialize(repeated, AppJson.Default.Invoice)!.Total);

try
{
    JsonNode.Parse(repeated, documentOptions: new JsonDocumentOptions { AllowDuplicateProperties = false });
}
catch (JsonException error)
{
    Console.WriteLine(error.Message);
}

Console.WriteLine(JsonSerializer.Serialize("Hà Nội <b>", AppJson.Default.String));

try
{
    JsonSerializer.Serialize(double.NaN, AppJson.Default.Double);
}
catch (ArgumentException error)
{
    Console.WriteLine(error.GetType().Name);
}

[JsonSourceGenerationOptions(PropertyNamingPolicy = JsonKnownNamingPolicy.CamelCase)]
[JsonSerializable(typeof(Invoice))]
[JsonSerializable(typeof(string))]
[JsonSerializable(typeof(double))]
partial class AppJson : JsonSerializerContext;

record Invoice(int Total);
```

```output for=round-trip
{"total":1,"total":2}
2
Duplicate property 'total' encountered during deserialization.
"H\u00E0 N\u1ED9i \u003Cb\u003E"
ArgumentException
```

- Verdict: stdlib. `System.Text.Json` is part of .NET. `JsonSerializer` maps JSON to types, `JsonDocument` gives a read-only tree, `JsonNode` gives a tree that can change, and `Utf8JsonReader` and `Utf8JsonWriter` read and write tokens.
- The examples use a source-generated `JsonSerializerContext`. A file-based app is compiled for native AOT by default, and that turns off the reflection-based serializer.
- A `long` field keeps the integer exactly, and a `decimal` keeps `0.10` with its scale. `GetValue<double>()` rounds to the nearest double.
- `JsonNode.Parse` keeps both members of a repeated name, and the serializer keeps the last value. `AllowDuplicateProperties = false` makes both reject the text.
- The default encoder escapes non-ASCII characters and the HTML characters `<`, `>`, `&` and `'`. `JavaScriptEncoder.UnsafeRelaxedJsonEscaping` or an encoder with a wider `UnicodeRange` writes more characters as they are.
- `NaN` and the infinities throw, unless `JsonNumberHandling.AllowNamedFloatingPointLiterals` is set.

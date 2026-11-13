```csharp run id=export
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Text.Json.Schema;

// Generate a schema from a .NET type: the schema describes the JSON that the serializer reads and writes.
JsonSerializerOptions options = new(JsonSerializerOptions.Default) { RespectNullableAnnotations = true };
JsonNode schema = options.GetJsonSchemaAsNode(typeof(User));

Console.WriteLine($"type      {schema["type"]!.ToJsonString()}");
foreach ((string name, JsonNode? property) in schema["properties"]!.AsObject())
{
    Console.WriteLine($"{name,-9} {property!.ToJsonString()}");
}
Console.WriteLine($"required  {schema["required"]!.ToJsonString()}");

record User(int Id, string Name, string? Email, Role Role, List<string> Tags);

enum Role { Admin, User }
```

```output for=export
type      ["object","null"]
Id        {"type":"integer"}
Name      {"type":"string"}
Email     {"type":["string","null"]}
Role      {"type":"integer"}
Tags      {"type":"array","items":{"type":["string","null"]}}
required  ["Id","Name","Email","Role","Tags"]
```

- Verdict: library. .NET has no JSON Schema validator in the base library, but the class `JsonSchemaExporter` in `System.Text.Json.Schema` generates a schema from a type. This is the code generation direction: the type is the source, and the schema describes its JSON contract. The output shows the choices of the serializer. A reference type such as `User` also allows `null`. The enum `Role` is an `integer`, because the default serializer writes enum values as numbers. All the parameters of the constructor are in `required`, also the nullable `Email`. The items of `Tags` allow `null`, although the type argument is `string` and not `string?`. To validate data against a schema, use a validator library.

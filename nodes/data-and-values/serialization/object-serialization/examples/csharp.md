```csharp run id=system-text-json-source-generator
using System.Text.Json;
using System.Text.Json.Serialization;

var ada = new Person("Ada", 1815, ["math"]);
string text = JsonSerializer.Serialize(ada, Ctx.Default.Person); // code made at compile time
Console.WriteLine(text);
var back = JsonSerializer.Deserialize(text, Ctx.Default.Person)!;
Console.WriteLine($"{back.Name} {back.Born} tags equal: {back.Tags.SequenceEqual(ada.Tags)}");

var web = new JsonSerializerOptions(JsonSerializerDefaults.Web) { TypeInfoResolver = Ctx.Default };
Console.WriteLine(JsonSerializer.Serialize(ada, web.GetTypeInfo(typeof(Person))));
Console.WriteLine(JsonSerializer.Serialize(new Login("ada", "secret"), Ctx.Default.Login));

const string newer = """{"Name":"Alan","Born":1912,"Died":1954}""";
var alan = JsonSerializer.Deserialize(newer, Ctx.Default.Person)!;
Console.WriteLine($"{alan.Name} {alan.Born}, Died ignored, Tags null: {alan.Tags is null}");
var strict = new JsonSerializerOptions
{
    UnmappedMemberHandling = JsonUnmappedMemberHandling.Disallow,
    TypeInfoResolver = Ctx.Default,
};
try
{
    JsonSerializer.Deserialize(newer, strict.GetTypeInfo(typeof(Person)));
}
catch (JsonException)
{
    Console.WriteLine("strict: JsonException for Died");
}

record Person(string Name, int Born, List<string> Tags);

record Login(string User, [property: JsonIgnore] string Password);

[JsonSerializable(typeof(Person))]
[JsonSerializable(typeof(Login))]
partial class Ctx : JsonSerializerContext;
```

```output for=system-text-json-source-generator
{"Name":"Ada","Born":1815,"Tags":["math"]}
Ada 1815 tags equal: True
{"name":"Ada","born":1815,"tags":["math"]}
{"User":"ada"}
Alan 1912, Died ignored, Tags null: True
strict: JsonException for Died
```

- Verdict: stdlib. `System.Text.Json` is part of .NET. A `JsonSerializerContext` with `[JsonSerializable]` attributes makes the serialization code at compile time, which works with native AOT and starts faster than reflection. The web defaults change property names to camel case, `[JsonIgnore]` leaves a property out, and by default unknown properties are ignored and missing ones get default values; `JsonUnmappedMemberHandling.Disallow` makes unknown properties an error. `BinaryFormatter` throws on use since .NET 9, because it could not be made secure.

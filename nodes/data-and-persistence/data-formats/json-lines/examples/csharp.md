```csharp run id=read-write
using System.Text;
using System.Text.Json;

string path = Path.GetTempFileName();
try
{
    var records = new[] { new Event("info", 1), new Event("warn", 2) };
    // Serialize writes no line break inside a value, so each record is one line.
    File.WriteAllLines(path, records.Select(e => JsonSerializer.Serialize(e, EventJson.Default.Event)), new UTF8Encoding(false));
    Console.Write(File.ReadAllText(path));

    // With topLevelValues: true, the serializer reads a sequence of values instead of one array.
    using FileStream stream = File.OpenRead(path);
    await foreach (Event? e in JsonSerializer.DeserializeAsyncEnumerable(stream, EventJson.Default.Event, topLevelValues: true))
    {
        Console.WriteLine(e);
    }
}
finally
{
    File.Delete(path);
}

record Event(string Level, int N);

[System.Text.Json.Serialization.JsonSourceGenerationOptions(PropertyNamingPolicy = System.Text.Json.Serialization.JsonKnownNamingPolicy.CamelCase)]
[System.Text.Json.Serialization.JsonSerializable(typeof(Event))]
partial class EventJson : System.Text.Json.Serialization.JsonSerializerContext;
```

```output for=read-write
{"level":"info","n":1}
{"level":"warn","n":2}
Event { Level = info, N = 1 }
Event { Level = warn, N = 2 }
```

- Verdict: stdlib. `JsonSerializer.Serialize` writes each record with no line break, and `File.WriteAllLines` adds the line ends. A source-generated `JsonSerializerContext` gives the type information. `DeserializeAsyncEnumerable` with `topLevelValues: true` reads the values one after the other from the stream, with no array around them.

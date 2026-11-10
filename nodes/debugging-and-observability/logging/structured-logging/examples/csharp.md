```csharp run id=records
using System.Text.Json;

var log = new JsonLog(Level.Information);
log.Write(Level.Debug, "cart loaded", ("items", 3)); // below Information: dropped
log.Write(Level.Information, "order placed", ("order", 1042), ("total_cents", 1999));
log.Write(Level.Warning, "stock low", ("sku", "A-17"), ("left", 2));
log.Write(Level.Information, "login failed", ("user", "bob\n{\"level\":\"Information\"}"));
try
{
    checked { int cents = int.MaxValue; cents += 1; }
}
catch (OverflowException e)
{
    log.Write(Level.Error, "total failed", ("error", e.GetType().Name), ("order", 1043));
}

// The levels of Microsoft.Extensions.Logging.LogLevel, with the same numbers.
enum Level { Trace, Debug, Information, Warning, Error, Critical }

// Writes one JSON object per line, with no timestamp.
class JsonLog(Level minLevel)
{
    public void Write(Level level, string message, params (string Key, object Value)[] fields)
    {
        if (level < minLevel)
        {
            return;
        }
        using var stream = Console.OpenStandardOutput();
        using (var json = new Utf8JsonWriter(stream))
        {
            json.WriteStartObject();
            json.WriteString("level", level.ToString());
            json.WriteString("msg", message);
            foreach (var (key, value) in fields)
            {
                switch (value)
                {
                    case int number: json.WriteNumber(key, number); break;
                    case bool flag: json.WriteBoolean(key, flag); break;
                    default: json.WriteString(key, value.ToString()); break;
                }
            }
            json.WriteEndObject();
        }
        stream.WriteByte((byte)'\n');
    }
}
```

```output for=records
{"level":"Information","msg":"order placed","order":1042,"total_cents":1999}
{"level":"Warning","msg":"stock low","sku":"A-17","left":2}
{"level":"Information","msg":"login failed","user":"bob\n{\u0022level\u0022:\u0022Information\u0022}"}
{"level":"Error","msg":"total failed","error":"OverflowException","order":1043}
```

- Verdict: library. `ILogger` in `Microsoft.Extensions.Logging` is the logging API of .NET applications, and it comes as a package. The base library has `System.Text.Json`, which writes the JSON here.
- The levels of the example have the numbers of `LogLevel`: Trace is 0 and Critical is 5.
- `Utf8JsonWriter` escapes the newline and, by default, writes the quote as `\u0022`.

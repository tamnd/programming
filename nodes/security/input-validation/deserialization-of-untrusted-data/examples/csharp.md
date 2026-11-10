```csharp wrong id=bind-entity
using System.Text.Json;
using System.Text.Json.Serialization;

var options = new JsonSerializerOptions
{
    PropertyNameCaseInsensitive = true,
    TypeInfoResolver = AccountJson.Default,
};
string body = """{"name": "mallory", "theme": "dark", "isAdmin": true}""";

// The request body binds straight to the stored account.
Account account = JsonSerializer.Deserialize<Account>(body, options)!;
Console.WriteLine($"{account.Name}: theme {account.Theme}, admin {account.IsAdmin}");

class Account
{
    public string Name { get; set; } = "";
    public string Theme { get; set; } = "light";
    public bool IsAdmin { get; set; }
}

[JsonSerializable(typeof(Account))]
partial class AccountJson : JsonSerializerContext;
```

```output for=bind-entity
mallory: theme dark, admin True
```

`System.Text.Json` creates only the type that the code names, so the bytes cannot choose a class. The risk moves to the fields. The request sets `IsAdmin`, because the stored type has that property. This is CWE-915, the peer of CWE-502.

```csharp run id=dto
using System.Text.Json;
using System.Text.Json.Serialization;

var options = new JsonSerializerOptions
{
    PropertyNameCaseInsensitive = true,
    UnmappedMemberHandling = JsonUnmappedMemberHandling.Disallow,
    TypeInfoResolver = RequestJson.Default,
};

foreach (string body in new[]
{
    """{"theme": "dark"}""",
    """{"theme": "dark", "isAdmin": true}""",
})
{
    try
    {
        SettingsRequest request = JsonSerializer.Deserialize<SettingsRequest>(body, options)!;
        Console.WriteLine($"theme {request.Theme}");
    }
    catch (JsonException)
    {
        Console.WriteLine("refused: unexpected field");
    }
}

// Only the fields that this operation may change.
record SettingsRequest(string Theme);

[JsonSerializable(typeof(SettingsRequest))]
partial class RequestJson : JsonSerializerContext;
```

```output for=dto
theme dark
refused: unexpected field
```

The request type has only the field that the operation may change, and the options refuse a key that the type does not have.

- Verdict: stdlib. Microsoft says that `BinaryFormatter` is insecure and cannot be made secure, and its in-box implementation throws exceptions on use. `System.Text.Json` builds only declared types, and a derived type needs a declared type discriminator.
- Deserialize into request types that hold only the fields that the operation needs.
- Do not use `BinaryFormatter`, `SoapFormatter`, `NetDataContractSerializer`, `LosFormatter` or `ObjectStateFormatter` on data from outside the process.

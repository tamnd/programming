```csharp run id=from-environment
Config LoadConfig(Func<string, string?> lookup)
{
    string? password = lookup("EXAMPLE_DB_PASSWORD");
    if (string.IsNullOrWhiteSpace(password))
    {
        throw new InvalidOperationException("EXAMPLE_DB_PASSWORD is not set");
    }
    return new Config(lookup("EXAMPLE_DB_USER") ?? "app", password);
}

try
{
    LoadConfig(Environment.GetEnvironmentVariable);
}
catch (InvalidOperationException e)
{
    Console.WriteLine($"refused to start: {e.Message}");
}

var deployment = new Dictionary<string, string> { ["EXAMPLE_DB_PASSWORD"] = "example-not-a-real-password" };
Console.WriteLine(LoadConfig(name => deployment.GetValueOrDefault(name)));

record Config(string User, string Password)
{
    // The generated ToString of a record prints every property, so replace it.
    public override string ToString() => $"user={User} password=<set>";
}
```

```output for=from-environment
refused to start: EXAMPLE_DB_PASSWORD is not set
user=app password=<set>
```

`LoadConfig` takes a `Func`, so production passes `Environment.GetEnvironmentVariable` and a test passes a dictionary lookup. The record overrides `ToString`, because the generated one prints every property.

- Verdict: stdlib. `Environment.GetEnvironmentVariable` returns `null` for a variable that is not set. The task [[read-an-environment-variable]] covers the details.
- A string literal in C# is in the metadata of the assembly. Decompilers such as ILSpy show it.
- In ASP.NET Core, the configuration system reads environment variables and other sources at run time. Secret Manager keeps development secrets in the user profile, outside the project, and a vault provider serves production.
- Do not put secrets in `appsettings.json` in the repository.

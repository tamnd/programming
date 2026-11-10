```csharp wrong id=interpolation
static string FindOrdersSql(string customer) =>
    $"SELECT id FROM orders WHERE customer = '{customer}'";

Console.WriteLine(FindOrdersSql("alice"));
Console.WriteLine(FindOrdersSql("x' OR '1'='1"));
```

```output for=interpolation
SELECT id FROM orders WHERE customer = 'alice'
SELECT id FROM orders WHERE customer = 'x' OR '1'='1'
```

```csharp run id=formattable
// EF Core's FromSql takes a FormattableString, not a string. The compiler
// keeps the format and the arguments apart, and the library turns each
// argument into a parameter. This function does the same thing.
static (string Sql, object?[] Args) Parameterize(FormattableString query)
{
    var names = new object[query.ArgumentCount];
    for (var i = 0; i < names.Length; i++)
    {
        names[i] = $"@p{i}";
    }
    return (string.Format(query.Format, names), query.GetArguments());
}

var customer = "x' OR '1'='1";
var (sql, parameters) = Parameterize($"SELECT id FROM orders WHERE customer = {customer}");
Console.WriteLine($"query: {sql}");
Console.WriteLine($"args:  [{string.Join(", ", parameters)}]");
```

```output for=formattable
query: SELECT id FROM orders WHERE customer = @p0
args:  [x' OR '1'='1]
```

- Verdict: stdlib. ADO.NET (`System.Data.Common`) is part of .NET: `DbCommand.Parameters` holds the values of a command. Each database needs a provider package, such as Microsoft.Data.Sqlite or Npgsql.
- With Microsoft.Data.Sqlite, write `$customer` in `CommandText` and call `command.Parameters.AddWithValue("$customer", customer)`.
- An interpolated string `$"..."` that is a `string` puts the input into the SQL text, as in the first example.
- EF Core has two families of methods. `FromSql`, `FromSqlInterpolated` and `ExecuteSql` take a `FormattableString`, and they turn each interpolated value into a parameter, as in the second example. `FromSqlRaw`, `SqlQueryRaw` and `ExecuteSqlRaw` take a `string`, so an interpolated string is already plain text when they get it.
- A parameter cannot hold a table or a column name. Choose such names from a fixed list.
- No example here uses a real database: the provider packages were not in the local NuGet cache when the examples were recorded.

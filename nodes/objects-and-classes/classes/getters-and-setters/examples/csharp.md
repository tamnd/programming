```csharp run id=auto-and-checked-properties
using System;

var e = new Employee("Ana", "ana@example.com");
e.Email = "  Ana@Example.COM ";
Console.WriteLine($"{e.Name} {e.Email} {e.Id}");
try
{
    e.Email = "not an address";
}
catch (ArgumentException ex)
{
    Console.WriteLine(ex.Message);
}
Console.WriteLine(e.Email);

class Employee
{
    private static int lastId;
    private string email = "";

    public Employee(string name, string email)
    {
        Name = name;
        Email = email;
        Id = ++lastId;
    }

    public string Name { get; } // auto-implemented, read-only after the constructor

    public int Id { get; private set; } // other classes can read it but not set it

    public string Email // a property with a check: the syntax of a field, the code of a method
    {
        get => email;
        set
        {
            var clean = value.Trim().ToLowerInvariant();
            if (!clean.Contains('@'))
            {
                throw new ArgumentException($"not an email address: {clean}");
            }
            email = clean;
        }
    }
}
```

```output for=auto-and-checked-properties
Ana ana@example.com 1
not an email address: not an address
ana@example.com
```

`Name` and `Id` are auto-implemented properties: the compiler writes the backing field and the accessors. `private set` keeps the setter inside the class. `Email` needs a check, so it has an explicit backing field and an explicit `set` accessor, but the caller still writes `e.Email = ...` as for a field. A change from an auto-implemented property to a checked one does not change the source of the callers.

```csharp run id=record
var ada = new Person("Ada", 36);
Console.WriteLine($"name: {ada.Name} age: {ada.Age}");

var older = ada with { Age = ada.Age + 1 };
Console.WriteLine($"copy: {older}");
Console.WriteLine($"original: {ada}");
Console.WriteLine("same fields equal: " + Yn(ada == new Person("Ada", 36)));
Console.WriteLine("same object: " + Yn(ReferenceEquals(ada, new Person("Ada", 36))));

static string Yn(bool b) => b ? "yes" : "no";

record Person(string Name, int Age);
```

```output for=record
name: Ada age: 36
copy: Person { Name = Ada, Age = 37 }
original: Person { Name = Ada, Age = 36 }
same fields equal: yes
same object: no
```

- Verdict: native. A positional `record` gets init-only properties, value equality, a formatted `ToString` and support for `with`.
- `with` makes a new object. The original keeps its age.
- A `record` is a reference type, but `==` compares the members. `record struct` makes a value type.

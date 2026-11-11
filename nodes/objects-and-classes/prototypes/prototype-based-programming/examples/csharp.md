```csharp run id=delegating-indexer
var traits = new Proto(null);
traits["describe"] = (Func<Proto, string>)(o => $"({o["x"]}, {o["y"]})");
var origin = new Proto(traits) { ["x"] = 0, ["y"] = 0 };

var p = new Proto(origin) { ["x"] = 3 };  // p delegates to origin; x shadows origin's
Console.WriteLine($"{origin.Send("describe")} {p.Send("describe")}");

traits["describe"] = (Func<Proto, string>)(o => $"<{o["x"]} {o["y"]}>");
Console.WriteLine($"{origin.Send("describe")} {p.Send("describe")}");
Console.WriteLine($"{p.Has("y")} {p.Parent == origin}");

// An object with its own slots and a parent to delegate to.
class Proto(Proto? parent)
{
    readonly Dictionary<string, object> slots = new();

    public Proto? Parent => parent;

    public bool Has(string name) => slots.ContainsKey(name);

    public object this[string name]
    {
        get
        {
            for (var o = this; o != null; o = o.Parent)
            {
                if (o.slots.TryGetValue(name, out var v)) return v;
            }
            throw new KeyNotFoundException($"no slot {name}");
        }
        set => slots[name] = value;  // writes always go to an own slot
    }

    public string Send(string name) => ((Func<Proto, string>)this[name])(this);
}
```

```output for=delegating-indexer
(0, 0) (3, 0)
<0 0> <3 0>
False True
```

```csharp wrong id=memberwise-clone
var prototype = new Form { Name = "form", Fields = ["id"] };

var login = prototype.Clone();  // copy the prototype
login.Name = "login";
login.Fields.Add("password");

Console.WriteLine($"{login.Name}: {string.Join(", ", login.Fields)}");
Console.WriteLine($"{prototype.Name}: {string.Join(", ", prototype.Fields)}");

class Form
{
    public string Name = "";
    public List<string> Fields = [];

    public Form Clone() => (Form)MemberwiseClone();  // copies the fields, not the list
}
```

```output for=memberwise-clone
login: id, password
form: id, password
```

- Verdict: absent. C# objects take their members from their class. The run block builds prototype objects from a dictionary of slots and a parent, with an indexer that reads through the chain and writes to the object itself. `ExpandoObject` and `DynamicObject` give objects whose members change at run time through `dynamic`, but they have no parent link of their own.
- The wrong block copies a prototype with `MemberwiseClone`, which makes a shallow copy. The clone gets its own `Name` but shares the `Fields` list with the prototype, so adding a field to `login` changes the prototype as well. A clone method must copy each mutable member, here with `new List<string>(Fields)`.

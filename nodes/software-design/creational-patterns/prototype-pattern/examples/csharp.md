```csharp run id=spawner
var registry = new Dictionary<string, IPrototype>
{
    ["goblin"] = new Monster("goblin", 30, ["dagger"]),
    ["spikes"] = new Trap("spikes", 5),
};

IPrototype Spawn(string key) => registry[key].Copy();

var first = (Monster)Spawn("goblin");
first.Items.Add("torch");
var second = Spawn("goblin");
var trap = Spawn("spikes");
foreach (var thing in new[] { first, second, trap })
{
    Console.WriteLine(thing.Describe());
}
Console.WriteLine("prototype " + registry["goblin"].Describe());

var bigger = (Trap)trap with { Damage = 9 };
Console.WriteLine($"with: {bigger.Describe()}, original: {trap.Describe()}");

interface IPrototype
{
    IPrototype Copy();
    string Describe();
}

class Monster(string name, int health, List<string> items) : IPrototype
{
    public string Name { get; } = name;
    public int Health { get; } = health;
    public List<string> Items { get; private set; } = items;

    public IPrototype Copy()
    {
        // MemberwiseClone copies the fields; the list is a reference, so copy it too.
        var copy = (Monster)MemberwiseClone();
        copy.Items = [.. Items];
        return copy;
    }

    public string Describe() => $"{Name}: health {Health}, items {string.Join(", ", Items)}";
}

record Trap(string Name, int Damage) : IPrototype
{
    public IPrototype Copy() => this with { };

    public string Describe() => $"{Name}: damage {Damage}";
}
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
with: spikes: damage 9, original: spikes: damage 5
```

- Verdict: stdlib. `Object.MemberwiseClone` is a `protected` method that makes a shallow copy: it copies each field, and a field of a reference type keeps the same object. The class then copies the fields that must not be shared.
- A record supports the `with` expression, which makes a copy with some members changed. The copy is shallow, as `MemberwiseClone` is.
- The interface method is `Copy`, not `Clone`. The C# compiler rejects a member named `Clone` in a record with error CS8859, because the compiler makes its own clone method for `with`.
- The program defines its own interface. Microsoft recommends that public APIs do not implement `ICloneable`, because the interface does not say whether `Clone` is deep or shallow.

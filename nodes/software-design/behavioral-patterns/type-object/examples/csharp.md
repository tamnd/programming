```csharp run id=breeds
using System;

var breeds = new[]
{
    // New kinds are data, for example from a file.
    new Breed("goblin", 20, "a dagger"),
    new Breed("dragon", 200, "fire"),
    new Breed("wyvern", 120, "its tail"),
};
foreach (var b in breeds)
{
    Console.WriteLine(b.NewMonster().Describe());
}

record Breed(string Name, int Health, string Attack)
{
    public Monster NewMonster() => new Monster(this);
}

class Monster
{
    private readonly Breed breed;
    private int health; // each monster has its own health

    public Monster(Breed breed)
    {
        this.breed = breed;
        health = breed.Health;
    }

    public string Describe() =>
        $"{breed.Name}: {health} health, attacks with {breed.Attack}";
}
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A record with one instance for each kind.

```csharp run id=virtual
var animals = new List<Animal> { new Dog("Rex"), new Cat("Tom") };
foreach (var a in animals)
{
    Console.WriteLine(a.Describe());
}

Animal hidden = new Quiet("Bo");
Console.WriteLine(hidden.Speak());
Console.WriteLine(((Quiet)hidden).Speak());

abstract class Animal(string name)
{
    public string Name { get; } = name;
    public abstract string Speak();
    public string Describe() => $"{Name} says {Speak()}";
}

class Dog(string name) : Animal(name)
{
    public override string Speak() => "woof";
}

class Cat(string name) : Animal(name)
{
    public override string Speak() => "meow";
}

class Quiet(string name) : Dog(name)
{
    public new string Speak() => "...";
}
```

```output for=virtual
Rex says woof
Tom says meow
woof
...
```

- Verdict: native. A C# class extends one class and implements any number of interfaces. Methods are not virtual by default: the base class marks a method `virtual` or `abstract`, and the derived class must write `override`.
- A method marked `new` hides the base method instead of overriding it. The call then depends on the static type of the variable, which is why `hidden.Speak()` prints `woof`.

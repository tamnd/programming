```csharp run id=consultants
using System;

var agg = new EventAggregator();
var alice = new Consultant("alice", "London", false, "London");
var bob = new Consultant("bob", "Paris", true, "Lyon");
agg.Listen(alice);
agg.Listen(bob);

agg.ConsultantChanged += c => Console.WriteLine($"screen: {c.Name} changed");
agg.ConsultantAvailabilityChanged += c => Console.WriteLine($"phone: availability of {c.Name} changed");

alice.Move("Leeds");
bob.Move("Paris");

class Consultant
{
    public event Action<Consultant>? LocationChanged;
    public event Action<Consultant>? AvailabilityChanged;
    public string Name { get; }
    private readonly string home;
    private readonly bool availableAtHome;
    private string location;

    public Consultant(string name, string home, bool availableAtHome, string location)
    {
        Name = name;
        this.home = home;
        this.availableAtHome = availableAtHome;
        this.location = location;
    }

    private bool AvailableAt(string place) => place == home ? availableAtHome : true;

    public void Move(string place)
    {
        Console.WriteLine($"{Name} moves to {place}");
        if (location != place)
        {
            LocationChanged?.Invoke(this);
            if (AvailableAt(place) != AvailableAt(location)) AvailabilityChanged?.Invoke(this);
            location = place;
        }
    }
}

class EventAggregator
{
    public event Action<Consultant>? ConsultantChanged;
    public event Action<Consultant>? ConsultantLocationChanged;
    public event Action<Consultant>? ConsultantAvailabilityChanged;

    public void Listen(Consultant subject)
    {
        subject.LocationChanged += c => { ConsultantLocationChanged?.Invoke(c); ConsultantChanged?.Invoke(c); };
        subject.AvailabilityChanged += c => { ConsultantAvailabilityChanged?.Invoke(c); ConsultantChanged?.Invoke(c); };
    }
}
```

```output for=consultants
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

- Verdict: idiom. C# events, with the event names of Fowler's example.

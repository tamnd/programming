```csharp run id=player
var player = new Player();
foreach (var e in new[] { Event.Play, Event.Pause, Event.Play, Event.Stop, Event.Pause })
{
    player.Handle(e);
}

enum State { Stopped, Playing, Paused }

enum Event { Play, Pause, Stop }

sealed class Player
{
    public State Current { get; private set; } = State.Stopped;

    public void Handle(Event e)
    {
        // One switch expression holds the whole transition table.
        var (next, action) = (Current, e) switch
        {
            (State.Stopped, Event.Play) => (State.Playing, "start at 0:00"),
            (State.Playing, Event.Pause) => (State.Paused, "hold the position"),
            (State.Paused, Event.Play) => (State.Playing, "resume"),
            (State.Playing or State.Paused, Event.Stop) => (State.Stopped, "rewind"),
            _ => (Current, "ignored"),
        };
        Console.WriteLine($"{Name(e)}: {action} ({Name(Current)} -> {Name(next)})");
        Current = next;
    }

    static string Name<T>(T value) where T : Enum => value.ToString().ToLowerInvariant();
}
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: idiom. The states are an enum, and a `switch` expression on the pair `(Current, e)` gives the next state and the action. Each arm is one row of a transition table.
- The `or` pattern joins two states that react to `Stop` in the same way.
- The `_` arm gives the default for every pair that the other arms do not match. A C# enum can hold any value of its underlying type, so a switch on an enum without a `_` arm gets warning CS8524, even when each named member has an arm.
- When each state needs its own data or its own code, a class for each state that implements one interface is the classic form.

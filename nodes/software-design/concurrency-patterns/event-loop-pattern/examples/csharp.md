```csharp run id=event-loop-pattern
using System;
using System.Collections.Generic;

// One thread takes ready callbacks from a queue and runs each one to the end.
class EventLoop
{
    readonly Queue<Action> ready = new Queue<Action>(); // callbacks to run in this turn or the next
    readonly PriorityQueue<Action, (int Due, int Order)> timers = new PriorityQueue<Action, (int, int)>();
    public int Now; // a counter clock, so the output does not depend on the machine
    int order;
    public int Turns;

    public void CallSoon(Action callback) => ready.Enqueue(callback);

    public void CallLater(int delay, Action callback) => timers.Enqueue(callback, (Now + delay, ++order));

    public void Run()
    {
        while (ready.Count > 0 || timers.Count > 0)
        {
            if (ready.Count == 0 && timers.TryPeek(out _, out var first)) Now = first.Due; // a real loop sleeps or waits for I/O here
            while (timers.TryPeek(out _, out var next) && next.Due <= Now) ready.Enqueue(timers.Dequeue());
            Turns++;
            int count = ready.Count; // callbacks added now wait for the next turn
            for (int i = 0; i < count; i++) ready.Dequeue()();
        }
    }
}

class Program
{
    static readonly EventLoop loop = new EventLoop();
    static readonly List<string> log = new List<string>();

    static Action Say(string text) => () => log.Add($"time {loop.Now}: {text}");

    static void Main()
    {
        loop.CallLater(20, Say("timer of 20"));
        loop.CallLater(10, Say("timer of 10"));
        loop.CallSoon(() =>
        {
            Say("first callback")();
            loop.CallSoon(Say("callback that the first callback added"));
        });
        loop.CallSoon(Say("second callback"));
        loop.Run();

        Console.WriteLine("order of the callbacks:");
        foreach (var line in log) Console.WriteLine("  " + line);
        Console.WriteLine("turns of the loop: " + loop.Turns);
    }
}
```

```output
order of the callbacks:
  time 0: first callback
  time 0: second callback
  time 0: callback that the first callback added
  time 10: timer of 10
  time 20: timer of 20
turns of the loop: 4
```

- Verdict: idiom. A small loop class with a `Queue<Action>` and a `PriorityQueue` of timers that has a tuple priority.

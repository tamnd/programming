```csharp run id=guarded-suspension
using System;
using System.Collections.Generic;
using System.Threading;

class Tank
{
    private readonly object gate = new object();
    private int level = 0;
    public int Lowest, Filled, Taken, Served; // read after the threads end

    public void Take(int amount)
    {
        lock (gate)
        {
            // Guarded suspension: wait in a loop until the guard is true.
            while (level < amount) Monitor.Wait(gate);
            level -= amount;
            Lowest = Math.Min(Lowest, level);
            Taken += amount;
            Served++;
        }
    }

    public void Fill(int amount)
    {
        lock (gate)
        {
            level += amount;
            Filled += amount;
            Monitor.PulseAll(gate); // each waiting taker checks its guard again
        }
    }

    public int Level()
    {
        lock (gate) return level;
    }
}

class Program
{
    static void Main()
    {
        var tank = new Tank();
        int[] amounts = { 5, 3, 8 };
        var takers = new List<Thread>();
        foreach (int a in amounts)
        {
            var t = new Thread(() => tank.Take(a));
            takers.Add(t);
            t.Start();
        }
        for (int i = 0; i < 10; i++) tank.Fill(2);
        foreach (var t in takers) t.Join();

        Console.WriteLine("units filled: " + tank.Filled);
        Console.WriteLine("units taken: " + tank.Taken);
        Console.WriteLine("takers served: " + tank.Served);
        Console.WriteLine("level never below 0: " + (tank.Lowest >= 0 ? "True" : "False"));
        Console.WriteLine("units left: " + tank.Level());
    }
}
```

```output
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

- Verdict: stdlib. `lock` with `Monitor.Wait` in a `while` loop and `Monitor.PulseAll`.

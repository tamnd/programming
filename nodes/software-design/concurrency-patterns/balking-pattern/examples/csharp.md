```csharp run id=balking-pattern
using System;
using System.Collections.Generic;
using System.Threading;

class Document
{
    private readonly object gate = new object();
    private readonly Action write; // the slow part of a save
    private bool changed = false;
    private bool saving = false;
    private int saves = 0;

    public Document(Action write) => this.write = write;

    public void Edit()
    {
        lock (gate) changed = true;
    }

    // Saves the document. Returns false at once if a save cannot start now.
    public bool Save()
    {
        lock (gate)
        {
            if (saving || !changed) return false; // balk: the object is not in the right state
            saving = true;
            changed = false;
        }
        write();
        lock (gate)
        {
            saving = false;
            saves++;
        }
        return true;
    }

    public int Saves()
    {
        lock (gate) return saves;
    }
}

class Program
{
    static void Main()
    {
        var started = new ManualResetEventSlim(false);
        var release = new ManualResetEventSlim(false);
        var doc = new Document(() =>
        {
            started.Set();
            release.Wait(); // the first save stays in progress until the other calls end
        });
        doc.Edit();
        var first = new Thread(() => doc.Save());
        first.Start();
        started.Wait();

        int calls = 0, balked = 0;
        var callers = new List<Thread>();
        for (int i = 0; i < 7; i++)
        {
            callers.Add(new Thread(() =>
            {
                Interlocked.Increment(ref calls);
                if (!doc.Save()) Interlocked.Increment(ref balked);
            }));
        }
        foreach (var t in callers) t.Start();
        foreach (var t in callers) t.Join();
        release.Set();
        first.Join();

        Console.WriteLine("calls during a save: " + calls);
        Console.WriteLine("calls that balked: " + balked);
        Console.WriteLine("saves run: " + doc.Saves());
        Console.WriteLine("save with no changes: " + (doc.Save() ? "ran" : "balked"));
        doc.Edit();
        Console.WriteLine("save after an edit: " + (doc.Save() ? "ran" : "balked"));
        Console.WriteLine("saves run: " + doc.Saves());
    }
}
```

```output
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

- Verdict: native. A `lock` statement makes the test and the change of the `saving` flag one step.

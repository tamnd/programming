```csharp run id=work-stealing-model
using System;
using System.Collections.Generic;
using System.Linq;

// A step-by-step model of a work-stealing scheduler with two workers.
// Each worker has a deque (a LinkedList). The first node is the top (oldest task); the last node is the bottom (newest task).
// A task is a range [lo, hi). A range of more than 2 numbers splits into two halves; a smaller range is a leaf that adds its numbers.
const int Workers = 2;
var deques = Enumerable.Range(0, Workers).Select(_ => new LinkedList<(int Lo, int Hi)>()).ToArray();
var leaves = new int[Workers];
int total = 0;

deques[0].AddLast((0, 8));
for (int round = 1; deques.Any(d => d.Count > 0); round++)
{
    for (int w = 0; w < Workers; w++)
    {
        (int Lo, int Hi) task;
        string how = "pops", source = "";
        if (deques[w].Count > 0)
        {
            task = deques[w].Last!.Value; // the owner takes the newest task (LIFO)
            deques[w].RemoveLast();
        }
        else
        {
            int victim = Enumerable.Range(0, Workers).FirstOrDefault(v => v != w && deques[v].Count > 0, -1);
            if (victim == -1)
            {
                Console.WriteLine($"round {round}: w{w} is idle");
                continue;
            }
            task = deques[victim].First!.Value; // a thief takes the oldest task (FIFO)
            deques[victim].RemoveFirst();
            how = "steals";
            source = $" from w{victim}";
        }
        var (lo, hi) = task;
        if (hi - lo > 2)
        {
            int mid = (lo + hi) / 2;
            deques[w].AddLast((lo, mid));
            deques[w].AddLast((mid, hi));
            Console.WriteLine($"round {round}: w{w} {how} [{lo}, {hi}){source} and splits it");
        }
        else
        {
            for (int i = lo; i < hi; i++) total += i;
            leaves[w]++;
            Console.WriteLine($"round {round}: w{w} {how} [{lo}, {hi}){source} and runs it");
        }
    }
}

Console.WriteLine($"leaves run: {string.Join(", ", leaves.Select((n, w) => $"w{w} {n}"))}");
Console.WriteLine($"sum: {total}");
```

```output for=work-stealing-model
round 1: w0 pops [0, 8) and splits it
round 1: w1 steals [0, 4) from w0 and splits it
round 2: w0 pops [4, 8) and splits it
round 2: w1 pops [2, 4) and runs it
round 3: w0 pops [6, 8) and runs it
round 3: w1 pops [0, 2) and runs it
round 4: w0 pops [4, 6) and runs it
round 4: w1 is idle
leaves run: w0 2, w1 2
sum: 28
```

- Verdict: stdlib. The .NET thread pool has a local queue for each of its threads, and an idle thread steals from the local queues of the other threads. Tasks that a task starts go to the local queue of its thread.
- This file is a model of the rules, with two `LinkedList` objects in place of two threads. `RemoveLast` is the owner's LIFO pop, and `RemoveFirst` is the thief's FIFO steal.
- The model shows why a thief takes the oldest task: in a recursive split, the oldest task is the largest piece of work.

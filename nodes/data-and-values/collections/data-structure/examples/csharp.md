```csharp run id=two-layouts
using System;
using System.Collections.Generic;

int[] array = { 10, 20, 30, 40, 50 };
Node? head = null;
for (int i = array.Length - 1; i >= 0; i--)
{
    head = new Node(array[i], head);
}

Console.WriteLine($"array[3] = {array[3]} (0 steps)");
Node n = head!;
int steps = 0;
for (int i = 3; i > 0; i--, steps++)
{
    n = n.Next!;
}
Console.WriteLine($"linked[3] = {n.Value} ({steps} steps)");

head = new Node(5, head);
var output = new List<string>();
for (Node? m = head; m != null; m = m.Next)
{
    output.Add(m.Value.ToString());
}
Console.WriteLine("after push_front: " + string.Join(" ", output));

record Node(int Value, Node? Next);
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: stdlib. `System.Collections.Generic` has `List<T>` (a dynamic array), `LinkedList<T>`, `Dictionary<TKey,TValue>` (a hash table) and `SortedDictionary<TKey,TValue>` (a tree).
- The record `Node` holds a value and a reference to the next node. `Node?` says that the reference can be null.

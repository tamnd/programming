```csharp run id=linkedlistnode
using System;
using System.Collections.Generic;

class Program
{
    static string Show<T>(LinkedList<T> list) => string.Join(",", list);

    static void Main()
    {
        // LinkedList<T> is doubly linked. Each LinkedListNode<T> knows its list and its neighbors.
        var list = new LinkedList<string>(new[] { "a", "c" });
        LinkedListNode<string> a = list.First!;
        LinkedListNode<string> b = list.AddAfter(a, "b");
        list.AddLast("d");
        Console.WriteLine($"{Show(list)} {list.Count} {b.Previous!.Value} {b.Next!.Value}");

        // With a node in hand, Remove and AddFirst are O(1) and move no other item.
        list.Remove(b);
        Console.WriteLine($"{Show(list)} {b.List == null}");
        list.AddFirst(b);
        Console.WriteLine(Show(list));

        // A node belongs to one list at a time. Adding it to a second list throws.
        var other = new LinkedList<string>();
        try
        {
            other.AddLast(b);
        }
        catch (InvalidOperationException e)
        {
            Console.WriteLine(e.GetType().Name);
        }

        // Find walks from the head and returns the first node with the value.
        Console.WriteLine($"{list.Find("c")?.Next?.Value} {list.Find("z") == null}");
    }
}
```

```output
a,b,c,d 4 a c
a,c,d True
b,a,c,d
InvalidOperationException
d True
```

- Verdict: stdlib. LinkedList<T> is doubly linked, and AddAfter returns the new LinkedListNode<T>, which knows its neighbors. Remove with a node takes constant time and sets the node's List to null. A node can be in one list only, so adding it to a second list throws InvalidOperationException. Find walks from the head.

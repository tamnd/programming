```csharp run id=tree
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

var a = new Tree(5, 3, 8, 1, 4, 9);
var b = new Tree(1, 3, 4, 5, 8, 9);
var c = new Tree(1, 3, 4, 5, 8);

Console.WriteLine($"in order: {string.Join(" ", a)}");
Console.WriteLine($"sum: {a.Sum()}");
Console.WriteLine($"first two: {string.Join(" ", a.Take(2))}");
Console.WriteLine($"same keys as b: {(a.SequenceEqual(b) ? "yes" : "no")}");
Console.WriteLine($"same keys as c: {(a.SequenceEqual(c) ? "yes" : "no")}");

sealed class Tree : IEnumerable<int>
{
    sealed class Node(int key)
    {
        public int Key { get; } = key;
        public Node? Left { get; set; }
        public Node? Right { get; set; }
    }

    Node? root;

    public Tree(params int[] keys)
    {
        foreach (int key in keys)
        {
            Insert(key);
        }
    }

    public void Insert(int key)
    {
        if (root is null)
        {
            root = new Node(key);
            return;
        }
        Node node = root;
        while (true)
        {
            if (key < node.Key)
            {
                if (node.Left is null) { node.Left = new Node(key); return; }
                node = node.Left;
            }
            else
            {
                if (node.Right is null) { node.Right = new Node(key); return; }
                node = node.Right;
            }
        }
    }

    // An iterator method: the compiler turns it into an IEnumerator<int> class.
    public IEnumerator<int> GetEnumerator()
    {
        var stack = new Stack<Node>();
        Node? node = root;
        while (stack.Count > 0 || node is not null)
        {
            while (node is not null)
            {
                stack.Push(node);
                node = node.Left;
            }
            node = stack.Pop();
            yield return node.Key;
            node = node.Right;
        }
    }

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
}
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: native. A class implements `IEnumerable<T>` with a `GetEnumerator` method, and `foreach` drives the `IEnumerator<T>` through `MoveNext` and `Current`.
- `GetEnumerator` is an iterator method. The compiler turns the body with `yield return` into a state machine class, and each call starts a fresh traversal.
- LINQ works on any `IEnumerable<T>`. `Take(2)` stops the enumerator after two keys, and `SequenceEqual` steps two enumerators together.
- The non-generic `IEnumerable.GetEnumerator` is still required, because `IEnumerable<T>` extends the older `IEnumerable`.

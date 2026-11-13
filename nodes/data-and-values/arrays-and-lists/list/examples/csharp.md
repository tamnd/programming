```csharp run id=ilist
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;

class Program
{
    static int Sum(IList<int> items)
    {
        int total = 0;
        for (int i = 0; i < items.Count; i++)
        {
            total += items[i];
        }
        return total;
    }

    static void Main()
    {
        // IList<T> is the interface. List<T> and arrays both implement it.
        var list = new List<int> { 3, 1, 2 };
        int[] array = { 3, 1, 2 };
        list.Insert(0, 9);
        Console.WriteLine($"{string.Join(",", list)} {Sum(list)} {Sum(array)}");

        // An array is an IList<T> with a fixed size, so Add throws.
        IList<int> fixedSize = array;
        try
        {
            fixedSize.Add(4);
        }
        catch (NotSupportedException e)
        {
            Console.WriteLine(e.GetType().Name);
        }

        // ReadOnlyCollection wraps a list. Changes to the list show through the wrapper.
        ReadOnlyCollection<int> view = list.AsReadOnly();
        list.Add(7);
        Console.WriteLine($"{view.Count} {view[^1]} {((IList<int>)view).IsReadOnly}");

        // Equals compares references. SequenceEqual compares the items in order.
        Console.WriteLine($"{list.Equals(new List<int>(list))} {list.SequenceEqual(new List<int>(list))}");
    }
}
```

```output
9,3,1,2 15 6
NotSupportedException
5 7 True
False True
```

- Verdict: stdlib. IList<T> is the interface, and both List<T> and arrays implement it. An array is a fixed-size IList<T>, so Add throws NotSupportedException. AsReadOnly gives a live view, so it shows the item 7 that the program adds later. Equals compares references, and SequenceEqual compares the items.

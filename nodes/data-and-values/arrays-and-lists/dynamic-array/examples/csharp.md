```csharp run id=list-capacity
using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        // List<T> shows its Capacity, the length of the array inside it.
        var list = new List<int>();
        var caps = new List<int> { list.Capacity };
        for (int i = 0; i < 100; i++)
        {
            list.Add(i);
            if (caps[^1] != list.Capacity)
            {
                caps.Add(list.Capacity);
            }
        }
        Console.WriteLine($"{list.Count}: {string.Join(" ", caps)}");

        // EnsureCapacity grows the array once. TrimExcess gives the spare slots back.
        var big = new List<int>();
        big.EnsureCapacity(1000);
        Console.WriteLine(big.Capacity >= 1000);
        big.Add(1);
        big.TrimExcess();
        Console.WriteLine($"{big.Count} {big.Capacity}");

        // Insert at index 0 moves every other item one place to the right.
        var small = new List<int> { 2, 3 };
        small.Insert(0, 1);
        Console.WriteLine(string.Join(",", small));

        // A change to the list during foreach makes the enumerator fail.
        try
        {
            foreach (int x in small)
            {
                small.Add(x);
            }
        }
        catch (InvalidOperationException e)
        {
            Console.WriteLine(e.GetType().Name);
        }
    }
}
```

```output
100: 0 4 8 16 32 64 128
True
1 1
1,2,3
InvalidOperationException
```

- Verdict: stdlib. List<T> shows its Capacity, which doubles from 4 to 128 while 100 items are added. EnsureCapacity grows the array once, and TrimExcess makes the capacity equal to the count. Insert at index 0 moves the items. A change to the list inside foreach throws InvalidOperationException.

```csharp run id=sort-and-orderby
using System;
using System.Collections.Generic;
using System.Linq;

// List<T>.Sort works in place and returns void.
var nums = new List<int> { 3, 1, 2 };
nums.Sort();
Console.WriteLine(string.Join(",", nums));

// OrderBy returns a new sequence, and it runs only when the program enumerates it.
var src = new List<int> { 3, 1, 2 };
var query = src.OrderBy(n => n);
src.Add(0);
Console.WriteLine(string.Join(",", src) + " | " + string.Join(",", query));

// ToList takes a snapshot.
var snapshot = src.OrderBy(n => n).ToList();
src.Add(-1);
Console.WriteLine(string.Join(",", snapshot));

// On a List<T>, Reverse() is the in-place method, not the LINQ copy.
var letters = new List<string> { "a", "b", "c" };
letters.Reverse();
Console.WriteLine(string.Join(",", letters));
var copy = Enumerable.Reverse(letters).ToList();
Console.WriteLine(string.Join(",", letters) + " | " + string.Join(",", copy));

// Array.Sort works in place on an array.
int[] arr = { 3, 1, 2 };
Array.Sort(arr);
Console.WriteLine(string.Join(",", arr));


// A copying call that drops its result changes nothing.
var marks = new List<int> { 3, 1, 2 };
marks.OrderBy(n => n);
Console.WriteLine(string.Join(",", marks));
```

```output
1,2,3
3,1,2,0 | 0,1,2,3
0,1,2,3
c,b,a
c,b,a | a,b,c
1,2,3
3,1,2
```

- Verdict: native. `List<T>.Sort()` changes the list. `OrderBy` returns a new sequence with deferred execution, so the item that the program adds after the call shows in the result, and `ToList()` takes a snapshot. On a `List<T>`, `Reverse()` is the in-place method, and `Enumerable.Reverse` makes a copy. A copying call such as `marks.OrderBy(n => n);` that drops its result changes nothing.

```csharp run id=invalid-operation-removeall-and-backward-for
var nums = new List<int> { 1, 2, 3, 4, 5 };
try
{
    foreach (var n in nums)
    {
        if (n % 2 == 0) nums.Remove(n);
    }
}
catch (InvalidOperationException e)
{
    Console.WriteLine($"caught {e.GetType().Name}");
}
Console.WriteLine($"after the error: {string.Join(",", nums)}");

// RemoveAll removes all matches in one call.
nums = new List<int> { 1, 2, 3, 4, 5 };
int removed = nums.RemoveAll(n => n % 2 == 0);
Console.WriteLine($"RemoveAll: {string.Join(",", nums)} ({removed} removed)");

// A for loop from the end does not use an enumerator.
nums = new List<int> { 1, 2, 2, 3, 4 };
for (int i = nums.Count - 1; i >= 0; i--)
{
    if (nums[i] % 2 == 0) nums.RemoveAt(i);
}
Console.WriteLine($"backward: {string.Join(",", nums)}");

// A dictionary: loop over a copy of the keys.
var stock = new Dictionary<string, int> { ["pen"] = 0, ["ink"] = 5 };
foreach (var key in stock.Keys.ToList())
{
    if (stock[key] == 0) stock.Remove(key);
}
Console.WriteLine($"stock: {string.Join(",", stock.Keys)}");
```

```output for=invalid-operation-removeall-and-backward-for
caught InvalidOperationException
after the error: 1,3,4,5
RemoveAll: 1,3,5 (2 removed)
backward: 1,3
stock: ink
```

- Verdict: native. The removal of 2 invalidates the enumerator of the list, and the next `MoveNext` throws an `InvalidOperationException`. `RemoveAll` does the loop inside the list and returns the count. A `for` loop over the indexes from the end has no enumerator, so it can remove elements. `ToList()` copies the keys, so the loop can remove entries from the dictionary.

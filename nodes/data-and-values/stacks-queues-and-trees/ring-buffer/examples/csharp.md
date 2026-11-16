```csharp run id=ring
var ring = new RingBuffer<int>(3);
foreach (int reading in new[] { 10, 20, 30, 40, 50 })
{
    ring.Push(reading);
    Console.WriteLine("after " + reading + ": " + string.Join(" ", ring.Items()));
}

int oldest = ring.Pop();
Console.WriteLine("read " + oldest + ", left: " + string.Join(" ", ring.Items()));

class RingBuffer<T>
{
    private readonly T[] items;
    private int start;
    private int count;

    public RingBuffer(int capacity) => items = new T[capacity];

    public void Push(T item)
    {
        items[(start + count) % items.Length] = item;
        if (count < items.Length)
        {
            count++;
        }
        else
        {
            start = (start + 1) % items.Length;
        }
    }

    public T Pop()
    {
        if (count == 0)
        {
            throw new InvalidOperationException("empty");
        }
        T item = items[start];
        start = (start + 1) % items.Length;
        count--;
        return item;
    }

    public IEnumerable<T> Items()
    {
        for (int i = 0; i < count; i++)
        {
            yield return items[(start + i) % items.Length];
        }
    }
}
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: idiom. .NET `Queue<T>` is a circular array that grows, so a fixed ring that overwrites is a small class.
- `Items` yields the items from the oldest to the newest.

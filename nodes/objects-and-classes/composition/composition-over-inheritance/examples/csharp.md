```csharp run id=forwarding-and-delegates
var s = new CountingSet<string>(new HashSet<string>());
s.Add("a");
s.UnionWith(["b", "c"]);
Console.WriteLine($"{s.Added} {s.Count}");

var t = new CountingSet<string>(new SortedSet<string>());  // any ISet<T> works
t.UnionWith(["c", "a", "b"]);
Console.WriteLine($"{t.Added} {string.Join(" ", t)}");

var log = new Logger(msg => Console.WriteLine(msg));
log.Log("ready");
log.Sink = msg => Console.WriteLine(msg.ToUpperInvariant());  // swap the part at run time
log.Log("ready");

// Composition: CountingSet has-a set and forwards to it.
sealed class CountingSet<T>(ISet<T> inner) : IEnumerable<T>
{
    public int Added { get; private set; }

    public bool Add(T item)
    {
        Added++;
        return inner.Add(item);
    }

    public void UnionWith(IEnumerable<T> items)
    {
        var list = items.ToList();
        Added += list.Count;
        inner.UnionWith(list);  // the inner set never calls back into this class
    }

    public int Count => inner.Count;

    public IEnumerator<T> GetEnumerator() => inner.GetEnumerator();

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() => GetEnumerator();
}

sealed class Logger(Action<string> sink)
{
    public Action<string> Sink { get; set; } = sink;

    public void Log(string msg) => Sink(msg);
}
```

```output for=forwarding-and-delegates
3 3
3 a b c
ready
READY
```

```csharp wrong id=hidden-method
var list = new CountingList();
list.Add("a");
ICollection<string> c = list;
c.Add("b");  // the call goes through the interface to List<string>.Add
list.AddRange(["c"]);  // AddRange never calls the new Add
Console.WriteLine($"{list.Added} {list.Count}");

// List<T>.Add is not virtual, so a subclass can only hide it with new.
class CountingList : List<string>
{
    public int Added;

    public new void Add(string item)
    {
        Added++;
        base.Add(item);
    }
}
```

```output for=hidden-method
1 3
```

- Verdict: idiom. `CountingSet` takes any `ISet<T>` in its primary constructor and forwards to it, so a `HashSet` and a `SortedSet` both work. `Logger` holds a delegate as its part, and the caller replaces it at run time. Classes are `sealed` because they are not designed for extension.
- The wrong block extends `List<string>` and declares a new `Add`. `List<T>.Add` is not virtual, so `new` only hides it: a call through `ICollection<string>` and a call to `AddRange` both reach the method of the base class, and the count is 1 for 3 elements. Wrap a `List<T>` in a field, or derive from `Collection<T>`, which has virtual methods for this purpose.

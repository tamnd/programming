```csharp run id=todo
var model = new TodoModel();
model.Changed += () =>
{
    var items = model.Items;
    for (var i = 0; i < items.Count; i++)
        Console.WriteLine($"  [{(items[i].Done ? "x" : " ")}] {i + 1} {items[i].Title}");
};
model.Changed += () =>
{
    var done = model.Items.Count(item => item.Done);
    Console.WriteLine($"  summary: {model.Items.Count - done} open, {done} done");
};
var controller = new Controller(model);
foreach (var line in new[] { "add buy milk", "add write report", "done 1", "done 7", "rename 2" })
    controller.Handle(line);

record Item(string Title, bool Done = false);

sealed class TodoModel
{
    private readonly List<Item> _items = [];

    public event Action? Changed;

    public IReadOnlyList<Item> Items => _items.AsReadOnly();

    public void Add(string title)
    {
        if (title.Length == 0) throw new ArgumentException("a title is required");
        _items.Add(new Item(title));
        Changed?.Invoke();
    }

    public void Complete(int number)
    {
        if (number < 1 || number > _items.Count) throw new ArgumentException($"no item {number}");
        _items[number - 1] = _items[number - 1] with { Done = true };
        Changed?.Invoke();
    }
}

sealed class Controller(TodoModel model)
{
    public void Handle(string line)
    {
        Console.WriteLine($"> {line}");
        var parts = line.Split(' ', 2);
        var argument = parts.Length > 1 ? parts[1] : "";
        try
        {
            if (parts[0] == "add") model.Add(argument);
            else if (parts[0] == "done" && int.TryParse(argument, out var n)) model.Complete(n);
            else Console.WriteLine($"  error: cannot read '{line}'");
        }
        catch (ArgumentException err)
        {
            Console.WriteLine($"  error: {err.Message}");
        }
    }
}
```

```output
> add buy milk
  [ ] 1 buy milk
  summary: 1 open, 0 done
> add write report
  [ ] 1 buy milk
  [ ] 2 write report
  summary: 2 open, 0 done
> done 1
  [x] 1 buy milk
  [ ] 2 write report
  summary: 1 open, 1 done
> done 7
  error: no item 7
> rename 2
  error: cannot read 'rename 2'
```

- Verdict: idiom. A C# `event` is the model's observer list, and the views are lambdas that the composition code adds with `+=`.
- `Items` returns a read-only wrapper, so a view can read the list and cannot change it.
- ASP.NET Core MVC uses the three names for the web form of the pattern: a controller class handles a request, works with the model, and chooses a view. The node page compares the two forms.

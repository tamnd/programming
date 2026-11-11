```csharp run id=events
var button = new Button();
button.Clicked += (sender, e) => Console.WriteLine($"handler 1: click {e.Count}");
EventHandler<ClickArgs> second = (sender, e) => Console.WriteLine("handler 2");
button.Clicked += second;

button.Click();
button.Clicked -= second;
button.Click();

class ClickArgs(int count) : EventArgs
{
    public int Count => count;
}

class Button
{
    int clicks;
    public event EventHandler<ClickArgs>? Clicked;
    public void Click() => Clicked?.Invoke(this, new ClickArgs(++clicks));
}
```

```output for=events
handler 1: click 1
handler 2
handler 1: click 2
```

```csharp fails expect=compile id=raise-outside
var button = new Button();
button.Clicked?.Invoke(button, EventArgs.Empty);

class Button
{
    public event EventHandler? Clicked;
}
```

- Verdict: native. The `event` keyword declares a list of delegates. Code outside the class can only add a handler with `+=` and remove one with `-=`. Only the class that declares the event can raise it, so the second block fails with error CS0070.
- `Clicked?.Invoke` calls nothing when no handler is attached. Windows Forms, WPF and other user interface libraries run an event loop on one thread and raise these events from it.

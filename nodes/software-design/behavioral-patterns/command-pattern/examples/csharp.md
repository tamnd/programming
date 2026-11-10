```csharp run id=editor
var buffer = new Buffer();
var history = new History();
void Show(string label) => Console.WriteLine($"{label}: \"{buffer.Text}\"");

history.Run(new Append(buffer, "hello")); Show("append");
history.Run(new Append(buffer, " world")); Show("append");
history.Run(new Delete(buffer, 5)); Show("delete");
history.Undo(); Show("undo");
history.Undo(); Show("undo");
history.Redo(); Show("redo");
history.Run(new Append(buffer, "!")); Show("append");
if (!history.Redo()) Console.WriteLine("redo: nothing to redo");

class Buffer
{
    public string Text { get; set; } = "";
}

interface IUndoableCommand
{
    void Execute();
    void Undo();
}

sealed class Append(Buffer buffer, string text) : IUndoableCommand
{
    public void Execute() => buffer.Text += text;
    public void Undo() => buffer.Text = buffer.Text[..^text.Length];
}

sealed class Delete(Buffer buffer, int count) : IUndoableCommand
{
    private string _removed = "";

    public void Execute()
    {
        _removed = buffer.Text[^count..];
        buffer.Text = buffer.Text[..^count];
    }

    public void Undo() => buffer.Text += _removed;
}

sealed class History
{
    private readonly Stack<IUndoableCommand> _undo = new();
    private readonly Stack<IUndoableCommand> _redo = new();

    public void Run(IUndoableCommand command)
    {
        command.Execute();
        _undo.Push(command);
        _redo.Clear();
    }

    public bool Undo()
    {
        if (!_undo.TryPop(out var command)) return false;
        command.Undo();
        _redo.Push(command);
        return true;
    }

    public bool Redo()
    {
        if (!_redo.TryPop(out var command)) return false;
        command.Execute();
        _undo.Push(command);
        return true;
    }
}
```

```output for=editor
append: "hello"
append: "hello world"
delete: "hello "
undo: "hello world"
undo: "hello"
redo: "hello world"
append: "hello world!"
redo: nothing to redo
```

- Verdict: idiom. An interface and a class for each command, with primary constructors for the receiver and the arguments.
- Ranges such as `[..^count]` and `[^count..]` cut the end of the string, so each command is short.
- `System.Windows.Input.ICommand` in .NET has `Execute`, `CanExecute` and a `CanExecuteChanged` event, and WPF buttons run commands through it. It has no `Undo`.
- A delegate such as `Action` is a command with no undo. A pair of `Action` values is the closure form.

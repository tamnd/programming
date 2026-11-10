```csharp run id=undo
using System;
using System.Collections.Generic;

var editor = new Editor();
var history = new History(editor);

history.Save();
editor.Type("Hello");
Console.WriteLine(editor.Show());
history.Save();
editor.Type(" world");
Console.WriteLine(editor.Show());
history.Save();
editor.MoveTo(5);
editor.Type(",");
Console.WriteLine(editor.Show());

for (int i = 0; i < 4; i++)
{
    Console.WriteLine(history.Undo() ? $"undo: {editor.Show()}" : "nothing to undo");
}

// The narrow interface. The caretaker sees a type with no members.
interface IMemento;

// The originator. Only it knows the type behind IMemento.
sealed class Editor
{
    private sealed record Snapshot(string Text, int Cursor) : IMemento;

    private string text = "";
    private int cursor;

    public void Type(string chars)
    {
        text = text[..cursor] + chars + text[cursor..];
        cursor += chars.Length;
    }

    public void MoveTo(int position) => cursor = Math.Clamp(position, 0, text.Length);

    public IMemento Save() => new Snapshot(text, cursor);

    public void Restore(IMemento memento)
    {
        if (memento is not Snapshot snapshot)
            throw new ArgumentException("not a memento of an Editor", nameof(memento));
        (text, cursor) = (snapshot.Text, snapshot.Cursor);
    }

    public string Show() => text[..cursor] + "|" + text[cursor..];
}

// The caretaker. It keeps mementos and cannot read them.
sealed class History(Editor editor)
{
    private readonly Stack<IMemento> undo = new();

    public void Save() => undo.Push(editor.Save());

    public bool Undo()
    {
        if (!undo.TryPop(out IMemento? memento)) return false;
        editor.Restore(memento);
        return true;
    }
}
```

```output for=undo
Hello|
Hello world|
Hello,| world
undo: Hello world|
undo: Hello|
undo: |
nothing to undo
```

- Verdict: idiom. An empty public interface is the narrow interface, and a `private` nested record that implements it is the wide one. Code outside `Editor` cannot name `Snapshot`, so it cannot cast to it.
- `interface IMemento;` with a semicolon and no body declares an empty interface.
- A positional record is immutable, and `string` is immutable, so a snapshot needs no deep copy.
- `Stack<T>.TryPop` returns `false` on an empty stack, where `Pop` would throw `InvalidOperationException`.

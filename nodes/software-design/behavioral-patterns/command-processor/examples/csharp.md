```csharp run id=processor
using System;
using System.Collections.Generic;

interface ICommand
{
    string Name { get; }
    void Execute();
    void Undo();
}

class Insert : ICommand
{
    private readonly List<string> words;
    private readonly string word;

    public Insert(List<string> words, string word)
    {
        this.words = words;
        this.word = word;
    }

    public string Name => $"insert {word}";
    public void Execute() => words.Add(word);
    public void Undo() => words.RemoveAt(words.Count - 1);
}

class Processor
{
    private readonly Stack<ICommand> done = new();

    public void DoIt(ICommand command)
    {
        Console.WriteLine($"do: {command.Name}");
        command.Execute();
        done.Push(command);
    }

    public void UndoIt()
    {
        var command = done.Pop();
        Console.WriteLine($"undo: {command.Name}");
        command.Undo();
    }
}

class Program
{
    static void Main()
    {
        var words = new List<string>();
        var processor = new Processor();
        processor.DoIt(new Insert(words, "hello"));
        processor.DoIt(new Insert(words, "world"));
        Console.WriteLine("text: " + string.Join(" ", words));
        processor.UndoIt();
        Console.WriteLine("text: " + string.Join(" ", words));
    }
}
```

```output
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

- Verdict: idiom. `ICommand` is the abstract command, and the processor keeps executed commands in a `Stack<ICommand>`.

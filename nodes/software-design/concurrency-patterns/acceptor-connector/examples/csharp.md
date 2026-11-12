```csharp run id=acceptor
using System;
using System.Collections.Generic;

interface IServiceHandler { void Open(Queue<string> channel); }

class StatusHandler : IServiceHandler
{
    Queue<string> channel = new();
    public void Open(Queue<string> c) { channel = c; Console.WriteLine("accept -> status handler open"); }
    public void Poll() => Console.WriteLine($"status: got {channel.Dequeue()}");
}

class CommandHandler : IServiceHandler
{
    Queue<string> channel = new();
    public void Open(Queue<string> c) { channel = c; Console.WriteLine("connect done -> command handler open"); }
    public void Send(string text) { Console.WriteLine($"command: send {text}"); channel.Enqueue(text); }
}

interface IAcceptor { void Accept(Queue<string> channel); }

class Acceptor<H> : IAcceptor where H : IServiceHandler, new()
{
    public static readonly Dictionary<string, IAcceptor> Listeners = new();
    public readonly List<H> Handlers = new();
    public Acceptor(string address) { Listeners[address] = this; Console.WriteLine($"listen {address}"); }
    public void Accept(Queue<string> channel) { var h = new H(); h.Open(channel); Handlers.Add(h); }
}

static class Connector
{
    public static H Connect<H>(string address, Dictionary<string, IAcceptor> listeners) where H : IServiceHandler, new()
    {
        Console.WriteLine($"connect {address}");
        var channel = new Queue<string>();
        listeners[address].Accept(channel);
        var handler = new H(); // complete: activate when the connection is up
        handler.Open(channel);
        return handler;
    }
}

static class Program
{
    static void Main()
    {
        var acceptor = new Acceptor<StatusHandler>("gateway:9000");
        var command = Connector.Connect<CommandHandler>("gateway:9000", Acceptor<StatusHandler>.Listeners);
        command.Send("reset");
        acceptor.Handlers[0].Poll();
    }
}
```

```output
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

- Verdict: idiom. The `new()` constraint lets the generic acceptor and connector create a handler of the type parameter.

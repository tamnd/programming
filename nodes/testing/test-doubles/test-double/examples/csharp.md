```csharp run id=register
using System;
using System.Collections.Generic;

interface IUsers
{
    bool Exists(string email);
    void Add(string email);
}

interface IMailer
{
    void Send(string to, string body);
}

// A fake: a working store in memory.
sealed class FakeUsers : IUsers
{
    private readonly HashSet<string> emails = new();
    public bool Exists(string email) => emails.Contains(email);
    public void Add(string email) => emails.Add(email);
}

// A spy: records each call for the test to read.
sealed class SpyMailer : IMailer
{
    public List<(string To, string Body)> Sent { get; } = new();
    public void Send(string to, string body) => Sent.Add((to, body));
}

static class Program
{
    static string Register(string email, IUsers users, IMailer mailer)
    {
        if (users.Exists(email))
        {
            return "taken";
        }
        users.Add(email);
        mailer.Send(email, "Welcome!");
        return "created";
    }

    static void Main()
    {
        var users = new FakeUsers();
        var mailer = new SpyMailer();
        Console.WriteLine(Register("ada@example.com", users, mailer));
        Console.WriteLine(Register("ada@example.com", users, mailer));
        Console.WriteLine(string.Join(", ", mailer.Sent));
    }
}
```

```output
created
taken
(ada@example.com, Welcome!)
```

- Verdict: idiom. .NET has no mock framework in its base class library. The seam is an interface, and the test passes a class that implements it.
- The spy keeps its calls in a list of named tuples. A tuple prints its fields in parentheses, as the last line shows.
- A double must implement every member of the interface. A large interface makes every double large, which is one reason to keep interfaces small.
- Methods that are not `virtual` cannot be replaced by a subclass, so code that a test must replace works through an interface or a delegate.

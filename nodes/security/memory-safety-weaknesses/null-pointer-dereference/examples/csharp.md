```csharp fails expect=exception:NullReferenceException id=or-default
var users = new Dictionary<string, User> { ["alice"] = new User(10), ["bob"] = new User(20) };

foreach (var name in new[] { "alice", "nobody" })
{
    User u = users.GetValueOrDefault(name)!; // null when the key is missing
    Console.WriteLine($"{name}: quota {u.Quota}");
}

record User(int Quota);
```

`GetValueOrDefault` returns `null` for a missing key. The `!` operator tells the compiler that the value is not null, so the nullable warning goes away, but the value is still null. The access to `u.Quota` throws `NullReferenceException`.

```csharp run id=try-get
var users = new Dictionary<string, User> { ["alice"] = new User(10), ["bob"] = new User(20) };

foreach (var name in new[] { "alice", "nobody" })
{
    if (users.TryGetValue(name, out User? u))
    {
        Console.WriteLine($"{name}: quota {u.Quota}");
    }
    else
    {
        Console.WriteLine($"{name}: no such user");
    }
}

record User(int Quota);
```

```output for=try-get
alice: quota 10
nobody: no such user
```

- Verdict: native. With nullable reference types, `User` and `User?` are different to the compiler, and it warns when the code can dereference a value that may be null. The runtime does not change, so a warning that is ignored is still a crash.
- Treat the nullable warnings as errors in the project file.
- Each `!` operator is a place to review. It hides the warning and does not check anything.
- Use `TryGetValue`, `?.` and `??` on values that can be absent.

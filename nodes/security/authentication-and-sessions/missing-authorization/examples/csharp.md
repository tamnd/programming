```csharp run id=central-policy
var sessions = new Dictionary<string, string> { ["session-alice"] = "alice", ["session-carol"] = "carol" };
var roles = new Dictionary<string, string> { ["alice"] = "customer", ["bob"] = "customer", ["carol"] = "admin" };
// The policy lists what each role may do. Anything not listed is denied.
var policy = new Dictionary<string, HashSet<Permission>>
{
    ["customer"] = [Permission.InvoiceRead],
    ["admin"] = [Permission.InvoiceRead, Permission.UserDelete],
};
Invoice[] invoices = [new(1, "alice", 120), new(2, "bob", 75)];

var routes = new Dictionary<string, Route>
{
    ["getInvoice"] = new(Permission.InvoiceRead, (user, arg) =>
    {
        // In SQL: SELECT ... FROM invoices WHERE id = @id AND owner = @user
        var invoice = invoices.FirstOrDefault(i => i.Id == int.Parse(arg) && i.Owner == user);
        return invoice is null ? (404, "not found") : (200, invoice.ToString());
    }),
    ["deleteUser"] = new(Permission.UserDelete, (user, arg) => (200, $"deleted {arg}")),
};

// Every request passes through this one check.
(int, string) Dispatch(string name, string session, string arg)
{
    if (!routes.TryGetValue(name, out var route)) return (404, "not found");
    if (!sessions.TryGetValue(session, out var user)) return (401, "log in first");
    var granted = roles.TryGetValue(user, out var role) && policy.TryGetValue(role, out var set) && set.Contains(route.Permission);
    if (!granted) return (403, "forbidden");
    return route.Handler(user, arg);
}

Console.WriteLine(Dispatch("getInvoice", "session-alice", "1"));
Console.WriteLine(Dispatch("getInvoice", "session-alice", "2"));
Console.WriteLine(Dispatch("deleteUser", "session-alice", "bob"));
Console.WriteLine(Dispatch("deleteUser", "session-carol", "bob"));
Console.WriteLine(Dispatch("getInvoice", "none", "1"));

// None is the default value of the enum, and no role has it.
enum Permission { None, InvoiceRead, UserDelete }

record Invoice(int Id, string Owner, int Total);

// The constructor needs a permission, so a route cannot exist without one.
record Route(Permission Permission, Func<string, string, (int, string)> Handler);
```

```output for=central-policy
(200, Invoice { Id = 1, Owner = alice, Total = 120 })
(404, not found)
(403, forbidden)
(200, deleted bob)
(401, log in first)
```

`Dispatch` checks the session and the policy before it calls a handler. The `getInvoice` handler matches the id and the owner together.

- Verdict: stdlib. In ASP.NET Core, the `Authorize` attribute and authorization policies give the same check. CodeQL has queries for a missing check and for direct object references in C# web code.
- `Permission.None` is the default value of the enum. A route built with `default` gets `None`, and no role has it, so the route is denied.
- In Entity Framework, add the owner to the query, as in `Where(i => i.Id == id && i.Owner == user)`.

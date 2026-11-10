```csharp run id=token
using System.Security.Cryptography;
using System.Text;

var sessions = new Dictionary<string, Session>
{
    ["c0ffee"] = new Session("alice", Convert.ToHexString(RandomNumberGenerator.GetBytes(32))),
};

string ChangeEmail(string cookie, string formToken, string email)
{
    if (!sessions.TryGetValue(cookie, out var s))
    {
        return "401 not logged in";
    }
    bool match = CryptographicOperations.FixedTimeEquals(
        Encoding.UTF8.GetBytes(formToken), Encoding.UTF8.GetBytes(s.Csrf));
    if (!match)
    {
        return "403 missing or wrong CSRF token";
    }
    return $"200 email of {s.User} is {email}";
}

Console.WriteLine(ChangeEmail("c0ffee", sessions["c0ffee"].Csrf, "alice@example.net"));
Console.WriteLine(ChangeEmail("c0ffee", "", "mallory@attacker.example"));

record Session(string User, string Csrf);
```

```output for=token
200 email of alice is alice@example.net
403 missing or wrong CSRF token
```

`RandomNumberGenerator` makes the token, and `CryptographicOperations.FixedTimeEquals` compares in constant time.

- Verdict: stdlib. ASP.NET Core has antiforgery tokens. MVC and Razor Pages validate them with built-in filters, and Blazor and Minimal APIs need the antiforgery middleware.
- Put `[ValidateAntiForgeryToken]` on each POST action, or add a global filter for all POST requests, as CodeQL query `cs/web/missing-token-validation` recommends.
- Protect the login form too, so that an attacker cannot log the user in to the attacker's account.

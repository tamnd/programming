```csharp run id=pipeline
using System.Net;
using static Shared;

// DelegatingHandler is the standard library's chain of responsibility for HTTP clients.
// Each handler has an InnerHandler, and base.SendAsync passes the request to it.
var app = new AppHandler();
var cache = new CacheHandler { InnerHandler = app };
var auth = new AuthHandler { InnerHandler = cache };
var log = new LogHandler { InnerHandler = auth };
using var invoker = new HttpMessageInvoker(log);

string?[] users = ["ada", null, "ada"];
foreach (var user in users)
{
    var request = new HttpRequestMessage(HttpMethod.Get, "http://shop.example/report");
    if (user is not null)
    {
        request.Headers.Add("X-User", user);
    }
    var response = await invoker.SendAsync(request, CancellationToken.None);
    var body = await response.Content.ReadAsStringAsync();
    Console.WriteLine($"{(int)response.StatusCode} {body}");
}

static class Shared
{
    public static string? UserOf(HttpRequestMessage request) =>
        request.Headers.TryGetValues("X-User", out var values) ? values.First() : null;

    public static HttpResponseMessage Reply(HttpStatusCode status, string body) =>
        new(status) { Content = new StringContent(body) };
}

class LogHandler : DelegatingHandler
{
    protected override Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, CancellationToken token)
    {
        Console.WriteLine($"log: {request.RequestUri!.AbsolutePath} from {UserOf(request) ?? "nobody"}");
        return base.SendAsync(request, token);
    }
}

class AuthHandler : DelegatingHandler
{
    protected override Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, CancellationToken token) =>
        UserOf(request) is null
            ? Task.FromResult(Reply(HttpStatusCode.Unauthorized, "sign in first"))
            : base.SendAsync(request, token);
}

class CacheHandler : DelegatingHandler
{
    private readonly Dictionary<(string?, string), (HttpStatusCode, string)> saved = [];

    protected override async Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, CancellationToken token)
    {
        var key = (UserOf(request), request.RequestUri!.AbsolutePath);
        if (saved.TryGetValue(key, out var hit))
        {
            return Reply(hit.Item1, hit.Item2 + " (cached)");
        }
        var response = await base.SendAsync(request, token);
        saved[key] = (response.StatusCode, await response.Content.ReadAsStringAsync(token));
        return response;
    }
}

// The last link is a plain HttpMessageHandler. It has no inner handler, so it must answer.
class AppHandler : HttpMessageHandler
{
    protected override Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, CancellationToken token) =>
        Task.FromResult(Reply(HttpStatusCode.OK, $"report for {UserOf(request)}"));
}
```

```output
log: /report from ada
200 report for ada
log: /report from nobody
401 sign in first
log: /report from ada
200 report for ada (cached)
```

- Verdict: stdlib. `System.Net.Http.DelegatingHandler` is a handler with a successor, the `InnerHandler` property. `base.SendAsync` passes the request on. `HttpMessageInvoker` sends a request into the first handler.
- No request goes to the network. `AppHandler` is the last link and answers every request itself.
- `HttpClient` takes the same chain in its constructor. Libraries add retries, logging and authentication to outgoing requests in this way.
- The helpers `UserOf` and `Reply` live in a static class. Local functions of the top-level statements cannot be used inside other classes, so `using static Shared` brings the helpers into scope instead.

```csharp run id=status-polling
var api = new Api();
var first = Show(0, "POST /reports key=k1", api.Post("k1", "2026-10", 0));
Show(0, "POST /reports key=k1 (retry)", api.Post("k1", "2026-10", 0));
Show(0, "POST /reports key=k2, no month", api.Post("k2", "", 0));
int wait = int.Parse(first.Headers["Retry-After"]);
string url = first.Headers["Location"];
int t = 0;
while (true)
{
    t += wait;
    var r = Show(t, "GET " + url, api.Get(url, t));
    if (r.Code == 303)
    {
        string loc = r.Headers["Location"];
        Show(t, "GET " + loc, api.Get(loc, t));
        break;
    }
}
Show(t, "GET /status/r9", api.Get("/status/r9", t));
Console.WriteLine($"jobs queued: {api.Queued}");

static Response Show(int t, string req, Response r)
{
    var parts = new List<string> { r.Code.ToString() };
    parts.AddRange(r.Pairs.Select(h => $"{h.Key}: {h.Value}"));
    if (r.Body != "")
    {
        parts.Add(r.Body);
    }
    Console.WriteLine($"t={t} {req} -> {string.Join(" ", parts)}");
    return r;
}

record Response(int Code, string Body, List<KeyValuePair<string, string>> Pairs)
{
    // A list keeps the order of the headers; the dictionary is for the lookups.
    public Dictionary<string, string> Headers => Pairs.ToDictionary(p => p.Key, p => p.Value);

    public static Response Of(int code, string body, params string[] h) =>
        new(code, body, Enumerable.Range(0, h.Length / 2).Select(i => KeyValuePair.Create(h[2 * i], h[2 * i + 1])).ToList());
}

class Api
{
    const int JobTime = 5; // seconds that the worker needs for one report
    readonly Dictionary<string, int> jobs = new(); // request id -> time the job started
    readonly Dictionary<string, string> keys = new(); // idempotency key -> request id
    public int Queued { get; private set; }

    public Response Post(string key, string month, int now)
    {
        if (month == "")
        {
            return Response.Of(400, "month is missing");
        }
        if (!keys.TryGetValue(key, out var rid))
        {
            rid = $"r{jobs.Count + 1}";
            keys[key] = rid;
            jobs[rid] = now;
            Queued++;
        }
        return Response.Of(202, "", "Location", "/status/" + rid, "Retry-After", "2");
    }

    public Response Get(string path, int now)
    {
        var parts = path.Trim('/').Split('/');
        if (!jobs.TryGetValue(parts[1], out var started))
        {
            return Response.Of(404, "");
        }
        int elapsed = now - started;
        if (parts[0] == "status")
        {
            if (elapsed < JobTime)
            {
                return Response.Of(200, $"status=Running percentComplete={elapsed * 100 / JobTime}");
            }
            return Response.Of(303, "", "Location", "/reports/" + parts[1]);
        }
        if (elapsed < JobTime)
        {
            return Response.Of(404, "");
        }
        return Response.Of(200, $"report {parts[1]}: 3 rows");
    }
}
```

```output
t=0 POST /reports key=k1 -> 202 Location: /status/r1 Retry-After: 2
t=0 POST /reports key=k1 (retry) -> 202 Location: /status/r1 Retry-After: 2
t=0 POST /reports key=k2, no month -> 400 month is missing
t=2 GET /status/r1 -> 200 status=Running percentComplete=40
t=4 GET /status/r1 -> 200 status=Running percentComplete=80
t=6 GET /status/r1 -> 303 Location: /reports/r1
t=6 GET /reports/r1 -> 200 report r1: 3 rows
t=6 GET /status/r9 -> 404
jobs queued: 1
```

- Verdict: idiom. A record `Response` keeps the headers as a list of `KeyValuePair` values, so they print in a fixed order.
- `TryGetValue` on the dictionary `keys` finds the request id of an idempotency key, so a second POST with the same key starts no second job.
- An ASP.NET Core API would return `Accepted(url)` for the 202; the handlers here are plain methods, so the program needs no network.

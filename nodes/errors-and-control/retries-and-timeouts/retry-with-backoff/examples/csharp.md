```csharp run id=budget
using System;

var budget = new RetryBudget(capacity: 5, refillEveryMs: 1000);
long now = 0;
int calls = 0;

for (int request = 1; request <= 10; request++)
{
    int retries = 0;
    for (int attempt = 0; attempt < 4; attempt++)
    {
        calls++;
        bool ok = false;
        if (ok || attempt == 3) break;
        if (!budget.TryTake(now)) break;
        retries++;
    }
    Console.WriteLine($"t={now,5} ms: request {request,2} failed after {retries} retries");
    now += 400;
}

Console.WriteLine($"calls with a budget: {calls}");
Console.WriteLine($"calls without a budget: {10 * 4}");

sealed class RetryBudget(int capacity, long refillEveryMs)
{
    private int tokens = capacity;
    private long lastRefill = 0;

    public bool TryTake(long now)
    {
        long earned = (now - lastRefill) / refillEveryMs;
        if (earned > 0)
        {
            tokens = (int)Math.Min(capacity, tokens + earned);
            lastRefill += earned * refillEveryMs;
        }
        if (tokens == 0) return false;
        tokens--;
        return true;
    }
}
```

```output
t=    0 ms: request  1 failed after 3 retries
t=  400 ms: request  2 failed after 2 retries
t=  800 ms: request  3 failed after 0 retries
t= 1200 ms: request  4 failed after 1 retries
t= 1600 ms: request  5 failed after 0 retries
t= 2000 ms: request  6 failed after 1 retries
t= 2400 ms: request  7 failed after 0 retries
t= 2800 ms: request  8 failed after 0 retries
t= 3200 ms: request  9 failed after 1 retries
t= 3600 ms: request 10 failed after 0 retries
calls with a budget: 18
calls without a budget: 40
```

- Verdict: stdlib. .NET has no retry type in the base class library. The example keeps time in a counter, so it runs at once.
- Each retry costs a token from a shared bucket, and the bucket earns one token each second. While the dependency is down, most requests fail at once, and the load from retries stays near one call each second.
- Without the budget, every request makes four calls, and the failing dependency gets four times the normal load.

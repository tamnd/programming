```csharp run id=observable
var sensor = new Sensor();
var sum = 0;
using var subscription = sensor.Where(t => t > 20).Subscribe(new Observer<int>(
    t => Console.WriteLine($"hot {t}, running sum {sum += t}"),
    () => Console.WriteLine("done")));

foreach (var t in new[] { 18, 22, 19, 25, 30 }) sensor.Publish(t);
sensor.Complete();

class Sensor : IObservable<int>
{
    readonly List<IObserver<int>> observers = new();

    public IDisposable Subscribe(IObserver<int> o)
    {
        observers.Add(o);
        return new Unsubscriber(() => observers.Remove(o));
    }

    public void Publish(int t) { foreach (var o in observers.ToList()) o.OnNext(t); }
    public void Complete() { foreach (var o in observers.ToList()) o.OnCompleted(); }
}

static class Operators
{
    public static IObservable<T> Where<T>(this IObservable<T> source, Func<T, bool> keep) =>
        new AnonymousObservable<T>(o => source.Subscribe(new Observer<T>(x => { if (keep(x)) o.OnNext(x); }, o.OnCompleted)));
}

class AnonymousObservable<T>(Func<IObserver<T>, IDisposable> subscribe) : IObservable<T>
{
    public IDisposable Subscribe(IObserver<T> o) => subscribe(o);
}

class Observer<T>(Action<T> next, Action completed) : IObserver<T>
{
    public void OnNext(T value) => next(value);
    public void OnCompleted() => completed();
    public void OnError(Exception e) => throw e;
}

class Unsubscriber(Action onDispose) : IDisposable
{
    public void Dispose() => onDispose();
}
```

```output for=observable
hot 22, running sum 22
hot 25, running sum 47
hot 30, running sum 77
done
```

- Verdict: stdlib. The interfaces `IObservable<T>` and `IObserver<T>` are in the `System` namespace of the base library, but the operators are not. Reactive Extensions for .NET (Rx.NET) gives `Where`, `Select`, `Throttle` and many more, and LINQ query syntax works on observables. The `Where` here is a small version of the Rx operator.

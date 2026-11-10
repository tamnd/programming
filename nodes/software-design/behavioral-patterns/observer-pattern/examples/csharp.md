```csharp run id=sensor
var sensor = new Sensor();
EventHandler<double> display = (sender, value) => Console.WriteLine($"display: {value:F1}");
sensor.Reading += display;
sensor.Reading += (sender, value) =>
{
    if (value > 30)
    {
        Console.WriteLine($"alarm: {value:F1} is above 30");
    }
};

sensor.Set(21.5);
sensor.Set(30.5);
sensor.Reading -= display;
sensor.Set(31.0);

class Sensor
{
    public event EventHandler<double>? Reading;

    public void Set(double value) => Reading?.Invoke(this, value);
}
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: native. An `event` member is a delegate field that other classes can only add to (`+=`) and remove from (`-=`). Only the declaring class can raise it.
- A delegate is immutable. `+=` makes a combined delegate with a longer invocation list. A raise calls the methods of the list in order, on the calling thread.
- `Reading?.Invoke` reads the field once. A handler that unsubscribes during the raise does not change the list that this raise calls.
- `-=` needs the same delegate, so the program keeps the display handler in a variable. A lambda written a second time is a different delegate, and `-=` with it removes nothing.
- `IObservable<T>` and `IObserver<T>` in the `System` namespace are the push-based stream form, which the Reactive Extensions library builds on.

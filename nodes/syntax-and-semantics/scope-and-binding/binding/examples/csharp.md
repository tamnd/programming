```csharp run id=ref-locals-and-captures
// A ref local binds a name to a storage location, so it is another name for that element.
int[] xs = { 1, 2, 3 };
ref int second = ref xs[1];
second = 20;
Console.WriteLine(string.Join(",", xs));

// A lambda captures the variable, not its value. A for loop has one variable for all
// iterations, and foreach has a new variable for each iteration.
var fromFor = new List<Func<int>>();
for (int i = 0; i < 3; i++) fromFor.Add(() => i);
var fromForeach = new List<Func<int>>();
foreach (int v in new[] { 0, 1, 2 }) fromForeach.Add(() => v);
Console.WriteLine(string.Join(",", fromFor.Select(f => f())) + " " + string.Join(",", fromForeach.Select(f => f())));

// virtual and override bind the call at run time; new hides the method, and the call binds
// at compile time to the declared type.
Base b = new Derived();
Console.WriteLine(b.Virtual() + " " + b.Hidden());

class Base
{
    public virtual string Virtual() => "Base.Virtual";
    public string Hidden() => "Base.Hidden";
}

class Derived : Base
{
    public override string Virtual() => "Derived.Virtual";
    public new string Hidden() => "Derived.Hidden";
}
```

```output for=ref-locals-and-captures
1,20,3
3,3,3 0,1,2
Derived.Virtual Base.Hidden
```

- Verdict: native. A `ref` local is another name for a storage location, so the assignment changes the array. A lambda captures the variable, so the closures from a `for` loop all see the last value, and the closures from `foreach` see one value each. `override` binds the call at run time, and `new` hides the method, so the call binds to the declared type.

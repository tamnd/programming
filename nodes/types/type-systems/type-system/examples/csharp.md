```csharp run id=casts-covariance-and-dynamic
#:property PublishAot=false

// Static checks: the compiler knows the type of each expression.
object o = "text";
Console.WriteLine(o is string s ? s.Length : -1);

// A cast is checked at run time.
try
{
    var i = (int)o;
    Console.WriteLine(i);
}
catch (InvalidCastException)
{
    Console.WriteLine("InvalidCastException");
}

// Array covariance moves a check to run time.
object[] items = new string[1];
try
{
    items[0] = 42;
}
catch (ArrayTypeMismatchException)
{
    Console.WriteLine("ArrayTypeMismatchException");
}

// dynamic turns off static checks, and the binder checks at run time.
dynamic d = 5;
Console.WriteLine(d + 1);
d = "five";
Console.WriteLine(d + 1);
try
{
    Console.WriteLine(d.NoSuchMethod());
}
catch (Microsoft.CSharp.RuntimeBinder.RuntimeBinderException)
{
    Console.WriteLine("RuntimeBinderException");
}

// as gives null instead of an exception.
string? t = o as string;
Uri? u = o as Uri;
Console.WriteLine($"{t} {u is null}");
```

```output for=casts-covariance-and-dynamic
4
InvalidCastException
ArrayTypeMismatchException
6
five1
RuntimeBinderException
text True
```

- Verdict: native. C# is static and nominal. A pattern `is string s` tests the type safely, and a cast `(int)o` throws `InvalidCastException`. Array covariance moves a check to run time, where it throws `ArrayTypeMismatchException`. `dynamic` turns off static checks, so `d + 1` is `6` for a number and `"five1"` for a string, and a call of a missing method throws `RuntimeBinderException`. `as` gives `null` when the type does not match.

```csharp run id=struct-class-tuple
using System;

// A tuple is a value type, so assignment copies it.
var t1 = (Name: "Ann", Age: 31);
var t2 = t1;
t2.Age = 32;
Console.WriteLine(t1.Age + " " + t2.Age);
Console.WriteLine(t1 == ("Ann", 31));

// A struct is copied, and a class is shared.
var s1 = new PointS { X = 1 };
var s2 = s1;
s2.X = 5;
var c1 = new PointC { X = 1 };
var c2 = c1;
c2.X = 5;
Console.WriteLine(s1.X + " " + c1.X);

// A record compares by value.
var r = new PointR(1, 2);
Console.WriteLine(r + " " + (r == new PointR(1, 2)));

// Arrays are reference types.
int[] a = { 1, 2 };
int[] b = a;
b[0] = 9;
Console.WriteLine(a[0]);

struct PointS { public int X; }
class PointC { public int X; }
record PointR(int X, int Y);
```

```output for=struct-class-tuple
31 32
True
1 5
PointR { X = 1, Y = 2 } True
9
```

- Verdict: native. A tuple and a `struct` are value types, so `t2` and `s2` are copies, and a change to them does not change `t1` and `s1`. A `class` is a reference type, so `c1` and `c2` are one object. A `record` is a class with value equality and a generated `ToString`. Arrays are reference types, as in Java.

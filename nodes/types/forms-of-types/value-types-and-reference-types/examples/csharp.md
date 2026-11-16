```csharp run id=struct-copy
using System;
using System.Collections.Generic;

var points = new List<PointS> { new PointS { X = 1 } };
var p = points[0]; // the indexer returns a copy
p.X = 9;
Console.WriteLine($"{points[0].X} {p.X}");

points[0] = p; // write the changed copy back
Console.WriteLine(points[0].X);

var boxes = new List<PointC> { new PointC { X = 1 } };
var c = boxes[0]; // a copy of the reference
c.X = 9;
Console.WriteLine(boxes[0].X);

Move(ref p);
Console.WriteLine(p.X);

static void Move(ref PointS q) => q.X += 1;

struct PointS
{
    public int X;
}

class PointC
{
    public int X;
}
```

```output for=struct-copy
1 9
9
9
10
```

- Verdict: native. `PointS` is a struct, so reading `points[0]` gives a copy, and the change to `p` does not reach the list until the code writes `p` back. `PointC` is a class, so `boxes[0]` gives a reference, and the change shows through the list. A `ref` parameter passes the variable itself, so `Move` changes the caller's struct.

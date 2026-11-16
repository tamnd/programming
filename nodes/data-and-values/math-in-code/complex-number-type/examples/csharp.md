```csharp run id=numerics-complex
using System;
using System.Numerics;

var z = new Complex(3, 4);
var w = new Complex(1, -2);
Console.WriteLine(z * w);
Console.WriteLine($"{z.Real} {z.Imaginary} {z.Magnitude} {Complex.ImaginaryOne.Phase}");
Console.WriteLine(Complex.Sqrt(-4));
Console.WriteLine(Complex.Conjugate(z) == new Complex(3, -4));
Complex fromInt = 5;
Console.WriteLine(fromInt);
```

```output for=numerics-complex
<11; -2>
3 4 5 1.5707963267948966
<0; 2>
True
<5; 0>
```

- Verdict: stdlib. `System.Numerics.Complex` is a struct with two `double` parts and the operators `+`, `-`, `*`, `/` and `==`. `Magnitude` and `Phase` give the polar form, and `Complex.Sqrt` accepts a negative number. An `int` or a `double` converts to `Complex` with no cast. `ToString` prints the form `<re; im>`.

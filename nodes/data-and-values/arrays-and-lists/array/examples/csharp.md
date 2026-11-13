```csharp run id=reference-type-arrays
using System;

class Program
{
    static void Main()
    {
        // new int[5] makes five items with the default value 0.
        int[] a = new int[5];
        a[0] = 7;
        a[^1] = 9;
        Console.WriteLine($"{string.Join(",", a)} {a.Length}");

        // An index outside 0 to Length - 1 throws IndexOutOfRangeException.
        try
        {
            a[5] = 1;
        }
        catch (IndexOutOfRangeException e)
        {
            Console.WriteLine(e.GetType().Name);
        }

        // An array is a reference type. Assignment copies the reference.
        int[] b = a;
        b[1] = 1;
        int[] c = (int[])a.Clone();
        c[1] = 2;
        Console.WriteLine($"{a[1]} {c[1]} {ReferenceEquals(a, b)}");

        // Array.Resize makes a new array and copies the items into it.
        int[] old = a;
        Array.Resize(ref a, 7);
        Console.WriteLine($"{a.Length} {old.Length} {ReferenceEquals(a, old)}");

        // Arrays of reference types are covariant, so the store check runs at run time.
        object[] objects = new string[2];
        try
        {
            objects[0] = 42;
        }
        catch (ArrayTypeMismatchException e)
        {
            Console.WriteLine(e.GetType().Name);
        }

        // A collection expression gives the items of a new array.
        int[] d = [1, 2, 3];
        Console.WriteLine(d.Length);
    }
}
```

```output
7,0,0,0,9 5
IndexOutOfRangeException
1 2 True
7 5 False
ArrayTypeMismatchException
3
```

- Verdict: native. An array is a reference type with a fixed length and default values, and every index is checked. a[^1] is the last item. Array.Resize makes a new array, so the old reference still has 5 items. Covariant arrays throw ArrayTypeMismatchException for a wrong store.

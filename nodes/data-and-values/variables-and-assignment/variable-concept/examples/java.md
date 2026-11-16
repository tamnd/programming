```java run id=boxes
public class Main {
    public static void main(String[] args) {
        int a = 1;
        int b = a;
        b = b + 1;
        System.out.println(a + " " + b);

        int[] xs = {1, 2};
        int[] ys = xs;
        ys[0] = 9;
        System.out.println(xs[0] + " " + ys[0]);

        var name = "Ada";
        System.out.println(name.length());
    }
}
```

```output for=boxes
1 2
9 9
3
```

- Verdict: native. A Java variable is a storage location with a type. `int b = a` copies the number, so a change to `b` does not change `a`. An array variable holds a reference, so `ys = xs` copies the reference, and both names show the same array. `var` takes the type from the initializer: `name` has the type `String`, and `name = 3` does not compile.

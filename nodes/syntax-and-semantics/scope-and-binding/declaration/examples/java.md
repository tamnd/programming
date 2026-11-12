```java run id=members-in-any-order-and-var
import java.util.List;

public class Main {
    // A method can be used before its declaration in the class body.
    static int twice(int x) { return add(x, x); }
    static int add(int a, int b) { return a + b; }

    // A field declaration with an initializer.
    static final List<String> NAMES = List.of("a", "b");

    public static void main(String[] args) {
        System.out.println("twice(4) = " + twice(4));

        // A local variable must be declared before it is used.
        int y;
        if (args.length == 0) { y = 1; } else { y = 2; }
        System.out.println("y = " + y);  // definitely assigned on each path

        var list = NAMES;  // var: the compiler infers the declared type
        System.out.println("list = " + list + ", size " + list.size());

        // A local class declaration.
        record Pair(int a, int b) { int sum() { return a + b; } }
        System.out.println("sum = " + new Pair(2, 3).sum());
    }
}
```

```output for=members-in-any-order-and-var
twice(4) = 8
y = 1
list = [a, b], size 2
sum = 5
```

- Verdict: native. A method is in scope in the whole class body, so `twice` can call `add`, which comes after it. A local variable must be declared before use, and the compiler checks that it has a value on each path. `var` declares a local variable with an inferred type. A record can be declared inside a method.

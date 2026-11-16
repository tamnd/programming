```java run id=strict
public class Main {
    static <T> T trace(String label, T value) {
        System.out.println("evaluating " + label);
        return value;
    }

    static int pick(boolean flag, int left, int right) {
        System.out.println("inside pick");
        return flag ? left : right;
    }

    public static void main(String[] args) {
        System.out.println(pick(trace("flag", true), trace("left", 1), trace("right", 2)));
        System.out.println(trace("x", false) && trace("y", true));
    }
}
```

```output for=strict
evaluating flag
evaluating left
evaluating right
inside pick
1
evaluating x
false
```

- Verdict: native. The Java Language Specification fixes the order: each argument is fully evaluated before the arguments to its right, and all of them before the method body.
- `&&`, `||` and `?:` are the non-strict operators. A `Supplier<T>` parameter gives a non-strict argument: the method calls `get()` only if it needs the value.

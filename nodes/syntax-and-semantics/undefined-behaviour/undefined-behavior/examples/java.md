```java run id=defined
public class Main {
    public static void main(String[] args) {
        int x = Integer.MAX_VALUE;
        System.out.println(x + 1);
        try {
            System.out.println(Math.addExact(x, 1));
        } catch (ArithmeticException e) {
            System.out.println(e);
        }
        int[] a = {1, 2, 3};
        try {
            System.out.println(a[3]);
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
        }
    }
}
```

```output for=defined
-2147483648
java.lang.ArithmeticException: integer overflow
Index 3 out of bounds for length 3
```

- Verdict: absent. The Java Language Specification gives a result for each operation. Integer addition that overflows gives the low-order bits of the true sum in two's complement.
- `Math.addExact` throws `ArithmeticException` when the result overflows an `int`.
- An index out of range throws `ArrayIndexOutOfBoundsException`. The check is part of the language, and it cannot be turned off.
- A data race gives surprising results, but the memory model limits them. A race does not make the meaning of the whole program undefined.

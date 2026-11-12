```java run id=defined-evaluation
public class Main {
    static int trace(String name, int value) {
        System.out.print(name + " ");
        return value;
    }

    public static void main(String[] args) {
        // The JLS fixes the order: operands are evaluated from left to right.
        int r = trace("a", 1) + trace("b", 2) * trace("c", 3);
        System.out.println("= " + r);

        int i = 1;
        i = i++ + ++i; // defined in Java: 1 + 3
        System.out.println("i = " + i);

        // Overflow wraps, and division truncates toward zero.
        System.out.println(Integer.MAX_VALUE + 1 == Integer.MIN_VALUE);
        System.out.println((-7 / 2) + " " + (-7 % 2));
    }
}
```

```output for=defined-evaluation
a b c = 7
i = 4
true
-3 -1
```

- Verdict: absent. The Java Language Specification is written in English, but it leaves little open: it fixes the order of evaluation, the result of overflow and the rounding of division, and the Java memory model gives a formal semantics for threads. So `i = i++ + ++i` has one result, 4, on every JVM, and `Integer.MAX_VALUE + 1` is `Integer.MIN_VALUE`. The same lines in C have no defined meaning.

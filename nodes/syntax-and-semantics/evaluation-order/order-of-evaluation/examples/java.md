```java run id=trace
public class Main {
    static int f(String name, int value) {
        System.out.println("evaluate " + name);
        return value;
    }

    public static void main(String[] args) {
        System.out.println(f("a", 1) + f("b", 2) * f("c", 3));

        int a = 9;
        a += (a = 3);
        int b = 9;
        b = b + (b = 3);
        System.out.println(a + " " + b);
    }
}
```

```output for=trace
evaluate a
evaluate b
evaluate c
7
12 12
```

- Verdict: native. The Java Language Specification guarantees that the operands of an operator appear to be evaluated from left to right. The left operand is fully evaluated before any part of the right operand.
- In a compound assignment, the value of the left-hand variable is saved before the right-hand side is evaluated. So `a += (a = 3)` adds 3 to the saved 9.
- The specification still recommends that code does not depend on this rule.

```java wrong id=self-increment
public class Main {
    public static void main(String[] args) {
        int count = 0;
        for (int n = 0; n < 3; n++) {
            count = count++;
        }
        System.out.println(count);
    }
}
```

```output for=self-increment
0
```

`count++` gives the old value, 0, and then increments `count`. The assignment then stores the old value back. The increment is lost at each step. Write `count++;` as its own statement.

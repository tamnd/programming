```java run id=switch-expression-and-yield
public class Main {
    static String label(int score) {
        return switch (score / 10) { // a switch expression has a value
            case 10, 9 -> "A";
            case 8, 7, 6, 5 -> "pass";
            default -> {
                String s = "fail";
                yield s; // a block in a switch expression yields its value
            }
        };
    }

    public static void main(String[] args) {
        System.out.println(label(95) + " " + label(70) + " " + label(30));

        int a, b;
        a = b = 5;
        System.out.println(a + " " + b);

        int[] data = {3, 1, 4, 0, 5};
        int i = 0, n, total = 0;
        while ((n = data[i++]) != 0) total += n;
        System.out.println(total);
        System.out.println(total > 5 ? "big" : "small");
    }
}
```

```output for=switch-expression-and-yield
A pass fail
5 5
8
big
```

- Verdict: native. Java has the statements and expressions of C, with assignment as an expression and `?:` as the conditional expression. Since Java 14, `switch` can also be an expression: each arrow case gives a value, a block case gives its value with `yield`, and the compiler checks that the cases cover every input.

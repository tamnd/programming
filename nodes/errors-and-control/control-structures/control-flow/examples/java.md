```java run id=switch-expression-and-abrupt-completion
public class Main {
    static String grade(int score) {
        // A switch with arrows has no fallthrough; each case is one branch.
        return switch (score / 10) {
            case 10, 9 -> "A";
            case 8 -> "B";
            case 7 -> "C";
            default -> "F";
        };
    }

    static int parse(String s) {
        // An exception completes the try block abruptly, and control goes to the catch.
        try {
            int v = Integer.parseInt(s);
            System.out.println("parsed " + v);
            return v;
        } catch (NumberFormatException e) {
            System.out.println("bad input " + s);
            return -1;
        } finally {
            System.out.println("finally " + s);
        }
    }

    public static void main(String[] args) {
        System.out.println(grade(95) + " " + grade(81) + " " + grade(42));
        parse("12");
        parse("x");

        // do-while runs its body before the first test.
        int n = 0;
        do {
            n++;
        } while (n < 0);
        System.out.println("do-while ran " + n + " time");

        // A loop with break and continue.
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 10; i++) {
            if (i % 2 == 0) continue;
            if (i > 7) break;
            sb.append(i).append(' ');
        }
        System.out.println(sb.toString().trim());
    }
}
```

```output for=switch-expression-and-abrupt-completion
A B F
parsed 12
finally 12
bad input x
finally x
do-while ran 1 time
1 3 5 7
```

- Verdict: native. A `switch` expression with arrows has no fallthrough. `Integer.parseInt("x")` throws, so the `try` block completes abruptly, control goes to `catch`, and `finally` runs on both paths. `do while` runs its body once before the first test. `continue` skips the even numbers and `break` stops the loop after 7.

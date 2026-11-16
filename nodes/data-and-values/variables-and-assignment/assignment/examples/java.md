```java run id=expression
public class Main {
    public static void main(String[] args) {
        int a, b, c;
        a = b = c = 7;
        System.out.println(a + b + c);
        int n;
        System.out.println((n = 4) * 10);
        int[] xs = {0, 0};
        int i = 0;
        xs[i] = i = 1;
        System.out.println(xs[0] + " " + xs[1] + " " + i);
    }
}
```

```output for=expression
21
40
1 0 1
```

- Verdict: native. An assignment is an expression whose value is the new value of the variable, so assignments chain from the right. Java evaluates the left operand of `=` first: in `xs[i] = i = 1`, the index `i` is still 0 when Java computes the place, so the statement sets `xs[0]`.

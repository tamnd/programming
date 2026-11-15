```java run id=two-counters-and-char-ranges
public class Main {
    public static void main(String[] args) {
        // Two variables in the header: i counts up and j counts down.
        for (int i = 0, j = 10; i < j; i += 3, j -= 3) {
            System.out.println(i + " " + j);
        }

        // A char counter loops over a range of letters.
        StringBuilder sb = new StringBuilder();
        for (char c = 'a'; c <= 'e'; c++) {
            sb.append(c);
        }
        System.out.println(sb);

        // A counter declared before the loop keeps its value after it.
        int k;
        for (k = 1; k * k < 50; k++) {
        }
        System.out.println("first k with k*k >= 50: " + k);
    }
}
```

```output for=two-counters-and-char-ranges
0 10
3 7
abcde
first k with k*k >= 50: 8
```

- Verdict: native. The basic `for` takes lists of declarations and updates, so `i` and `j` step together until they cross. A `char` counter goes from `'a'` to `'e'`. A variable declared in the header exists only in the loop, so the last loop declares `k` before it to read its final value.

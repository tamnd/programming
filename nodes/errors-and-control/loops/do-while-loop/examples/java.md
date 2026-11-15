```java run id=digit-count-do-and-while
public class Main {
    static int digitsDo(int n) {
        int count = 0;
        do {
            n /= 10;
            count++;
        } while (n != 0);
        return count;
    }

    static int digitsWhile(int n) {
        int count = 0;
        while (n != 0) {
            n /= 10;
            count++;
        }
        return count;
    }

    public static void main(String[] args) {
        for (int n : new int[] {0, 5, 12345, -987}) {
            System.out.println(n + ": do " + digitsDo(n) + ", while " + digitsWhile(n));
        }
    }
}
```

```output for=digit-count-do-and-while
0: do 1, while 0
5: do 1, while 1
12345: do 5, while 5
-987: do 3, while 3
```

- Verdict: native. The two methods differ only in the place of the test. For 0, the `do` loop runs its body once and gives 1, the right number of digits, and the `while` loop never runs and gives 0. Integer division in Java rounds toward zero, so negative numbers also reach 0.

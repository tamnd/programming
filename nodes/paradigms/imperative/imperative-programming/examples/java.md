```java run id=in-place
import java.util.Arrays;

public class Main {
    static void reverseInPlace(int[] xs) {
        for (int i = 0, j = xs.length - 1; i < j; i++, j--) {
            int t = xs[i];
            xs[i] = xs[j];
            xs[j] = t;
        }
    }

    public static void main(String[] args) {
        int[] data = {1, 2, 3, 4, 5};
        reverseInPlace(data);
        System.out.println(Arrays.toString(data));

        int count = 0;
        for (char ch : "imperative".toCharArray()) {
            if ("aeiou".indexOf(ch) >= 0) {
                count++;
            }
        }
        System.out.println(count);

        StringBuilder sb = new StringBuilder();
        for (int i = 3; i > 0; i--) {
            sb.append(i).append(' ');
        }
        System.out.println(sb.append("go"));
    }
}
```

```output for=in-place
[5, 4, 3, 2, 1]
5
3 2 1 go
```

- Verdict: native. The Java specification says that "The sequence of execution of a program is controlled by statements, which are executed for their effect and do not have values."
- A method receives a copy of a reference to the array, so it can change the elements of the array of the caller. `final` stops reassignment of a variable, not changes to the object.

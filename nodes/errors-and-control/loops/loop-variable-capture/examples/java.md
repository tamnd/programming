```java run id=enhanced-for-copy-and-a-mutable-box
import java.util.ArrayList;
import java.util.List;
import java.util.function.Supplier;

public class Main {
    static String run(List<? extends Supplier<?>> fs) {
        List<String> out = new ArrayList<>();
        for (Supplier<?> f : fs) out.add(String.valueOf(f.get()));
        return String.join(" ", out);
    }

    public static void main(String[] args) {
        // The variable of an enhanced for is new in each pass.
        List<Supplier<String>> names = new ArrayList<>();
        for (String s : List.of("a", "b", "c")) {
            names.add(() -> s);
        }
        System.out.println("enhanced for: " + run(names));

        // The counter i changes, so () -> i does not compile.
        // A copy in the body is effectively final.
        List<Supplier<Integer>> copies = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            int copy = i;
            copies.add(() -> copy);
        }
        System.out.println("copy: " + run(copies));

        // An array is effectively final, but its element changes.
        int[] box = {0};
        List<Supplier<Integer>> boxed = new ArrayList<>();
        for (box[0] = 0; box[0] < 3; box[0]++) {
            boxed.add(() -> box[0]);
        }
        System.out.println("box: " + run(boxed));
    }
}
```

```output for=enhanced-for-copy-and-a-mutable-box
enhanced for: a b c
copy: 0 1 2
box: 3 3 3
```

- Verdict: native. Java lets a lambda capture only a variable that is final or effectively final. The variable of the enhanced `for` is declared again in each pass, so the lambdas get `a`, `b` and `c`. The counter of a basic `for` changes, so the code copies it into `copy`. The rule is about the variable, not the object, so a lambda that captures an array reads the element when it runs and sees 3 three times.

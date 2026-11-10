```java run id=frames
public class Main {
    static int deepest = 0;

    static void descend(int level) {
        deepest = level;
        descend(level + 1);
    }

    public static void main(String[] args) {
        try {
            descend(1);
        } catch (StackOverflowError error) {
            System.out.println("StackOverflowError after more than 1000 frames: " + (deepest > 1000));
        }

        int count = 5;
        int copy = count;
        copy++;
        System.out.println(count + " " + copy);

        int[] totals = {5};
        int[] alias = totals;
        alias[0]++;
        System.out.println(totals[0]);

        Object boxed = count;
        System.out.println(boxed.getClass().getName());
    }
}
```

```output
StackOverflowError after more than 1000 frames: true
5 6
6
java.lang.Integer
```

- Verdict: native. Each thread has a stack of frames, and every object and array lives on the heap. A local of a primitive type such as `int` holds the value itself.
- `StackOverflowError` is an error that the program can catch. The JVM throws it when a thread needs a larger stack than the limit, which `-Xss` sets.
- Assigning an `int` copies the number. Assigning an array copies the reference, so `alias` and `totals` share one array.
- Assigning an `int` to an `Object` boxes it into an `Integer`. HotSpot escape analysis can remove some allocations, but it never moves an object to the stack.

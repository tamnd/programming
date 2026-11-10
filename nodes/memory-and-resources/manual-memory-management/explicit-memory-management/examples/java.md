```java run id=confined-arena
import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

public class Main {
    public static void main(String[] args) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment totals = arena.allocate(ValueLayout.JAVA_INT, 4);
            for (int i = 0; i < 4; i++) {
                totals.setAtIndex(ValueLayout.JAVA_INT, i, i * 10);
            }
            System.out.println("last total: " + totals.getAtIndex(ValueLayout.JAVA_INT, 3));
        } // close() frees every segment of the arena
        System.out.println("arena closed");
    }
}
```

```output
last total: 30
arena closed
```

```java fails expect=exception:IllegalStateException id=use-after-close
import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

public class Main {
    public static void main(String[] args) {
        Arena arena = Arena.ofConfined();
        MemorySegment totals = arena.allocate(ValueLayout.JAVA_INT, 4);
        arena.close();
        System.out.println(totals.getAtIndex(ValueLayout.JAVA_INT, 0));
    }
}
```

- Verdict: stdlib. The Foreign Function and Memory API allocates memory outside the Java heap, and `Arena.close()` frees it.
- An arena frees all its segments at once, so the program frees a group of blocks and not each block.
- After `close`, an access to a segment throws `IllegalStateException`. In C the same mistake is a use after free with undefined behaviour.
- A confined arena belongs to the thread that made it. `Arena.ofShared()` makes an arena that several threads can use and close.

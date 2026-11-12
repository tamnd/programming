```java run id=foreign-memory-segment
import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

public class Main {
    public static void main(String[] args) {
        // Java objects have no visible address; the garbage collector can move them.
        Object o = new Object();
        Object p = o;
        System.out.println("same object: " + (o == p));

        // The foreign memory API gives native memory with a real address.
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment seg = arena.allocate(ValueLayout.JAVA_INT, 4);
            for (int i = 0; i < 4; i++) seg.setAtIndex(ValueLayout.JAVA_INT, i, (i + 1) * 10);
            MemorySegment slice = seg.asSlice(8);
            System.out.println("slice address - base address = " + (slice.address() - seg.address()));
            System.out.println("int at offset 8: " + seg.get(ValueLayout.JAVA_INT, 8));
            System.out.println("aligned to 4: " + (seg.address() % 4 == 0));
            try {
                seg.get(ValueLayout.JAVA_INT, 16);
            } catch (IndexOutOfBoundsException e) {
                System.out.println("offset 16 is out of bounds");
            }
        }
    }
}
```

```output for=foreign-memory-segment
same object: true
slice address - base address = 8
int at offset 8: 30
aligned to 4: true
offset 16 is out of bounds
```

- Verdict: stdlib. Java objects have no visible address, and `==` on references compares identity. The foreign memory API in `java.lang.foreign` allocates native memory in an arena. A `MemorySegment` has an `address()`, a slice at offset 8 starts 8 bytes after the base, and reads at a byte offset are checked, so offset 16 in a segment of 16 bytes throws `IndexOutOfBoundsException`. Closing the arena frees the memory.

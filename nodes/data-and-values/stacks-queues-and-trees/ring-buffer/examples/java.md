```java run id=ring
public class Main {
    static final class RingBuffer {
        private final int[] items;
        private int start = 0;
        private int count = 0;

        RingBuffer(int capacity) {
            items = new int[capacity];
        }

        void push(int item) {
            items[(start + count) % items.length] = item;
            if (count < items.length) {
                count++;
            } else {
                start = (start + 1) % items.length;
            }
        }

        int pop() {
            if (count == 0) {
                throw new IllegalStateException("empty");
            }
            int item = items[start];
            start = (start + 1) % items.length;
            count--;
            return item;
        }

        String contents() {
            StringBuilder out = new StringBuilder();
            for (int i = 0; i < count; i++) {
                out.append(" ").append(items[(start + i) % items.length]);
            }
            return out.toString();
        }
    }

    public static void main(String[] args) {
        RingBuffer ring = new RingBuffer(3);
        for (int reading : new int[] {10, 20, 30, 40, 50}) {
            ring.push(reading);
            System.out.println("after " + reading + ":" + ring.contents());
        }

        int oldest = ring.pop();
        System.out.println("read " + oldest + ", left:" + ring.contents());
    }
}
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: idiom. The standard `ArrayDeque` grows, so a fixed ring that overwrites is a small class over an `int` array.
- `pop` throws `IllegalStateException` when the buffer is empty.

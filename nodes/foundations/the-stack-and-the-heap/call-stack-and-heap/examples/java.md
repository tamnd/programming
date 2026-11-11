```java run id=frames-heap-and-thread-stacks
public class Main {
    static int calls = 0;

    static int[] squares(int n) {
        int[] out = new int[n]; // the array is on the heap, out is in the frame
        for (int i = 0; i < n; i++) out[i] = i * i;
        return out; // the frame goes, the array stays
    }

    static void down() {
        calls++;
        down();
    }

    static long deep(int n) {
        return n == 0 ? 0 : 1 + deep(n - 1);
    }

    public static void main(String[] args) throws InterruptedException {
        int[] s = squares(4);
        System.out.println(s[3] + " " + s.length);

        try {
            down();
        } catch (StackOverflowError e) {
            System.out.println(e.getClass().getSimpleName() + " " + (calls > 1000));
        }

        long[] result = new long[1];
        Thread big = new Thread(null, () -> result[0] = deep(1_000_000), "big", 512L << 20);
        big.start(); // a thread with a 512 MiB stack
        big.join();
        System.out.println(result[0]);
    }
}
```

```output for=frames-heap-and-thread-stacks
9 4
StackOverflowError true
1000000
```

- Verdict: native. Each Java thread has its own stack of frames, which hold local variables and partial results, and all threads share one heap, which holds every object and array. `squares` returns a reference to an array on the heap, so the array outlives the frame that made it.
- When a thread's stack is full, the JVM throws `StackOverflowError`, which a program can catch, though the state after it is often hard to trust. The fourth argument of the `Thread` constructor asks for a stack size, and a thread with a 512 MiB stack can recurse a million levels deep. The `-Xss` option sets the default size.

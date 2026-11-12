```java run id=fork-join
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final int THRESHOLD = 10_000; // a range this small is summed directly
    static final AtomicInteger leaves = new AtomicInteger();
    static final AtomicInteger largest = new AtomicInteger();

    // Returns the sum of lo, lo + 1, ..., hi - 1.
    static class Sum extends RecursiveTask<Long> {
        final long lo, hi;

        Sum(long lo, long hi) {
            this.lo = lo;
            this.hi = hi;
        }

        @Override
        protected Long compute() {
            if (hi - lo <= THRESHOLD) {
                leaves.incrementAndGet();
                largest.accumulateAndGet((int) (hi - lo), Math::max);
                long s = 0;
                for (long i = lo; i < hi; i++) s += i;
                return s;
            }
            long mid = (lo + hi) / 2;
            Sum left = new Sum(lo, mid);
            left.fork(); // fork: the pool can run the left half on another thread
            long right = new Sum(mid, hi).compute(); // this thread solves the right half
            return left.join() + right; // join: wait for the left half
        }
    }

    public static void main(String[] args) {
        long n = 1_000_000;
        long total = ForkJoinPool.commonPool().invoke(new Sum(1, n + 1));
        System.out.println("sum: " + total);
        System.out.println("leaf tasks: " + leaves.get());
        System.out.println("largest leaf, at most 10000: " + (largest.get() <= THRESHOLD ? "True" : "False"));
        System.out.println("matches n(n+1)/2: " + (total == n * (n + 1) / 2 ? "True" : "False"));
    }
}
```

```output
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

- Verdict: stdlib. `RecursiveTask` gives `fork` and `join`, and `ForkJoinPool.commonPool()` runs the tasks.

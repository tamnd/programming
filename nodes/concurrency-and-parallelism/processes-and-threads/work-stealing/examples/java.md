```java run id=work-stealing-model
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public class Main {
    // A real fork/join task: split the range until it is small, then add its numbers.
    static class SumTask extends RecursiveTask<Long> {
        final int lo, hi;

        SumTask(int lo, int hi) {
            this.lo = lo;
            this.hi = hi;
        }

        @Override
        protected Long compute() {
            if (hi - lo <= 10_000) {
                long sum = 0;
                for (int i = lo; i < hi; i++) sum += i;
                return sum;
            }
            int mid = (lo + hi) >>> 1;
            SumTask left = new SumTask(lo, mid);
            left.fork(); // push the left half on this worker's deque, where an idle worker can steal it
            long right = new SumTask(mid, hi).compute();
            return right + left.join();
        }
    }

    public static void main(String[] args) {
        model();
        try (ForkJoinPool pool = new ForkJoinPool(4)) {
            System.out.println("ForkJoinPool sum of [0, 1000000): " + pool.invoke(new SumTask(0, 1_000_000)));
        }
    }

    // A step-by-step model of a work-stealing scheduler with two workers.
    // Each worker has a deque. The first element is the top (oldest task); the last is the bottom (newest task).
    // A task is a range [lo, hi). A range of more than 2 numbers splits into two halves; a smaller range is a leaf that adds its numbers.
    static void model() {
        final int workers = 2;
        List<ArrayDeque<int[]>> deques = new ArrayList<>();
        for (int w = 0; w < workers; w++) deques.add(new ArrayDeque<>());
        int[] leaves = new int[workers];
        int total = 0;

        deques.get(0).addLast(new int[] {0, 8});
        for (int round = 1; deques.stream().anyMatch(d -> !d.isEmpty()); round++) {
            for (int w = 0; w < workers; w++) {
                int[] task;
                String how = "pops", source = "";
                if (!deques.get(w).isEmpty()) {
                    task = deques.get(w).pollLast(); // the owner takes the newest task (LIFO)
                } else {
                    int victim = -1;
                    for (int v = 0; v < workers; v++) {
                        if (v != w && !deques.get(v).isEmpty()) {
                            victim = v;
                            break;
                        }
                    }
                    if (victim == -1) {
                        System.out.printf("round %d: w%d is idle%n", round, w);
                        continue;
                    }
                    task = deques.get(victim).pollFirst(); // a thief takes the oldest task (FIFO)
                    how = "steals";
                    source = " from w" + victim;
                }
                int lo = task[0], hi = task[1];
                if (hi - lo > 2) {
                    int mid = (lo + hi) / 2;
                    deques.get(w).addLast(new int[] {lo, mid});
                    deques.get(w).addLast(new int[] {mid, hi});
                    System.out.printf("round %d: w%d %s [%d, %d)%s and splits it%n", round, w, how, lo, hi, source);
                } else {
                    for (int i = lo; i < hi; i++) total += i;
                    leaves[w]++;
                    System.out.printf("round %d: w%d %s [%d, %d)%s and runs it%n", round, w, how, lo, hi, source);
                }
            }
        }
        System.out.printf("leaves run: w0 %d, w1 %d%n", leaves[0], leaves[1]);
        System.out.println("sum: " + total);
    }
}
```

```output for=work-stealing-model
round 1: w0 pops [0, 8) and splits it
round 1: w1 steals [0, 4) from w0 and splits it
round 2: w0 pops [4, 8) and splits it
round 2: w1 pops [2, 4) and runs it
round 3: w0 pops [6, 8) and runs it
round 3: w1 pops [0, 2) and runs it
round 4: w0 pops [4, 6) and runs it
round 4: w1 is idle
leaves run: w0 2, w1 2
sum: 28
ForkJoinPool sum of [0, 1000000): 499999500000
```

- Verdict: stdlib. `ForkJoinPool` is a work-stealing pool. `fork` pushes a task on the deque of the current worker, and `join` waits for it, running other tasks in the meantime.
- The first part is a model of the rules, with two `ArrayDeque` objects in place of two threads. `pollLast` is the owner's LIFO pop, and `pollFirst` is the thief's FIFO steal.
- The second part uses a real `ForkJoinPool` with four workers. Which worker runs which half changes from run to run, but the sum does not.
- `Executors.newWorkStealingPool` also gives a work-stealing pool.

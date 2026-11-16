// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

// Submit one job for each number to a fixed pool of worker threads and collect the results in order.

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public final class Pool {
  private Pool() {}

  static int collatzSteps(long n) {
    int steps = 0;
    while (n != 1) {
      n = n % 2 == 0 ? n / 2 : 3 * n + 1;
      steps++;
    }
    return steps;
  }

  /** Returns the step count of each number, in the order of numbers. */
  public static int[] stepsOnPool(int workers, int[] numbers)
      throws InterruptedException, ExecutionException {
    List<Future<Integer>> futures = new ArrayList<>();
    // The pool has at most `workers` threads; jobs wait in its queue for a free thread.
    // close() waits until every submitted job has finished, and the threads then end.
    try (ExecutorService pool = Executors.newFixedThreadPool(workers)) {
      for (int n : numbers) {
        futures.add(pool.submit(() -> collatzSteps(n)));
      }
    }
    int[] results = new int[numbers.length];
    for (int i = 0; i < results.length; i++) {
      // get() returns the value, or throws ExecutionException with the exception of the job.
      results[i] = futures.get(i).get();
    }
    return results;
  }
}

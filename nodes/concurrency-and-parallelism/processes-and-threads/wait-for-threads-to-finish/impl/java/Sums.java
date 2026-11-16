// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

/** Sums each chunk on its own thread, waits for all of them and collects the sums. */
public final class Sums {
  private Sums() {}

  public static long[] parallelSums(long[][] chunks) throws InterruptedException {
    long[] sums = new long[chunks.length];
    Thread[] threads = new Thread[chunks.length];
    for (int i = 0; i < chunks.length; i++) {
      int index = i;
      threads[i] =
          Thread.ofPlatform()
              .start(
                  () -> {
                    long sum = 0;
                    for (long n : chunks[index]) {
                      sum += n;
                    }
                    sums[index] = sum;
                  });
    }
    // Wait for every thread. The end of a thread happens-before the return of its join().
    for (Thread thread : threads) {
      thread.join();
    }
    return sums;
  }
}

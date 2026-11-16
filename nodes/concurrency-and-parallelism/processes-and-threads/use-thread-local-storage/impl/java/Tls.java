// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

// Count on a thread-local counter in one thread for each count, with no lock and no shared counter.

public final class Tls {
  private Tls() {}

  // One ThreadLocal for all threads. Each thread gets its own copy, which starts at 0.
  private static final ThreadLocal<Integer> COUNTER = ThreadLocal.withInitial(() -> 0);

  static int bump() {
    int next = COUNTER.get() + 1;
    COUNTER.set(next);
    return next;
  }

  static int current() {
    return COUNTER.get();
  }

  public static int[] countPerThread(int[] counts) throws InterruptedException {
    int[] results = new int[counts.length];
    Thread[] threads = new Thread[counts.length];
    for (int i = 0; i < counts.length; i++) {
      int index = i;
      threads[i] =
          Thread.ofPlatform()
              .start(
                  () -> {
                    for (int k = 0; k < counts[index]; k++) {
                      bump();
                    }
                    // Each thread writes only its own slot. join() makes the write visible to the caller.
                    results[index] = current();
                  });
    }
    for (Thread t : threads) {
      t.join();
    }
    return results;
  }
}

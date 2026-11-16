// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Queue;

final class RoundRobin {
  private RoundRobin() {}

  /** Returns the job numbers in the order in which round-robin finishes them. */
  static List<Integer> finishOrder(List<Integer> work, int slice) {
    int[] left = new int[work.size()];
    Queue<Integer> queue = new ArrayDeque<>();
    for (int i = 0; i < left.length; i++) {
      left[i] = work.get(i);
      queue.offer(i); // enqueue at the tail
    }
    List<Integer> done = new ArrayList<>();
    while (!queue.isEmpty()) {
      int job = queue.poll(); // dequeue from the head
      left[job] -= Math.min(slice, left[job]);
      if (left[job] == 0) {
        done.add(job);
      } else {
        queue.offer(job);
      }
    }
    return done;
  }
}

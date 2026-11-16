// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;

final class ServeOrder {
  private ServeOrder() {}

  /**
   * Returns the request numbers in serve order: the largest priority first, and for equal
   * priorities, the smaller request number first.
   */
  static List<Integer> serveOrder(List<Integer> priorities) {
    // The head of a PriorityQueue is the least element for its comparator.
    // This comparator makes the largest priority the least, and for equal
    // priorities, the smaller request number.
    Comparator<Integer> order =
        Comparator.<Integer>comparingInt(priorities::get)
            .reversed()
            .thenComparingInt(i -> i);
    PriorityQueue<Integer> queue = new PriorityQueue<>(order);
    for (int i = 0; i < priorities.size(); i++) {
      queue.offer(i);
    }
    List<Integer> served = new ArrayList<>();
    while (!queue.isEmpty()) {
      served.add(queue.poll());
    }
    return served;
  }
}

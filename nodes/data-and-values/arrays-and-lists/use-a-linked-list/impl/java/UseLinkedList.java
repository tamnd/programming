// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

import java.util.LinkedList;

final class UseLinkedList {
  private UseLinkedList() {}

  /** Runs the operations on a new linked list and returns its items. */
  static int[] runOps(String[] ops) {
    LinkedList<Integer> list = new LinkedList<>();
    for (String op : ops) {
      String[] p = op.split(" ");
      switch (p[0]) {
        case "push_front" -> list.addFirst(Integer.parseInt(p[1]));
        case "push_back" -> list.addLast(Integer.parseInt(p[1]));
        case "pop_front" -> list.removeFirst();
        case "pop_back" -> list.removeLast();
        // remove(Object) removes a value. remove(int) would remove an index.
        case "remove" -> list.remove(Integer.valueOf(p[1]));
        default -> throw new IllegalArgumentException("bad operation: " + op);
      }
    }
    return list.stream().mapToInt(Integer::intValue).toArray();
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

final class Concat {
  private Concat() {}

  /** Returns a new array with the items of a, then the items of b. */
  static int[] concat(int[] a, int[] b) {
    // An array cannot grow, so make one of the full length and copy twice.
    // For a List, copy a into a new ArrayList and call addAll(b).
    int[] c = new int[a.length + b.length];
    System.arraycopy(a, 0, c, 0, a.length);
    System.arraycopy(b, 0, c, a.length, b.length);
    return c;
  }
}

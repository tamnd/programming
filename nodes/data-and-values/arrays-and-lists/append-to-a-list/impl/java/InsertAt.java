// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

final class InsertAt {
  private InsertAt() {}

  /** Returns a new array with value at position index. index items.length appends. */
  static int[] insertAt(int[] items, int index, int value) {
    if (index < 0 || index > items.length) {
      throw new IndexOutOfBoundsException(index);
    }
    // An array cannot grow, so make one with one more slot and copy the two parts.
    // For an ArrayList, list.add(index, value) does the same in place.
    int[] out = new int[items.length + 1];
    System.arraycopy(items, 0, out, 0, index);
    out[index] = value;
    System.arraycopy(items, index, out, index + 1, items.length - index);
    return out;
  }
}

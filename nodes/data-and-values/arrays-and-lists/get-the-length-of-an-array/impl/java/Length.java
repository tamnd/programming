// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

final class Length {
  private Length() {}

  /** Returns the number of items in items. */
  static int length(String[] items) {
    // length is a field of every array, so it has no parentheses.
    return items.length;
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

final class Classify {
  private Classify() {}

  private record Point(int x, int y) {}

  /** Names the place of the point (x, y): origin, x-axis, y-axis, diagonal or other. */
  static String classify(int x, int y) {
    // Record patterns take the point apart, and each when clause is a guard.
    return switch (new Point(x, y)) {
      case Point(int a, int b) when a == 0 && b == 0 -> "origin";
      case Point(int a, int b) when b == 0 -> "x-axis";
      case Point(int a, int b) when a == 0 -> "y-axis";
      case Point(int a, int b) when a == b -> "diagonal";
      case Point(int a, int b) -> "other";
    };
  }
}

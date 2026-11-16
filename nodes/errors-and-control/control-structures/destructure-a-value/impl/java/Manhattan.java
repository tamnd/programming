// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

final class Manhattan {
  private Manhattan() {}

  private record Point(int x, int y) {}

  private record Segment(Point from, Point to) {}

  /** Returns the Manhattan distance between the points p = {x, y} and q = {x, y}. */
  static int manhattan(int[] p, int[] q) {
    Segment s = new Segment(new Point(p[0], p[1]), new Point(q[0], q[1]));
    // A nested record pattern takes the segment and both points apart at once.
    return switch (s) {
      case Segment(Point(int x1, int y1), Point(int x2, int y2)) ->
          Math.abs(x1 - x2) + Math.abs(y1 - y2);
    };
  }
}

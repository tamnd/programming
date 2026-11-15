// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

import java.util.List;

/** A point on a grid. A record has final fields, a constructor, equals and toString. */
public record Point(int x, int y) {
  static final Point ORIGIN = new Point(0, 0);

  /** Returns the point moved by (dx, dy). */
  public Point translate(int dx, int dy) {
    return new Point(x + dx, y + dy);
  }

  /** Returns the grid distance |dx| + |dy| to another point. */
  public int manhattan(Point other) {
    return Math.abs(x - other.x) + Math.abs(y - other.y);
  }

  public static List<Integer> pointReport(int x, int y, int dx, int dy) {
    Point p = new Point(x, y);
    Point q = p.translate(dx, dy);
    return List.of(q.x(), q.y(), p.manhattan(q), q.manhattan(ORIGIN));
  }
}

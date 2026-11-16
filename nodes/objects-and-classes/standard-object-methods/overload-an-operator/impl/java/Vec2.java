// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

import java.util.List;

/**
 * A 2D vector. Java has no user operators, so the type has a plus method in place of +, and the
 * record generates equals in place of ==.
 */
public record Vec2(long x, long y) {
  public static final Vec2 ZERO = new Vec2(0, 0);

  public Vec2 plus(Vec2 other) {
    return new Vec2(x + other.x, y + other.y);
  }

  public static List<Long> vectorSum(List<Long> xs, List<Long> ys) {
    if (xs.size() != ys.size()) {
      throw new IllegalArgumentException("the lists must have the same length");
    }
    Vec2 total = ZERO;
    for (int i = 0; i < xs.size(); i++) {
      total = total.plus(new Vec2(xs.get(i), ys.get(i)));
    }
    return List.of(total.x, total.y);
  }
}

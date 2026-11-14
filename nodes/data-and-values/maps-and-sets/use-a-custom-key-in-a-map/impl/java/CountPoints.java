// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

final class CountPoints {
  private CountPoints() {}

  /** A record gets equals and hashCode from its fields, so it works as a map key. */
  record Point(int x, int y) {}

  /** Returns [x, y, count] for each distinct point, in order of x, then y. */
  static List<List<Integer>> countPoints(List<List<Integer>> points) {
    Map<Point, Integer> counts = new HashMap<>();
    for (List<Integer> p : points) {
      counts.merge(new Point(p.get(0), p.get(1)), 1, Integer::sum);
    }
    Comparator<Point> order = Comparator.comparingInt(Point::x).thenComparingInt(Point::y);
    return counts.entrySet().stream()
        .sorted(Map.Entry.comparingByKey(order))
        .map(e -> List.of(e.getKey().x(), e.getKey().y(), e.getValue()))
        .toList();
  }
}

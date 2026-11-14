// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

import java.util.List;
import java.util.Set;
import java.util.TreeSet;

final class CompareSets {
  private CompareSets() {}

  /** The results of the set operations. Each list is in ascending order. */
  record Result(
      List<String> union,
      List<String> intersection,
      List<String> difference,
      List<String> symmetricDifference,
      boolean subset) {}

  static Result compareSets(List<String> a, List<String> b) {
    // A TreeSet keeps its items in order. String.compareTo compares UTF-16
    // code units, which is the byte order for ASCII.
    Set<String> setA = new TreeSet<>(a);
    Set<String> setB = new TreeSet<>(b);
    // addAll, retainAll and removeAll change the set, so each one works on a copy.
    Set<String> union = new TreeSet<>(setA);
    union.addAll(setB);
    Set<String> intersection = new TreeSet<>(setA);
    intersection.retainAll(setB);
    Set<String> difference = new TreeSet<>(setA);
    difference.removeAll(setB);
    Set<String> symmetric = new TreeSet<>(union);
    symmetric.removeAll(intersection);
    return new Result(
        List.copyOf(union),
        List.copyOf(intersection),
        List.copyOf(difference),
        List.copyOf(symmetric),
        setB.containsAll(setA));
  }
}

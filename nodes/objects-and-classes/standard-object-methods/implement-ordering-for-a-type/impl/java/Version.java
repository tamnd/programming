// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

import java.util.Comparator;
import java.util.List;

/** A version MAJOR.MINOR.PATCH whose natural ordering compares the numbers, most important first. */
public record Version(int major, int minor, int patch) implements Comparable<Version> {
  private static final Comparator<Version> ORDER =
      Comparator.comparingInt(Version::major)
          .thenComparingInt(Version::minor)
          .thenComparingInt(Version::patch);

  public static Version parse(String text) {
    String[] p = text.split("\\.");
    return new Version(Integer.parseInt(p[0]), Integer.parseInt(p[1]), Integer.parseInt(p[2]));
  }

  /** Consistent with equals: it returns 0 only when all three fields are equal. */
  @Override
  public int compareTo(Version other) {
    return ORDER.compare(this, other);
  }

  @Override
  public String toString() {
    return major + "." + minor + "." + patch;
  }

  public static List<String> sortVersions(List<String> versions) {
    return versions.stream().map(Version::parse).sorted().map(Version::toString).toList();
  }
}

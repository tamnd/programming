// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.function.Function;

/** Removes duplicates from lists and keeps the input order. */
public final class Unique {

  /** A record of a user. A record gets equals and hashCode from its components. */
  public record User(String name, String email) {}

  private Unique() {}

  /** Returns a new list of the items without later copies. The first copies keep their order. */
  public static <T> List<T> unique(List<T> items) {
    // For an ordered stream, distinct keeps the first copy of each item.
    return items.stream().distinct().toList();
  }

  /** Returns a new list of the first item for each key, in input order. */
  public static <T, K> List<T> uniqueBy(List<T> items, Function<? super T, ? extends K> key) {
    Set<K> seen = new HashSet<>();
    List<T> out = new ArrayList<>();
    for (T item : items) {
      // add returns false if the set already has the key.
      if (seen.add(key.apply(item))) {
        out.add(item);
      }
    }
    return out;
  }

  /** Returns the first user for each email. Case does not matter in the emails. */
  public static List<User> uniqueByEmail(List<User> users) {
    // Locale.ROOT: the rules of the default locale can change the letters, as in Turkish.
    return uniqueBy(users, u -> u.email().toLowerCase(Locale.ROOT));
  }
}

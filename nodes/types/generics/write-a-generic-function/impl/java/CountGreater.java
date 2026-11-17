// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

import java.util.List;

/** A generic method with a bounded type parameter. */
public final class CountGreater {
    private CountGreater() {}

    /** Returns how many items are greater than limit. */
    public static <T extends Comparable<T>> int countGreater(List<T> items, T limit) {
        int count = 0;
        for (T item : items) {
            if (item.compareTo(limit) > 0) {
                count++;
            }
        }
        return count;
    }
}

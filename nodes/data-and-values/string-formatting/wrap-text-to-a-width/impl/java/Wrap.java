// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

import java.util.ArrayList;
import java.util.List;

/** Greedy word wrap. */
public final class Wrap {
    private Wrap() {}

    /**
     * Returns the lines of text, each at most width characters long, with the
     * greedy method. A word longer than width is a line of its own.
     */
    public static List<String> wrap(String text, int width) {
        List<String> lines = new ArrayList<>();
        StringBuilder line = new StringBuilder();
        for (String word : text.strip().split("\\s+")) {
            if (word.isEmpty()) {
                continue;
            }
            if (line.isEmpty()) {
                line.append(word);
            } else if (line.length() + 1 + word.length() <= width) {
                line.append(' ').append(word);
            } else {
                lines.add(line.toString());
                line.setLength(0);
                line.append(word);
            }
        }
        if (!line.isEmpty()) {
            lines.add(line.toString());
        }
        return lines;
    }

    public static void main(String[] args) {
        wrap("The quick brown fox jumps over the lazy dog", 10).forEach(System.out::println);
    }
}

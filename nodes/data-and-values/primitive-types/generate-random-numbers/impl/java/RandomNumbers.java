// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

import java.security.SecureRandom;
import java.util.Collections;
import java.util.HexFormat;
import java.util.List;
import java.util.Random;
import java.util.function.IntSupplier;

/** Random integers in a range, a shuffle, a seeded generator and tokens. */
public final class RandomNumbers {
    private static final long WORDS = 1L << 32;
    private static final SecureRandom SECURE = new SecureRandom();

    private RandomNumbers() {}

    /**
     * Returns an integer from lo to hi, each with equal probability. nextWord gives all 32 bits of
     * an int, and the method reads them as an unsigned value. A word below 2^32 % n is rejected,
     * so that each remainder comes from the same number of words.
     */
    public static long intInRange(IntSupplier nextWord, long lo, long hi) {
        // hi - lo can overflow a long, but read as unsigned it is the true difference.
        if (hi < lo || Long.compareUnsigned(hi - lo, WORDS - 1) > 0) {
            throw new IllegalArgumentException("need lo <= hi and at most 2^32 values");
        }
        long n = hi - lo + 1;
        long least = WORDS % n;
        while (true) {
            long word = Integer.toUnsignedLong(nextWord.getAsInt());
            if (word >= least) {
                return lo + word % n;
            }
        }
    }

    /** Shuffles items in place with the Fisher-Yates method. */
    public static void shuffle(IntSupplier nextWord, List<?> items) {
        for (int i = items.size() - 1; i > 0; i--) {
            int j = (int) intInRange(nextWord, 0, i);
            Collections.swap(items, i, j);
        }
    }

    /** Returns a word source that gives the same words for the same seed. */
    public static IntSupplier seededWords(long seed) {
        return new Random(seed)::nextInt;
    }

    /** Returns a word source from the secure generator of the platform. */
    public static IntSupplier secureWords() {
        return SECURE::nextInt;
    }

    /** Returns nbytes secure random bytes as 2 * nbytes lowercase hexadecimal digits. */
    public static String tokenHex(int nbytes) {
        byte[] bytes = new byte[nbytes];
        SECURE.nextBytes(bytes);
        return HexFormat.of().formatHex(bytes);
    }
}

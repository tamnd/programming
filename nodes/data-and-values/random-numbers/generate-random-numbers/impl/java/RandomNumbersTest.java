// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.function.IntSupplier;
import java.util.stream.LongStream;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

class RandomNumbersTest {
    static Stream<Arguments> rangeCases() {
        return Stream.of(
                Arguments.of("coin", new long[] {5L, 2L, 7L}, 0L, 1L, 3, List.of(1L, 0L, 1L)),
                Arguments.of("no-results", new long[] {}, 1L, 6L, 0, List.of()),
                Arguments.of("die", new long[] {7L, 12L, 1000000L, 4294967295L}, 1L, 6L, 4, List.of(2L, 1L, 5L, 4L)),
                Arguments.of("negative-range", new long[] {100L, 2000000000L, 4294967294L}, -10L, 10L, 3, List.of(6L, -5L, -8L)),
                Arguments.of("one-value", new long[] {0L, 99L}, 5L, 5L, 2, List.of(5L, 5L)),
                Arguments.of("reject-low-words", new long[] {0L, 3L, 4L}, 1L, 6L, 1, List.of(5L)),
                Arguments.of("accept-least", new long[] {3L, 4L, 9L}, 0L, 5L, 2, List.of(4L, 3L)),
                Arguments.of("power-of-two", new long[] {0L, 4294967295L}, 0L, 7L, 2, List.of(0L, 7L)),
                Arguments.of("worst-case", new long[] {2147483646L, 2147483647L}, 0L, 2147483648L, 1, List.of(2147483647L)),
                Arguments.of("full-unsigned-32", new long[] {0L, 4294967295L, 123456789L}, 0L, 4294967295L, 3, List.of(0L, 4294967295L, 123456789L)),
                Arguments.of("full-signed-32", new long[] {0L, 2147483648L, 4294967295L}, -2147483648L, 2147483647L, 3, List.of(-2147483648L, 0L, 2147483647L)),
                Arguments.of("near-int64-max", new long[] {4294967295L, 6L}, 9223372036854775798L, 9223372036854775807L, 2, List.of(9223372036854775803L, 9223372036854775804L)),
                Arguments.of("near-int64-min", new long[] {4294967295L, 9L}, Long.MIN_VALUE, -9223372036854775799L, 2, List.of(-9223372036854775803L, -9223372036854775799L)),
                Arguments.of("wide-int64", new long[] {4294967295L, 0L}, 9223372032559808512L, 9223372036854775807L, 2, List.of(9223372036854775807L, 9223372032559808512L))
        );
    }

    static Stream<Arguments> shuffleCases() {
        return Stream.of(
                Arguments.of("empty", new long[] {}, List.of(), List.of()),
                Arguments.of("one-item", new long[] {}, List.of("a"), List.of("a")),
                Arguments.of("two-items-swap", new long[] {0L}, List.of("a", "b"), List.of("b", "a")),
                Arguments.of("two-items-keep", new long[] {1L}, List.of("a", "b"), List.of("a", "b")),
                Arguments.of("five-items", new long[] {7L, 3L, 4L, 1L}, List.of("a", "b", "c", "d", "e"), List.of("a", "e", "b", "d", "c")),
                Arguments.of("reject-in-shuffle", new long[] {0L, 2L, 0L}, List.of("a", "b", "c"), List.of("b", "a", "c")),
                Arguments.of("duplicates", new long[] {1L, 1L}, List.of("x", "x", "y"), List.of("x", "y", "x")),
                Arguments.of("non-ascii", new long[] {4294967295L, 4294967294L, 0L}, List.of("é", "日本", "🙂", "a"), List.of("日本", "é", "🙂", "a"))
        );
    }

    /** A word source that gives the words in order. */
    private static final class FakeWords implements IntSupplier {
        private final long[] words;
        private int next;

        FakeWords(long[] words) {
            this.words = words;
        }

        @Override
        public int getAsInt() {
            return (int) words[next++];
        }

        boolean usedAll() {
            return next == words.length;
        }
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("rangeCases")
    void intInRange(String name, long[] words, long lo, long hi, int count, List<Long> want) {
        FakeWords source = new FakeWords(words);
        List<Long> got = LongStream.range(0, count).mapToObj(k -> RandomNumbers.intInRange(source, lo, hi)).toList();
        assertEquals(want, got, name);
        assertTrue(source.usedAll(), name);
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("shuffleCases")
    void shuffle(String name, long[] words, List<String> items, List<String> want) {
        FakeWords source = new FakeWords(words);
        List<String> got = new ArrayList<>(items);
        RandomNumbers.shuffle(source, got);
        assertEquals(want, got, name);
        assertTrue(source.usedAll(), name);
    }

    @Test
    void badRangeThrows() {
        assertThrows(IllegalArgumentException.class, () -> RandomNumbers.intInRange(() -> 0, 1, 0));
        assertThrows(IllegalArgumentException.class, () -> RandomNumbers.intInRange(() -> 0, 0, 1L << 32));
        assertThrows(IllegalArgumentException.class, () -> RandomNumbers.intInRange(() -> 0, Long.MIN_VALUE, Long.MAX_VALUE));
    }

    @Test
    void sameSeedSameWords() {
        IntSupplier a = RandomNumbers.seededWords(42);
        IntSupplier b = RandomNumbers.seededWords(42);
        IntSupplier c = RandomNumbers.seededWords(43);
        List<Integer> first = Stream.generate(a::getAsInt).limit(10).toList();
        assertEquals(first, Stream.generate(b::getAsInt).limit(10).toList());
        assertNotEquals(first, Stream.generate(c::getAsInt).limit(10).toList());
    }

    @Test
    void secureWordsInRange() {
        IntSupplier source = RandomNumbers.secureWords();
        for (int i = 0; i < 100; i++) {
            long v = RandomNumbers.intInRange(source, 1, 6);
            assertTrue(v >= 1 && v <= 6);
        }
    }

    @ParameterizedTest
    @ValueSource(ints = {0, 1, 16, 32})
    void tokenHex(int nbytes) {
        assertTrue(RandomNumbers.tokenHex(nbytes).matches("[0-9a-f]{" + 2 * nbytes + "}"));
    }

    @Test
    void tokensDiffer() {
        assertNotEquals(RandomNumbers.tokenHex(16), RandomNumbers.tokenHex(16));
    }
}

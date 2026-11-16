// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.IntStream;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SumsTest {
  static Stream<Arguments> cases() {
    long[][] many = IntStream.range(0, 32).mapToObj(i -> new long[] {i, i, i}).toArray(long[][]::new);
    long[] manyWant = IntStream.range(0, 32).mapToLong(i -> 3L * i).toArray();
    long g = 1_000_000_000L;
    return Stream.of(
        Arguments.of("two", new long[][] {{1, 2, 3}, {4, 5}}, new long[] {6, 9}),
        Arguments.of("three", new long[][] {{10}, {20, 30}, {-5, 5}}, new long[] {10, 50, 0}),
        Arguments.of("empty", new long[][] {}, new long[] {}),
        Arguments.of("empty-chunk", new long[][] {{}, {7}}, new long[] {0, 7}),
        Arguments.of("single", new long[][] {{42}}, new long[] {42}),
        Arguments.of("negative", new long[][] {{-1, -2}, {-3}}, new long[] {-3, -3}),
        Arguments.of("limit", new long[][] {{g, g, g}, {-g, -g, -g}}, new long[] {3 * g, -3 * g}),
        Arguments.of("many", many, manyWant));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void parallelSums(String name, long[][] chunks, long[] want) throws InterruptedException {
    assertArrayEquals(want, Sums.parallelSums(chunks), name);
  }
}

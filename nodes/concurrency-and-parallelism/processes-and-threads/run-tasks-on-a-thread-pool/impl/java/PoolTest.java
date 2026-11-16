// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.concurrent.ExecutionException;
import java.util.stream.IntStream;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PoolTest {
  static Stream<Arguments> cases() {
    int[] many = IntStream.rangeClosed(1, 200).toArray();
    int[] manyWant = IntStream.rangeClosed(1, 200).map(n -> Pool.collatzSteps(n)).toArray();
    return Stream.of(
        Arguments.of("four-jobs", 2, new int[] {1, 2, 3, 6}, new int[] {0, 1, 7, 8}),
        Arguments.of("long-runs", 3, new int[] {27, 97, 871}, new int[] {111, 118, 178}),
        Arguments.of("empty", 2, new int[] {}, new int[] {}),
        Arguments.of("one-worker", 1, new int[] {7, 8, 9}, new int[] {16, 3, 19}),
        Arguments.of("more-workers-than-jobs", 8, new int[] {5, 6}, new int[] {5, 8}),
        Arguments.of("duplicates", 3, new int[] {27, 27, 27, 1}, new int[] {111, 111, 111, 0}),
        Arguments.of("wide-intermediate", 2, new int[] {113383, 837799}, new int[] {247, 524}),
        Arguments.of("many", 4, many, manyWant));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void stepsOnPool(String name, int workers, int[] numbers, int[] want)
      throws InterruptedException, ExecutionException {
    assertArrayEquals(want, Pool.stepsOnPool(workers, numbers), name);
  }
}

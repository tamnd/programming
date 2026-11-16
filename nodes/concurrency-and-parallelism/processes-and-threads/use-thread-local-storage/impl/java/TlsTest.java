// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.IntStream;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TlsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three-threads", new int[] {1, 2, 3}),
        Arguments.of("uneven", new int[] {10, 0, 5, 1000}),
        Arguments.of("empty", new int[] {}),
        Arguments.of("zero", new int[] {0}),
        Arguments.of("one-thread", new int[] {7}),
        Arguments.of("equal-counts", new int[] {100, 100, 100, 100}),
        Arguments.of("many-threads", IntStream.rangeClosed(1, 16).toArray()),
        Arguments.of("large", new int[] {100000, 100000}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countPerThread(String name, int[] counts) throws InterruptedException {
    assertArrayEquals(counts, Tls.countPerThread(counts), name);
  }

  @Test
  void callerCounterDoesNotChange() throws InterruptedException {
    int[] seen = new int[1];
    // Run in a new thread, so that other tests on this thread do not matter.
    Thread caller =
        Thread.ofPlatform()
            .start(
                () -> {
                  for (int k = 0; k < 3; k++) {
                    Tls.bump();
                  }
                  try {
                    Tls.countPerThread(new int[] {5, 6});
                  } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                  }
                  seen[0] = Tls.current();
                });
    caller.join();
    assertEquals(3, seen[0]);
  }
}

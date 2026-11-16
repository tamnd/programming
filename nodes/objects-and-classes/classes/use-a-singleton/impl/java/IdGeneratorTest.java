// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class IdGeneratorTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three-calls", 3, List.of(2L, 1L, 1L)),
        Arguments.of("ten-calls", 10, List.of(9L, 1L, 1L)),
        Arguments.of("single-call", 1, List.of(0L, 1L, 1L)),
        Arguments.of("pair", 2, List.of(1L, 1L, 1L)),
        Arguments.of("limit", 1000000, List.of(999999L, 1L, 1L)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void singletonReport(String name, int calls, List<Long> want) {
    assertEquals(want, IdGenerator.singletonReport(calls));
  }

  @Test
  void threadsShareOneInstance() throws Exception {
    ExecutorService pool = Executors.newFixedThreadPool(8);
    try {
      Callable<IdGenerator> task = IdGenerator::instance;
      List<Future<IdGenerator>> got = pool.invokeAll(Collections.nCopies(8, task));
      for (Future<IdGenerator> f : got) {
        assertSame(IdGenerator.instance(), f.get());
      }
    } finally {
      pool.shutdown();
    }
    assertEquals(1, IdGenerator.CONSTRUCTIONS.get());
  }
}

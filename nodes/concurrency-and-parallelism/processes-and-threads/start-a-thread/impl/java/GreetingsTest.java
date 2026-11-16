// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.IntStream;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class GreetingsTest {
  static Stream<Arguments> cases() {
    List<String> many = IntStream.range(0, 32).mapToObj(i -> "t" + i).toList();
    return Stream.of(
        Arguments.of("two", List.of("Ada", "Linus"), List.of("hello from Ada", "hello from Linus")),
        Arguments.of(
            "three",
            List.of("main", "worker", "logger"),
            List.of("hello from main", "hello from worker", "hello from logger")),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("single", List.of("solo"), List.of("hello from solo")),
        Arguments.of("empty-name", List.of(""), List.of("hello from ")),
        Arguments.of("duplicate", List.of("twin", "twin"), List.of("hello from twin", "hello from twin")),
        Arguments.of("unicode", List.of("Zoë", "李雷"), List.of("hello from Zoë", "hello from 李雷")),
        Arguments.of("many", many, many.stream().map(n -> "hello from " + n).toList()));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void threadGreetings(String name, List<String> names, List<String> want) throws InterruptedException {
    assertEquals(want, Greetings.threadGreetings(names), name);
  }
}

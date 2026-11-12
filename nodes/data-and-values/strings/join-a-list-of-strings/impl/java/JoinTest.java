// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class JoinTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("comma", List.of("a", "b", "c"), ",", "a,b,c"),
        Arguments.of("comma-space", List.of("red", "green", "blue"), ", ", "red, green, blue"),
        Arguments.of("words", List.of("Hello", "world"), " ", "Hello world"),
        Arguments.of("empty-list", List.of(), ",", ""),
        Arguments.of("single", List.of("only"), ",", "only"),
        Arguments.of("empty-items", List.of("", "", ""), ",", ",,"),
        Arguments.of("empty-middle", List.of("a", "", "b"), "-", "a--b"),
        Arguments.of("empty-sep", List.of("a", "b", "c"), "", "abc"),
        Arguments.of("long-sep", List.of("x", "y"), " -> ", "x -> y"),
        Arguments.of("non-ascii", List.of("naïve", "日本"), "·", "naïve·日本"),
        Arguments.of("astral-sep", List.of("a", "b"), "🙂", "a🙂b"),
        Arguments.of("sep-in-item", List.of("a,b", "c"), ",", "a,b,c"));
  }

  static Stream<Arguments> numberCases() {
    return Stream.of(
        Arguments.of("numbers", new int[] {1, 2, 3}, ",", "1,2,3"),
        Arguments.of("negative", new int[] {-1, 0, 1}, " ", "-1 0 1"),
        Arguments.of("empty", new int[] {}, ",", ""),
        Arguments.of("single", new int[] {42}, ",", "42"),
        Arguments.of("int32-ends", new int[] {2147483647, -2147483648}, ";", "2147483647;-2147483648"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void join(String name, List<String> items, String sep, String want) {
    assertEquals(want, Join.join(items, sep));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("numberCases")
  void joinNumbers(String name, int[] items, String sep, String want) {
    assertEquals(want, Join.joinNumbers(items, sep));
  }
}

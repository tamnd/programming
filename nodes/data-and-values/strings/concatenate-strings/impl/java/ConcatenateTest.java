// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ConcatenateTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("hello-world", "Hello, ", "world", "Hello, world"),
        Arguments.of("two-words", "good", "bye", "goodbye"),
        Arguments.of("keep-spaces", "a ", " b", "a  b"),
        Arguments.of("empty-both", "", "", ""),
        Arguments.of("empty-left", "", "abc", "abc"),
        Arguments.of("empty-right", "abc", "", "abc"),
        Arguments.of("non-ascii", "naï", "ve", "naïve"),
        Arguments.of("astral", "a", "🙂", "a🙂"),
        Arguments.of("combining-mark", "e", "\u0301", "e\u0301"),
        Arguments.of("flag-halves", "🇫", "🇷", "🇫🇷"),
        Arguments.of("digit-strings", "1", "2", "12"));
  }

  static Stream<Arguments> numberCases() {
    return Stream.of(
        Arguments.of("positive", "item", 3, "item3"),
        Arguments.of("zero", "n=", 0, "n=0"),
        Arguments.of("negative", "x", -5, "x-5"),
        Arguments.of("empty-prefix", "", 42, "42"),
        Arguments.of("int32-max", "max=", 2147483647, "max=2147483647"),
        Arguments.of("int32-min", "min=", -2147483648, "min=-2147483648"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void concat(String name, String a, String b, String want) {
    assertEquals(want, Concatenate.concat(a, b), name);
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("numberCases")
  void concatNumber(String name, String s, int n, String want) {
    assertEquals(want, Concatenate.concatNumber(s, n), name);
  }
}

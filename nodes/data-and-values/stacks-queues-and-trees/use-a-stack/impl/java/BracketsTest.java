// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class BracketsTest {
  static Stream<Arguments> cases() {
    String deep = "(".repeat(1000) + ")".repeat(1000);
    return Stream.of(
        Arguments.of("pair", "()", true),
        Arguments.of("nested", "{[()()]}", true),
        Arguments.of("code", "f(a[0], {b: 1})", true),
        Arguments.of("crossed", "([)]", false),
        Arguments.of("empty", "", true),
        Arguments.of("no-brackets", "abc", true),
        Arguments.of("unclosed", "((", false),
        Arguments.of("extra-close", "())", false),
        Arguments.of("close-first", ")(", false),
        Arguments.of("wrong-kind", "(]", false),
        Arguments.of("deep", deep, true));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void balanced(String name, String text, boolean want) {
    assertEquals(want, Brackets.balanced(text));
  }
}

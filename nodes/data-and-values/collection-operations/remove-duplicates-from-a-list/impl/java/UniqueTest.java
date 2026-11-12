// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class UniqueTest {

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("repeats", List.of("apple", "pear", "apple", "fig", "pear"), List.of("apple", "pear", "fig")),
        Arguments.of("words", List.of("to", "be", "or", "not", "to", "be"), List.of("to", "be", "or", "not")),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("single", List.of("a"), List.of("a")),
        Arguments.of("all-same", List.of("x", "x", "x"), List.of("x")),
        Arguments.of("no-duplicates", List.of("c", "a", "b"), List.of("c", "a", "b")),
        Arguments.of("not-adjacent", List.of("a", "b", "a", "b"), List.of("a", "b")),
        Arguments.of("adjacent", List.of("a", "a", "b", "b", "a"), List.of("a", "b")),
        Arguments.of("empty-string", List.of("", "a", "", "b"), List.of("", "a", "b")),
        Arguments.of("case", List.of("a", "A", "a"), List.of("a", "A")),
        Arguments.of("normalization", List.of("\u00e9", "e\u0301", "\u00e9"), List.of("\u00e9", "e\u0301")),
        Arguments.of("whitespace", List.of(" a", "a", "a ", " a"), List.of(" a", "a", "a ")),
        Arguments.of("number-like", List.of("1", "01", "1.0", "1"), List.of("1", "01", "1.0")),
        Arguments.of("non-ascii", List.of("日本", "🙂", "日本", "🙂", "ü"), List.of("日本", "🙂", "ü")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void keepsTheFirstCopies(String name, List<String> given, List<String> want) {
    List<String> input = new ArrayList<>(given);
    assertEquals(want, Unique.unique(input), name);
    assertEquals(given, input, "the input must not change");
  }

  private static List<Unique.User> users(String... nameEmail) {
    List<Unique.User> out = new ArrayList<>();
    for (int i = 0; i < nameEmail.length; i += 2) {
      out.add(new Unique.User(nameEmail[i], nameEmail[i + 1]));
    }
    return out;
  }

  /** The cases of cases/by-key.jsonl. Each list is name, email, name, email and so on. */
  static Stream<Arguments> byKey() {
    return Stream.of(
        Arguments.of(
            "case-insensitive",
            users("Ada", "ada@example.com", "Bob", "bob@example.com", "Ada L.", "ADA@Example.com"),
            users("Ada", "ada@example.com", "Bob", "bob@example.com")),
        Arguments.of("empty", users(), users()),
        Arguments.of(
            "same-name-other-email",
            users("Sam", "sam@a.example", "Sam", "sam@b.example"),
            users("Sam", "sam@a.example", "Sam", "sam@b.example")),
        Arguments.of(
            "first-record-wins",
            users("Old", "x@example.com", "New", "X@EXAMPLE.COM"),
            users("Old", "x@example.com")),
        Arguments.of(
            "dots-differ",
            users("Al", "a.b@example.com", "Bo", "ab@example.com"),
            users("Al", "a.b@example.com", "Bo", "ab@example.com")),
        Arguments.of(
            "not-adjacent",
            users("A", "a@example.com", "B", "b@example.com", "C", "A@example.com", "D", "B@EXAMPLE.com"),
            users("A", "a@example.com", "B", "b@example.com")));
  }

  @ParameterizedTest(name = "by-key {0}")
  @MethodSource("byKey")
  void keepsTheFirstUserForEachEmail(String name, List<Unique.User> given, List<Unique.User> want) {
    assertEquals(want, Unique.uniqueByEmail(given), name);
  }
}
